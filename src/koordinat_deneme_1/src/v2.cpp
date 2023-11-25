#include "ros/init.h"
#include <ros/ros.h>
#include <cmath>
#include <fs_msgs/ControlCommand.h>
#include <sensor_msgs/LaserScan.h>
#include <iostream>

float PI = 3.14159;
float   leftConeDist;
float   rightConeDist;

float getCosAlpha(float alpha, float hipo)
{
    float cosAlphaDist = cos(alpha) * hipo;
    return (cosAlphaDist);
}

float getSinAlpha(float alpha, float hipo)
{
    float sinAlphaDist = sin(alpha) * hipo;
    return (sinAlphaDist);
}

void minDist(std::vector<float>& dists)
{
    leftConeDist = 10;
    rightConeDist = 10;
    for (std::vector<float>::iterator it = dists.begin(); it != dists.end(); it++)
    {
        std::cout << "IT: " << *it << std::endl;
        if (*it < 0)
        {
            if (abs(*it) < rightConeDist)
                rightConeDist = abs(*it);
        }
        else
        {
            if (*it < leftConeDist)
                leftConeDist = *it;
        }
    }
}

float   degreeToRadian(float degree)
{
    float radian = (degree * PI) / 180;
    return (radian);
}

void lidarScan(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    std::vector<float> data = msg->ranges;
    int dataSize = data.size();
    float rate = 180;
    rate /= dataSize;

    std::vector<float> coneDists;
    for (int angle = 0; angle < (dataSize - 1); angle++)
    {
        // if (data[angle] == std::numeric_limits<float>::infinity())
        //     data[angle] = 8;
        if (data[angle] != std::numeric_limits<float>::infinity())
        {
            while (angle < (dataSize - 1) && data[angle] != std::numeric_limits<float>::infinity());
            {
                ROS_INFO("Girdim %f\n", data[angle]);
                coneDists.push_back(getCosAlpha(degreeToRadian(angle * rate), data[angle]));
                angle++;
            }
        }
    }
    minDist(coneDists);
}

int main(int ac, char **av)
{

    ros::init(ac, av, "v2", 0);
    ros::NodeHandle nodeManage;
    ros::Subscriber subs;
    ros::Publisher pub;

    leftConeDist = 0;
    rightConeDist = 0;
    
    ros::Rate rateHz(10);
    pub = nodeManage.advertise<fs_msgs::ControlCommand>("/fsds/control_command", 1);
    fs_msgs::ControlCommand cmd = fs_msgs::ControlCommand();
    cmd.header.stamp = ros::Time::now();
    subs = nodeManage.subscribe("scan", 1000, lidarScan);

    int throttleManageCount = 0;
    cmd.throttle = 0;
    cmd.steering = 0;
    cmd.brake = 0;
    while (ros::ok())
    {
        // ROS_INFO("left: %lld\n", leftConeDist);
        // ROS_INFO("right: %lld\n", rightConeDist);
        float fark = ((leftConeDist + rightConeDist) / 2) - leftConeDist;
        // if (leftConeDist <= 0.1)
        //     fark = 2 - rightConeDist;
        // else if (rightConeDist <= 0.1)
        //     fark = 2 - leftConeDist;
        fark *= -1;
        ROS_INFO("FARK : %f y:%f\n", fark, leftConeDist + rightConeDist);
        if (fark != 0)
            cmd.steering = (fark * (0.4));
        if (fark > 2)
            cmd.brake = 0.2;
        else
            cmd.brake = 0;
        if (throttleManageCount % 120 == 0)
            cmd.throttle = 0.3;
        else if (throttleManageCount < 60)
            cmd.throttle = 1;
        else if (throttleManageCount % 10 == 0)
            cmd.throttle = 0;
        else if (throttleManageCount % 130 == 0)
            throttleManageCount = 60;
        throttleManageCount++;
        pub.publish(cmd);
        rateHz.sleep();
        ros::spinOnce();
    }
    return (0);
}

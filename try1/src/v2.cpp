#include "ros/init.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <fs_msgs/ControlCommand.h>
#include <sensor_msgs/LaserScan.h>
#include <iostream>

float leftConeDist;
float rightConeDist;

float coneDataAverage(std::vector<float> &array)
{
    float average = 0;
    for (size_t i = 0; i < array.size(); i++)
    {
        average += array.at(i);
    }
    average /= array.size();

    return (average);
}

void lidarScan(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    std::vector<float> data = msg->ranges;
    int dataSize = data.size();
    int coneDataCount;
    int coneId;
    
    coneId = 1;
    for (int i = 0; i < dataSize ; i++)
    {

        if (data[i] == std::numeric_limits<float>::infinity())
            data[i] = 8;
        else
        {
            // ROS_INFO("Cone DATA : %f\n", data[i]);
            coneDataCount = 0;
            std::vector<float> coneDatas;
            while (data[i] != std::numeric_limits<float>::infinity())
            {
                // ROS_INFO("Cone DATA : %f\n", data[i]);
                coneDatas.push_back(data[i]);
                coneDataCount++;
                i++;

            }
            if (coneId == 2)
            {
                leftConeDist = coneDataAverage(coneDatas);
            }
            else if (coneId == 3 && data[i] != 0)
            {
                // ROS_INFO("rghttttttttttttttttttt : %d\n", coneDataAverage(coneDatas)); 
                rightConeDist = coneDataAverage(coneDatas);
            }
            coneId++;
        }
    }
}

int main(int ac, char **av)
{

    ros::init(ac, av, "v2", 0);
    ros::NodeHandle nodeManage;
    ros::Subscriber subs;
    ros::Publisher pub;

    
    ros::Rate rateHz(10);
    pub = nodeManage.advertise<fs_msgs::ControlCommand>("/fsds/control_command", 1);
    fs_msgs::ControlCommand cmd = fs_msgs::ControlCommand();
    cmd.header.stamp = ros::Time::now();
    subs = nodeManage.subscribe("scan", 1000, lidarScan);

    leftConeDist = 0;
    rightConeDist = 0;
    int throttleManageCount = 0;
    cmd.throttle = 0;
    cmd.steering = 0;
    cmd.brake = 0;
    // int steeringManip = 0;
    while (ros::ok())
    {
        // ROS_INFO("left: %lld\n", leftConeDist);
        // ROS_INFO("right: %lld\n", rightConeDist);
        // if (leftConeDist > 5 && steeringManip % 15 == 0)
        //     cmd.steering = -0.5;
        // else if (rightConeDist > 5 && steeringManip % 30 == 0)
        //     cmd.steering = +0.5;
        // if (steeringManip % 40 == 0){
        //     cmd.steering = 0;
        //     steeringManip = 0;
        // }
        float fark = ((leftConeDist + rightConeDist) / 2) - leftConeDist;
        ROS_INFO("FARK : %f\n", fark);
        if (fark != 0)
            cmd.steering = fark * (fark / 10);
        if (throttleManageCount % 50 == 0)
            cmd.throttle = 0.5;
        else if (throttleManageCount % 10 == 0)
            cmd.throttle = 0;
        else if (throttleManageCount % 60 == 0)
            throttleManageCount = 0;
        throttleManageCount++;
        // steeringManip++;
        pub.publish(cmd);
        rateHz.sleep();
        ros::spinOnce();
    }
    return (0);
}

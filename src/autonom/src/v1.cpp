#include "ros/init.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <fs_msgs/ControlCommand.h>
#include <sensor_msgs/LaserScan.h>
#include <iostream>

unsigned long long zaa;

void my_func (const sensor_msgs::LaserScan::ConstPtr& msg){
    ros::NodeHandle nodeHandle;
    std::vector<float> data = msg->ranges;
    ros::Publisher publisher;
    fs_msgs::ControlCommand cmd = fs_msgs::ControlCommand();
    cmd.header.stamp = ros::Time::now();
    publisher = nodeHandle.advertise<fs_msgs::ControlCommand>("/fsds/control_command", 1);
    int len = data.size();
    int coneCounter = 1;
    zaa++;
    cmd.brake = 0;
    cmd.steering = 0;
    if (zaa <= 600)
    {
        ROS_INFO("zaa: %d\n", zaa);

        cmd.throttle = 1;
        publisher.publish(cmd);
        return ;
    }
    if (zaa % 700 == 0)
    {
        cmd.throttle = 0.1;
        zaa = 601;
    }
    else
        cmd.throttle = 0;
    bool steer = false;
    for (int i = 0; i < len; i++)
    {
        if (steer == false)
            cmd.steering = 0;
        cmd.brake = 0;
        if (data[i] == std::numeric_limits<float>::infinity())
            data[i] = 8;
        else
        {
            int a = 1;
            while (data[i] != std::numeric_limits<float>::infinity())
            {
                int tempData = static_cast<int>(roundf(data[i]));
                if ((coneCounter == 1 || coneCounter == 4) && tempData > 7 && tempData <= 8)
                {
                    steer = true;
                    if (coneCounter == 1)
                        cmd.steering = -1;
                    else
                        cmd.steering = 1;
                }
                if ((coneCounter == 1 || coneCounter == 4) && tempData < 7)
                    steer = false;
                // if (i <= (len / 2))
                //     ROS_INFO("LEFT   --  Cone %d : Laser %d :  %f", coneCounter, a, data[i]);
                // else
                //     ROS_INFO("RIGHT  --  Cone %d : Laser %d :  %f", coneCounter, a, data[i]);
                i++;
                a++;
            }
            coneCounter++;
        }
        publisher.publish(cmd);
    }
}

int main(int ac, char **av) {

    zaa = 0;

  ros::init(ac, av, "v1", 0);

  ros::NodeHandle nodeHandle;

  ros::Subscriber subscriber;


  subscriber = nodeHandle.subscribe("scan", 1000, my_func);

  //ros::Rate rateHz(10);

  //publisher = nodeHandle.advertise<std_msgs::String>("Anything", 1000);
  //publisher = nodeHandle.advertise<fs_msgs::ControlCommand>("/fsds/control_command", 1);
  ros::spin();

}
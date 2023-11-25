#include "ros/init.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <fs_msgs/ControlCommand.h>

int main(int ac, char **av) {

  ros::init(ac, av, "pub_script", 0);

  //std_msgs::String message;
  ros::NodeHandle nodeHandle;
  fs_msgs::ControlCommand cmd = fs_msgs::ControlCommand();
  cmd.header.stamp = ros::Time::now();

  ros::Publisher publisher;

  ros::Rate rateHz(10);

  //publisher = nodeHandle.advertise<std_msgs::String>("Anything", 1000);
  publisher = nodeHandle.advertise<fs_msgs::ControlCommand>("/fsds/control_command", 1);
  // döngünün koşulu olan "ok()" methodu eğer döngü sonlanmadıysa veya ros
  // konusu hala yayındaysa "true" döner ve çalışmaya devam eder.
  int key;
  int tour;
  tour = 0;
  while (ros::ok()) {
    ROS_INFO("Calisiyorum!");
    key = getchar();
    if (key == 65 || key == 97)
    {
        ROS_INFO("Pressed A!");
        cmd.steering -= 1;
    }
    else if (key == 68 || key == 100)
    {
        ROS_INFO("Pressed D!");
        cmd.steering += 1;
    }
    else if (key == 87 || key == 119)
    {
        ROS_INFO("Pressed W!");
        if (cmd.throttle == 0)
            cmd.throttle += 0.2;
    }
    else if (key == 83 || key == 115)
    {
        ROS_INFO("Pressed S!");
        if (cmd.brake == 1)
            cmd.brake -= 1;       
    }
    publisher.publish(cmd);
    tour++;
    if (tour % 2 == 0)
    {
        cmd.brake = 0;
        cmd.throttle = 0;
        cmd.steering = 0;
        tour = 0;
    }
    rateHz.sleep();
    ros::spinOnce();
  }
}
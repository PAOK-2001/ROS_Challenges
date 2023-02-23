#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <ros/console.h>
#include "week1_signal_nodes/signal_msg.h"


int main(int argc, char* argv[]){
  ros::init(argc, argv, "signal_generator");
  ros::NodeHandle handler1 , handler2;
  ros::Publisher signalPub = handler1.advertise<week1_signal_nodes::signal_msg>("/signal", 20);
  ros::Rate rate(20); // ROS Rate at 20Hz

  week1_signal_nodes::signal_msg output;
  output.time_x = 0;
  while (ros::ok()) {
        output.signal_y = tan(pow(log(1/exp(sin(output.time_x))),3))+cos(output.time_x);
        ROS_INFO_STREAM("Sending " << output.signal_y);
        output.time_x+=0.05;
        signalPub.publish(output);
        rate.sleep();
    }
}

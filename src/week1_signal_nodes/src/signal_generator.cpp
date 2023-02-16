#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <ros/console.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "signal_generator");
  ros::NodeHandle handler1 , handler2;
  ros::Publisher signalPub = handler1.advertise<std_msgs::Float32>("/signal", 20);
  ros::Publisher timePub = handler2.advertise<std_msgs::Float32>("/time", 20);
  ros::Rate rate(20); // ROS Rate at 20Hz

  std_msgs::Float32  x;
  x.data = 0;
  std_msgs::Float32 signal;
  while (ros::ok()) {
        signal.data = tan(pow(log(1/exp(sin(x.data))),3))+cos(x.data);
        ROS_INFO_STREAM("Sending " << signal.data);
        x.data+=0.05;
        signalPub.publish(signal);
        timePub.publish(x);
        rate.sleep();
    }
}

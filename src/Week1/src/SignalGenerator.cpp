#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <ros/console.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "signal_generator");
  ros::NodeHandle handler;
  ros::Publisher publisher = handler.advertise<std_msgs::Float32>("/signal", 10);
  ros::Rate rate(20); // ROS Rate at 10Hz

  float x = 0;
  std_msgs::Float32 signal;
  while (ros::ok()) {
        signal.data = tan(pow(log(1/exp(sin(x))),3))+cos(x);
        ROS_INFO_STREAM("Sending " << signal.data);
        publisher.publish(signal);
        x+=0.1;
        rate.sleep();
    }
}

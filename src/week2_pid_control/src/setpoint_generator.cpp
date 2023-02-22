#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <ros/console.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "setpoint_generator");
  ros::NodeHandle handler;
  double time = ros::Time::now().toSec();
  while (ros::ok()) {
    time = ros::Time::now().toSec();
    double ref = abs(13*tan(pow(log(1/exp(sin(time))),3))+cos(time));
    ros::param::set("/set_point",ref);
  }

}
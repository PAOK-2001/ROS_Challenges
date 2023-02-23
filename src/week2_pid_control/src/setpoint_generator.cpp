#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <ros/console.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "setpoint_generator");
  ros::NodeHandle handler;
  ros::Publisher setpointPublisher = handler.advertise<std_msgs::Float32>("/setpoint", 10);
  double time = ros::Time::now().toSec();
  
  int nodeRate = 1000;
  ros::param::get("/system_node_rate", nodeRate);
  ros::Rate rate(nodeRate); 

  while (ros::ok()) {
    time = ros::Time::now().toSec();
    float refVal = abs(3*tan(pow(log(1/exp(sin(time*0.3))),3))+cos(time*0.3));
    std_msgs::Float32 ref;
    ref.data = refVal;
    setpointPublisher.publish(ref);
    rate.sleep();
  }

}
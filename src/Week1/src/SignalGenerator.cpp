#include <ros/ros.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "signal_processor");
  ros::NodeHandle handler;
  ros::Publisher publisher = handler.advertise<std_msgs::Int32>("/signal", 10);
  ros::Rate rate(10); // ROS Rate at 10Hz

  while (ros::ok()) {
        std_msgs::Int32 signal;
        signal.data = 3;
        publisher.publish(signal);
        rate.sleep();
    }
}
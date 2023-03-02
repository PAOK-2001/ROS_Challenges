#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <cmath>

float pwm;
double _time = 0.0;

enum CMD {
    none,
    step,
    square,
    sine
};

void receive_command(const std_msgs::Int16 &msg) {
  switch (msg.data) {
    case step:
        pwm = _time > 0 ? 1 : 0;
        break;
    case square:
        pwm = std::fmod(_time,2) > 1 ? 1 : -1;
        break;
    case sine:
        pwm = sin(_time);
        break;
  }
}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "sender");
    ros::NodeHandle handler;
    ros::NodeHandle handler2;
    int nodeRate = 1000;
    ros::Subscriber cmdSub = handler.subscribe("/input", 10, receive_command);
    ros::Publisher signalPub = handler2.advertise<std_msgs::Float32>("/pwm",10);
    ros::Rate rate(nodeRate);
    std_msgs::Float32 pwmOut;
    pwmOut.data = 0.0;
    while (ros::ok()) {
        _time = ros::Time::now().toSec();
        pwmOut.data = pwm;
        signalPub.publish(pwmOut);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

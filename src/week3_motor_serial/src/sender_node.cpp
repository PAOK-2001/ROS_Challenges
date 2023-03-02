#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <cmath>

float pwm;
double time = 0.0;

enum CMD {
    none,
    step,
    square,
    sine
}

void receive_command(const std_msgs::Int16 &msg) {
  switch (msg.data) {
    case CMD.step:
        pwm = time > 0 ? 1 : 0;
        break;
    case CMD.square:
        pwm = time % 2 ? 1 : -1;
        break;
    case CMD.sine:
        pwm = sin(time);
        break;
  }
}

int main(int argc, char const *argv[]) {
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
        time = ros::Time::now().toSec();
        pwmOut.data = pwm;
        signalPub.publish(pwmOut);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

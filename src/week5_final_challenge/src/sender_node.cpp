#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <cmath>

float pwm = 0.0;
double _time = 0.0;

enum CMD {
    none,
    step,
    square,
    sine
};

void define_command(int command) {
  switch (command) {
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
    int nodeRate = 100;
    ros::Publisher signalPub = handler.advertise<std_msgs::Float32>("/pwm",10);
    ros::Rate rate(nodeRate);
    std_msgs::Float32 pwmOut;
    int cmd = 0;
    pwmOut.data = 0.0;
    while (ros::ok()) {
        ros::param::get("/pwm_type", cmd);
        _time = ros::Time::now().toSec();
        define_command(cmd);
        pwmOut.data = pwm;
        signalPub.publish(pwmOut);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

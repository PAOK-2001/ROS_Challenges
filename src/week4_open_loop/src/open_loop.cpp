#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <cmath>

float pwm = 0.0;
float target_speed = 0.0;
int nodeRate = 100;

void open_loop_control(float ref) {
    pwm = ref/15.34;
}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "sender");
    ros::NodeHandle handler;
    ros::Publisher signalPub = handler.advertise<std_msgs::Float32>("/motor_input",10);
    ros::Rate rate(nodeRate);

    std_msgs::Float32 pwmOut;
    pwmOut.data = 0.0;

    while (ros::ok()) {
        ros::param::get("/ref", target_speed);
        open_loop_control(target_speed);
        pwmOut.data = pwm;
        signalPub.publish(pwmOut);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

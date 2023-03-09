#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <cmath>

float set_point = 0.0;
double _time = 0.0;
float ref = 15;
enum CMD {
    none,
    step,
    square,
    sine
};

void define_command(int command) {
  switch (command) {
    case step:
        set_point = _time > 0 ? 1 * ref : 0;
        break;
    case square:
        set_point = std::fmod(_time,2) > 1 ? 1 * ref : -1 * ref;
        break;
    case sine:
        set_point = sin(_time);
        break;
  }
}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "sender");
    ros::NodeHandle handler;
    int nodeRate = 100;
    ros::Publisher signalPub = handler.advertise<std_msgs::Float32>("/set_point",10);
    ros::Rate rate(nodeRate);
    std_msgs::Float32 set_point_out;
    int cmd = 0;
    set_point_out.data = 0.0;
    while (ros::ok()) {
        ros::param::get("/sp_type", cmd);
        _time = ros::Time::now().toSec();
        define_command(cmd);
        set_point_out.data = set_point;
        signalPub.publish(set_point_out);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

#include <ros/ros.h>
#include <ros/console.h>
#include <iostream>
#include <std_msgs/Int16.h>

int input;

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "input");
    ros::NodeHandle handler;
    int nodeRate = 1000;
    ros::Publisher commandOutput = handler.advertise<std_msgs::Int16>("/input",10);
    ros::Rate rate(nodeRate);
    std_msgs::Int16 commandOut;
    while (ros::ok()) {
        std::cout << "Type 1 for step signal, 2 for square or 3 for sine." << std::endl;
        std::cin >> input;
        commandOut.data = input % 4; // No funny business
        commandOutput.publish(commandOut);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}


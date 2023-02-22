#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <cmath>
#include <string>
#include <iostream>
#include "week2_pid_control/motor_input.h"
#include "week2_pid_control/motor_output.h" 
//#include "week2_pid_control/set_point.h"

float motor_out;

void receive_feedback(const week2_pid_control::motor_output::ConstPtr &msg) {
  motor_out = msg->output;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "controller_node");
  ros::NodeHandle handler;
  ros::NodeHandle handler2;
  int nodeRate = 20;
  float Kp, Ti, Td;
  ros::param::get("/system_node_rate", nodeRate);
  ros::param::get("/Kp",Kp);
  ros::param::get("/Ti",Ti);
  ros::param::get("/Td",Td);
  
  ros::Subscriber systemFeedback = handler.subscribe("/motor_output", 10, receive_feedback);
  ros::Publisher controllerOutput = handler2.advertise<week2_pid_control::motor_input>("/motor_input", 10);
  ros::Rate rate(nodeRate); 
  double time = ros::Time::now().toSec();
  float error, rateError, dTime;
  float lastError = 0.0;
  float cumError = 0.0;
  float ref;
  week2_pid_control::motor_input motor_in;
  motor_in.input = 0.0;
  controllerOutput.publish(motor_in);
  while (ros::ok()) {
        ros::param::get("/set_point",ref);
        dTime = ros::Time::now().toSec()- time;
        time = ros::Time::now().toSec();

        if(dTime > 0) {
          error = ref - motor_out;
          cumError += error*dTime;
          rateError = (error - lastError) / dTime;
          lastError = error;
          motor_in.input = Kp*error+Td*rateError+Ti*cumError;
          controllerOutput.publish(motor_in);
          //std::cout << motor_out << std::endl;
        }
        ros::spinOnce();
        rate.sleep();
    }
}
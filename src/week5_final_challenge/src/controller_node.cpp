#include <cmath>
#include <string>
#include <iostream>
#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include "week5_final_challenge/motor_input.h"
#include "week5_final_challenge/motor_output.h" 

float motor_out = 0, ref = 0;
bool motor_init = false;

void receive_feedback(const week2_pid_control::motor_output::ConstPtr &msg) {
  motor_out = msg->output;
  motor_init = true;
}

void receive_setpoint(const std_msgs::Float32::ConstPtr &setpoint) {
  ref = setpoint->data;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "controller_node");
  ros::NodeHandle handler;
  ros::NodeHandle handler2;
  int nodeRate = 1000;
  float Kp, Ti, Td;
  ros::param::get("/Kp",Kp);
  ros::param::get("/Ti",Ti);
  ros::param::get("/Td",Td);
  ros::param::get("/set_point",ref);
  
  ros::Subscriber systemFeedback = handler.subscribe("/motor_output", 10, receive_feedback);
  ros::Publisher controllerOutput = handler2.advertise<week2_pid_control::motor_input>("/motor_input", 10);
  ros::Subscriber setPointSubscr = handler.subscribe("/setpoint", 10, receive_setpoint);
  ros::Rate rate(nodeRate); 
  double time = ros::Time::now().toSec();
  float error, rateError, dTime;
  float lastError = 0.0;
  float cumError = 0.0;
  week2_pid_control::motor_input motor_in;
  motor_in.input = 0.0;
  controllerOutput.publish(motor_in);
  while (ros::ok()) {
    
        dTime = ros::Time::now().toSec()- time;
        if(dTime > 0 && motor_init) {
          time = ros::Time::now().toSec();
          error = ref - motor_out;
          cumError += error*dTime;
          rateError = (error - lastError) / dTime;
          lastError = error;
          motor_in.input = (Kp*error)+(Td*rateError)+(Ti*cumError);
          controllerOutput.publish(motor_in);
        } else {
          week2_pid_control::motor_input motor_in;
          motor_in.input = 0;
          controllerOutput.publish(motor_in);
        }
        ros::spinOnce();
        rate.sleep();
    }
}
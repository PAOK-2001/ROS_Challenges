#include <cmath>
#include <string>
#include <iostream>
#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include "week5_final_challenge/motor_input.h"
#include "week5_final_challenge/motor_output.h"
#include "week5_final_challenge/sender.h"

float motor_out = 0, ref = 0;
bool motor_init = false;

void receive_feedback(const week5_final_challenge::motor_output::ConstPtr &msg) {
  motor_out = msg->output;
  motor_init = true;
}

void receive_setpoint(const week5_final_challenge::sender::ConstPtr &setpoint) {
  ref = setpoint->set_point_data;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "controller_node");
  ros::NodeHandle handler;
  ros::NodeHandle handler2;
  int nodeRate = 100;
  float Kp, Ti, Td;
  ros::param::get("/Kp",Kp);
  ros::param::get("/Ti",Ti);
  ros::param::get("/Td",Td);
  
  ros::Subscriber systemFeedback = handler.subscribe("/motor_output", 10, receive_feedback);
  ros::Publisher controllerOutput = handler2.advertise<week5_final_challenge::motor_input>("/motor_input", 10);
  ros::Subscriber setPointSubscr = handler.subscribe("/set_point", 10, receive_setpoint);
  ros::Rate rate(nodeRate); 
  double _time = ros::Time::now().toSec();
  float error, rateError, dTime;
  float lastError = 0.0;
  float cumError = 0.0;
  week5_final_challenge::motor_input motor_in;
  motor_in.input = 0.0;
  controllerOutput.publish(motor_in);
  while (ros::ok()) {
      if(motor_init){
        dTime = ros::Time::now().toSec()- _time;
        if(dTime > 0) {
          error = ref - motor_out;
          cumError += error*dTime;
          rateError = (error - lastError) / dTime;
          lastError = error;
          motor_in.input = (Kp*error)+(Td*rateError)+(Ti*cumError);
          if(abs(motor_in.input)>1){
            motor_in.input = motor_in.input/abs(motor_in.input);
          }
          controllerOutput.publish(motor_in);
        } else {
          week5_final_challenge::motor_input motor_in;
          motor_in.input = 0;
          controllerOutput.publish(motor_in);
        }
      }
      _time = ros::Time::now().toSec();
      ros::spinOnce();
      rate.sleep();
    }
}
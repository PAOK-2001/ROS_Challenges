#include <ros.h>
#include <Encoder.h>
#include <std_msgs/Float32.h>

#define FWD_PIN_B 18
#define BWD_PIN_B 15
#define PWM_B 4

#define ENCR_A 34
#define ENCR_B 36

std_msgs::Float32 encB_vel;

long currentPos = -999;
long oldPos = 0;

ros::NodeHandle nh;

Encoder encR(ENCR_A,ENCR_B);

void pwmCallback(const std_msgs::Float32 &pwmMsg){
  ledcWrite(0, abs((int)(pwmMsg.data*255)));
  if (pwmMsg.data > 0) {
    digitalWrite(FWD_PIN_B, 1);
    digitalWrite(BWD_PIN_B, 0);
  } else {
    digitalWrite(FWD_PIN_B, 0);
    digitalWrite(BWD_PIN_B, 1);
  }
}

void calculateSpeed(int newPos){
  if (newPos != currentPos){
    currentPos = newPos;
  }
  encB_vel.data = (currentPos - oldPos)/(POLLING_TIME);
}

ros::Subscriber<std_msgs::Float32> pwm_receiver("/pwm", pwmCallback);
ros::Publisher motor_velocity("/motor_output", &encB_vel);

void setup() {
  ledcSetup(0, 980, 8);
  pinMode(FWD_PIN_B, OUTPUT);
  pinMode(BWD_PIN_B, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  
  ledcAttachPin(PWM_B, 0);
  nh.initNode();
  nh.subscribe(pwm_receiver);
  nh.advertise(motor_velocity);
}

void loop() {
  calculateSpeed(encR.read());
  motor_velocity.publish(&encB_vel);
  nh.spinOnce();
  delay(1);
}

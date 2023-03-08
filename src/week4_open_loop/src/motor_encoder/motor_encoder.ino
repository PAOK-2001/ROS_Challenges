#include <ros.h>
#include <Encoder.h>
#include <std_msgs/Float32.h>

#define FWD_PIN_B 18
#define BWD_PIN_B 15
#define PWM_B 4

#define ENCA_B 34
#define ENCB_B 36

float encbPos = 0;
float newPos  = 0;
float oldPos  = 0;
float newTime = 0;
float oldTime = 0;
std_msgs::Float32 encB_vel;

ros::NodeHandle nh;

void pwmCallback(const std_msgs::Float32 &pwmMsg) {
  ledcWrite(0, abs((int)(pwmMsg.data*255)));
  if (pwmMsg.data > 0) {
    digitalWrite(FWD_PIN_B, 1);
    digitalWrite(BWD_PIN_B, 0);
  } else {
    digitalWrite(FWD_PIN_B, 0);
    digitalWrite(BWD_PIN_B, 1);
  }
}

std_msgs::Float32 calculateSpeed(){
  //TODO measure encoder pulses
  std_msgs::Float32 speed;
  return speed;


}

ros::Subscriber<std_msgs::Float32> pwm_receiver("/pwm", pwmCallback);
ros::Publisher motor_velocity("/motor_output", &encB_vel);

void setup() {
  ledcSetup(0, 980, 8);
  pinMode(FWD_PIN_B, OUTPUT);
  pinMode(BWD_PIN_B, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  pinMode(ENCA_B, INPUT_PULLUP);
  pinMode(ENCB_B, INPUT_PULLUP);

  ledcAttachPin(PWM_B, 0);
  nh.initNode();
  nh.subscribe(pwm_receiver);
  nh.advertise(motor_velocity);
}

void loop() {
  encB_vel = calculateSpeed();
  motor_velocity.publish(&encB_vel);
  nh.spinOnce();
  delay(1);
}

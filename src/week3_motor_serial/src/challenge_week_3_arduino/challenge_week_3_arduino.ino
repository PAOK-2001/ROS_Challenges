#include <ros.h>
#include <std_msgs/Float32.h>
#define FWD_PIN 18
#define BWD_PIN 15
#define PWM 4

ros::NodeHandle nh;

void pwmCallback(const std_msgs::Float32 &pwmMsg) {
  ledcWrite(0, abs((int)(pwmMsg.data*255)));
  if (pwmMsg.data > 0) {
    digitalWrite(FWD_PIN, 1);
    digitalWrite(BWD_PIN, 0);
  } else {
    digitalWrite(FWD_PIN, 0);
    digitalWrite(BWD_PIN, 1);
  }
}

ros::Subscriber<std_msgs::Float32> sub("/pwm", pwmCallback);

void setup() {
  ledcSetup(0, 980, 8);
  pinMode(FWD_PIN, OUTPUT);
  pinMode(BWD_PIN, OUTPUT);
  pinMode(PWM, OUTPUT);
  ledcAttachPin(PWM, 0);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
  delay(1);
}

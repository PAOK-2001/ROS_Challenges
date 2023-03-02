#include <ros.h>
#include <std_msgs/Float32.h>
#define FWD_PIN 1
#define BWD_PIN 2

void pwmCallback(const std_msgs::Float32 &pwmMsg) {
  if (pwmMsg.data > 0) {
    ledcWrite(0, (int)(pwmMsg.data*255));
    ledcWrite(1, 0);
  } else {
    ledcWrite(0, 0);
    ledcWrite(1, (int)(pwmMsg.data*255));
  }
  delay(15);
}

ros::NodeHandle nh;
ros::Subscriber<std_msgs::UInt8> sub("/pwm", &pwmCallback);

void setup() {
  nh.initNode();
  nh.subscribe(sub);
  ledcSetup(0, 980, 8);
  ledcSetup(1, 980, 8);
  ledcAttachPin(FWD_PIN, 0);
  ledcAttachPin(BWD_PIN, 0);
}

void loop() {
  nh.spinOnce();
  delay(1);
}

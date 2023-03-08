#include <ESP32Encoder.h>
#include <ros.h>
#include <std_msgs/Float32.h>

#define FWD_PIN_B 18
#define BWD_PIN_B 15
#define PWM_B 4

#define ENCR_A 34
#define ENCR_B 36
#define PI 3.14159265358979

const int    POLLING_TIME  = 50;
const double TICKS_PER_REV = 22;
const double RADS_PER_TICK = (2*PI)/TICKS_PER_REV;

std_msgs::Float32 encB_vel;

long currentPos = -999;
long oldPos = 0;
long currTime = millis();

ros::NodeHandle nh;

ESP32Encoder encR;

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
  Serial.println(newPos);
  if (newPos != currentPos){
    currentPos = newPos;
  }
  encB_vel.data = RADS_PER_TICK*1000*((currentPos - oldPos)/POLLING_TIME); // Rads per second
  oldPos = currentPos;
}

ros::Subscriber<std_msgs::Float32> pwm_receiver("/motor_input", pwmCallback);
ros::Publisher motor_velocity("/motor_output", &encB_vel);

void setup() {
  // Encoder initial setup
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encR.attachHalfQuad(ENCR_A, ENCR_B);
  encR.setCount(0);
  encR.clearCount();
  // MOTOR_RIGHT config
  ledcSetup(0, 980, 8);
  pinMode(FWD_PIN_B, OUTPUT);
  pinMode(BWD_PIN_B, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  // ROS Init
  ledcAttachPin(PWM_B, 0);
  nh.initNode();
  nh.subscribe(pwm_receiver);
  nh.advertise(motor_velocity);
}

void loop() {
  if(millis()- currTime >= POLLING_TIME){
    calculateSpeed(encR.getCount());
    motor_velocity.publish(&encB_vel);
    currTime = millis();
  }
  nh.spinOnce();
  delay(200);
}

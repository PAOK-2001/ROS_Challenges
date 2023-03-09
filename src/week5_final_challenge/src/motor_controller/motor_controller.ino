#include <ESP32Encoder.h>
#include <ros.h>
#include <week5_final_challenge/motor_output.h>
#include <week5_final_challenge/motor_input.h>

#define FWD_PIN_B 18
#define BWD_PIN_B 15
#define PWM_B 4

#define ENCR_A 36
#define ENCR_B 34
#define PI 3.14159265358979

const int    POLLING_TIME  = 5;
const double TICKS_PER_REV = 737;
const double RADS_PER_TICK = (2*PI)/TICKS_PER_REV;

week5_final_challenge::motor_output encB_vel;

long currentPos = -999;
long oldPos = 0;
long currTime = millis();

ros::NodeHandle nh;

ESP32Encoder encR;

void pwmCallback(const week5_final_challenge::motor_input &pwmMsg){
  ledcWrite(0, abs((int)(pwmMsg.input*255)));
  if (pwmMsg.input > 0) {
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
  encB_vel.output = (((RADS_PER_TICK*(currentPos - oldPos))*1000)/POLLING_TIME); // Rads per second
  oldPos = currentPos;
}

ros::Subscriber<week5_final_challenge::motor_input> pwm_receiver("/motor_input", pwmCallback);
ros::Publisher motor_velocity("/motor_output", &encB_vel);

void setup() {
  week5_final_challenge::motor_ouput motor_out;
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
}

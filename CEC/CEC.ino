#include <Servo.h>

#define motor_left_1A 8
#define motor_left_2A 9
#define enable_left   7

#define motor_right_1A 10
#define motor_right_2A 11
#define enable_right    6

#define time_unit 500

#define e_magnet 2
#define arm_pin 3

Servo arm;

void Engine_Enable()
{
  digitalWrite(enable_left, HIGH);
  digitalWrite(enable_right, HIGH);
}

void Motor_Disable()
{
  digitalWrite(enable_left, LOW);
  digitalWrite(enable_right, LOW);
}

void Left_Motor_Fwd()
{
  digitalWrite(motor_left_1A, LOW);
  digitalWrite(motor_left_2A, HIGH);
}

void Right_Motor_Fwd()
{
  digitalWrite(motor_right_1A, LOW);
  digitalWrite(motor_right_2A, HIGH);
}

void Left_Motor_Back()
{
  digitalWrite(motor_left_1A, HIGH);
  digitalWrite(motor_left_2A, LOW);
}

void Right_Motor_Back()
{
  digitalWrite(motor_right_1A, HIGH);
  digitalWrite(motor_right_2A, LOW);
}

void V_Forward()
{
  Engine_Enable();
  Left_Motor_Fwd();
  Right_Motor_Fwd();
}

void V_Back()
{
  Engine_Enable();
  Left_Motor_Back();
  Right_Motor_Back();
}

void V_Stop()
{
  Engine_Enable();
  digitalWrite(motor_left_1A, HIGH);
  digitalWrite(motor_left_2A, HIGH);
  digitalWrite(motor_right_1A, HIGH);
  digitalWrite(motor_right_2A, HIGH);
  delay(time_unit);
}

void Turn_Right()
{
  Engine_Enable();
  Left_Motor_Fwd();
  Right_Motor_Back();
}

void Turn_Left()
{
  Engine_Enable();
  Left_Motor_Back();
  Right_Motor_Fwd();
}

void grab()
{
  arm.write(0);
  delay(time_unit);
  digitalWrite(e_magnet, HIGH);
  delay(time_unit);
  arm.write(90);
  delay(time_unit);
}

void release()
{
  arm.write(0);
  delay(time_unit);
  digitalWrite(e_magnet, LOW);
  arm.write(180);
}

void setup() {

  pinMode(motor_left_1A, OUTPUT);
  pinMode(motor_left_2A, OUTPUT);
  pinMode(enable_left, OUTPUT);

  pinMode(motor_right_1A, OUTPUT);
  pinMode(motor_right_2A, OUTPUT);
  pinMode(enable_right, OUTPUT);

  pinMode(e_magnet, OUTPUT);
  arm.attach(arm_pin);
}

void loop() {
  V_Forward();
  delay(time_unit*2);
  V_Stop();
  grab();  
  V_Back();
  delay(time_unit*2);
  V_Stop();    
  release();
  delay(time_unit*4);
}


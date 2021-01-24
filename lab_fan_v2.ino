#include "Nextion.h"


int FAN1 = 10;
int FAN2 = 11;
int FAN3 = 12;
int PWM3 = 3;
int PWM6 = 6;
int PWM5 = 5;
int val = 0;
int valFan;
int f1 = 0;
int f2 = 0;
int f3 = 0;

NexDSButton bt1 = NexDSButton(0, 6, "btn_fan1");  // Dual state button added
NexDSButton bt2 = NexDSButton(0, 7, "btn_fan2");
NexDSButton bt3 = NexDSButton(0, 8, "btn_fan3");

NexTouch *nex_listen_list[] =
{
  &bt1,
  &bt2,
  &bt3,
  NULL  // String terminated
};

void bt1PopCallback(void *ptr)  // Release event for dual state button bt1
{
  uint32_t number1 = 0;  // Create variable to store value we are going to get
  bt1.getValue(&number1);  // Read value of dual state button to know the state (0 or 1)
  f1 = number1;
  if (number1 == 1) {

    digitalWrite(FAN1, HIGH);
  } else {
    analogWrite(FAN1, 0);

  }
}  // End of release event
void bt2PopCallback(void *ptr)
{
  uint32_t number2 = 0;
  bt2.getValue(&number2);
  f2 = number2;
  if (number2 == 1) {

    digitalWrite(FAN2, HIGH);
  } else {

    digitalWrite(FAN2, LOW);
  }
}
void bt3PopCallback(void *ptr)
{
  uint32_t number3 = 0;
  bt3.getValue(&number3);
  f3 = number3;
  if (number3 == 1) {
    digitalWrite(FAN3, HIGH);
  } else {
    digitalWrite(FAN3, LOW);
  }
}


void setPage0SpeedPicMotor(int _val) {
  int val = map(_val, 0, 120, 10, 65);
  //Serial.println("SpeedMotor = "+String(val));
  if (val >= 11 && val <= 61) {
    Serial.print("pic_smotor.pic=" + String(val));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }
}
void setPage0PicAnimationFan(int _val) {
  int val = map(_val, 0, 120, 420, 20);
  if (val >= 50 && val <= 500) {
    //Serial.println("AnimationFan = " + String(val));
    Serial.print("page0.val_sfan.val=" + String(val));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }
}
void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.print("baud=115200");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.end();

  Serial.begin(115200);

  bt1.attachPop(bt1PopCallback);
  bt2.attachPop(bt2PopCallback);
  bt3.attachPop(bt3PopCallback);
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);
  pinMode(FAN3, OUTPUT);

  pinMode(PWM3, OUTPUT);
  pinMode(PWM6, OUTPUT);
  pinMode(PWM5, OUTPUT);
}

void loop() {

  val = analogRead(A0);
  valFan = val;
  val = map(val, 0, 1023, 0, 120);
  valFan = map(valFan, 0, 1023, 0, 255);

  setPage0SpeedPicMotor(val);
  setPage0PicAnimationFan(val);
  if (f1 == 1) {
    analogWrite(PWM6, valFan);
  } else {
    analogWrite(PWM6, 0);
  }
  if (f2 == 1) {
    analogWrite(PWM5, valFan);
  } else {
    analogWrite(PWM5, 0);
  }
  if (f3 == 1) {
    analogWrite(PWM3, valFan);
  } else {
    analogWrite(PWM3, 0);
  }

  nexLoop(nex_listen_list);

}

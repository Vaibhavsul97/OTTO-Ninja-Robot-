#include <Servo.h>
 
#include "ESP8266WiFi.h"
#define REMOTEXY_SERVER_PORT 6377
 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <RemoteXY.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] ={ 255,6,0,0,0,66,0,13,8,0,5,32,3,12,41,41,1,26,31,1,3,79,16,16,12,1,31,82,240,159,166,190,0,1,3,56,39,18,12,1,31,240,159,146,191,0,1,3,79,39,17,12,1,31,240,159,166,191,0,1,3,56,16,17,12,1,31,76,240,159,166,190,0 };
struct {
int8_t J_x; // =-100..100 x-coordinate joystick position
int8_t J_y; // =-100..100 y-coordinate joystick position
uint8_t button_B; // =1 if button pressed, else =0
uint8_t button_X; // =1 if button pressed, else =0
uint8_t button_Y; // =1 if button pressed, else =0
uint8_t button_A; // =1 if button pressed, else =0
uint8_t connect_flag;  // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)
 
 
int currentmillis1 = 0;
int currentmillis2 = 0;
int currentmillis3 = 0;
int ModeCounter = 0;
 
 
const uint8_t ServoLeftLegPin =D8; // D8 or 15
const uint8_t ServoRightLegPin=D4; // D4 or 2
const uint8_t ServoLeftFootPin  =D7; // D7 or 13
const uint8_t ServoRightFootPin =D3; // D3 or 0
const uint8_t ServoLeftArmPin =D0; // D0 or 16
const uint8_t ServoRightArmPin =3; // RX or 3
const uint8_t ServoHeadPin   =1; // TX or 1
Servo myservoLeftLeg;
Servo myservoLeftFoot;
Servo myservoRightFoot;
Servo myservoRightLeg;
Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;
 
int LA0= 60 +0; // Left Leg standing Position; 0 = Full Tilt Right   180 = Full Tilt Left    Default = 60
int RA0= 120 +0;// Right Leg standing position; 0 = Full Tilt Right   180 = Full Tilt Left    Default = 120
int LA1= 180; // Left Leg roll Position; 0 = Full Tilt Right   180 = Full Tilt Left    Default = 170
int RA1= 0;  // Right Leg roll position,0 = Full Tilt Right   180 = Full Tilt Left     Default = 10
int LATL= LA0 +40;  // Left Leg tilt left walking position,0 = Full Tilt Right   180 = Full Tilt Left    Default = 90
int RATL= RA0 +60;  // Right Leg tilt left walking position,0 = Full Tilt Right   180 = Full Tilt Left    Default = 180
int LATR= LA0 -60;  // Left Leg tilt right walking position,0 = Full Tilt Right   180 = Full Tilt Left     Default = 0
int RATR= RA0 -40;  // Right Leg tilt right walking position,0 = Full Tilt Right   180 = Full Tilt Left     Default = 90
 
int LFFWRS=20; // Left foot forward walking rotation Speed; 0 = Slowest  90 = Fastest    Default = 20
int RFFWRS=20 ; // Right foot forward walking rotation Speed; 0 = Slowest  90 = Fastest    Default = 20
int LFBWRS= 20; // Left foot Backward walking rotation Speed; 0 = Slowest  90 = Fastest    Default = 20
int RFBWRS= 20; // Right foot Backward walking rotation Speed; 0 = Slowest  90 = Fastest    Default = 20
 
long ultrasound_distance_simple() {
   long duration, distance;
   digitalWrite(D1,LOW);
   delayMicroseconds(2);
   digitalWrite(D1, HIGH);
   delayMicroseconds(10);
   digitalWrite(D1, LOW);
   duration = pulseIn(D2, HIGH);
   distance = duration/58;
   return distance;
}
 
#define REMOTEXY_WIFI_SSID "MyBipeadNinja"
#define REMOTEXY_WIFI_PASSWORD "12345678"
 
void NinjaStop()
 {
myservoLeftFoot.detach();
 myservoRightFoot.detach();
 myservoLeftLeg.detach();
 myservoRightLeg.detach();
}
void NinjaSetWalk()
 {
myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoLeftArm.write(90);
  myservoRightArm.write(90);
 delay(200);
 myservoLeftArm.detach();
 myservoRightArm.detach();
 myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
myservoLeftLeg.write(LA0);
myservoRightLeg.write(RA0);
 delay(300);
 myservoLeftLeg.detach();
 myservoRightLeg.detach();
 myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
myservoRightArm.attach(ServoRightArmPin, 544, 2400);
 myservoLeftArm.write(180);
 myservoRightArm.write(0);
 delay(300);
 myservoLeftArm.detach();
 myservoRightArm.detach();
}
void NinjaSetRoll()
 {
myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
myservoRightArm.attach(ServoRightArmPin, 544, 2400);
myservoLeftArm.write(90);
myservoRightArm.write(90);
 delay(200);
 myservoLeftArm.detach();
myservoRightArm.detach();
myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
myservoLeftLeg.write(LA1);
myservoRightLeg.write(RA1);
 delay(300);
 myservoLeftLeg.detach();
myservoRightLeg.detach();
myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
myservoRightArm.attach(ServoRightArmPin, 544, 2400);
 myservoLeftArm.write(180);
 myservoRightArm.write(0);
 delay(300);
 myservoLeftArm.detach();
 myservoRightArm.detach();
}
void NinjaWalkStop()
{
myservoLeftFoot.write(90);
 myservoRightFoot.write(90);
 myservoLeftLeg.write(LA0);
 myservoRightLeg.write(RA0);}
void NinjaRollStop()
 {
myservoLeftFoot.write(90);
 myservoRightFoot.write(90);
 myservoLeftFoot.detach();
 myservoRightFoot.detach();
}
void NinjaLeftArm()
{
myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
myservoLeftArm.write(90);
}
void NinjaRightArm()
{
myservoRightArm.attach(ServoRightArmPin, 544, 2400);
myservoRightArm.write(90);
}
void NinjaLeftArmDown()
{
myservoLeftArm.write(180);
}
void NinjaRightArmDown()
{
myservoRightArm.write(0);
}
 
void NinjaRollForward(int speed)
{
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoLeftFoot.write(90 + speed);
  myservoRightFoot.write(90 - speed);
}
void NinjaRollBackward(int speed)
{
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoLeftFoot.write(90 - speed);
  myservoRightFoot.write(90 + speed);
}
void NinjaRollLeft(int speed)
{
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoLeftFoot.write(90 - speed);
  myservoRightFoot.write(90 - speed);
}
void NinjaRollRight(int speed)
{
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
  myservoLeftFoot.write(90 + speed);
  myservoRightFoot.write(90 + speed);
}
 
void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT);
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
myservoRightArm.attach(ServoRightArmPin, 544, 2400);
myservoHead.attach(ServoHeadPin, 544, 2400);
myservoHead.write(90);
myservoLeftArm.write(90);
myservoRightArm.write(90);
delay(300);
myservoLeftFoot.write(90);
myservoRightFoot.write(90);
myservoLeftLeg.write(60);
myservoRightLeg.write(120);
delay(300);
myservoLeftArm.write(180);
myservoRightArm.write(0);
delay(500);
myservoLeftFoot.detach();
myservoRightFoot.detach();
myservoLeftLeg.detach();
myservoRightLeg.detach();
myservoLeftArm.detach();
myservoRightArm.detach();
myservoHead.detach();
RemoteXY_Init ();
 
 
}
 
void loop() {
    // This block should be use always alone for this particular example
    // after upload this code
    // 1. Install and open the mobile App https://remotexy.com/en/download/
    // 2. Press + to add new device
    // 3. Select Wifi point
    // 4. "MyOttoNinja" should be visible in the options
    // 5. Password 12345678
    RemoteXY_Handler ();
    if (RemoteXY.button_X == HIGH){NinjaSetRoll(); ModeCounter = 1;}
    if (RemoteXY.button_Y == HIGH){NinjaSetWalk(); ModeCounter = 0;}
    if (RemoteXY.button_A == HIGH){NinjaLeftArm();}
    if (RemoteXY.button_A == LOW) {NinjaLeftArmDown();}
    if (RemoteXY.button_B == HIGH){NinjaRightArm();}
    if (RemoteXY.button_B == LOW) { NinjaRightArmDown();}
    if (ModeCounter == 0) {
    if ((RemoteXY.J_x >= -10)&&(RemoteXY.J_x <= 10)&&(RemoteXY.J_y >= -10)&&(RemoteXY.J_y <= 10)) { NinjaWalkStop();  }
    if (RemoteXY.J_y > 0)  {
    int lt= map(RemoteXY.J_x, 100, -100, 200, 700);
    int rt= map(RemoteXY.J_x, 100, -100, 700, 200);
    int Interval1 = 250;
    int Interval2 = 250 + rt;
    int Interval3 = 250 + rt + 250;
    int Interval4 = 250 + rt + 250 + lt;
    int Interval5 = 250 + rt + 250 + lt + 50;
    if(millis() > currentmillis1 + Interval5){ currentmillis1 = millis();}
    if(millis() - currentmillis1 <= Interval1) {
    myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
    myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
    myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
    myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
    myservoLeftLeg.write(LATR);
    myservoRightLeg.write(RATR);}
    if((millis() - currentmillis1 >= Interval1)&&(millis() - currentmillis1 <= Interval2)){myservoRightFoot.write(90-RFFWRS);}
    if((millis() - currentmillis1 >= Interval2)&&(millis() - currentmillis1 <= Interval3)){myservoRightFoot.detach();myservoLeftLeg.write(LATL); myservoRightLeg.write(RATL);}
    if((millis() - currentmillis1 >= Interval3)&&(millis() - currentmillis1 <= Interval4)){myservoLeftFoot.write(90+LFFWRS);}
    if((millis() - currentmillis1 >= Interval4)&&(millis() - currentmillis1 <= Interval5)){myservoLeftFoot.detach();  }  }
    if (RemoteXY.J_y < 0){
    int lt= map(RemoteXY.J_x, 100, -100, 200, 700);
    int rt= map(RemoteXY.J_x, 100, -100, 700, 200);
    int Interval1 = 250;
    int Interval2 = 250 + rt;
    int Interval3 = 250 + rt + 250;
    int Interval4 = 250 + rt + 250 + lt;
    int Interval5 = 250 + rt + 250 + lt + 50;
    if(millis() > currentmillis1 + Interval5){currentmillis1 = millis();}
    if(millis() - currentmillis1 <= Interval1){
    myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
    myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
    myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
    myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
    myservoLeftLeg.write(LATR);
    myservoRightLeg.write(RATR);}
    if((millis() - currentmillis1 >= Interval1)&&(millis() - currentmillis1 <= Interval2)){myservoRightFoot.write(90+RFBWRS);}
    if((millis() - currentmillis1 >= Interval2)&&(millis() - currentmillis1 <= Interval3)){myservoRightFoot.detach();myservoLeftLeg.write(LATL); myservoRightLeg.write(RATL);}
    if((millis() - currentmillis1 >= Interval3)&&(millis() - currentmillis1 <= Interval4)){myservoLeftFoot.write(90-LFFWRS);}
    if((millis() - currentmillis1 >= Interval4)&&(millis() - currentmillis1 <= Interval5)){myservoLeftFoot.detach();  }  }}
    if (ModeCounter == 1){
    if ((RemoteXY.J_x >= -10)&&(RemoteXY.J_x <= 10)&&(RemoteXY.J_y >= -10)&&(RemoteXY.J_y <= 10)){NinjaRollStop();}
     else{myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);
    int LWS= map(RemoteXY.J_y, 100, -100, 135,    45);
    int RWS= map(RemoteXY.J_y, 100, -100,  45,   135);
    int LWD= map(RemoteXY.J_x, 100, -100,  45,     0);
    int RWD= map(RemoteXY.J_x, 100, -100,   0,   -45);
    myservoLeftFoot.write(LWS+LWD);
    myservoRightFoot.write(RWS+RWD); }}
     Serial.print("  X: ");
     Serial.print(RemoteXY.J_x);
     Serial.print("  Y: ");
     Serial.print(RemoteXY.J_y);
     Serial.print("  MC: ");
     Serial.println(ModeCounter);
    if (ultrasound_distance_simple() < 15) {
      NinjaRollBackward(map(100, 0, 100, 0, 90));
    }
 
}
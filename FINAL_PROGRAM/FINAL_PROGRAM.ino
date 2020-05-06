//#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055_t3.h>
#include <utility/imumaths.h>
#include <i2c_t3.h>
#include "config.h"

// #include <Adafruit_Sensor.h>
// #include <Mahony.h>
// #include <Madgwick.h>
// #include <Adafruit_FXAS21002C.h>
// #include <Adafruit_FXOS8700.h>
//#define NXP_FXOS8700_FXAS21002
//#define AHRS_VARIANT   NXP_FXOS8700_FXAS21002
//Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
//Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

const int lightThresh[48] = {705,660,678,650,662,649,1023,657,538,437,641,468,646,680,659,630,602,730,726,672,737,658,699,639,629,613,664,664,698,602,673,540,1023,601,629,1023,698,662,665,572,431,601,444,452,629,533,581,319};
const float ANGULAR_MULT = 0.01;
// Offsets applied to raw x/y/z mag values
const float mag_offsets[3]            = { 24.21F, -18.05F, 53.68F };
// Soft iron error compensation matrix
const float mag_softiron_matrix[3][3] = { {  0.950,  0.011,  0.011 },
                                    {  0.011,  0.964, -0.004 },
                                    {  0.011, -0.004,  1.092 } };
const float mag_field_strength        = 38.56F;
// Offsets applied to compensate for gyro zero-drift error for x/y/z
const float gyro_zero_offsets[3]      = { 0.0F, 0.0F, 0.0F };
// Mahony filter;
//Madgwick filter;

char data[200];
int FLout, FRout, BLout, BRout,
lightVals[48], lightDetect[48], lightCnt=0,
ball[3], bgoal[13], ygoal[13];  //all data is Y then X
float cmpoffset=0, cmpangle, angleError, pidAngleError, desiredAngle = 0, lastError, lastAngle,
lineAng, prevLineAng, lineLen,
ballAng, ballDist, ballOffset, ballMult,
yGoalAng, yGoalDist, bGoalAng, bGoalDist,
robotAng=0, robotBearing=0, robotSpeed=80;
bool onLine=false, prevOnLine=false, IMUjumped = false,
yGoalForward = true;  //true=yellow goal infront, false=blue goal infront
unsigned long lightTimer, compassTimer=0;

Adafruit_BNO055 bno = Adafruit_BNO055(WIRE_BUS, -1, BNO055_ADDRESS_B, I2C_MASTER, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_100, I2C_OP_MODE_ISR);
void setup() {
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000);
  //Wire.begin();
  Serial.begin(115200);
  Serial1.begin(230400);
  //init compass
  bno.begin();
  bno.setExtCrystalUse(false);

  Serial.println("inited");

  //initIMU();
  initMotors();

  pinMode(SIG_0,OUTPUT);pinMode(SIG_1,OUTPUT);pinMode(SIG_2,OUTPUT);pinMode(SIG_3,OUTPUT);
  digitalWriteFast(SIG_0,LOW);digitalWriteFast(SIG_1,LOW);digitalWriteFast(SIG_2,LOW);digitalWriteFast(SIG_3,LOW);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  /*compassTimer = millis();
  while ((millis()-compassTimer)<3000) {
    readIMU();
  }
  cmpoffset=cmpangle;*/

  /*uint8_t system, gyro, accel, mag = 0;
  while(system!=3 && gyro!=3) {
    readIMU();
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.println("c");
  }*/

  //digitalWrite(LED_BUILTIN,HIGH);
  //delay(4000);
  //digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  readIMU();
  cmpoffset = cmpangle;
  digitalWrite(LED_BUILTIN,HIGH); 
}
unsigned long lastBallTime=millis();
bool gotBall=false;

void loop() {
  if((micros()-compassTimer)>5000) {
    readIMU();
    processIMU();
    Serial.println(cmpangle);
    compassTimer=micros();
  }
  

  getRaspistr(false); 
  Serial.print("loop   ");
  if(ball[0]==0) {  //no ball
    Serial.print("no ball      ");
    angular_drive(0,0,pidAngleError);
    sendAction(OFF_FRONT_DRIB);
    sendAction(OFF_BACK_DRIB);
    Serial.println("noballs");
    /*if((millis()-lastBallTime)>1000 && !gotBall) {
      sendAction(OFF_FRONT_DRIB);
      sendAction(OFF_BACK_DRIB);
    } else {
      noBallTimer = millis();
      gotBall = false;
    }*/
    
  }
  else {  //got ball
    //lastBallTime = millis();
    //gotBall=true;
    calcBallAngle();
    calcGoalAngle();
    calcRobotAngle();
    angular_drive(80,robotAng,pidAngleError);

    if(ballAng<10 || ballAng>350) {
      sendAction(ON_FRONT_DRIB);
      sendAction(OFF_BACK_DRIB);
    }
    /*if(robotAng>165 && robotAng<195) {
      sendAction(ON_BACK_DRIB);
      sendAction(OFF_FRONT_DRIB);
    }*/
   Serial.print(angleError);
   Serial.print(" ");
  Serial.print(ballAng);
  Serial.print(" ");
   Serial.print(ballDist);
   Serial.print(" ");
   Serial.print(ballOffset*ballMult);
   Serial.print(" ");
   Serial.print(desiredAngle);
   Serial.print(" ");
  Serial.print(robotAng);
  Serial.print(" ");
  Serial.print(bGoalAng);
  Serial.print(" ");
  Serial.println(pidAngleError);
  }
  
  //line avoidance
  updateLight();
  processLight();
  processLine();
  lineAvoid();

  //angular_drive(60,0,0);
  //angular_drive(robotSpeed, robotAng, pidAngleError);
  move_OUT();
  // Serial.print(analogRead(A10));
  // Serial.print(" ");
  // Serial.println(analogRead(A11));
  //Serial.println();
  /*for(int i=0; i<360; i++) {
    readIMU();
    processIMU();
    angular_drive(60,i,pidAngleError);
    delay(3);
    move_OUT();
    //Serial.printf("%f %f \n", desiredAngle, pidAngleError);
    //Serial.printf("%d %d %d %d \n",FLout, FRout, BLout, BRout);
  }*/

  //Serial.printf("%d %d %d %d",FLout, FRout, BLout, BRout);
  //Serial.println(angleError);
}

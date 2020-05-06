void angular_drive(int speed, float angle, float angVel) {
  float radAng = angle*pi/180;

  float x_co = sinf(radAng)*0.707;  //all robot vectors are +-(1/sqrt2)
  float y_co = cosf(radAng)*0.707;

  float fl=(x_co+y_co + ANGULAR_MULT * angVel);
  float bl=(-x_co+y_co + ANGULAR_MULT * angVel);
  float fr=-(x_co-y_co + ANGULAR_MULT * angVel);
  float br=-(-x_co-y_co + ANGULAR_MULT * angVel);

  FLout = round(fl*speed);
  FRout = round(fr*speed);
  BLout = round(bl*speed);
  BRout = round(br*speed);
}

void move_OUT() {
  analogWrite(FL_PWM,abs(FLout));
  digitalWrite(FL_A, (FLout>0 ? HIGH : LOW));
  digitalWrite(FL_B, (FLout<0 ? HIGH : LOW));

  analogWrite(FR_PWM,abs(FRout));
  digitalWrite(FR_A, (FRout>0 ? HIGH : LOW));
  digitalWrite(FR_B, (FRout<0 ? HIGH : LOW));

  analogWrite(BL_PWM,abs(BLout));
  digitalWrite(BL_A, (BLout>0 ? HIGH : LOW));
  digitalWrite(BL_B, (BLout<0 ? HIGH : LOW));

  analogWrite(BR_PWM,abs(BRout));
  digitalWrite(BR_A, (BRout>0 ? HIGH : LOW));
  digitalWrite(BR_B, (BRout<0 ? HIGH : LOW));
}

void initMotors() {
  pinMode(FL_PWM,OUTPUT);
  pinMode(FR_PWM,OUTPUT);
  pinMode(BL_PWM,OUTPUT);
  pinMode(BR_PWM,OUTPUT);
  pinMode(FL_A,OUTPUT);
  pinMode(FR_A,OUTPUT);
  pinMode(BL_A,OUTPUT);
  pinMode(BR_A,OUTPUT);
  pinMode(FL_B,OUTPUT);
  pinMode(FR_B,OUTPUT);
  pinMode(BL_B,OUTPUT);
  pinMode(BR_B,OUTPUT);
  analogWriteFrequency(2,10000);
  analogWriteFrequency(3,10000);
  analogWriteFrequency(4,10000);
  analogWriteFrequency(5,10000);
}
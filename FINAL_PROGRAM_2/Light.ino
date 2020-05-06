void updateLight() {
  if(micros()-lightTimer>80) {
    lightVals[lightCnt] = analogRead(ALOG_1);
    lightVals[lightCnt+16] = analogRead(ALOG_2);
    lightVals[lightCnt+32] = analogRead(ALOG_3);

    lightCnt = lightCnt>=15 ? 0 : lightCnt+1;
    
    digitalWriteFast(SIG_0, sigTable[lightCnt][0]);
    digitalWriteFast(SIG_1, sigTable[lightCnt][1]);
    digitalWriteFast(SIG_2, sigTable[lightCnt][2]);
    digitalWriteFast(SIG_3, sigTable[lightCnt][3]);

    lightTimer = micros();
  }
}

int processLight() {
  int cnt=0;
  float ang=0, tempAng=0, len=0;
  int clusterStart, clusterEnd;
  for(int i=0; i<36; i++) {
    if(lightVals[i]>lightThresh[i]) {
      lightDetect[cnt] = i;
      cnt++;
    }
  }
  if(cnt==0) {  //no light detected
    lineAng = 400;
    lineLen = 3;
    prevOnLine = onLine;
    if(onLine) onLine = false;
    return 0;
  }
  else if (cnt==1) {
    lineAng = lightDetect[0]*10.0;
    lineLen = 0;
  }
  else {
    //find furthest chord
    for(int i=0; i<cnt-1; i++) {  //inefficient version
      for(int j=0; j<cnt; j++) {
        float tempAng = smallestAngleBetween(lightDetect[i]*10,lightDetect[j]*10);
        if(tempAng>ang) {
          ang = tempAng;
          clusterStart=lightDetect[i]*10;
          clusterEnd=lightDetect[j]*10;
        }
      }
    }
    lineAng = angleBetween(clusterStart, clusterEnd) <= 180 ? midAngleBetween(clusterStart, clusterEnd) : midAngleBetween(clusterEnd, clusterStart);
    lineLen = smallestAngleBetween(clusterStart,clusterEnd)/180;
  }
  prevOnLine = onLine;
  if(!onLine) onLine = true;
  return 1;
}

void processLine() {
  if(prevOnLine && onLine) {  //was on line, stil on line
    if(smallestAngleBetween(prevLineAng,lineAng)>=90) { //robot on outside of line
      lineAng = mod(lineAng+180,360);
      lineLen = 2-lineLen;
    }
  }
  else if(!prevOnLine && onLine) {  //was not on line, now on line

  } else if(prevOnLine && !onLine) {  //was on line, now not on line
    if(lineLen <= 1) {  //last seen inside of line, in field
      lineLen=0;
      lineAng=0;
    } else {  //last seen outside of line, out of field
      lineLen=3;
    }
  }
  prevLineAng = lineAng;
  
}

void lineAvoid() {
  if(onLine) {
    if(lineLen > LIGHT_RETURN_MAX) {
      if(lineLen==3) angular_drive(LIGHT_RETURN_SPEED,mod((lineAng+180),360),0);
      else {
        angular_drive(lineLen*0.5*LIGHT_RETURN_SPEED,mod((lineAng+180),360),0);
        //Serial.print(lineLen*0.5*LIGHT_RETURN_SPEED);
      }
      
      //Serial.print(" ");
      //Serial.println(mod((lineAng+180),360));
    } else if(lineLen > LIGHT_RETURN_MIN && isOutsideLine(ballAng)) {
      //angular_drive(0,0,0);
      if(lineLen==3) angular_drive(LIGHT_RETURN_SPEED,mod((lineAng+180),360),0);
      else {
        angular_drive(lineLen*0.5*LIGHT_RETURN_SPEED,mod((lineAng+180),360),0);
        //Serial.print(lineLen*0.5*LIGHT_RETURN_SPEED);
      }
      
      //Serial.print(" ");
      //Serial.println(mod((lineAng+180),360));
    }
  }
}

bool isOutsideLine(float angle) {
  if(onLine) {
    if(smallestAngleBetween(lineAng,angle) < 20) return true;
    else return false;
  } else {  //in field cant tell
    return false;
  }
}

void printLight() {
  for(int i=0; i<48; i++) {
    Serial.print(lightVals[i]);
    Serial.print(",");
  }
}


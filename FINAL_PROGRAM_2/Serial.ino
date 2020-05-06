void getRaspistr(bool sp) {
  String s;
  Serial1.write('a');
  if (Serial1.available()) {
    while (Serial1.available()) {
      s = Serial1.readStringUntil('c');
    }
    if (sp) Serial.println(s);
    int cnt = 0, ncoor = 0, varcnt = 0, temp = 0;
    for (int i = 0; i < s.length(); i++) {
      if (s[i] == 'b') {
        temp = 0;
        for (cnt; cnt > 0; cnt--) {
          temp += (powf(10, cnt - 1) * (s[i - cnt] - '0'));
        }
        if (varcnt == 1) ball[ball[0] - ncoor + 1] = temp;
        else if (varcnt == 2) bgoal[bgoal[0] - ncoor + 1] = temp;
        else ygoal[ygoal[0] - ncoor + 1] = temp;
        ncoor--;
      }
      else if (s[i] == 'a') {
        ncoor = 0;
        for (cnt; cnt > 0; cnt--) {
          ncoor += powf(10, cnt - 1) * (s[i - cnt] - '0');
        }
        varcnt++;
        if (varcnt == 2) bgoal[0] = ncoor;
        else if (varcnt == 3) ygoal[0] = ncoor;
        else ball[0] = ncoor;
      }
      else cnt++;
    }
    if(sp){
      for(int i = 0; i <= ball[0]; i++){
        Serial.print(ball[i]);
        Serial.print(" ");
      }
      Serial.println();
      for(int i = 0; i <= bgoal[0]; i++){
        Serial.print(bgoal[i]);
        Serial.print(" ");
      }
      Serial.println();
      for(int i = 0; i <= ygoal[0]; i++){
        Serial.print(ygoal[i]);
        Serial.print(" ");
      }
      Serial.println();
      
    }
  }
}

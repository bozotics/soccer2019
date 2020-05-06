/*void initIMU() {
  gyro.begin();
  accelmag.begin(ACCEL_RANGE_4G);
  filter.begin(75);
}*/ 

/*void readIMU() {
  sensors_event_t gyro_event;
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  // Get new data samples
  gyro.getEvent(&gyro_event);
  accelmag.getEvent(&accel_event, &mag_event);
  // Apply mag offset compensation (base values in uTesla)
  float x = mag_event.magnetic.x - mag_offsets[0];
  float y = mag_event.magnetic.y - mag_offsets[1];
  float z = mag_event.magnetic.z - mag_offsets[2];
  // Apply mag soft iron error compensation
  float mx = x * mag_softiron_matrix[0][0] + y * mag_softiron_matrix[0][1] + z * mag_softiron_matrix[0][2];
  float my = x * mag_softiron_matrix[1][0] + y * mag_softiron_matrix[1][1] + z * mag_softiron_matrix[1][2];
  float mz = x * mag_softiron_matrix[2][0] + y * mag_softiron_matrix[2][1] + z * mag_softiron_matrix[2][2];
  // Apply gyro zero-rate error compensation
  float gx = gyro_event.gyro.x + gyro_zero_offsets[0];
  float gy = gyro_event.gyro.y + gyro_zero_offsets[1];
  float gz = gyro_event.gyro.z + gyro_zero_offsets[2];
  // The filter library expects gyro data in degrees/s, but adafruit sensor
  // uses rad/s so we need to convert them first (or adapt the filter lib
  // where they are being converted)
  gx *= 57.2958F;
  gy *= 57.2958F;
  gz *= 57.2958F;
  // Update the filter
  filter.update(gx, gy, gz,
                accel_event.acceleration.x, accel_event.acceleration.y, accel_event.acceleration.z,
                mx, my, mz);
  
  cmpangle = filter.getYaw();
}*/

void readIMU() {
  imu::Quaternion q = bno.getQuat();
  double siny_cosp = +2.0 * (q.w() * q.z() + q.y() * q.x());
  double cosy_cosp = +1.0 - 2.0 * (q.x() * q.x() + q.z() * q.z());  
  cmpangle = atan2(siny_cosp, cosy_cosp)*180/3.141592654;
}

void processIMU() {
  /*velX += (lastaccX+accX)/2 * (float)((micros()-startTime)/1000000.00);
  velY += (lastaccY+accY)/2 * (float)((micros()-startTime)/1000000.00);
  lastaccX = accX;
  lastaccY = accY;*/
  cmpangle = 360 - mod(cmpangle-cmpoffset,360);

  if (cmpangle > 180) angleError = cmpangle-360; //convert angle to degrees from north
  else angleError = cmpangle;

  if(desiredAngle>180) {    //convert to -180 to 180
    desiredAngle = mod(desiredAngle,360) - 360;
  }

  float error = desiredAngle - angleError;
  //Serial.println(error);
  float P = error*ANGLE_KP;
  float I = (I+error*ANGLE_KI_MULT)*ANGLE_KI;
  float D = (error-lastError)*ANGLE_KD;
  pidAngleError = P+I+D;
  //Serial.printf("%f %f %f %f %f ", error, P,I,D,pidAngleError);
  lastAngle = cmpangle;
  lastError=error;
}

float mod(float x, float y) {
  x = fmod(x,y);
  return x < 0 ? x+y : x;
}

float angleBetween(float x, float y) {
  return mod((y-x),360);
}

float smallestAngleBetween(float x, float y) {
  float ang = angleBetween(x,y);
  return fmin(ang,360-ang);
}

float midAngleBetween(float x, float y) {
  return mod(x + angleBetween(x,y)/2.0, 360);
}



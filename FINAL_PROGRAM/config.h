#define e		2.7182818284590452353602874713527
#define pi 	3.1415926535897932384626433832795

//motor pins
#define FL_A 	31
#define FR_A	24
#define BL_A 	28
#define BR_A 	27
#define FL_B 	30
#define FR_B 	25
#define BL_B 	29
#define BR_B 	26
#define FL_PWM 	5
#define FR_PWM 	2
#define BL_PWM 	4
#define BR_PWM 	3

//light pins
#define SIG_0 	36
#define SIG_1 	37
#define SIG_2 	38
#define SIG_3 	39
#define ALOG_1 	33
#define ALOG_2 	34
#define ALOG_3 	35

//multiplexer binary
const int sigTable[16][4] = {
  {0,0,0,0}, //0
  {1,0,0,0}, //1
  {0,1,0,0}, //2
  {1,1,0,0}, //3
  {0,0,1,0}, //4
  {1,0,1,0}, //5
  {0,1,1,0}, //6
  {1,1,1,0}, //7
  {0,0,0,1}, //8
  {1,0,0,1}, //9
  {0,1,0,1}, //10
  {1,1,0,1}, //11
  {0,0,1,1}, //12
  {1,0,1,1}, //13
  {0,1,1,1}, //14
  {1,1,1,1}  //15
};

//line return thresholds
#define LIGHT_RETURN_MIN 	0.2
#define LIGHT_RETURN_MAX 	0.5
#define LIGHT_RETURN_SPEED 200

//camera calc values
#define CAMERA_CENTER_X 	198
#define CAMERA_CENTER_Y 	170
#define BALL_DIST_MAX 		300
#define BALL_ROTATION_ANG 	45

//compass PID
#define ANGLE_KP 0.5
#define ANGLE_KI 0.01
#define ANGLE_KI_MULT 0.7
#define ANGLE_KD 5.0

//i2c addresses
#define MAPLE_ADDR        0x69

//catchment area stuff
#define OFF_FRONT_DRIB    1
#define ON_FRONT_DRIB     2
#define KICK_FRONT        3
#define OFF_BACK_DRIB     4
#define ON_BACK_DRIB      5
#define KICK_BACK         6

#define FRONT_GATE_THRESH 700
#define BACK_GATE_THRESH 500
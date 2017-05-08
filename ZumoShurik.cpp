#include "ZumoShurik.h"
/*Custom functions for Zumo 32U4 robot
 * written by Alexander Kirilov
 * shurik179@gmail.com
 */
Zumo32U4Encoders encoders;
Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
//global variable describing mode of Zumo robot: LINEARRAY or PROXIMITY
ZumoMode mode;

/*  Encoder ticks (of each motor) for 1 cm traveled
 *  1 wheel revolution=75.81*12=909 encoder counts = 3.14*3.9cm=12cm
 *  1 cm=75 encoder counts
 */
const int ENCODER_PER_CM=75;

/*
 * Encoder ticks per 360 degree turn of the robot
 */
const long ENCODER_PER_TURN=2700;

/*  
 *   Light threshhold for determining light/dark. Note that sensor values ranges 0-1000;
 *   0=white
 *   1000= black
 */
const int LIGHT_THRESHOLD=300; 
  
/*
 * Array for storing sensor values
 */
uint16_t lineSensorValues[5];

/*
 * Print two lines to LCD screen
 * Each line msut be at most 8 symbols 
 */
void printLcd(String s1, String s2) {
    lcd.clear();
    lcd.gotoXY(0,0);
    lcd.print(s1);
    lcd.gotoXY(0,1);
    lcd.print(s2);
}

void beep(int frequency, int duration){
    buzzer.playFrequency(frequency, duration, 15);
}


/* ***********************************************
 *  Motion control commands                       
 *  distance is in mm, speed between 0 - 100
 * ***********************************************/
void goForward(long distance, int speed){
  long target=(distance*ENCODER_PER_CM)/5; //target sum of encoder values
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  motors.setSpeeds(speed*4, speed*4);
  while ((encoders.getCountsLeft()+encoders.getCountsRight()) <target) {
    delay(20);
  }
  motors.setSpeeds(0,0);
}  
void goBackward(long distance, int speed){
  long target=(distance*ENCODER_PER_CM)/5;
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  motors.setSpeeds(-speed*4, -speed*4);
  while (-(encoders.getCountsLeft()+encoders.getCountsRight()) <target) {
    delay(20);
  }
  motors.setSpeeds(0,0);
}  

void setMotors(int leftSpeed, int rightSpeed){
  motors.setSpeeds(leftSpeed*4, rightSpeed*4);
}

void stopMotors(){
  motors.setSpeeds(0,0);
}
/*
 * Positive angle=clockwise turn
 */
void turn (int angle, int speed){
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  long target = (abs(angle)*ENCODER_PER_TURN)/360; //target encoder value, positive
  if (angle>0) {
     //turning clockwise
     motors.setSpeeds(speed*4, -speed*4);
     while (encoders.getCountsLeft()<target) {
      delay(20);   
     }
     motors.setSpeeds(0,0);
  } else {
     //turning counterclockwise
     motors.setSpeeds(-speed*4, speed*4);
     while (encoders.getCountsRight()<target) {
      delay(20);   
     }
     motors.setSpeeds(0,0);
  }
  
}

void resetEncoders(){
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

//returns distance in mm
int distanceTraveled(){
 return(5*(encoders.getCountsRight()+encoders.getCountsLeft())/ENCODER_PER_CM);
}

int angleTurned(){
    return ((180.0*(encoders.getCountsLeft()-encoders.getCountsRight()))/ENCODER_PER_TURN);
}


void calibrateSensors(ZumoMode m){
  mode=m; //change the global variable
  if (m==LINEARRAY) {
    lineSensors.initFiveSensors();
  } else {
    lineSensors.initThreeSensors();
    proxSensors.initThreeSensors();
  }
  encoders.getCountsAndResetLeft();
  motors.setSpeeds(200,-200);
  while (encoders.getCountsLeft()<ENCODER_PER_TURN){
    lineSensors.calibrate();
  }
  motors.setSpeeds(0,0);
}

void readLineSensors(){
  lineSensors.readCalibrated(lineSensorValues);
}

void readProxSensors(){
  proxSensors.read();
}

boolean sensorOnWhite(int sensorIndex)
{ 
    if (mode==LINEARRAY) {   
        return lineSensorValues[sensorIndex] <=LIGHT_THRESHOLD ;
    }
    //otherwise, we are in 3 sensor mode, so lineSensorValues only has 3 values
    switch (sensorIndex) {
        case DN1: return lineSensorValues[0] <=LIGHT_THRESHOLD ;
            break;
        case DN3: return lineSensorValues[1] <=LIGHT_THRESHOLD ;
            break;
        case DN5: return lineSensorValues[2] <=LIGHT_THRESHOLD ;
            break;
        default: return false;
    }
}
boolean sensorOnBlack(int sensorIndex)
{
    if (mode==LINEARRAY) {   
        return lineSensorValues[sensorIndex] >LIGHT_THRESHOLD ;
    }
    //otherwise, we are in 3 sensor mode, so lineSensorValues only has 3 values
    switch (sensorIndex) {
        case DN1: return lineSensorValues[0] >LIGHT_THRESHOLD ;
            break;
        case DN3: return lineSensorValues[1] >LIGHT_THRESHOLD ;
            break;
        case DN5: return lineSensorValues[2] >LIGHT_THRESHOLD ;
            break;
        default: return false;
    } 
}
boolean allOnWhite()
{ if (mode==LINEARRAY) {
     return (lineSensorValues[0] <=LIGHT_THRESHOLD 
          && lineSensorValues[1] <=LIGHT_THRESHOLD
          && lineSensorValues[2] <=LIGHT_THRESHOLD
          && lineSensorValues[3] <=LIGHT_THRESHOLD
          && lineSensorValues[4] <=LIGHT_THRESHOLD
         );
  } else {
     return (lineSensorValues[0] <=LIGHT_THRESHOLD 
          && lineSensorValues[1] <=LIGHT_THRESHOLD
          && lineSensorValues[2] <=LIGHT_THRESHOLD
         );
     
  }
}
boolean allOnBlack()
{ 
   if (mode==LINEARRAY) {
      return (lineSensorValues[0] >LIGHT_THRESHOLD 
          && lineSensorValues[1] >LIGHT_THRESHOLD
          && lineSensorValues[2] >LIGHT_THRESHOLD
          && lineSensorValues[3] >LIGHT_THRESHOLD
          && lineSensorValues[4] >LIGHT_THRESHOLD
         );
   } else {
      return (lineSensorValues[0] >LIGHT_THRESHOLD 
          && lineSensorValues[1] >LIGHT_THRESHOLD
          && lineSensorValues[2] >LIGHT_THRESHOLD
         );
    
   }
}
int linePosition(){
  return ((lineSensors.readLine(lineSensorValues, QTR_EMITTERS_ON, true)-2000)/20);
  /* readLine returns number between 0-4000; we rescale it to be between -100 and 100
   */
}

int proxSensorLevel(int sensorNum) {
  switch (sensorNum) {
    case PROX_L:
         return proxSensors.countsWithLeftLeds(0);
         break;
    case PROX_LF:
         return proxSensors.countsWithLeftLeds(1);
         break;
    case PROX_RF:
         return proxSensors.countsWithRightLeds(1);
         break;
    case PROX_R:
         return proxSensors.countsWithRightLeds(2);    
         break;
   }
  
}


#include <Wire.h>
#include <Zumo32U4.h>
#pragma once
/*Custom functions for Zumo 32U4 robot
 * written by Alexander Kirilov
 * shurik179@gmail.com
 */

extern Zumo32U4Encoders encoders;
extern Zumo32U4LCD lcd;
extern Zumo32U4Buzzer buzzer;
extern Zumo32U4Motors motors;
extern Zumo32U4ButtonA buttonA;
extern Zumo32U4ButtonB buttonB;
extern Zumo32U4ButtonC buttonC;
extern Zumo32U4LineSensors lineSensors;
extern Zumo32U4ProximitySensors proxSensors;
/*
 * Used to indicate whether the robot is configures to use all 5 line array sensors
 * or only 3 of line sensor plus the proximity sensors
 */
enum ZumoMode {
   LINEARRAY,
   PROXIMITY
};

// proximity sensor constants
#define PROX_L 0
#define PROX_LF 1 //left front
#define PROX_RF 2 //right front
#define PROX_R 3

// Line array sensor constants
#define DN1 0
#define DN2 1
#define DN3 2
#define DN4 3
#define DN5 4



// global variable determinign the mode
extern ZumoMode mode;
/*
 * Print two lines to LCD screen
 * Each line msut be at most 8 symbols 
 */
void printLcd(String s1, String s2);

/*
 * Produces a beep of specifeid frequency (in Hz)
 * and duration (in ms)
 */
void beep(int frequency, int duration=500);


/*
 * Forward/backward  motion. distance is in mm, speed between 0 - 100 (do not use negative speed!)
 */
void goForward(long distance, int speed=70);
void goBackward(long distance, int speed=70);
/*
 * Turns in place. Angle is in degree clockwise; can be negative (for counterclockwise turns). 
 * Speed must be between 0-100 (do not use negative speed!)
 */
void turn(int angle, int speed=50);
void setMotors(int leftSpeed, int rightSpeed);
void stopMotors();
void resetEncoders();
/*
 * Measures distance traveled since the last time encoder were reset. 
 * Result is in mm; can be negative (if moving backwards).
 */
int distanceTraveled();

/*
 * Measures angle turned  since the last time encoder were reset. 
 * Result is in degrees (clockwise is positive)
 */
int angleTurned();

/*
 * Line/proximity sensor related functions
 */
/*
 * Initiaize and calibrate the sensors (i.e., determine black and white values)
 * if mode m=LINEARRAY, all 5 line sensors will be active and proximity sensors will be disabled
 * if mode m=PROXIMITY, only 3 line sensors are active, and in addition, proximity snsors will be active
 */
void calibrateSensors(ZumoMode m);
/*
 * Updates the reading of all line sensors; 
 * readings can be retrieved using functions below
 */
void readLineSensors();
/*
 * Updates the reading of all proximity sensors; 
 * readings can be retrieved using functions below
 * Warning: takes significant time, about 20ms
 */
void readProxSensors();
/*
 * Checks whether given front line sensor is on white/black
 * (after calibration).
 * Allowed Sensor values are DN1 (leftmost) to DN5 (rightmost)
 * In 3-sensor mode (aka PROXIMITY), only DN1, DN3 and DN5 are allowed
 * sensorOnWhite gives same result as !sensorOnBlack
 * Note that it uses sensor readings obtained last time 
 * readLineSensors() or linePosition() were called
 */
boolean sensorOnWhite(int sensorNum);
boolean sensorOnBlack(int sensorNum);
boolean allOnWhite();
boolean allOnBlack();
/*
 * Returns position of line under the front sensor array (assuming white line on black)
 * Returns number between -100  and 100
 * -100: all the way to the left; 0 - center; 100 - all the way to the right
 */
int linePosition();

/*
 * Returns the reading of proximity sensor, obtained at last readProxSensors
 * The number returned is 0-7; the higher the number, the stronger the signal 
 * sensorNum can be:
 * PROX_L (left LEDs/left sensor)
 * PROX_LF  (left LEDs/front sensor)
 * PROX_RF  (right LEDs/front sensor)
 * PROX_R (right LEDs/right sensor)
 */
int proxSensorLevel(int sensorNum);
 

#include <Wire.h>
#include <Zumo32U4.h>
#include "ZumoShurik.h"

void setup() {
  // put your setup code here, to run once:
  printLcd("Press A", "to start");
  // wait until user presses button A
  buttonA.waitForButton();
}

void loop() {
  // put your main code here, to run repeatedly:
  goForward(400);  //go forward for 400mm=40cm
  turn(90);        // turn right 90 degrees
  beep(400);       //produce a beep with frequency 400 hz
  delay(1000);     //wait for 1000ms=1 sec
}

/*
  Timing.h Implementation

  Last Edit: Craig Piunno 4/15/14
*/

#include "Arduino.h"
#include "Timing_Baja.h"

Timing_Baja::Timing_Baja() {
  prevT = 0;
  tenthTimer = 0;
  fifthTimer = 0;
  oneTimer = 0;
  tenthTimer = 0;
}

void Timing_Baja::updateTimer() {
  t = millis(); //miliseconds that have passed since starting the arduino
  accountedT = t - prevT;
  
  tenthTimer += accountedT;
  fifthTimer += accountedT;
  oneTimer += accountedT;
  tenTimer += accountedT;

  prevT = t;
}

bool Timing_Baja::checkTenth() {
  if(tenthTimer < 100) {
    return false;
  } else {
    tenthTimer = 0;
    return true;
  }
}
bool Timing_Baja::checkFifth() {
  if(fifthTimer < 200) {
    return false;
  } else {
    fifthTimer = 0;
    return true;
  }
}
bool Timing_Baja::checkOne() {
  if(oneTimer < 1000) {
    return false;
  } else {
    oneTimer = 0;
    return true;
  }
}
bool Timing_Baja::checkTen() {
  if(tenTimer < 10000) {
    return false;
  } else {
    tenTimer = 0;
    return true;
  }
}
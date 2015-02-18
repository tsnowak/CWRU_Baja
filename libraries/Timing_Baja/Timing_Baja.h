/*
	Timing_Baja.h

	Sets up timing for Baja instument Panel control loops

	Last Edit: Craig Piunno 4/15/14
*/

#ifndef Timing_Baja_h
#define Timing_Baja_h

#include "Arduino.h"

class Timing_Baja {
  public:
    Timing_Baja();

    void updateTimer();

    bool checkTenth();
    bool checkFifth();
    bool checkOne();
    bool checkTen();

  private:
  	unsigned long t;
  	unsigned long accountedT;
  	unsigned long prevT;
   	unsigned long tenthTimer;
  	unsigned long fifthTimer;
  	unsigned long oneTimer;
  	unsigned long tenTimer;
};

#endif
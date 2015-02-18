/*
	Event_Baja.h

	Event object for Baja Instrument Panel

	Last Edit: Craig Piunno 4/15/14
*/

#ifndef Event_Baja_h
#define Event_Baja_h

#include "Arduino.h"

class Event_Baja {
  public:
  	Event_Baja();

  private:
  	bool isLapping;
  	byte currentLapId;
  	word eventTime;
  	word currentLapTime;
  	word bestLapTime;
  	// 0.065217 or ~0.065535 deg -> ~4.5 miles (*1,000,000)
};

#endif
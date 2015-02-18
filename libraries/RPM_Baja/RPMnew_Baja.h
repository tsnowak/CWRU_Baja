/*
Author: Ted Nowak

RPM_Baja_h

Last Edited: 9/18/2014 TSN
*/

#ifndef RPMnew_Baja_h
#define RPMnew_Baja_h

#include "Arduino.h"
#include "../Types_Baja.h"

class RPMnew_Baja {
  public:
  	RPMnew_Baja();

  	int RPM;
  	double tempTime;

  private:

  	//Decide on Pin to use for this **INT0 INT1 or can be enabled on any signal pin
  	const int triggerPin = 10;
  	const double timeRPM;

  };

  #endif
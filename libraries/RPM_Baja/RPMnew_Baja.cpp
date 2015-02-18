/*
Author: Ted Nowak
RPM_Baja_h

Last Edited: 9/18/2014 TSN
*/

#ifndef RPMnew_Baja_h
#define RPMnew_Baja_h

#include "Arduino.h"
#include "RPMnew_Baja_h"

void setupRPM()
{
	//allocate arduino pin as triggerPin
	triggerPin = ;
	pinMode(triggerPin, INPUT);

}

/*
durationLow = pulseIn(pin, LOW);
durationHigh = PulseIn(pin,HIGH);
RPMduration = durationLow + durationHigh;
RPM = 60000000/RPMduration;
/*

PCintPort::attachInterrupt(triggerPIN, trigger,RISING);

int trigger()
{
	tempTime = Timer1.read();
	Timer1.restart();
	Timer1.start();
	RPM = tempTime/timeRPM;
}
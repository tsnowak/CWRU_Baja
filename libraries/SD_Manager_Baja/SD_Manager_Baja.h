/*
	SD_Manager_Baja.h

	SD class for Baja Instrument Panel

	Last Edit: Theodore Nowak 02/19/2015
*/

#ifndef SD_Manager_Baja_h
#define SD_Manager_Baja_h

#include "Arduino.h"
#include "SD.h"
#include "SPI.h"
#include <GPS_Baja.h>
#include <DAQ_Baja.h>

class SD_Manager_Baja {
  public:
    SD_Manager_Baja(const int chipSelect);

    void initializeSDCard();
    void checkStatus();
    bool getStatus();

    void writeRPM(unsigned int gearRPM, unsigned int engineRPM);

    void creatNewEvent(GPSData gpsData);

    void createNewFile();

    void resetCounter();

    bool startStop;

  private:
  	static const String emptyString;
  	static const char tab;

     File rpmFile;
    
    char fileChar;

  	const int chipSelect;
  	bool isSD;

    int rpmCounter;

  	unsigned long newEventId;
  	File gpsFile;
  	File gpsIndexFile;
  	File analogFile;
  	File analogIndexFile;

  	void setupDirStructConditional();

  	unsigned long getNextEventId();
  	String readToStringUntil(File file, char endChar);
  	void toCharArrayFromString(char charArray[], String fromString);
};

#endif
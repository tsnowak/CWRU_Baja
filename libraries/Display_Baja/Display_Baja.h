/*
	Display_Baja.h

	Touch screen display class for Baja Instrument Panel

	Last Edit: Theodore Nowak 02/19/2015
*/

#ifndef Display_Baja_h
#define Display_Baja_h

#include "Arduino.h"
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <GPS_Baja.h>
#include <SD_Manager_Baja.h>

class Display_Baja {
  public:
  	Display_Baja(UTFT *ptrDisplayObject, UTouch *ptrTouchObject, UTFT_Buttons *ptrButtonsObject, SD_Manager_Baja *ptrSDObject);

    void initializeDisplay();

    void displayLoadScreen();
  	void displayMainScreen();

    void setDisplayFont(uint8_t font);
    void setDisplayColor(word color);
    void updateMainDisplay(GPSData gpsData, bool isSD, DAQData daqData);
    

    void updateRPM(unsigned int engineRPM);
    void updateGearRPM(unsigned int gearRPM);

    bool getstartStop();
    void setTrue();
    void setFalse();
    void updateRecording();

  private:
    static const String LOAD_SCREEN_TITLE[2];

  	UTFT *_displayObject;
  	UTouch *_touchObject;
    UTFT_Buttons *_buttonsObject;
    SD_Manager_Baja *_SDObject;
    
    int brightPlusBut, brightMinusBut, eventBut, lapBut;
    bool startStop;

    void setupButtons();

    void updateGPSFix(bool isGPSFixed, byte fixQuality, byte numOfSatellites);
    void updateAntennaMode(byte antennaCode);
    void updateSDStatus(bool isSD);
    void updateDateTime(byte realTime[], byte realDate[]);
    void updateSpeed(double speedMPH);
    void updateAverageSpeed(double avgSpeed);
    void updateTopSpeed(double topSpeed);
    void updateDistance(double distance);
    void updateTemp(word temperature);
    void updateFuel(byte fuelLevel);

};

#endif
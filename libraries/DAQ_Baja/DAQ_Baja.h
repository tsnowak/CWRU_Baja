/*
	DAQ_Baja.h

	All operations pertaining to the data displayed to screen and stored to SD card

	Last Edit: Theodore Nowak 02/19/2015
*/

#ifndef DAQ_Baja_h
#define DAQ_Baja_h

#include "Arduino.h"
#include "../Types_Baja.h"

class DAQ_Baja {
  public:
    
  	DAQ_Baja();

    void calculateRPM();
    unsigned int getEngineRPM();
    DAQData getDAQData();
    void updateDAQ(GPSData gpsData);

    void calculateGearRPM();
    unsigned int getGearRPM(); 

  private:

    double gearRPM;
    double engineRPM;  

    double averageSpeed;
    double topSpeed;
    double tripDistance;
    unsigned long gpsPointsAcq;

    unsigned long startFuelTimer;
    unsigned long currentFuelTimer;
    byte fuelLevel;
    word temperature;
    bool isFuelTimer;

    void updateAverageSpeedDAQ(double speedMPH);
    void updateTopSpeedDAQ(double speedMPH);
    void updateTripDistanceDAQ(double speedMPH);
    void updateFuelDAQ();
    void updateTemperatureDAQ();
    void fillFuelTank();
};

#endif
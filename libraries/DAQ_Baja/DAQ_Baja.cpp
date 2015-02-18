/*
  DAQ_Baja.h Implementation

  Last Edit: Theodore Nowak 02/19/2015
*/

#include "Arduino.h"
#include "DAQ_Baja.h"

DAQ_Baja::DAQ_Baja() {
	gpsPointsAcq = 0;
	averageSpeed = 0.0;
	topSpeed = 0.0;
	tripDistance = 0.0;
	fuelLevel = 0;
	temperature = 0;
	isFuelTimer = false;
}

// Returns the RPM at the CVT
unsigned int DAQ_Baja::getGearRPM() {
	return (unsigned int) gearRPM;
}

// Returns the RPM at the piston
unsigned int DAQ_Baja::getEngineRPM() {
	return (unsigned int) engineRPM;
}

// Retrieves all DAC data
DAQData DAQ_Baja::getDAQData() {
	DAQData data;

	data.averageSpeed = averageSpeed;
	data.topSpeed = topSpeed;
	data.tripDistance = tripDistance;
	data.fuelLevel = fuelLevel;
	data.temperature = temperature;

	return data;
}

// Calculates the RPM at the CVT
void DAQ_Baja::calculateGearRPM() {
	static unsigned long long tempGearRPMTimer;
	static unsigned long long gearRPMTimer;

	if (gearRPMTimer != 0){
		tempGearRPMTimer = micros();
		gearRPM = 60000000/(double)(tempGearRPMTimer - gearRPMTimer);
		gearRPMTimer = micros();

		if (gearRPM > 5000){
			gearRPM = 5000;
		}
	}

	//Prevents numbers from exploding with sensor noise
	else {
		gearRPMTimer = micros();
	}
}

//Calculates the RPM at the piston
void DAQ_Baja::calculateRPM() {
 	static unsigned long long tempRPMTimer;
 	static unsigned long long rpmTimer;

	if(rpmTimer != 0) {
		tempRPMTimer = micros();
		engineRPM = 60000000.0 / (double)(tempRPMTimer - rpmTimer);
		rpmTimer = micros();

		if (engineRPM > 5000){
			engineRPM = 5000;
		}
	} 

	//Prevents numbers from exploding with sensor noise
	else {
		rpmTimer = micros();
	}
}

// Updates the DAC data
void DAQ_Baja::updateDAQ(GPSData gpsData) {
	if(gpsData.speedMPH >= 1.0) {
		updateAverageSpeedDAQ(gpsData.speedMPH);
		updateTopSpeedDAQ(gpsData.speedMPH);
		updateTripDistanceDAQ(gpsData.speedMPH);
	}
	updateFuelDAQ();
	updateTemperatureDAQ();
}

void DAQ_Baja::updateAverageSpeedDAQ(double speedMPH) {
	gpsPointsAcq++;
	if(gpsPointsAcq > 1) {
		averageSpeed = ((averageSpeed * (gpsPointsAcq - 1)) + speedMPH)/(double)gpsPointsAcq;
	} else {
		averageSpeed = speedMPH;
	}
}

void DAQ_Baja::updateTopSpeedDAQ(double speedMPH) {
	if(speedMPH > topSpeed) {
		topSpeed = speedMPH; // ***** Does the speed need filtering from ramndom errors? (DGPS) *****
	}
}

void DAQ_Baja::updateTripDistanceDAQ(double speedMPH) {
	tripDistance += speedMPH * 720;
}

void DAQ_Baja::fillFuelTank() {
	startFuelTimer = millis();
	isFuelTimer = true;
}

void DAQ_Baja::updateFuelDAQ() {
	currentFuelTimer = millis();
	static unsigned long timerDifference = currentFuelTimer - startFuelTimer;
	if(timerDifference < 60000 && isFuelTimer) {
		fuelLevel = floor((60000 - timerDifference) / 60000.0);
	} else {
		fuelLevel = 0;
		isFuelTimer = false;
	}
}

void DAQ_Baja::updateTemperatureDAQ() {
	temperature = 0;
}
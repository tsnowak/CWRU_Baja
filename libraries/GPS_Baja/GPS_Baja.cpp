/*
  GPS_Baja.h Implementation

  Currently still under construction
  Last Edit: Theodore Nowak 02/19/2015
*/

#include "Arduino.h"
#include "GPS_Baja.h"

const String GPS_Baja::GPS_UPDATE_5HZ = "$PMTK220,10000*1F";//"$PMTK220,200*2C"; 1Hz
const String GPS_Baja::TRANSMIT_RMC = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29";
const String GPS_Baja::TRANSMIT_RMC_GGA = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";
const String GPS_Baja::ANTENNA_DATA = "$PGCMD,33,1*6C";
const String GPS_Baja::ENABLE_SBAS = "$PMTK313,1*2E\r";
const String GPS_Baja::ENABLE_WAAS_DGPS = "$PMTK301,2*2E\r";
const String GPS_Baja::RMC_ID = "$GPRMC";
const String GPS_Baja::GGA_ID = "$GPGGA";
const String GPS_Baja::ANTENNA_ID = "$PGTOP";

const char GPS_Baja::comma = ',';
const char GPS_Baja::dot = '.';

// Constructor ***Maybe make all variables static so multiple instances do not cause errors***
GPS_Baja::GPS_Baja() {
	isGPSFixed = false;
	gpsSentence = "";
	antennaCode = 2;
}

void GPS_Baja::initializeGPS() {
	Serial1.println(GPS_UPDATE_5HZ);
	Serial1.println(TRANSMIT_RMC_GGA);
	Serial1.println(ANTENNA_DATA);
	Serial1.println(ENABLE_SBAS);
	Serial1.println(ENABLE_WAAS_DGPS);
}

// Returns a struct with the GPS data
GPSData GPS_Baja::getGPSData() {
	GPSData data;

	data.realTime[0] = realTime[0];
	data.realTime[1] = realTime[1];
	data.realTime[2] = realTime[2];
  	data.isGPSFixed = isGPSFixed;
  	data.degLat = degLat;
  	data.latBearing = latBearing;
  	data.degLong = degLong;
  	data.longBearing = longBearing;
  	data.speedMPH = speedMPH;
  	data.realDate[0] = realDate[0];
  	data.realDate[1] = realDate[1];
  	data.realDate[2] = realDate[2];
  	data.fixQuality = fixQuality;
  	data.numOfSatellites = numOfSatellites;
  	data.antennaCode = antennaCode;

	return data;
}

// Read serial data from GPS
void GPS_Baja::readGPS() {
	char c = Serial1.read();
	if(c != '\n') {
		gpsSentence += c;
	} else {
		parseGPSSentence(gpsSentence);
		gpsSentence = "";
	}
}

// Parse GPS RMC data into class variables
void GPS_Baja::parseGPSSentence(String gpsSentence) {
	if(gpsSentence.startsWith(RMC_ID)) {
		isGPSFixed = saveRMCToVar(gpsSentence);
	} else if(gpsSentence.startsWith(GGA_ID)) {
		saveGGAToVar(gpsSentence);
	} else if(gpsSentence.startsWith(ANTENNA_ID)) {
		saveAntennaToVar(gpsSentence);
	}
}

bool GPS_Baja::saveRMCToVar(String gpsSentence) {
	// Time data
	static byte firstCommaAfter = gpsSentence.indexOf(comma) + 1;
	static byte secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	static String subGPSSentence = gpsSentence.substring(firstCommaAfter, secondCommaAt);
	static byte tempTime[3];
	tempTime[0] = (byte)subGPSSentence.substring(0,2).toInt();
	tempTime[1] = (byte)subGPSSentence.substring(2,4).toInt();
	tempTime[2] = (byte)subGPSSentence.substring(4).toInt();

	// GPS status
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	if(!gpsSentence.substring(firstCommaAfter, secondCommaAt).equals("A")) {
		accountForTimeZone(tempTime);
		return false;
	}

	// Latitude data
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	subGPSSentence = gpsSentence.substring(firstCommaAfter, secondCommaAt);
	unsigned long degrees = (byte)subGPSSentence.substring(0,2).toInt();
	double minutes = (double)(subGPSSentence.substring(2,4).toInt() + subGPSSentence.substring(5).toInt() / 10000.0);
	degLat = (unsigned long)(degrees + (minutes * 166666.66));

	// Latitude bearing
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	latBearing = gpsSentence.substring(firstCommaAfter, secondCommaAt).charAt(0);

	// Longitude data
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	subGPSSentence = gpsSentence.substring(firstCommaAfter, secondCommaAt);
	degrees = (unsigned long)subGPSSentence.substring(0,3).toInt();
	minutes = (double)(subGPSSentence.substring(3,5).toInt() + subGPSSentence.substring(6).toInt() / 10000.0);
	degLong = (unsigned long)(degrees + (minutes * 166666.66));

	// Longitude bearing
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	longBearing = gpsSentence.substring(firstCommaAfter, secondCommaAt).charAt(0);

	// Speed data
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	subGPSSentence  = gpsSentence.substring(firstCommaAfter, secondCommaAt);
	byte decPlace = (byte)(subGPSSentence.length() - 1 - subGPSSentence.indexOf(dot));
	speedMPH = (double)(subGPSSentence.substring(0,subGPSSentence.indexOf('.')).toInt() + subGPSSentence.substring(subGPSSentence.indexOf('.') + 1).toInt() / (double)pow(10.0, decPlace)) * 1.15078;;

	// Tracking angle data ***Unused as of now***
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	//subGPSSentence = gpsSentence.substring(firstCommaAfter, secondCommaAt);

	// Date data
	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	subGPSSentence = gpsSentence.substring(firstCommaAfter, secondCommaAt);
	static byte tempDate[3];
	tempDate[0] = (byte)subGPSSentence.substring(0,2).toInt();
	tempDate[1] = (byte)subGPSSentence.substring(2,4).toInt();
	tempDate[2] = (byte)subGPSSentence.substring(4).toInt();

	accountForTimeZone(tempTime, tempDate);

	return true;
}

void GPS_Baja::saveGGAToVar(String gpsSentence) {
	static byte firstCommaAfter = gpsSentence.indexOf(comma) + 1;
	static byte secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);

	for(byte i = 0 ; i < 3 ; i++) {
		firstCommaAfter = secondCommaAt + 1;
		secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	}
	fixQuality = (byte)(gpsSentence.substring(firstCommaAfter, secondCommaAt).toInt());

	firstCommaAfter = secondCommaAt + 1;
	secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	numOfSatellites = (byte)(gpsSentence.substring(firstCommaAfter, secondCommaAt).toInt());
}

void GPS_Baja::saveAntennaToVar(String gpsSentence) {
	static byte firstCommaAfter = gpsSentence.indexOf(comma) + 1;
	static byte secondCommaAt = gpsSentence.indexOf(comma, firstCommaAfter);
	antennaCode = (byte)(gpsSentence.substring((secondCommaAt + 1), (secondCommaAt + 2)).toInt());
}

void GPS_Baja::accountForTimeZone(byte tempTime[]) {
	static byte tempDate[3] = {0,0,0};
	accountForTimeZone(tempTime, tempDate);
}

void GPS_Baja::accountForTimeZone(byte tempTime[], byte tempDate[]) {
	if(tempTime[0] < 4 && tempDate[0] != 0) {
		resetTimeZoneDate(tempDate);
	} else {
		realDate[0] = tempDate[0];
		realDate[1] = tempDate[1];
		realDate[2] = tempDate[2];
	}

	if(tempTime[0] == 0) {
		tempTime[0] = 12;
	} else if(tempTime[0] > 12) {
		tempTime[0] = tempTime[0] - 12;
	}

	if(tempTime[0] < 5) {
		tempTime[0] = tempTime[0] + 12;
	}

	tempTime[0] = tempTime[0] - 4;

	realTime[0] = tempTime[0];
	realTime[1] = tempTime[1];
	realTime[2] = tempTime[2];
}

void GPS_Baja::resetTimeZoneDate(byte tempDate[]) {
	if(tempDate[0] > 1) {
		realDate[0] = tempDate[0] - 1;
		realDate[1] = realDate[1];
		realDate[2] = realDate[2];
	} else {
		switch(tempDate[1]) {
			case 1:
				realDate[0] = 31;
				realDate[1] = 12;
				realDate[2] = tempDate[2] - 1;
				break;
			case 2:
				realDate[0] = 31;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 3:
				realDate[0] = 28;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 4:
				realDate[0] = 31;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 5:
				realDate[0] = 30;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 6:
				realDate[0] = 31;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 7:
				realDate[0] = 30;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 8:
				realDate[0] = 31;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 9:
				realDate[0] = 31;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 10:
				realDate[0] = 30;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 11:
				realDate[0] = 31;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
				break;
			case 12:
				realDate[0] = 30;
				realDate[1] = tempDate[1] - 1;
				realDate[2] = realDate[2];
					break;
			default:
				break;
		}
	}
}
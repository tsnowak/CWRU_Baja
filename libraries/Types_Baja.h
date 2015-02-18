#ifndef Types_Baja_h
#define Types_Baja_h

struct GPSData {
	String gpsSentence;
	byte realTime[3];
	bool isGPSFixed;
	unsigned long degLat;
	char latBearing;
	unsigned long degLong;
	char longBearing;
	double speedMPH;
	byte realDate[3];
	byte fixQuality;
	byte numOfSatellites;
	byte antennaCode;
};

struct DAQData {
	double averageSpeed;
	double topSpeed;
	double tripDistance;
	byte fuelLevel;
	word temperature;
};

#endif
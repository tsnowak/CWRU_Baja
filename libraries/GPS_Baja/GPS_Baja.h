/*
	GPS_Baja.h

	GPS class for Baja Instrument Panel

	Last Edit: Craig Piunno 4/15/14
*/

#ifndef GPS_Baja_h
#define GPS_Baja_h

#include "Arduino.h"
#include "../Types_Baja.h"

class GPS_Baja {
  public:
    GPS_Baja();

    void initializeGPS();

    void readGPS();
    GPSData getGPSData();

  private:
  	static const String GPS_UPDATE_5HZ;
  	static const String TRANSMIT_RMC;
    static const String TRANSMIT_RMC_GGA;
  	static const String ANTENNA_DATA;
    static const String ENABLE_SBAS;
    static const String ENABLE_WAAS_DGPS;
  	static const String RMC_ID;
    static const String GGA_ID;
  	static const String ANTENNA_ID;

  	static const char comma;
  	static const char dot;

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

  	void parseGPSSentence(String gpsSentence);
  	bool saveRMCToVar(String gpsSentence);
    void saveGGAToVar(String gpsSentence);
  	void saveAntennaToVar(String gpsSentence);

  	void accountForTimeZone(byte tempTime[]);
  	void accountForTimeZone(byte tempTime[], byte tempDate[]);
  	void resetTimeZoneDate(byte tempDate[]);
};

#endif
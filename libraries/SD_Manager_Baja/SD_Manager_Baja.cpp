/*
  SD_Manager_Baja.h Implementation

  Last Edit: Theodore Nowak 02/19/2015
*/

#include "Arduino.h"
#include "SD_Manager_Baja.h"

const String FILE_PATH[6] = {"GPS/GPS_", "GPS/index/GPS_", "Analog/Str_", "Analog/index/Str_", ".txt", "_i.txt"};

const String SD_Manager_Baja::emptyString = "";
const char SD_Manager_Baja::tab = '\t';

SD_Manager_Baja::SD_Manager_Baja(const int chipSelect):chipSelect(chipSelect) {
	newEventId = 0; 
	rpmCounter = 0; 
	isSD = false;
}

// Initializes SD card- returns false if no SD card, true if SD card exists
void SD_Manager_Baja::initializeSDCard() {
	if(!SD.begin(chipSelect)) {
		isSD = false;
		return;
	} else {
		setupDirStructConditional();
		isSD = true;
	}
}

// Resets the rpm counter to deliniate new data
void SD_Manager_Baja::resetCounter(){
	rpmCounter = 0;
}

// Writes the recorded rpm data to the text file RPMFile.txt
void SD_Manager_Baja::writeRPM(unsigned int gearRPM, unsigned int engineRPM) {
	if (isSD){

		rpmFile = SD.open("RPMFile.txt", FILE_WRITE);

		if (rpmCounter == 0){
			rpmFile.println("----------------------------------------------------------");
		}

		rpmFile.print(rpmCounter);
		rpmFile.print(", ");
		rpmFile.print(millis());
		rpmFile.print(", ");
		rpmFile.print(gearRPM);
		rpmFile.print(", ");
		rpmFile.print(engineRPM);
		rpmFile.println("");
		//rpmFile.print(gpsData.speedMPH);
		//rpmFile.println("");
		rpmFile.close();

		rpmCounter ++;
	}
}

// Checks for the existance of RPMFile.txt
void SD_Manager_Baja::checkStatus() {
	if(SD.exists("RPMFile.txt")) {
		isSD = true;
	} else {
		isSD = false;
	}
}

// Creates RPMFile.txt for rpm recordings if non-existent
void SD_Manager_Baja::createNewFile(){
	if (isSD){
		SD.mkdir("RPMFile.txt");
	}
	else {
		checkStatus();
	}
}

// Public function to get isSD
bool SD_Manager_Baja::getStatus() {
	return isSD;
}

// If SD is missing Events.txt sets up necesary directories
void SD_Manager_Baja::setupDirStructConditional() {
	if(!SD.exists("Events.txt")) {
		SD.mkdir("GPS/index");
		SD.mkdir("Analog/index");
	}
}

// Creates a new event file
void SD_Manager_Baja::creatNewEvent(GPSData gpsData) {
	newEventId = getNextEventId();
	File eventsFile = SD.open("Events.txt", FILE_WRITE);
	eventsFile.print(newEventId);
	eventsFile.print(tab);
	eventsFile.println(emptyString);
	eventsFile.close();

	char filePath[30];
	toCharArrayFromString(filePath, (FILE_PATH[0] + newEventId + FILE_PATH[4]));
	gpsFile = SD.open(filePath, FILE_WRITE);
	toCharArrayFromString(filePath, (FILE_PATH[1] + newEventId + FILE_PATH[5]));
	gpsIndexFile = SD.open(filePath, FILE_WRITE);
	toCharArrayFromString(filePath, (FILE_PATH[2] + newEventId + FILE_PATH[4]));
	analogFile = SD.open(filePath, FILE_WRITE);
	toCharArrayFromString(filePath, (FILE_PATH[3] + newEventId + FILE_PATH[5]));
	analogIndexFile = SD.open(filePath, FILE_WRITE);
}

// Determines the next available event ID
unsigned long SD_Manager_Baja::getNextEventId() {
	if(newEventId != 0) {
		return newEventId++;
	}

	File eventsFile = SD.open("Events.txt", FILE_READ);

	unsigned long tempId;
	if(eventsFile.peek() == -1) {
		return 1;
	} else {
		do {
			tempId = (unsigned long)readToStringUntil(eventsFile, tab).toInt();
			while(eventsFile.read() != '\n') {
				if(eventsFile.peek() == -1) {
					break;
				}
			}
		} while (eventsFile.peek() != -1); // While condition not necessary, but adds extra protection from error
	}

	eventsFile.close();

	return tempId + 1;
}


String SD_Manager_Baja::readToStringUntil(File file, char endChar) {
		String fileSegment = "";
		byte i = 0;
		while(file.peek() != endChar) {
			fileSegment += (char)file.read();
			i++;
		}

		return fileSegment;
}

void SD_Manager_Baja::toCharArrayFromString(char charArray[], String fromString) {
	fromString.toCharArray(charArray, fromString.length() + 1);
}
/*
  Display_Baja.h Implementadtion

  Last Edit: Theodore Nowak 02/19/2015
*/

#include "Arduino.h"
#include "Display_Baja.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

const String Display_Baja::LOAD_SCREEN_TITLE[2] = {"BAJA INSTUMENT PANEL", "CASE WESTERN RESERVE UNIVERSITY"};

Display_Baja::Display_Baja(UTFT *ptrDisplayObject, UTouch *ptrTouchObject, UTFT_Buttons *ptrButtonsObject, SD_Manager_Baja *ptrSDObject) {
	_displayObject = ptrDisplayObject;
	_touchObject = ptrTouchObject;
	_buttonsObject = ptrButtonsObject;
	_SDObject = ptrSDObject;
}

void Display_Baja::initializeDisplay() {
	_displayObject->InitLCD(LANDSCAPE);
	setupButtons();
}

void Display_Baja::displayLoadScreen() {
	_displayObject->clrScr();
	_displayObject->setFont(BigFont);
	_displayObject->setColor(VGA_BLUE);
	_displayObject->print(LOAD_SCREEN_TITLE[0], CENTER, 223);
	_displayObject->print(LOAD_SCREEN_TITLE[1], CENTER, 255);
}

void Display_Baja::displayMainScreen() {
	_displayObject->clrScr();

	// Info display
	_displayObject->setFont(SmallFont);
	_displayObject->setColor(VGA_AQUA);
	_displayObject->print("FIX:", 709, 410);
	_displayObject->print("ANT:", 709, 433);
	_displayObject->print("SD:", 709, 456);

	_displayObject->setFont(BigFont);
	_displayObject->print("Speed:", 1, 1);
	_displayObject->print("MPH", 150, 182);

	_displayObject->print("RPM:", 200,1);
	_displayObject->print("x1", 366, 182);

	_displayObject->print("AvgS:", 1, 200);
	_displayObject->print("MPH", 150, 282);
	_displayObject->print("TopS:", 100, 200);
	_displayObject->print("MPH", 50, 282);
	_displayObject->print("Distance:", 200, 200);
	_displayObject->print("Miles", 318, 282);
	_displayObject->print("Temperature:", 1, 300);
	_displayObject->print("deg F", 118, 382);
	_displayObject->print("Fuel Level:", 200, 300);
	_displayObject->print("%", 382, 382);

	// Setup borders
	_displayObject->setColor(VGA_SILVER);
	_displayObject->drawLine(199, 0, 199, 299);
	_displayObject->drawLine(0, 199, 399, 199);
	_displayObject->drawLine(0, 299, 399, 299);
	_displayObject->drawLine(99, 199, 99, 299);
	_displayObject->drawLine(199, 199, 199, 479);
	_displayObject->drawLine(399, 0, 399, 479);
	_displayObject->drawLine(0, 399, 799, 399);
	_displayObject->drawLine(99, 399, 99, 479);
	_displayObject->drawLine(199, 399, 199, 479);
	_displayObject->drawLine(599, 399, 599, 479);
	_displayObject->drawLine(699, 399, 699, 479);

	// Date and time display
	_displayObject->setColor(VGA_WHITE);
	_displayObject->print(":", 267, 415);
	_displayObject->print(".", 315, 415);
	_displayObject->print("/", 267, 447);
	_displayObject->print("/", 315, 447);

	_buttonsObject->setButtonColors(VGA_SILVER, VGA_BLACK, VGA_BLACK, VGA_AQUA, VGA_BLACK);
	_buttonsObject->drawButton(brightPlusBut);
	_buttonsObject->drawButton(brightMinusBut);
	_buttonsObject->setButtonColors(VGA_SILVER, VGA_BLACK, VGA_BLACK, VGA_AQUA, VGA_BLACK);
	_buttonsObject->drawButton(eventBut);
	_buttonsObject->setButtonColors(VGA_SILVER, VGA_RED, VGA_BLACK, VGA_AQUA, VGA_BLACK);
	_buttonsObject->drawButton(lapBut);
}

void Display_Baja::setupButtons() {
	_buttonsObject->setTextFont(BigFont);
	brightPlusBut = _buttonsObject->addButton(0, 400, 98, 79, "Br +");
	brightMinusBut = _buttonsObject->addButton(100, 400, 98, 79, "Br -");
	eventBut = _buttonsObject->addButton(400, 400, 198, 79, "New Event");
	lapBut = _buttonsObject->addButton(600, 400, 98, 79, "Events");
	_buttonsObject->disableButton(brightPlusBut);
	_buttonsObject->disableButton(brightMinusBut);
	_buttonsObject->disableButton(eventBut);
	_buttonsObject->disableButton(lapBut);
}

void Display_Baja::setDisplayFont(uint8_t font) {
	_displayObject->setFont(&font);
}

void Display_Baja::setDisplayColor(word color) {
	_displayObject->setColor(color);
}

void Display_Baja::updateMainDisplay(GPSData gpsData, bool isSD, DAQData daqData) {
	updateGPSFix(gpsData.isGPSFixed, gpsData.fixQuality, gpsData.numOfSatellites);
	updateAntennaMode(gpsData.antennaCode);
	updateSDStatus(isSD);
	if(gpsData.isGPSFixed) {
		_displayObject->setColor(VGA_WHITE);
	} else {
		_displayObject->setColor(VGA_RED);
	}

	//Currently unused values are commented out
	updateDateTime(gpsData.realTime, gpsData.realDate);
	//updateSpeed(gpsData.speedMPH); 						
	_displayObject->setColor(VGA_WHITE);
	updateRecording();
	//updateAverageSpeed(daqData.averageSpeed);
	//updateTopSpeed(daqData.topSpeed);
	//updateDistance(daqData.tripDistance);
	//updateTemp(daqData.temperature);
	//updateFuel(daqData.fuelLevel);
}

void Display_Baja::updateGPSFix(bool isGPSFixed, byte fixQuality, byte numOfSatellites) {
	_displayObject->setFont(SmallFont);
	if(isGPSFixed && fixQuality != 0) {
		switch(fixQuality) {
			case 1:
				_displayObject->setColor(VGA_BLUE);
				_displayObject->print(" GPS", 741, 433);
				_displayObject->printNumI(numOfSatellites, 765, 433, 2, '0');
				break;
			case 2:
				_displayObject->setColor(VGA_GREEN);
				_displayObject->print("DGPS", 741, 433);
				_displayObject->printNumI(numOfSatellites, 765, 433, 2, '0');
				break;
			default:
				_displayObject->setColor(VGA_RED);
				_displayObject->print("   N/A", 741, 433);
				break;
		}
		_displayObject->setColor(VGA_GREEN);
		_displayObject->print("FIX", 769, 410);
	} else {
		_displayObject->setColor(VGA_RED);
		_displayObject->print("ACQ", 769, 410);
	}
}

void Display_Baja::updateAntennaMode(byte antennaCode) {
	_displayObject->setFont(SmallFont);
	switch(antennaCode) {
		case 2:
			_displayObject->setColor(VGA_BLUE);
			_displayObject->print("INT", 770, 433);
			break;
		case 3:
			_displayObject->setColor(VGA_GREEN);
			_displayObject->print("EXT", 770, 433);
			break;
		default:
			_displayObject->setColor(VGA_RED);
			_displayObject->print("ERR", 770, 433);
			break;
	}
}

void Display_Baja::updateSDStatus(bool isSD) {
	_displayObject->setFont(SmallFont);
	if(isSD) {
		_displayObject->setColor(VGA_GREEN);
		_displayObject->print("Active", 745, 456);
	} else {
		_displayObject->setColor(VGA_RED);
		_displayObject->print("  None", 745, 456);
	}
}

void Display_Baja::updateDateTime(byte realTime[], byte realDate[]) {
	_displayObject->setFont(BigFont);
  
	_displayObject->printNumI(realTime[0], 235, 415, 2, '0');
	_displayObject->printNumI(realTime[1], 283, 415, 2, '0');
	_displayObject->printNumI(realTime[2], 331, 415, 2, '0');

	_displayObject->printNumI(realDate[1], 235, 447, 2, '0');
	_displayObject->printNumI(realDate[0], 283, 447, 2, '0');
	_displayObject->printNumI(realDate[2], 331, 447, 2, '0');
}

void Display_Baja::updateSpeed(double speedMPH) {
	_displayObject->setFont(SevenSegNumFont);
	_displayObject->printNumI((int)round(speedMPH), 67, 74, 2, '0');
}

void Display_Baja::updateRPM(unsigned int engineRPM) {
	_displayObject->setFont(SevenSegNumFont);
	_displayObject->printNumI(engineRPM, 400, 74, 4, '0');
}


void Display_Baja::updateGearRPM(unsigned int gearRPM){
	_displayObject->setFont(SevenSegNumFont);
	_displayObject->printNumI(gearRPM, 235, 74, 4, '0');
}

void Display_Baja::updateAverageSpeed(double averageSpeed) {
	_displayObject->setFont(BigFont);
	_displayObject->printNumF(averageSpeed, 1, 17, 241, '.', 4, '0');
}
void Display_Baja::updateTopSpeed(double topSpeed) {
	_displayObject->setFont(BigFont);
	_displayObject->printNumF(topSpeed, 1, 117, 241, '.', 4, '0');
}
void Display_Baja::updateDistance(double distance) {
	_displayObject->setFont(BigFont);
	_displayObject->printNumF(distance, 3, 250, 241, '.', 6, '0');
}
void Display_Baja::updateTemp(word temperature) {
	_displayObject->setFont(SevenSegNumFont);
	_displayObject->printNumI(temperature, 51, 324, 3, '0');
}
void Display_Baja::updateFuel(byte fuelLevel) {
	_displayObject->setFont(SevenSegNumFont);
	_displayObject->printNumI(fuelLevel, 267, 324, 2, '0');
}

bool Display_Baja::getstartStop(){
	return startStop;
}

void Display_Baja::setTrue(){
	startStop = true;
}

void Display_Baja::setFalse(){
	startStop = false;
}

void Display_Baja::updateRecording(){
	if (startStop){
		_displayObject->setColor(VGA_RED);
		_displayObject->print("Recording", 500, 200);
	}
	else{
		_displayObject->setColor(VGA_GREEN);
		_displayObject->print("Stopped Recording", 500, 200);
	}

}
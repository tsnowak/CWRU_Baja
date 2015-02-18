// Arduino Libraries
#include <Scheduler.h>
#include <SD.h>
#include <SPI.h>

// LCD Screen Libraries
#include <UTFT.h> // CTE70
#include <UTouch.h>
#include <UTFT_Buttons.h>

// Custom Libraries
#include <Timing_Baja.h>
#include <GPS_Baja.h>
#include <SD_Manager_Baja.h>
#include <Event_Baja.h>
#include <Display_Baja.h>
#include <DAQ_Baja.h>
#include <Graph_Baja.h>
#include <Course_Map_Baja.h>

//Recording on/off switch
int startStopPin = 11;

// Custom Library Objects: those commented out to be implemented later
Timing_Baja timingObject;
GPS_Baja gpsObject;
SD_Manager_Baja sdObject(52);
//Event_Baja eventObject;
UTFT displayObject(CTE70,38,39,40,41);
UTouch touchObject(6,5,4,3,2);
UTFT_Buttons buttonsObject(&displayObject, &touchObject);
Display_Baja totalDisplayObject(&displayObject, &touchObject, &buttonsObject, &sdObject);
DAQ_Baja daqObject;
//Graph_Baja graphObject;
//Course_Map_Baja courseMapObject;

void setup() {
  
  pinMode(startStopPin, INPUT);
  
  if (digitalRead(startStopPin) == HIGH) totalDisplayObject.setTrue();
  else totalDisplayObject.setFalse();
  
  totalDisplayObject.initializeDisplay();
  totalDisplayObject.displayLoadScreen();
  
  // Initialize serial port for testing/debugging
  Serial.begin(9600);
  while(!Serial) {
    ;
  }
  // Initialize serial port for GPS communication
  Serial1.begin(9600);
    while(!Serial1) {
      ;
  }
  
  analogReadResolution(12); // Set DAC for Arduino Due
  analogWrite(7, 255); //consider removing
  
  //gpsObject.initializeGPS(); //uncomment for gps use
  sdObject.initializeSDCard();
  
  //interrupts for rpm recording
  attachInterrupt(9, rpmInterrupt, RISING);    
  attachInterrupt(10, gearRPMInterrupt, RISING);
  
  //Scheduler.startLoop(gpsLoop); //loop to update gps information
  Scheduler.startLoop(nonGPSLoop);
  
  if(sdObject.getStatus()) {
    delay(2000);
  }
  
  totalDisplayObject.displayMainScreen();
}

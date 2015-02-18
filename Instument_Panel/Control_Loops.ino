// Main loop: Timing
void loop() {
  timingObject.updateTimer();
  yield();
}

// Control loop for all GPS data reception
//void gpsLoop() {
  //if(Serial1.available() > 0) {
    //gpsObject.readGPS();
  //}
 //else yield(); //If error occurs, see old version of code (might need if/else to stop yield from causing lost GPS data)
//}


// Control loop for all time dependent operations
void nonGPSLoop() {
  if(timingObject.checkTenth()) {
    // Strain Data would go here too
    
    //Recording Switch Detection
    if (digitalRead(startStopPin) == HIGH){ //if statement to see if recording is on or off
    
      totalDisplayObject.setTrue();
      
      if(sdObject.getStatus()){
        sdObject.writeRPM(daqObject.getGearRPM(), daqObject.getEngineRPM());
      } 
      else {
        sdObject.initializeSDCard();
        sdObject.createNewFile();
      }
    }
    else {
      sdObject.resetCounter();
      totalDisplayObject.setFalse();
    }
    
    //gps is not currently working/enabled, most code is written though. Refer to GPS_Baja.cpp for more info
    //daqObject.updateDAQ(gpsObject.getGPSData());
    totalDisplayObject.updateGearRPM(daqObject.getGearRPM());
    totalDisplayObject.updateRPM(daqObject.getEngineRPM());
    yield();
  }
  
  if(timingObject.checkFifth()) {
    
  }
 if(timingObject.checkOne()) {
   sdObject.checkStatus();
   totalDisplayObject.updateMainDisplay(gpsObject.getGPSData(), sdObject.getStatus(), daqObject.getDAQData());
 }
  if(timingObject.checkTen()) {    
  }
  yield();
}

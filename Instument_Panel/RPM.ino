//RPM recording interrupts
void rpmInterrupt() {
  daqObject.calculateRPM();
}

void gearRPMInterrupt() {
  daqObject.calculateGearRPM();
}

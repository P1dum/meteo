#include "Lux.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Luminence getlux() : "); Serial.print(getLux()); Serial.println(" lx\n");
}

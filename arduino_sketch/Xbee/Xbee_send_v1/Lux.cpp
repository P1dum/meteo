#include "Lux.h"

float getLux(){
  int valCAN = analogRead(PIN_ENTREE_LUX); // valeur variant de 0 à 1023
  float tension =(VIN*valCAN)/VAL_CAN_MAX;
  float courant = (tension*10); // courant en µA --> courant = (tension/R)*100 000 -->soit 10 = 100 000/R
  float lux = 2*courant; // eclairement lumineux

  return lux;
}
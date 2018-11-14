#ifndef PRES_TEMP_H
#define PRES_TEMP_H

#include "Arduino.h"

#include <Wire.h>

#define I2C_ADDR 0x60
#define CONVERT 0x12
#define a0_MSB_DEFINE 0x04
#define Padc_MSB_DEFINE 0x00
#define CONVERT_TIME 300 // Time between start convert command and data available in the Pressure and Temperature registers

struct Coefficients{
  float a0;
  float b1;
  float b2;
  float c12;
};

struct Pcomp_and_Tadc{
  float Pcomp;
  int Tadc;
};

struct Pres_and_Temp{
  float Pressure;
  float Temperature;
};

struct Coefficients getCoef();

struct Pcomp_and_Tadc dataConvert_and_pressureComp(float a0, float b1, float b2, float c12);

struct Pres_and_Temp getPres_and_getTemp(float Pcomp, int Tadc);

#endif
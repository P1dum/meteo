#include "Pres_temp.h"

struct Coefficients coef;

void setup() {
  Serial.begin(9600);
  
  coef = getCoef();
}

void loop() {

  struct Pcomp_and_Tadc Pcomp_and_Tadc1;
  struct Pres_and_Temp Pres_and_Temp1;
  
  Pcomp_and_Tadc1 = dataConvert_and_pressureComp(coef.a0, coef.b1, coef.b2, coef.c12);
  
  Pres_and_Temp1 = getPres_and_getTemp(Pcomp_and_Tadc1.Pcomp, Pcomp_and_Tadc1.Tadc);
  Serial.print("Pressure = ");Serial.print(Pres_and_Temp1.Pressure); Serial.println(" kPa");
  Serial.print("Pressure = "); Serial.print(Pres_and_Temp1.Temperature); Serial.println(" C");

}

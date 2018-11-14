#include "Pres_temp.h"

struct Coefficients getCoef(){
	struct Coefficients coef;
  // Init some value
  unsigned int coef_data[8]={0};
  unsigned int a0_MSB = 0, a0_LSB = 0, b1_MSB = 0, b1_LSB = 0, b2_MSB = 0, b2_LSB = 0, c12_MSB = 0, c12_LSB =0;
  float a0 = 0.0, b1 = 0.0, b2 = 0.0, c12 = 0.0;

  Wire.begin();

  // ##### Reading conefficient data #####
  //Begin a transmission to the I2C slave device with the given address.
  Wire.beginTransmission(I2C_ADDR); // transmit to device I2C_ADDR (0x60)
  
  for(int i=0; i<8; i++){
    // Choose coefficient address
    Wire.write(byte(a0_MSB_DEFINE+i)); // sends instruction byte
    Wire.endTransmission(); // stop transmitting
    
    Wire.requestFrom(I2C_ADDR, 1); // request 1 bytes from slave device I2C_ADDR
    while(Wire.available() == 1){ // slave may send less than requested
      coef_data[i]=Wire.read();
    }
  }
  
  a0_MSB = coef_data[0];
  a0_LSB = coef_data[1];
  a0 = ((a0_MSB << 8) | a0_LSB) / 8.0; // Fractional bits 3 --> 2^3=8
  
  b1_MSB = coef_data[2];
  b1_LSB = coef_data[3];
  b1 = ((b1_MSB << 8) | b1_LSB);
  if (b1>32767){
    b1 = b1 - 65536;
  }
  b1 = b1 / 8192.0; // Fractional bits 13 --> 2^13 = 8192
  
  b2_MSB = coef_data[4];
  b2_LSB = coef_data[5];
  b2 = ((b2_MSB << 8) | b2_LSB);
  if (b2>32767){
    b2 = b2 - 65536;
  }
  b2 = b2 / 16384.0; // Fractional bits 14 --> 2^14 = 16384
  
  c12_MSB = coef_data[6];
  c12_LSB = coef_data[7];
  c12 = (c12_MSB << 8) | c12_LSB;
  c12 = (((c12_MSB << 8) | c12_LSB) / 4.0) / 4194304.0; // Fractional bits 13 + 9 dec pt zero pad --> 2^22 = 4194304 --> divide by 4 WHY ?

  coef.a0 = a0;
  coef.b1 = b1;
  coef.b2 = b2;
  coef.c12 = c12;
  return coef;
}

struct Pcomp_and_Tadc dataConvert_and_pressureComp(float a0, float b1, float b2, float c12){
  struct Pcomp_and_Tadc Pcomp_and_Tadc1;
  // ##### Data conversion #####
  unsigned int adc_data[8]={0};
  unsigned int Padc_MSB = 0, Padc_LSB = 0, Tadc_MSB = 0, Tadc_LSB = 0;
  int Padc = 0, Tadc = 0;
  
  //Begin a transmission to the I2C slave device with the given address.
  Wire.beginTransmission(I2C_ADDR); // transmit to device I2C_ADDR (0x60)
  //Wire.write(byte(CONVERT)); // sends instruction byte
  Wire.write(CONVERT); // sends instruction to get pressure --> 0x12
  Wire.write(Padc_MSB_DEFINE); // Start conversion -->0x00
  Wire.endTransmission(); // stop transmitting
  delay(CONVERT_TIME); 

  // ##### Compensated pressure reading #####
  //Begin a transmission to the I2C slave device with the given address.
  Wire.beginTransmission(I2C_ADDR); // transmit to device I2C_ADDR (0x60)
  // Choose coefficient address
  //Wire.write(byte(Padc_MSB_DEFINE+i)); // sends instruction byte
  Wire.write(Padc_MSB_DEFINE); // sends instruction byte
  Wire.endTransmission(); // stop transmitting
  
  Wire.requestFrom(I2C_ADDR, 4); // request 4 bytes from slave device I2C_ADDR
  while(Wire.available() == 4){ // slave may send less than requested
    adc_data[0]=Wire.read();
    adc_data[1]=Wire.read();
    adc_data[2]=Wire.read();
    adc_data[3]=Wire.read();
  }

  Padc_MSB = adc_data[0];
  Padc_LSB = adc_data[1];
  Padc = ((Padc_MSB << 8) | Padc_LSB) / 64;
  
  Tadc_MSB = adc_data[2];
  Tadc_LSB = adc_data[3];
  Tadc = ((Tadc_MSB << 8) | Tadc_LSB) / 64;

  // Calculate pressure compensation
  float Pcomp = a0 + (b1 + c12 * Tadc) * Padc + b2 * Tadc;

  Pcomp_and_Tadc1.Pcomp = Pcomp;
  Pcomp_and_Tadc1.Tadc = Tadc;

  return Pcomp_and_Tadc1;
}

struct Pres_and_Temp getPres_and_getTemp(float Pcomp, int Tadc){
  struct Pres_and_Temp Pres_and_Temp1;

  float Pressure = (65.0 / 1023.0) * Pcomp + 50.0;
  float Temperature = (Tadc - 498) / (-5.35) + 25.0;

  Pres_and_Temp1.Pressure = Pressure;
  Pres_and_Temp1.Temperature = Temperature;

  return Pres_and_Temp1;
}
#include <Wire.h>


void readRegister(uint8_t address, uint8_t reg, uint8_t* val){
  Wire.beginTransmission(address);
  Wire.write(reg);
  int result = Wire.endTransmission(address);
  if(result!=0) error(1);
  Wire.requestFrom(address, 1);
  if(Wire.available() > 0) *val = Wire.read();
  else error(1);
}

void writeRegister(uint8_t address, uint8_t reg, uint8_t val){
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(val);
  if(Wire.endTransmission(address)!=0) error(1);
}

int writeRegisterWithError(uint8_t address, uint8_t reg, uint8_t val){
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission(address);
}

void readRegisters(uint8_t address, uint8_t startReg, uint8_t size, uint8_t *data){
  Wire.beginTransmission(address);
  Wire.write(startReg);
  int result = Wire.endTransmission(address);
  if(result!=0) error(1);
  Wire.requestFrom(address, size);
  for(int i = 0; i<size;i++){
    if(!Wire.available()) error(1);
    data[i] = Wire.read();
  }
}


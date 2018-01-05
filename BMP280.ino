/*#include <Wire.h>
#define BMP280_ADDRESS 0x76
#define BMP280_ID 0xD0
#define BMP280_ID_VALUE 0x58
#define BMP280_RESET 0xE0
#define BMP280_RESET_CMD 0xB6
#define BMP280_CTRL_MEAS 0xF4
#define BMP280_CONFIG 0xF5
#define BMP280_DATA_BURST_START 0xF7

#define BMP280_SUCCESS 0x10
#define BMP280_ERROR_NOT_FOUND 0x11
#define BMP280_ERROR_WRONG_ID 0x12
  
uint16_t readUshortLE(int regl){
  uint16_t result;
  byte data[2];
  readRegisters(BMP280_ADDRESS, regl, 2, data);
  result = data[1];
  return (result << 8) | data[0];
}

int16_t readShortLE(int regl){
  int16_t result;
  byte data[2];
  readRegisters(BMP280_ADDRESS, regl, 2, data);
  result = data[1];
  return (result << 8) | data[0];
}

void bmp280_print_adjustment_data(Stream* s){
  s->print(F("unsigned int dig_T1=")); s->print(readUshortLE(0x88)); s->println(';');
  s->print(F("int dig_T2=")); s->print(readShortLE(0x8A)); s->println(';');
  s->print(F("int dig_T3=")); s->print(readShortLE(0x8C)); s->println(';');
  s->print(F("unsigned int dig_P1=")); s->print(readUshortLE(0x8E)); s->println(';');
  s->print(F("int dig_P2=")); s->print(readShortLE(0x90)); s->println(';');
  s->print(F("int dig_P3=")); s->print(readShortLE(0x92)); s->println(';');
  s->print(F("int dig_P4=")); s->print(readShortLE(0x94)); s->println(';');
  s->print(F("int dig_P5=")); s->print(readShortLE(0x96)); s->println(';');
  s->print(F("int dig_P6=")); s->print(readShortLE(0x98)); s->println(';');
  s->print(F("int dig_P7=")); s->print(readShortLE(0x9A)); s->println(';');
  s->print(F("int dig_P8=")); s->print(readShortLE(0x9C)); s->println(';');
  s->print(F("int dig_P9=")); s->print(readShortLE(0x9E)); s->println(';'); s->println();
}


int bmp280_init(){
  if(writeRegister(BMP280_ADDRESS, BMP280_RESET, BMP280_RESET_CMD) != 0) return BMP280_ERROR_NOT_FOUND;
  delay(100);
  byte id;
  readRegister(BMP280_ADDRESS, BMP280_ID, &id);
  if(id != BMP280_ID_VALUE) return BMP280_ERROR_WRONG_ID;
  
  writeRegister(BMP280_ADDRESS, BMP280_CTRL_MEAS, 0b01010100); //sleep mode so CONFIG changes take effect. Also 16x pressure oversample and 2x temperature oversample
  writeRegister(BMP280_ADDRESS, BMP280_CONFIG, 0b00000100); //0.5 ms delay between measurements, low pass filter disabled
  writeRegister(BMP280_ADDRESS, BMP280_CTRL_MEAS, 0b01010111); //continous measurement
 
  return BMP280_SUCCESS;
}

void bmp280_read_raw(uint8_t* output){
  readRegisters(BMP280_ADDRESS, BMP280_DATA_BURST_START, 6, output); //nice, single line
}
*/



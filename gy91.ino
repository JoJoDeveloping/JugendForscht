
#include <Wire.h>
#include <SD.h>

#define AK8963_SUCCESS 0x20
#define AK8963_ERROR_NOT_FOUND 0x21
#define AK8963_ERROR_WRONG_ID 0x22
#define AK8963_ERROR_SELF_TEST_FAILED 0x23
#define MPU6500_SUCCESS 0x30
#define MPU6500_ERROR_NOT_FOUND 0x31
#define MPU6500_ERROR_WRONG_ID 0x32
#define MPU6500_ERROR_SELF_TEST_FAILED 0x33
#define BMP280_SUCCESS 0x10
#define BMP280_ERROR_NOT_FOUND 0x11
#define BMP280_ERROR_WRONG_ID 0x12
#define ever (;;)


char filename[22] = "/JuFo/JF00000.BIN";
File file;

#define GREEN 3
#define RED 4

void error(unsigned int err) __attribute__ ((noreturn));

void error(unsigned int err) {
    PORTD &= ~0b00001000; //G low
    for ever{
      for(int i = 0; i < err; i++){
        PORTD |= 0b00010000; //R high
        delay(250);
        PORTD &= ~0b00010000; //R low
        delay(250);
      }
      delay(1750);
    }
}

void mkFilename(unsigned int i, char* result){ //prints i into the appropiate place in filename
  for(int c = 12; i > 0; i/= 10, c--){
    result[c] = '0'+(i%10);
  }
}

void setup(){
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  PORTD |= 0b00011000; //R&G high
  delay(500);
  PORTD &= ~0b00010000; //R low
  
  Wire.begin();
  Wire.setClock(400000);
  auto folder = F("/JuFo/");

  if(!SD.begin(8)){
    error(2);
  }
  if(!SD.exists(folder))
    SD.mkdir(folder);
    
  int i = 0;
  do{
    mkFilename(i, filename);
    i++;
  }while(SD.exists(filename));
    
  if(!(file = SD.open(filename, FILE_WRITE))){
    error(3);
  }
  
  int res;
  //Serial.println(F("Starting MPU6500"));
  res = mpu6500_init();
  switch(res){
    case MPU6500_ERROR_NOT_FOUND: error(4);
    case MPU6500_ERROR_WRONG_ID: error(5);
    case MPU6500_ERROR_SELF_TEST_FAILED: error(6);
  }
  //Serial.println(F("Starting AK8963"));
  res = ak8963_init();
  switch(res){
    case AK8963_ERROR_NOT_FOUND: error(7);
    case AK8963_ERROR_WRONG_ID: error(8);
    case AK8963_ERROR_SELF_TEST_FAILED: error(9);
  }
  /*res = bmp280_init();
  switch(res){
    case BMP280_ERROR_NOT_FOUND: errora = 9; return;
    case BMP280_ERROR_WRONG_ID: errora = 10; return;
  }*/
  File adjust = SD.open(F("/JuFo/adjust.h"), FILE_WRITE);
  mpu6500_print_adjustment_data(&adjust);
  ak8963_print_adjustment_data(&adjust);
  //bmp280_print_adjustment_data(&adjust);
  adjust.close();

  mpu6500_init_complete(); //resets FIFO

  PORTD &= ~0b00001000; //G low
  
}

#define BUF_SIZE 22*16 //22*23 is maximum, but ram starts to run out before. The higher, the better
byte data[BUF_SIZE];

void loop()
{
  if(mpu6500_get_fifo_count() >= BUF_SIZE){
    PORTD &= ~0b00010000; //R low
    PORTD |=  0b00001000; //G high
    int amount = mpu6500_read_fifo(data, BUF_SIZE);
    if(amount == -1){
      memset(data, 255, 22);//overflow marker
      file.write(data, 22); //to detect overflow later. 
      PORTD |= 0b00010000; //R high
      return;
    }
    file.write(data, amount);
    file.flush();
    file.close();
    if(!(file = SD.open(filename, FILE_WRITE))){
      error(3);
    }
    PORTD &= ~0b00001000; //G low
  }
}

#define AK8963_ADDRESS 0x0C
#define AK8963_DEVICE_ID 0x00
#define AK8963_DEVICE_ID_VALUE 0x48
#define AK8963_INFO 0x01
#define AK8963_STATUS1 0x02
#define AK8963_DATA_START 0x03
#define AK8963_STATUS2 0x09
#define AK8963_CONTROL1 0x0A
#define AK8963_CONTROL2 0x0B
#define AK8963_SELF_TEST 0x0C
#define AK8963_I2C_DISABLE 0x0F
#define AK8963_FUSE_ROM_START 0x10

#define AK8963_SUCCESS 0x20
#define AK8963_ERROR_NOT_FOUND 0x21
#define AK8963_ERROR_WRONG_ID 0x22
#define AK8963_ERROR_SELF_TEST_FAILED 0x23

void ak8963_print_adjustment_data(Stream *s){
 //dump fuse rom
 
 mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x0F); //fuse rom access
 byte sensitivity_adjust[3];
 for(int i = 0; i < 3; i++)
   mpu6500_readClientRegister(AK8963_ADDRESS, AK8963_FUSE_ROM_START+i, &sensitivity_adjust[i]);
  
  
 s->print(F("int ASAX = ")); s->print(sensitivity_adjust[0]); s->println(';');
 s->print(F("int ASAY = ")); s->print(sensitivity_adjust[1]); s->println(';');
 s->print(F("int ASAZ = ")); s->print(sensitivity_adjust[2]); s->println(';'); 
 
 mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x10); //power down
 mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x16); //16 bit accuracy, 100Hz
}

bool ak8963_self_test(){
  
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x0F); //fuse rom access
  byte sensitivity_adjust[3];
  for(int i = 0; i < 3; i++)
    mpu6500_readClientRegister(AK8963_ADDRESS, AK8963_FUSE_ROM_START+i, &sensitivity_adjust[i]);

    
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x10); //power down
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_SELF_TEST, 0x40); //self test step 1
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x18); //self test step 2
  byte value = 0;
  while(value&1!=1)
    mpu6500_readClientRegister(AK8963_ADDRESS, AK8963_STATUS1, &value); //wait for measurement to be completed
  byte meas[7];
  for(int i = 0; i < 7; i++)
    mpu6500_readClientRegister(AK8963_ADDRESS, AK8963_DATA_START+i, &meas[i]);
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_SELF_TEST, 0x00); //self disable
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x10); //power down
  float Xadj = ((float)(meas[0]+(meas[1]<<8)))*(sensitivity_adjust[0]/256. + 0.5);
  float Yadj = ((float)(meas[2]+(meas[3]<<8)))*(sensitivity_adjust[1]/256. + 0.5);
  float Zadj = ((float)(meas[4]+(meas[5]<<8)))*(sensitivity_adjust[2]/256. + 0.5);

  return Xadj >= -200. && Xadj <= 200. && Yadj >= -200. && Yadj <= 200. && Zadj >= -3200. && Zadj <= 800.;
}

int ak8963_init(){
  if(mpu6500_writeClientRegisterWithError(AK8963_ADDRESS, AK8963_CONTROL2, 1) != 0) return AK8963_ERROR_NOT_FOUND; 
  delay(100);
  byte id;
  mpu6500_readClientRegister(AK8963_ADDRESS, AK8963_DEVICE_ID, &id);
  if(id != AK8963_DEVICE_ID_VALUE) return AK8963_ERROR_WRONG_ID;


  if(!ak8963_self_test())
    return AK8963_ERROR_SELF_TEST_FAILED;

  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x10); //power down
  mpu6500_writeClientRegister(AK8963_ADDRESS, AK8963_CONTROL1, 0x16); //16 bit accuracy, 100Hz

  return AK8963_SUCCESS;
}


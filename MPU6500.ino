/*
 * The MPU9250 consists of two chips: The MPU6500 6-axis accelerometer and gyroscope,
 * and the MPU6500, a 3-axis compass/magnetometer. Latter is connected to the MPU6500
 * as a client device and requires some manipulation of the latter to be accessed by 
 * the Arduino.
 * 
 */

#define MPU6500_ADDRESS 0x68
#define MPU6500_GYRO_TEST_X 0x00
#define MPU6500_GYRO_TEST_Y 0x01
#define MPU6500_GYRO_TEST_Z 0x02
#define MPU6500_ACCEL_TEXT_X 0x0D
#define MPU6500_ACCEL_TEST_Y 0x0E
#define MPU6500_ACCEL_TEST_Z 0x0F
#define MPU6500_GYRO_OFFSET_XH 0x13
#define MPU6500_GYRO_OFFSET_XL 0x14
#define MPU6500_GYRO_OFFSET_YH 0x15
#define MPU6500_GYRO_OFFSET_YL 0x16
#define MPU6500_GYRO_OFFSET_ZH 0x17
#define MPU6500_GYRO_OFFSET_ZL 0x18
#define MPU6500_SAMPLE_RATE 0x19
#define MPU6500_CONFIG 0x1A
#define MPU6500_GYRO_CONFIG 0x1B
#define MPU6500_ACCEL_CONFIG 0x1C
#define MPU6500_ACCEL_CONFIG2 0x1D
#define MPU6500_LP_ACCEL_ODR 0x1E
#define MPU6500_WOM_THR 0x1F
#define MPU6500_FIFO_EN 0x23
#define MPU6500_I2C_MST_CTRL 0x24
#define MPU6500_I2C_SLV0_ADDR 0x25
#define MPU6500_I2C_SLV0_REG 0x26
#define MPU6500_I2C_SLV0_CTRL 0x27
#define MPU6500_I2C_SLV1_ADDR 0x28
#define MPU6500_I2C_SLV1_REG 0x29
#define MPU6500_I2C_SLV1_CTRL 0x2A
#define MPU6500_I2C_SLV2_ADDR 0x2B
#define MPU6500_I2C_SLV2_REG 0x2C
#define MPU6500_I2C_SLV2_CTRL 0x2D
#define MPU6500_I2C_SLV3_ADDR 0x2E
#define MPU6500_I2C_SLV3_REG 0x2F
#define MPU6500_I2C_SLV3_CTRL 0x30
#define MPU6500_I2C_SLV4_ADDR 0x31
#define MPU6500_I2C_SLV4_REG 0x32
#define MPU6500_I2C_SLV4_DATA_OUT 0x33
#define MPU6500_I2C_SLV4_CTRL 0x34
#define MPU6500_I2C_SLV4_DATA_IN 0x35
#define MPU6500_I2C_MASTER_STATUS 0x36
#define MPU6500_INT_PIN_CFG 0x37
#define MPU6500_INT_ENABLE 0x38
#define MPU6500_INT_STATUS 0x3A
#define MPU6500_EXT_SENSOR_START 0x49
#define MPU6500_I2C_MST_DELAY_CTRL 0x67
#define MPU6500_SIGNAL_PATH_RESET 0x68
#define MPU6500_MOT_DETECT_CTRL 0x69
#define MPU6500_USER_CONTROL 0x6A
#define MPU6500_PWR_MGMT_1 0x6B
#define MPU6500_PWR_MGMT_2 0x6C
#define MPU6500_FIFO_SIZEH 0x72
#define MPU6500_FIFO_SIZEL 0x73
#define MPU6500_FIFO_RW 0x74
#define MPU6500_DEVICE_ID 0x75
#define MPU6500_ACCEL_OFFSET_XH 0x77
#define MPU6500_ACCEL_OFFSET_XL 0x78
#define MPU6500_ACCEL_OFFSET_YH 0x7A
#define MPU6500_ACCEL_OFFSET_YL 0x7B
#define MPU6500_ACCEL_OFFSET_ZH 0x7D
#define MPU6500_ACCEL_OFFSET_ZL 0x7E

#define MPU6500_RESET_CMD 0x80
#define MPU6500_DEVICE_ID_VALUE 0x73

#define MPU6500_SUCCESS 0x30
#define MPU6500_ERROR_NOT_FOUND 0x31
#define MPU6500_ERROR_WRONG_ID 0x32
#define MPU6500_ERROR_SELF_TEST_FAILED 0x33

void mpu6500_print_adjustment_data(Stream *s){
  byte data[6];
  readRegisters(MPU6500_ADDRESS, MPU6500_GYRO_OFFSET_XH, 6, data);
  
  s->print(F("int GYRO_OFFSET_X = ")); s->print((((int)(data[0]))<<7)+(data[1]>>1)); s->println(';');
  s->print(F("int GYRO_OFFSET_Y = ")); s->print((((int)(data[2]))<<7)+(data[3]>>1)); s->println(';');
  s->print(F("int GYRO_OFFSET_Z = ")); s->print((((int)(data[4]))<<7)+(data[5]>>1)); s->println(';');

  readRegisters(MPU6500_ADDRESS, MPU6500_ACCEL_OFFSET_XH, 2, &data[0]); // These registers have "reserved" registers in between them,
  readRegisters(MPU6500_ADDRESS, MPU6500_ACCEL_OFFSET_YH, 2, &data[2]); // so we cannot read them all in one go!
  readRegisters(MPU6500_ADDRESS, MPU6500_ACCEL_OFFSET_ZH, 2, &data[4]); // This is the best way.

  s->print(F("int ACCEL_OFFSET_X = ")); s->print((((int)(data[0]))<<8)+data[1]); s->println(';');
  s->print(F("int ACCEL_OFFSET_Y = ")); s->print((((int)(data[2]))<<8)+data[3]); s->println(';');
  s->print(F("int ACCEL_OFFSET_Z = ")); s->print((((int)(data[4]))<<8)+data[5]); s->println(';');
}

bool mpu6500_self_test(){
  return true; //accurate description of self test procedure is not given in data sheets, so just assume it works
}

int mpu6500_init(){
  if(writeRegisterWithError(MPU6500_ADDRESS, MPU6500_PWR_MGMT_1, MPU6500_RESET_CMD) != 0) return MPU6500_ERROR_NOT_FOUND;
  delay(200);
  byte id;
  readRegister(MPU6500_ADDRESS, MPU6500_DEVICE_ID, &id);
  if(id != MPU6500_DEVICE_ID_VALUE) return MPU6500_ERROR_WRONG_ID;

  writeRegister(MPU6500_ADDRESS, MPU6500_SAMPLE_RATE, 4);// -> 1kHz/(4+1) = 200Hz fifo sample rate
  writeRegister(MPU6500_ADDRESS, MPU6500_CONFIG, 1); //no FSYNC, FIFO overwrites oldest on overflow, DLPF = 1 -> 184Hz gyro output rate, but measure at 200/sec because that's the lowest higher. We can't make the accel faster because then we then can't control the sample rate anymore
  writeRegister(MPU6500_ADDRESS, MPU6500_GYRO_CONFIG, 0b00011000); //no self test, full gyro range (2000 °/s), fchoice = 0b11, so this is 0b00
  writeRegister(MPU6500_ADDRESS, MPU6500_ACCEL_CONFIG, 0b00011000); //no self test, full accel range (16g)
  writeRegister(MPU6500_ADDRESS, MPU6500_ACCEL_CONFIG2, 0b00000000); //fchoice = 1, so this is 0, also DLPFCFG is 0 -> 460 Hz, 
  writeRegister(MPU6500_ADDRESS, MPU6500_FIFO_EN, 0b11111001); //output temp, gyro, accel and slave 0 to FIFO
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_MST_CTRL, 0b01011101); //no multi master, wait for ext sensor, 400kHz I2C rate
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV0_ADDR, 0x0C | 0x80); //Magnetometer address, we want to read
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV0_REG, 0x02); //status reg
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV0_CTRL, 0b10001000); //enable slave 0, read 8 bytes (status regs and data regs)
  writeRegister(MPU6500_ADDRESS, MPU6500_INT_PIN_CFG, 0b11100000); //INT pin is active high, open drain, held high, INT status is cleared only when reading there, disable FSYNC, disable bypass
  writeRegister(MPU6500_ADDRESS, MPU6500_INT_ENABLE, 0b00000000); //no interrupts, because the pin is not exposed.
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_MST_DELAY_CTRL, 0b10000001); //wait for external sensors, read slv0 only every xth cycle
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_CTRL, 0x01); //x above is 2 -> read slv0 only every 2nd cycle (200 Hz / 2 = 100Hz = mag sensor data read rate)
  writeRegister(MPU6500_ADDRESS, MPU6500_USER_CONTROL, 0b01100000); //enable FIFO, enable I2C master for secondary bus
  writeRegister(MPU6500_ADDRESS, MPU6500_PWR_MGMT_1, 0b00000011); //disable sleep, low power, standby or temp sensor pwrdwn, and set clock to PLL Z
  writeRegister(MPU6500_ADDRESS, MPU6500_PWR_MGMT_2, 0b00000000); //enable gyro, accelerometer

  if(!mpu6500_self_test())
    return MPU6500_ERROR_SELF_TEST_FAILED;

  return MPU6500_SUCCESS;
}

void mpu6500_init_complete(){
  writeRegister(MPU6500_ADDRESS, MPU6500_USER_CONTROL, 0b01100101); //reset the FIFO, sensor data
  delay(10);
  writeRegister(MPU6500_ADDRESS, MPU6500_USER_CONTROL, 0b01100100); //reset the FIFO only so the first sensor data is OK
  byte buf;
  readRegister(MPU6500_ADDRESS, MPU6500_INT_STATUS, &buf); //clear interrupts
}

void mpu6500_writeClientRegister(uint8_t address, uint8_t reg, uint8_t val){
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_ADDR, address &~0x80);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_REG, reg);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_DATA_OUT, val);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_CTRL, 0x81); //write!
  byte status;
  do{
    readRegister(MPU6500_ADDRESS, MPU6500_I2C_MASTER_STATUS, &status);
  }while((status & 0x70)==0);
  if(status & 0x10) error(10); //NACK
  if(status & 0x20) error(10); //other error
}

int mpu6500_writeClientRegisterWithError(uint8_t address, uint8_t reg, uint8_t val){
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_ADDR, address &~0x80);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_REG, reg);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_DATA_OUT, val);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_CTRL, 0x81); //write!
  byte status;
  do{
    readRegister(MPU6500_ADDRESS, MPU6500_I2C_MASTER_STATUS, &status);
  }while((status & 0x70)==0);
  if(status & 0x10) return 2; //NACK
  if(status & 0x20) return 4; //other error
  return 0;
}

void mpu6500_readClientRegister(uint8_t address, uint8_t reg, uint8_t *val){
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_ADDR, address | 0x80);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_REG, reg);
  writeRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_CTRL, 0x81); //read!
  byte status;
  do{
    readRegister(MPU6500_ADDRESS, MPU6500_I2C_MASTER_STATUS, &status);
  }while((status & 0x70)==0);
  if(status & 0x10) error(10); //NACK
  if(status & 0x20) error(10); //other error
  readRegister(MPU6500_ADDRESS, MPU6500_I2C_SLV4_DATA_IN, val);
}

int mpu6500_get_fifo_count() {
  byte buf[2];
  readRegisters(MPU6500_ADDRESS, MPU6500_FIFO_SIZEH, 2, buf);
  return buf[1]+(buf[0]<<8);
}

int mpu6500_read_fifo(byte* dest, int amount){
  byte intrp;
  readRegister(MPU6500_ADDRESS, MPU6500_INT_STATUS, &intrp);
  if((intrp & 0x10) != 0){ //handle Overflow
    writeRegister(MPU6500_ADDRESS, MPU6500_USER_CONTROL, 0b01100101); //reset the FIFO, sensor data
    return -1;
  }
  int i = mpu6500_get_fifo_count(), c;
  if(amount > i) amount = i;
  if(i==0) return 0;
  for(c = 0; amount > 32; (amount -= 32) , (c+=32)) //why 32? - that appears to be a magic number -- also: all hail the comma operator
  {
    readRegisters(MPU6500_ADDRESS, MPU6500_FIFO_RW, 32, &dest[c]);
  }
  if(amount > 0)
    readRegisters(MPU6500_ADDRESS, MPU6500_FIFO_RW, amount, &dest[c]);
  return amount+c;
}






/*!
  @file DFRobot_BMI160.cpp
  @brief Define the basic structure of class DFRobot_BMI160 
  @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author  DFRobot_haoJ(hao.jiang@dfrobot.com)
  @version V1.0
  @date 2017-12-01
  @url https://github.com/DFRobot/DFRobot_BMI160
*/

#include "DFRobot_BMI160.h"

const uint8_t int_mask_lookup_table[13] = {
    BMI160_INT1_SLOPE_MASK,
    BMI160_INT1_SLOPE_MASK,
    BMI160_INT2_LOW_STEP_DETECT_MASK,
    BMI160_INT1_DOUBLE_TAP_MASK,
    BMI160_INT1_SINGLE_TAP_MASK,
    BMI160_INT1_ORIENT_MASK,
    BMI160_INT1_FLAT_MASK,
    BMI160_INT1_HIGH_G_MASK,
    BMI160_INT1_LOW_G_MASK,
    BMI160_INT1_NO_MOTION_MASK,
    BMI160_INT2_DATA_READY_MASK,
    BMI160_INT2_FIFO_FULL_MASK,
    BMI160_INT2_FIFO_WM_MASK
};

DFRobot_BMI160::DFRobot_BMI160()
{
  Wire.begin();
  Obmi160=(struct bmi160Dev *)malloc(sizeof(struct bmi160Dev));
  //Obmi160->id = BMI160_I2C_ADDR;
  //Obmi160->interface = BMI160_I2C_INTF; 
  Oaccel= (struct bmi160SensorData*)malloc(sizeof(struct bmi160SensorData));
  Ogyro = (struct bmi160SensorData*)malloc(sizeof(struct bmi160SensorData));
}

uint8_t DFRobot_BMI160::getChipID() {
  uint8_t data = 0;
  int8_t result = getRegs(BMI160_CHIP_ID_ADDR, &data, 1, Obmi160);
  if (result == BMI160_OK) {
    return data;
  }
  else {
    return 0x00;
  }
}

int8_t DFRobot_BMI160::I2cInit(int8_t i2c_addr)
{
  Obmi160->comAddress = i2c_addr;
  Obmi160->interface = BMI160_I2C_INTF;
  return DFRobot_BMI160::I2cInit(Obmi160);
}

int8_t DFRobot_BMI160::SPIInit()
{
  //SPI.begin();
  //pinMode(10,OUTPUT);
  //Obmi160->id = 0;
  //Obmi160->interface = BMI160_SPI_INTF;
  return BMI160_OK;
}

int8_t DFRobot_BMI160::SPIInit(struct bmi160Dev *dev)
{
  return BMI160_OK;
}

int8_t DFRobot_BMI160::I2cInit(struct bmi160Dev *dev)
{
  int8_t result=BMI160_OK;
  uint8_t chip_id=0;
  uint8_t data=0;
  if (dev==NULL){
    return BMI160_E_NULL_PTR; 
  }
  
  if (dev->interface == BMI160_SPI_INTF){
    result = getRegs(BMI160_SPI_COMM_TEST_ADDR, &data, 1, dev);
  }
  if (result == BMI160_OK){
    result = getRegs(BMI160_CHIP_ID_ADDR, &chip_id, 1, dev);
    if ((result == BMI160_OK)&&(chip_id==BMI160_CHIP_ID)){
      dev->any_sig_sel = eBmi160BothAnySigMotionDisabled;
      dev->chipId = chip_id;
      result = softReset(dev);
    }
    else{
      result = BMI160_E_DEV_NOT_FOUND;
    }
  }
  return result;
}

int8_t DFRobot_BMI160::softReset()
{
  int8_t result=BMI160_OK;
  if (Obmi160 == NULL){
    result = BMI160_E_NULL_PTR;
  }  
  result = softReset(Obmi160);
  return result;
}

int8_t DFRobot_BMI160::softReset(struct bmi160Dev *dev)
{
  int8_t result=BMI160_OK;
  uint8_t data = BMI160_SOFT_RESET_CMD;
  if (dev==NULL){
    result = BMI160_E_NULL_PTR;
  }
  result = DFRobot_BMI160::setRegs(BMI160_COMMAND_REG_ADDR, &data, 1, dev);
  delay(BMI160_SOFT_RESET_DELAY_MS);
  if (result == BMI160_OK){
    DFRobot_BMI160::defaultParamSettg(dev);
  }  
  return result;
}

void DFRobot_BMI160::defaultParamSettg(struct bmi160Dev *dev)
{
  /* Initializing accel and gyro params with
  * default values */
  dev->accelCfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
  dev->accelCfg.odr = BMI160_ACCEL_ODR_100HZ;
  dev->accelCfg.power = BMI160_ACCEL_SUSPEND_MODE;
  dev->accelCfg.range = BMI160_ACCEL_RANGE_2G;
  dev->gyroCfg.bw = BMI160_BW_NORMAL_MODE;
  dev->gyroCfg.odr = BMI160_GYRO_ODR_100HZ;
  dev->gyroCfg.power = BMI160_GYRO_SUSPEND_MODE;
  dev->gyroCfg.range = BMI160_GYRO_RANGE_2000_DPS;

  /* To maintain the previous state of accel configuration */
  dev->prevAccelCfg = dev->accelCfg;
  /* To maintain the previous state of gyro configuration */
  dev->prevGyroCfg = dev->gyroCfg;
}

int8_t DFRobot_BMI160::setSensorConfig(uint8_t address, 
                                   uint8_t accelRate, uint8_t accelRange, 
                                   uint8_t accelFilterMode,
                                   uint8_t gyroRate, uint8_t gyroRange, 
                                   uint8_t gyroFilterMode) {
  int8_t result = I2cInit(address);
  if (result == BMI160_OK) {
    Obmi160->accelCfg.odr = accelRate;
    Obmi160->accelCfg.range = accelRange;
    Obmi160->accelCfg.bw = accelFilterMode;

    Obmi160->gyroCfg.odr = gyroRate;
    Obmi160->gyroCfg.range = gyroRange;
    Obmi160->gyroCfg.bw = gyroFilterMode;
    result = DFRobot_BMI160::setSensorConfig(Obmi160); 
  }
  return result;
}

int8_t DFRobot_BMI160::setSensorConfig(struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  dev->accelCfg.power = BMI160_ACCEL_NORMAL_MODE;
  dev->gyroCfg.power = BMI160_GYRO_NORMAL_MODE; 

  result = DFRobot_BMI160::setAccelConf(dev);
  if (result == BMI160_OK) {
    result = DFRobot_BMI160::setGyroConf(dev);
    if (result == BMI160_OK) {
      /* write power mode for accel and gyro */
      result = DFRobot_BMI160::setPowerMode(dev);
      if (result == BMI160_OK)
        result = DFRobot_BMI160::checkInvalidSettg(dev);
    }
  }
  return result; 
}

int8_t DFRobot_BMI160::setAccelConf(struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  uint8_t data[2] = {0};
  result = DFRobot_BMI160::checkAccelConfig(data, dev);
  if (result == BMI160_OK) {
    result = DFRobot_BMI160::setRegs(BMI160_ACCEL_CONFIG_ADDR, &data[0], 1, dev);
    if (result == BMI160_OK) {
      dev->prevAccelCfg.odr = dev->accelCfg.odr;
      dev->prevAccelCfg.bw = dev->accelCfg.bw;
      delay(BMI160_ONE_MS_DELAY);
      result = DFRobot_BMI160::setRegs(BMI160_ACCEL_RANGE_ADDR, &data[1], 1, dev);
      if (result == BMI160_OK){
        dev->prevAccelCfg.range = dev->accelCfg.range;
      }
    }
  }
  return result;   
}

int8_t DFRobot_BMI160::checkAccelConfig(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result;

  /* read accel Output data rate and bandwidth */
  result = DFRobot_BMI160::getRegs(BMI160_ACCEL_CONFIG_ADDR, data, 2, dev);
  if (result == BMI160_OK) {
    result = DFRobot_BMI160::processAccelOdr(&data[0], dev);
    if (result == BMI160_OK) {
      result = DFRobot_BMI160::processAccelBw(&data[0], dev);
      if (result == BMI160_OK)
        result = DFRobot_BMI160::processAccelRange(&data[1], dev);
    }
  }

  return result;
}

int8_t DFRobot_BMI160::processAccelOdr(uint8_t *data,  struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t temp = 0;
  uint8_t odr = 0;

  if (dev->accelCfg.odr <= BMI160_ACCEL_ODR_MAX) {
    if (dev->accelCfg.odr != dev->prevAccelCfg.odr) {
      odr = (uint8_t)dev->accelCfg.odr;
      temp = *data & ~BMI160_ACCEL_ODR_MASK;
      /* Adding output data rate */
      *data = temp | (odr & BMI160_ACCEL_ODR_MASK);
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result;  
}

int8_t DFRobot_BMI160::processAccelBw(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t temp = 0;
  uint8_t bw = 0;

  if (dev->accelCfg.bw <= BMI160_ACCEL_BW_MAX) {
    if (dev->accelCfg.bw != dev->prevAccelCfg.bw) {
      bw = (uint8_t)dev->accelCfg.bw;
      temp = *data & ~BMI160_ACCEL_BW_MASK;
      /* Adding bandwidth */
      *data = temp | ((bw << 4) & BMI160_ACCEL_ODR_MASK);
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result; 
}

int8_t DFRobot_BMI160::processAccelRange(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t temp = 0;
  uint8_t range = 0;

  if (dev->accelCfg.range <= BMI160_ACCEL_RANGE_MAX) {
    if (dev->accelCfg.range != dev->prevAccelCfg.range) {
      range = (uint8_t)dev->accelCfg.range;
      temp = *data & ~BMI160_ACCEL_RANGE_MASK;
      /* Adding range */
      *data = temp | (range & BMI160_ACCEL_RANGE_MASK);
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result;  
}

int8_t DFRobot_BMI160::setGyroConf(struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t data[2]={0};

  result = DFRobot_BMI160::checkGyroConfig(data, dev);
  
  if (result == BMI160_OK) {
    // Write output data rate and bandwidth 
    result = DFRobot_BMI160::setRegs(BMI160_GYRO_CONFIG_ADDR, &data[0], 1, dev);
    if (result == BMI160_OK) {
      dev->prevGyroCfg.odr = dev->gyroCfg.odr;
      dev->prevGyroCfg.bw = dev->gyroCfg.bw;
      delay(BMI160_ONE_MS_DELAY);
      // Write gyro range 
      result = DFRobot_BMI160::setRegs(BMI160_GYRO_RANGE_ADDR, &data[1], 1, dev);
      if (result == BMI160_OK)
        dev->prevGyroCfg.range = dev->gyroCfg.range;
    }
  }
  
  return result;  
}

int8_t DFRobot_BMI160::checkGyroConfig(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result;

  /* read gyro Output data rate and bandwidth */
  result = DFRobot_BMI160::getRegs(BMI160_GYRO_CONFIG_ADDR, data, 2, dev);
  if (result == BMI160_OK) {
    result = DFRobot_BMI160::processGyroOdr(&data[0], dev);
    if (result == BMI160_OK) {
      result = DFRobot_BMI160::processGyroBw(&data[0], dev);
      if (result == BMI160_OK)
        result = DFRobot_BMI160::processGyroRange(&data[1], dev);
    }
  }

  return result;
}

int8_t DFRobot_BMI160::processGyroOdr(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t temp = 0;
  uint8_t odr = 0;

  if (dev->gyroCfg.odr <= BMI160_GYRO_ODR_MAX) {
    if (dev->gyroCfg.odr != dev->prevGyroCfg.odr) {
      odr = (uint8_t)dev->gyroCfg.odr;
      temp = (*data & ~BMI160_GYRO_ODR_MASK);
      /* Adding output data rate */
      *data = temp | (odr & BMI160_GYRO_ODR_MASK);
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result;
}

int8_t DFRobot_BMI160::processGyroBw(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t temp = 0;
  uint8_t bw = 0;

  if (dev->gyroCfg.bw <= BMI160_GYRO_BW_MAX) {
    bw = (uint8_t)dev->gyroCfg.bw;
    temp = *data & ~BMI160_GYRO_BW_MASK;
    /* Adding bandwidth */
    *data = temp | ((bw << 4) & BMI160_GYRO_BW_MASK);
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result;
}

int8_t DFRobot_BMI160::processGyroRange(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t temp = 0;
  uint8_t range = 0;

  if (dev->gyroCfg.range <= BMI160_GYRO_RANGE_MAX) {
    if (dev->gyroCfg.range != dev->prevGyroCfg.range) {
      range = (uint8_t)dev->gyroCfg.range;
      temp = *data & ~BMI160_GYRO_RANGE_MSK;
      /* Adding range */
      *data = temp | (range & BMI160_GYRO_RANGE_MSK);
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result;
}

int8_t DFRobot_BMI160::setPowerMode(struct bmi160Dev *dev)
{
  int8_t result = 0;
  result = DFRobot_BMI160::setAccelPwr(dev);
  if (result == BMI160_OK){
    result = DFRobot_BMI160::setGyroPwr(dev);
  }
  return result;
}

int8_t DFRobot_BMI160::setAccelPwr(struct bmi160Dev *dev)
{
  int8_t result = 0;
  uint8_t data = 0;
  
  if ((dev->accelCfg.power >= BMI160_ACCEL_SUSPEND_MODE) &&
    (dev->accelCfg.power <= BMI160_ACCEL_LOWPOWER_MODE)) {
    if (dev->accelCfg.power != dev->prevAccelCfg.power) {
      result = DFRobot_BMI160::processUnderSampling(&data, dev);
      if (result == BMI160_OK) {
        /* Write accel power */
        result = DFRobot_BMI160::setRegs(BMI160_COMMAND_REG_ADDR, &dev->accelCfg.power, 1, dev);
        /* Add delay of 5 ms */
        if (dev->prevAccelCfg.power == BMI160_ACCEL_SUSPEND_MODE){
          delay(BMI160_ACCEL_DELAY_MS);
        }
        dev->prevAccelCfg.power = dev->accelCfg.power;
      }
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result; 
}

int8_t DFRobot_BMI160::processUnderSampling(uint8_t *data, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t temp = 0;
  uint8_t pre_filter = 0;

  result = DFRobot_BMI160::getRegs(BMI160_ACCEL_CONFIG_ADDR, data, 1, dev);
  if (result == BMI160_OK) {
    if (dev->accelCfg.power == BMI160_ACCEL_LOWPOWER_MODE) {
      temp = *data & ~BMI160_ACCEL_UNDERSAMPLING_MASK;
      /* Set under-sampling parameter */
      *data = temp | ((1 << 7) & BMI160_ACCEL_UNDERSAMPLING_MASK);
      /* Write data */
      result = setRegs(BMI160_ACCEL_CONFIG_ADDR, data, 1, dev);
      /* disable the pre-filter data in
       * low power mode */
      if (result == BMI160_OK)
        /* Disable the Pre-filter data*/
        result = DFRobot_BMI160::setRegs(BMI160_INT_DATA_0_ADDR, &pre_filter, 2, dev);
    } else {
      if (*data & BMI160_ACCEL_UNDERSAMPLING_MASK) {
        temp = *data & ~BMI160_ACCEL_UNDERSAMPLING_MASK;
        /* disable under-sampling parameter
        if already enabled */
        *data = temp;
        /* Write data */
        result = DFRobot_BMI160::setRegs(BMI160_ACCEL_CONFIG_ADDR, data, 1, dev);
      }
    }
  }

  return result; 
}

int8_t DFRobot_BMI160::setGyroPwr(struct bmi160Dev *dev)
{
  int8_t result = 0;
  if ((dev->gyroCfg.power == BMI160_GYRO_SUSPEND_MODE) || (dev->gyroCfg.power == BMI160_GYRO_NORMAL_MODE)
    || (dev->gyroCfg.power == BMI160_GYRO_FASTSTARTUP_MODE)) {
    if (dev->gyroCfg.power != dev->prevGyroCfg.power) {
      /* Write gyro power */
      result = DFRobot_BMI160::setRegs(BMI160_COMMAND_REG_ADDR, &dev->gyroCfg.power, 1, dev);
      if (dev->prevGyroCfg.power == BMI160_GYRO_SUSPEND_MODE) {
        /* Delay of 81 ms */
        delay(BMI160_GYRO_DELAY_MS);
      } else if ((dev->prevGyroCfg.power == BMI160_GYRO_FASTSTARTUP_MODE)
        && (dev->gyroCfg.power == BMI160_GYRO_NORMAL_MODE)) {
        /* This delay is required for transition from
        fast-startup mode to normal mode */
        delay(10);
      } else {
        /* do nothing */
      }
      dev->prevGyroCfg.power = dev->gyroCfg.power;
    }
  } else {
    result = BMI160_E_OUT_OF_RANGE;
  }

  return result; 
}

int8_t DFRobot_BMI160::checkInvalidSettg( struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t data = 0;

  // read the error reg 
  result = DFRobot_BMI160::getRegs(BMI160_ERROR_REG_ADDR, &data, 1, dev);

  data = data >> 1;
  data = data & BMI160_ERR_REG_MASK;
  if (data == 1)
    result = BMI160_E_ACCEL_ODR_BW_INVALID;
  else if (data == 2)
    result = BMI160_E_GYRO_ODR_BW_INVALID;
  else if (data == 3)
    result = BMI160_E_LWP_PRE_FLTR_INT_INVALID;
  else if (data == 7)
    result = BMI160_E_LWP_PRE_FLTR_INVALID;

  return result;
}

int8_t DFRobot_BMI160::getSensorTime(uint32_t* time) {
  uint8_t data[3] = {0,0,0};
  int8_t result = getRegs(BMI160_SENSOR_TIME_ADDR, data, 3, Obmi160);
  if (result == BMI160_OK) {
    *time = uint32_t(
      (((uint32_t)data[2]) << 16) |
      (((uint32_t)data[1]) << 8)  |
      ((uint32_t)data[0])
    );
  }
  return result;
}

int8_t DFRobot_BMI160::getTemperature(int16_t* output) {
  uint8_t* data = new uint8_t[2];
  int8_t result = getRegs(BMI160_TEMPERATURE_ADDR, data, 2, Obmi160);
  if (result == BMI160_OK) {
    *output = (((int16_t)data[1]) << 8) | data[0];
  }
  return result;
}

int8_t DFRobot_BMI160::getSensorData(uint8_t type, int16_t* data)
{
  int8_t result=BMI160_OK;
  if (type==onlyGyro){
    result = getSensorData(BMI160_GYRO_SEL, NULL, Ogyro, Obmi160);
    if(result == BMI160_OK){
      data[0]=Ogyro->x;
      data[1]=Ogyro->y;
      data[2]=Ogyro->z;
    }
  }else if(type==onlyAccel){
    result = getSensorData(BMI160_ACCEL_SEL, Oaccel, NULL, Obmi160);
    if(result == BMI160_OK){
      data[0]=Oaccel->x;
      data[1]=Oaccel->y;
      data[2]=Oaccel->z;
    }
  }else if(type==bothAccelGyro){
    result = DFRobot_BMI160::getSensorData((BMI160_ACCEL_SEL | BMI160_GYRO_SEL),Oaccel, Ogyro, Obmi160);
    if(result == BMI160_OK){
      data[0]=Ogyro->x;
      data[1]=Ogyro->y;
      data[2]=Ogyro->z;
      data[3]=Oaccel->x;
      data[4]=Oaccel->y;
      data[5]=Oaccel->z;
    } 
  }else{
    result = BMI160_ERR_CHOOSE; 
  }
  return result;
}

int8_t DFRobot_BMI160::getAccelData( int16_t* data)
{
  int8_t result=BMI160_OK;
  result = getSensorData(BMI160_ACCEL_SEL, Oaccel, NULL, Obmi160);
  if(result == BMI160_OK){
    data[0]=Oaccel->x;
    data[1]=Oaccel->y;
    data[2]=Oaccel->z;
  }
  return result;
}

int8_t DFRobot_BMI160::getAccelData( int16_t* data, uint32_t* timestamp)
{
  int8_t result=BMI160_OK;
  result = getSensorData((BMI160_ACCEL_SEL | BMI160_TIME_SEL), Oaccel, NULL, Obmi160);
  if(result == BMI160_OK){
    data[0]=Oaccel->x;
    data[1]=Oaccel->y;
    data[2]=Oaccel->z;
    *timestamp=Oaccel->sensortime;
  }
  return result;
}

int8_t DFRobot_BMI160::getGyroData( int16_t* data)
{
  int result = BMI160_OK;
  result = getSensorData(BMI160_GYRO_SEL, NULL, Ogyro, Obmi160); 
  if(result == BMI160_OK){
    data[0]=Ogyro->x;
    data[1]=Ogyro->y;
    data[2]=Ogyro->z;
  }
  return result;
}

int8_t DFRobot_BMI160::getGyroData( int16_t* data, uint32_t* timestamp)
{
  int result = BMI160_OK;
  result = getSensorData((BMI160_GYRO_SEL | BMI160_TIME_SEL), NULL, Ogyro, Obmi160); 
  if(result == BMI160_OK){
    data[0]=Ogyro->x;
    data[1]=Ogyro->y;
    data[2]=Ogyro->z;
    *timestamp=Ogyro->sensortime;
  }
  return result;
}

int8_t DFRobot_BMI160::getAccelGyroData( int16_t* data)
{
  int8_t result = BMI160_OK;
  result = getSensorData((BMI160_ACCEL_SEL | BMI160_GYRO_SEL),Oaccel, Ogyro, Obmi160);
  if(result == BMI160_OK){
    data[0]=Ogyro->x;
    data[1]=Ogyro->y;
    data[2]=Ogyro->z;
    data[3]=Oaccel->x;
    data[4]=Oaccel->y;
    data[5]=Oaccel->z;
  }
  return result;  
}

int8_t DFRobot_BMI160::getAccelGyroData( int16_t* data, uint32_t* timestamp)
{
  int8_t result = BMI160_OK;
  result = getSensorData((BMI160_ACCEL_SEL | BMI160_GYRO_SEL | BMI160_TIME_SEL),Oaccel, Ogyro, Obmi160);
  if(result == BMI160_OK){
    data[0]=Ogyro->x;
    data[1]=Ogyro->y;
    data[2]=Ogyro->z;
    data[3]=Oaccel->x;
    data[4]=Oaccel->y;
    data[5]=Oaccel->z;
    timestamp[0]=Oaccel->sensortime;
    timestamp[1]=Ogyro->sensortime;
  }
  return result;  
}



int8_t DFRobot_BMI160::getSensorData(uint8_t select_sensor, struct bmi160SensorData *accel, struct bmi160SensorData *gyro,struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  uint8_t time_sel;
  uint8_t sen_sel;
  uint8_t len = 0;

  /*Extract the sensor  and time select information*/
  sen_sel = select_sensor & BMI160_SEN_SEL_MASK;
  time_sel = ((sen_sel & BMI160_TIME_SEL) >> 2);
  sen_sel = sen_sel & (BMI160_ACCEL_SEL | BMI160_GYRO_SEL);
  if (time_sel == 1)
    len = 3;

  /* Null-pointer check */
  if (dev != NULL) {
    switch (sen_sel) {
    case eBmi160AccelOnly:
      /* Null-pointer check */
      if (accel == NULL)
        result = BMI160_E_NULL_PTR;
      else
        result = DFRobot_BMI160::getAccelData(len, accel, dev);
      break;
    case eBmi160GyroOnly:
      /* Null-pointer check */
      if (gyro == NULL)
        result = BMI160_E_NULL_PTR;
      else
        result = DFRobot_BMI160::getGyroData(len, gyro, dev);
      break;
    case eBmi160BothAccelAndGyro:
      /* Null-pointer check */
      if ((gyro == NULL) || (accel == NULL))
        result = BMI160_E_NULL_PTR;
      else
        result = DFRobot_BMI160::getAccelGyroData(len, accel, gyro, dev);
      break;
    default:
      result = BMI160_E_INVALID_INPUT;
      break;
    }
  } else {
    result = BMI160_E_NULL_PTR;
  }
  return result;
}

int8_t DFRobot_BMI160::getAccelData(uint8_t len, struct bmi160SensorData *accel, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t idx = 0;
  uint8_t data_array[9] = {0};
  uint8_t time_0 = 0;
  uint16_t time_1 = 0;
  uint32_t time_2 = 0;
  uint8_t lsb;
  uint8_t msb;
  float msblsb;

  result = DFRobot_BMI160::getRegs(BMI160_ACCEL_DATA_ADDR, data_array, 6 + len, dev);

  if (result == BMI160_OK){
    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    accel->x = msblsb; /* Data in X axis */
    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    accel->y = msblsb; /* Data in X axis */
    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    accel->z = msblsb; /* Data in X axis */

    if (len == 3){
      time_0 = data_array[idx++];
      time_1 = (uint16_t)(data_array[idx++] << 8);
      time_2 = (uint32_t)(data_array[idx++] << 16);
      accel->sensortime = (uint32_t)(time_2 | time_1 | time_0);
    }
  }else{
    result = BMI160_E_COM_FAIL;
  }
  return result;
}

int8_t DFRobot_BMI160::getGyroData(uint8_t len, struct bmi160SensorData *gyro, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t idx = 0;
  uint8_t data_array[15] = {0};
  uint8_t time_0 = 0;
  uint16_t time_1 = 0;
  uint32_t time_2 = 0;
  uint8_t lsb;
  uint8_t msb;
  float msblsb;
  if (len == 0) {
    /* read gyro data only */
    result = DFRobot_BMI160::getRegs(BMI160_GYRO_DATA_ADDR, data_array, 6, dev);
    if (result == BMI160_OK) {
      /* Gyro Data */
      lsb = data_array[idx++];
      msb = data_array[idx++];
      msblsb = (int16_t)((msb << 8) | lsb);
      gyro->x = msblsb; /* Data in X axis */

      lsb = data_array[idx++];
      msb = data_array[idx++];
      msblsb = (int16_t)((msb << 8) | lsb);
      gyro->y = msblsb; /* Data in Y axis */

      lsb = data_array[idx++];
      msb = data_array[idx++];
      msblsb = (int16_t)((msb << 8) | lsb);
      gyro->z = msblsb; /* Data in Z axis */
    } else {
      result = BMI160_E_COM_FAIL;
    }
  } else {
    /* read gyro sensor data along with time */
    result = DFRobot_BMI160::getRegs(BMI160_GYRO_DATA_ADDR, data_array, 12 + len, dev);
    if (result == BMI160_OK) {
      /* Gyro Data */
      lsb = data_array[idx++];
      msb = data_array[idx++];
      msblsb = (int16_t)((msb << 8) | lsb);
      gyro->x = msblsb; /* gyro X axis data */

      lsb = data_array[idx++];
      msb = data_array[idx++];
      msblsb = (int16_t)((msb << 8) | lsb);
      gyro->y = msblsb; /* gyro Y axis data */

      lsb = data_array[idx++];
      msb = data_array[idx++];
      msblsb = (int16_t)((msb << 8) | lsb);
      gyro->z = msblsb; /* gyro Z axis data */

      idx = idx + 6;
      time_0 = data_array[idx++];
      time_1 = (uint16_t)(data_array[idx++] << 8);
      time_2 = (uint32_t)(data_array[idx++] << 16);
      gyro->sensortime = (uint32_t)(time_2 | time_1 | time_0);

    } else {
      result = BMI160_E_COM_FAIL;
    }
  }

  return result;
}

int8_t DFRobot_BMI160::getAccelGyroData(uint8_t len, struct bmi160SensorData *accel, struct bmi160SensorData *gyro, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t idx = 0;
  uint8_t data_array[15] = {0};
  uint8_t time_0 = 0;
  uint16_t time_1 = 0;
  uint32_t time_2 = 0;
  uint8_t lsb;
  uint8_t msb;
  int16_t msblsb;

  /* read both accel and gyro sensor data
   * along with time if requested */
  result = DFRobot_BMI160::getRegs(BMI160_GYRO_DATA_ADDR, data_array, 12 + len, dev);
  if (result == BMI160_OK) {
    /* Gyro Data */
    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    gyro->x = msblsb; /* gyro X axis data */

    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    gyro->y = msblsb; /* gyro Y axis data */

    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    gyro->z = msblsb; /* gyro Z axis data */

    /* Accel Data */
    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    accel->x = (int16_t)msblsb; /* accel X axis data */

    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    accel->y = (int16_t)msblsb; /* accel Y axis data */

    lsb = data_array[idx++];
    msb = data_array[idx++];
    msblsb = (int16_t)((msb << 8) | lsb);
    accel->z = (int16_t)msblsb; /* accel Z axis data */

    if (len == 3) {
      time_0 = data_array[idx++];
      time_1 = (uint16_t)(data_array[idx++] << 8);
      time_2 = (uint32_t)(data_array[idx++] << 16);
      accel->sensortime = (uint32_t)(time_2 | time_1 | time_0);
      gyro->sensortime = (uint32_t)(time_2 | time_1 | time_0);
    } else {
      accel->sensortime = 0;
      gyro->sensortime = 0;
      ;
    }
  } else {
    result = BMI160_E_COM_FAIL;
  }

  return result;
} 

int8_t DFRobot_BMI160::getErrorRegister(uint8_t* code) {
  int8_t result = getRegs(BMI160_ERROR_REG_ADDR, code, 1, Obmi160);
  return result;
}

int8_t DFRobot_BMI160::getBits(uint8_t address, uint8_t bitStart, uint8_t length, uint8_t* data) {
  uint8_t byte;
  int8_t result = getRegs(address, &byte, 1, Obmi160);
  if (result == BMI160_OK) {
    uint8_t mask = ((1 << length) - 1) << bitStart;
    byte &= mask;
    byte >>= bitStart;
    *data = byte;
  }
  return result;
}

// int8_t DFRobot_BMI160::getRegister(uint8_t address, uint8_t data) {
//   return getRegs(address, &data, 1, Obmi160);
// }

int8_t DFRobot_BMI160::getRegs(uint8_t reg_addr, uint8_t *data, uint8_t len, struct bmi160Dev *dev)
{
  
  int8_t result = BMI160_OK;
  //Null-pointer check 
  if (dev == NULL) {
    result = BMI160_E_NULL_PTR;
  } else {
    //Configuring reg_addr for SPI Interface 
    if (dev->interface == BMI160_SPI_INTF){
      reg_addr = (reg_addr | BMI160_SPI_RD_MASK);
      result = DFRobot_BMI160::SPIGetRegs(dev, reg_addr, data, len);
    }else{
      result = DFRobot_BMI160::I2cGetRegs(dev, reg_addr, data, len);
    }
    delay(1);
    if (result != BMI160_OK){
      result = BMI160_E_COM_FAIL;  
    }
  }

  return result;  
}

int8_t DFRobot_BMI160::I2cGetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
  Wire.beginTransmission(dev->comAddress);
  Wire.write(reg_addr);
  Wire.endTransmission(true);
  delay(10);
  Wire.requestFrom(dev->comAddress,len);

  for(int i = 0; i < len; i++){
    data[i]=Wire.read();
    delay(1);
  }
  return BMI160_OK;
}

int8_t DFRobot_BMI160::setRegister(uint8_t address, uint8_t data) {
  return setRegs(address, &data, 1, Obmi160);
}

int8_t DFRobot_BMI160::setBits(uint8_t address, uint8_t bitStart, uint8_t length, uint8_t data) {
  uint8_t byte;
  int8_t result = getRegs(address, &byte, 1, Obmi160);
  if (result == BMI160_OK) {
    uint8_t mask = ((1 << length) - 1) << bitStart;
    data <<= bitStart;
    data &= mask;
    byte &= ~(mask);
    byte |= data;
    result = setRegs(address, &byte, 1, Obmi160);
  }
  return result;
}

int8_t DFRobot_BMI160::setRegs(uint8_t reg_addr, uint8_t *data, uint8_t len, struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  //Null-pointer check
  if (dev == NULL) {
    result = BMI160_E_NULL_PTR;
  } else {
    //Configuring reg_addr for SPI Interface
    if (dev->interface == BMI160_SPI_INTF){
      reg_addr = (reg_addr & BMI160_SPI_WR_MASK);
      result = DFRobot_BMI160::SPISetRegs(dev,reg_addr,data,len);
    }else{
      result = DFRobot_BMI160::I2cSetRegs(dev,reg_addr,data,len);
    }
    delay(1);

    if (result != BMI160_OK)
      result = BMI160_E_COM_FAIL;
  }

  return result;
}

void DFRobot_BMI160::setMagDeviceAddress(uint8_t address) {
  uint8_t addressShifted = address << 1;
  setRegister(BMI160_AUX_IF_0_ADDR, addressShifted);
}

int8_t DFRobot_BMI160::setMagRegister(uint8_t address, uint8_t data) {
  int8_t result;
  result = setRegister(BMI160_AUX_IF_4_ADDR, data);
  if (result == BMI160_OK) {
    result = setRegister(BMI160_AUX_IF_3_ADDR, address);
    if (result == BMI160_OK) {
      delay(3);
      uint8_t buffer = 0x00;
      result = getRegister(BMI160_ERROR_REG_ADDR, buffer);
      if (result == BMI160_OK) {
        if (buffer & BMI160_ERR_MASK_I2C_FAIL) {
          printf("BMI160: mag register proxy write error\n");
          result = BMI160_E_COM_FAIL;
        }
      }
    }
  }
  return result;
}

int8_t DFRobot_BMI160::I2cSetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
  if ((dev->prevAccelCfg.power == BMI160_ACCEL_NORMAL_MODE)||(dev->prevGyroCfg.power == BMI160_GYRO_NORMAL_MODE)){
    Wire.beginTransmission(dev->comAddress);
    Wire.write(reg_addr);
    for(int i = 0; i < len; i++){
      Wire.write(data[i]);
      delay(1);
    }
    Wire.endTransmission(true);
  }else{
    for(int i = 0; i < len; i++){
      Wire.beginTransmission(dev->comAddress);
      Wire.write(reg_addr);
      Wire.write(data[i]);
      
      Wire.endTransmission(true);
      delay(1);
    }
  }
  return BMI160_OK;
}

int8_t DFRobot_BMI160::SPIGetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
  return BMI160_OK;
}
int8_t DFRobot_BMI160::SPISetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
  return BMI160_OK;
}

int8_t DFRobot_BMI160::setInt(int intNum)
{
  return setInt(Obmi160,intNum);  
}

int8_t DFRobot_BMI160::setInt(struct bmi160Dev *dev, int intNum)
{
  int8_t result=BMI160_OK;
  if (dev == NULL){
    result = BMI160_E_NULL_PTR;
  }
  struct bmi160IntSettg intConfig;
  if (intNum == 1){
    intConfig.intChannel = BMI160_INT_CHANNEL_1;
  }else if(intNum == 2){
    intConfig.intChannel = BMI160_INT_CHANNEL_2;
  }else{
    return BMI160_E_NULL_PTR;
  }
  
  /* Select the Interrupt type */
  intConfig.intType = BMI160_STEP_DETECT_INT;// Choosing Step Detector interrupt
  /* Select the interrupt channel/pin settings */
  intConfig.intPinSettg.outputEn = BMI160_ENABLE;// Enabling interrupt pins to act as output pin
  intConfig.intPinSettg.outputMode = BMI160_DISABLE;// Choosing push-pull mode for interrupt pin
  intConfig.intPinSettg.outputType = BMI160_ENABLE;// Choosing active High output
  intConfig.intPinSettg.edgeCtrl = BMI160_ENABLE;// Choosing edge triggered output
  intConfig.intPinSettg.inputEn = BMI160_DISABLE;// Disabling interrupt pin to act as input
  intConfig.intPinSettg.latchDur =BMI160_LATCH_DUR_NONE;// non-latched output

  /* Select the Step Detector interrupt parameters, Kindly use the recommended settings for step detector */
  intConfig.intTypeCfg.accStepDetectInt.stepDetectorMode = BMI160_STEP_DETECT_NORMAL;
  intConfig.intTypeCfg.accStepDetectInt.stepDetectorEn = BMI160_ENABLE;// 1-enable, 0-disable the step detector
  result = DFRobot_BMI160::setIntConfig(&intConfig, dev);
  return result;
}

int8_t DFRobot_BMI160::setIntConfig(struct bmi160IntSettg *intConfig, struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  switch (intConfig->intType) {
  case BMI160_ACC_ANY_MOTION_INT:
    /*Any-motion  interrupt*/
    //result = set_accel_any_motion_int(intConfig, dev);
    break;
  case BMI160_ACC_SIG_MOTION_INT:
    /* Significant motion interrupt */
    //result = set_accel_sig_motion_int(intConfig, dev);
    break;
  case BMI160_ACC_SLOW_NO_MOTION_INT:
    /* Slow or no motion interrupt */
    //result = set_accel_no_motion_int(intConfig, dev);
    break;
  case BMI160_ACC_DOUBLE_TAP_INT:
  case BMI160_ACC_SINGLE_TAP_INT:
     /* Double tap and single tap Interrupt */
    //result = set_accel_tap_int(intConfig, dev);
    break;
  case BMI160_STEP_DETECT_INT:
    /* Step detector interrupt */
    result = setAccelStepDetectInt(intConfig, dev);
    break;
  case BMI160_ACC_ORIENT_INT:
    /* Orientation interrupt */
    //result = set_accel_orientation_int(intConfig, dev);
    break;
  case BMI160_ACC_FLAT_INT:
    /* Flat detection interrupt */
    //result = set_accel_flat_detect_int(intConfig, dev);
    break;
  case BMI160_ACC_LOW_G_INT:
     /* Low-g interrupt */
    //result = set_accel_low_g_int(intConfig, dev);
    break;
  case BMI160_ACC_HIGH_G_INT:
    /* High-g interrupt */
    //result = set_accel_high_g_int(intConfig, dev);
    break;
  case BMI160_ACC_GYRO_DATA_RDY_INT:
    /* Data ready interrupt */
    //result = set_accel_gyro_data_ready_int(intConfig, dev);
    break;
  case BMI160_ACC_GYRO_FIFO_FULL_INT:
     /* Fifo full interrupt */
    //result = set_fifo_full_int(intConfig, dev);
    break;
  case BMI160_ACC_GYRO_FIFO_WATERMARK_INT:
     /* Fifo water-mark interrupt */
    //result = set_fifo_watermark_int(intConfig, dev);
    break;
  default:
    break;
  }
  return result;  
}

int8_t DFRobot_BMI160::setAccelStepDetectInt(struct bmi160IntSettg *intConfig, struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;

  /* Null-pointer check */
  if (dev == NULL){
    result = BMI160_E_NULL_PTR;
  }
  if ((result != BMI160_OK) || (intConfig == NULL)) {
    result = BMI160_E_NULL_PTR;
  } else {
    /* updating the interrupt structure to local structure */
    struct bmi160AccStepDetectIntCfg *stepDetectIntCfg =
                &(intConfig->intTypeCfg.accStepDetectInt);

    result = enableStepDetectInt(stepDetectIntCfg, dev);
    if (result == BMI160_OK) {
      /* Configure Interrupt pins */
      result = setIntrPinConfig(intConfig, dev);
      if (result == BMI160_OK) {
        result = mapFeatureInterrupt(intConfig, dev);
        if (result == BMI160_OK){
          result = configStepDetect(stepDetectIntCfg, dev);
        }
      }
    }
  }
  return result;
}

int8_t DFRobot_BMI160::enableStepDetectInt(struct bmi160AccStepDetectIntCfg *stepDetectIntCfg, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t data = 0;
  uint8_t temp = 0;

  /* Enable data ready interrupt in Int Enable 2 register */
  result = getRegs(BMI160_INT_ENABLE_2_ADDR, &data, 1, dev);
  if (result == BMI160_OK) {
    temp = data & ~BMI160_STEP_DETECT_INT_EN_MASK;
    data = temp | ((stepDetectIntCfg->stepDetectorEn << 3) & BMI160_STEP_DETECT_INT_EN_MASK);
    /* Writing data to INT ENABLE 2 Address */
    result = setRegs(BMI160_INT_ENABLE_2_ADDR, &data, 1, dev);
  }
  return result;
}

int8_t DFRobot_BMI160::setIntrPinConfig(struct bmi160IntSettg *intConfig, struct bmi160Dev *dev)
{
  int8_t result;

  /* configure the behavioural settings of interrupt pin */
  result = configIntOutCtrl(intConfig, dev);
  if (result == BMI160_OK)
    result = configIntLatch(intConfig, dev);

  return result;
}

int8_t DFRobot_BMI160::configIntOutCtrl(struct bmi160IntSettg *intConfig, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t temp = 0;
  uint8_t data = 0;

  /* Configuration of output interrupt signals on pins INT1 and INT2 are
   * done in BMI160_INT_OUT_CTRL_ADDR register*/
  result = getRegs(BMI160_INT_OUT_CTRL_ADDR, &data, 1, dev);

  if (result == BMI160_OK) {
    /* updating the interrupt pin structure to local structure */
    const struct bmi160IntPinSettg *intr_pin_sett = &(intConfig->intPinSettg);

    /* Configuring channel 1 */
    if (intConfig->intChannel == BMI160_INT_CHANNEL_1) {

      /* Output enable */
      temp = data & ~BMI160_INT1_OUTPUT_EN_MASK;
      data = temp | ((intr_pin_sett->outputEn << 3) & BMI160_INT1_OUTPUT_EN_MASK);

      /* Output mode */
      temp = data & ~BMI160_INT1_OUTPUT_MODE_MASK;
      data = temp | ((intr_pin_sett->outputMode << 2) & BMI160_INT1_OUTPUT_MODE_MASK);

      /* Output type */
      temp = data & ~BMI160_INT1_OUTPUT_TYPE_MASK;
      data = temp | ((intr_pin_sett->outputType << 1) & BMI160_INT1_OUTPUT_TYPE_MASK);

      /* edge control */
      temp = data & ~BMI160_INT1_EDGE_CTRL_MASK;
      data = temp | ((intr_pin_sett->edgeCtrl) & BMI160_INT1_EDGE_CTRL_MASK);

    } else {
      /* Configuring channel 2 */
      /* Output enable */
      temp = data & ~BMI160_INT2_OUTPUT_EN_MASK;
      data = temp | ((intr_pin_sett->outputEn << 7) & BMI160_INT2_OUTPUT_EN_MASK);

      /* Output mode */
      temp = data & ~BMI160_INT2_OUTPUT_MODE_MASK;
      data = temp | ((intr_pin_sett->outputMode << 6) & BMI160_INT2_OUTPUT_MODE_MASK);

      /* Output type */
      temp = data & ~BMI160_INT2_OUTPUT_TYPE_MASK;
      data = temp | ((intr_pin_sett->outputType << 5) & BMI160_INT2_OUTPUT_TYPE_MASK);

      /* edge control */
      temp = data & ~BMI160_INT2_EDGE_CTRL_MASK;
      data = temp | ((intr_pin_sett->edgeCtrl << 4) & BMI160_INT2_EDGE_CTRL_MASK);
    }

    result = setRegs(BMI160_INT_OUT_CTRL_ADDR, &data, 1, dev);
  }

  return result;
}

int8_t DFRobot_BMI160::configIntLatch(struct bmi160IntSettg *intConfig, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t temp = 0;
  uint8_t data = 0;

  /* Configuration of latch on pins INT1 and INT2 are done in
   * BMI160_INT_LATCH_ADDR register*/
  result = getRegs(BMI160_INT_LATCH_ADDR, &data, 1, dev);

  if (result == BMI160_OK) {
    /* updating the interrupt pin structure to local structure */
    const struct bmi160IntPinSettg *intr_pin_sett = &(intConfig->intPinSettg);

    if (intConfig->intChannel == BMI160_INT_CHANNEL_1) {
      /* Configuring channel 1 */
      /* Input enable */
      temp = data & ~BMI160_INT1_INPUT_EN_MASK;
      data = temp | ((intr_pin_sett->inputEn << 4) & BMI160_INT1_INPUT_EN_MASK);
    } else {
      /* Configuring channel 2 */
      /* Input enable */
      temp = data & ~BMI160_INT2_INPUT_EN_MASK;
      data = temp | ((intr_pin_sett->inputEn << 5) & BMI160_INT2_INPUT_EN_MASK);
    }

    /* In case of latch interrupt,update the latch duration */
    /* Latching holds the interrupt for the amount of latch
     * duration time */
    temp = data & ~BMI160_INT_LATCH_MASK;
    data = temp | (intr_pin_sett->latchDur & BMI160_INT_LATCH_MASK);

    /* OUT_CTRL_INT and LATCH_INT address lie consecutively,
     * hence writing data to respective registers at one go */
    result = setRegs(BMI160_INT_LATCH_ADDR, &data, 1, dev);
  }
  return result;
}

int8_t DFRobot_BMI160::mapFeatureInterrupt(struct bmi160IntSettg *intConfig, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t data[3] = {0, 0, 0};
  uint8_t temp[3] = {0, 0, 0};

  result = getRegs(BMI160_INT_MAP_0_ADDR, data, 3, dev);

  if (result == BMI160_OK) {
    temp[0] = data[0] & ~int_mask_lookup_table[intConfig->intType];
    temp[2] = data[2] & ~int_mask_lookup_table[intConfig->intType];

    switch (intConfig->intChannel) {
    case BMI160_INT_CHANNEL_NONE:
      data[0] = temp[0];
      data[2] = temp[2];
      break;
    case BMI160_INT_CHANNEL_1:
      data[0] = temp[0] | int_mask_lookup_table[intConfig->intType];
      data[2] = temp[2];
      break;
    case BMI160_INT_CHANNEL_2:
      data[2] = temp[2] | int_mask_lookup_table[intConfig->intType];
      data[0] = temp[0];
      break;
    case BMI160_INT_CHANNEL_BOTH:
      data[0] = temp[0] | int_mask_lookup_table[intConfig->intType];
      data[2] = temp[2] | int_mask_lookup_table[intConfig->intType];
      break;
    default:
      result = BMI160_E_OUT_OF_RANGE;
    }
    if (result == BMI160_OK)
      result = setRegs(BMI160_INT_MAP_0_ADDR, data, 3, dev);
  }

  return result;
}

int8_t DFRobot_BMI160::configStepDetect(struct bmi160AccStepDetectIntCfg *stepDetectIntCfg, struct bmi160Dev *dev)
{
  int8_t result;
  uint8_t temp = 0;
  uint8_t data_array[2] = {0};


  if (stepDetectIntCfg->stepDetectorMode == BMI160_STEP_DETECT_NORMAL) {
    /* Normal mode setting */
    data_array[0] = 0x15;
    data_array[1] = 0x03;
  } else if (stepDetectIntCfg->stepDetectorMode == BMI160_STEP_DETECT_SENSITIVE) {
    /* Sensitive mode setting */
    data_array[0] = 0x2D;
    data_array[1] = 0x00;
  } else if (stepDetectIntCfg->stepDetectorMode == BMI160_STEP_DETECT_ROBUST) {
    /* Robust mode setting */
    data_array[0] = 0x1D;
    data_array[1] = 0x07;
  } else if (stepDetectIntCfg->stepDetectorMode == BMI160_STEP_DETECT_USER_DEFINE) {
    /* Non recommended User defined setting */
    /* Configuring STEP_CONFIG register */
    result = getRegs(BMI160_INT_STEP_CONFIG_0_ADDR, &data_array[0], 2, dev);

    if (result == BMI160_OK) {
      temp = data_array[0] & ~BMI160_STEP_DETECT_MIN_THRES_MASK;
      /* Adding minThreshold */
      data_array[0] = temp | ((stepDetectIntCfg->minThreshold << 3)
          & BMI160_STEP_DETECT_MIN_THRES_MASK);

      temp = data_array[0] & ~BMI160_STEP_DETECT_STEPTIME_MIN_MASK;
      /* Adding steptimeMin */
      data_array[0] = temp | ((stepDetectIntCfg->steptimeMin)
          & BMI160_STEP_DETECT_STEPTIME_MIN_MASK);

      temp = data_array[1] & ~BMI160_STEP_MIN_BUF_MASK;
      /* Adding steptimeMin */
      data_array[1] = temp | ((stepDetectIntCfg->stepMinBuf) & BMI160_STEP_MIN_BUF_MASK);

    }
  }

  /* Write data to STEP_CONFIG register */
  result = setRegs(BMI160_INT_STEP_CONFIG_0_ADDR, data_array, 2, dev);

  return result;
}

int8_t DFRobot_BMI160::setStepPowerMode(uint8_t model)
{
  return DFRobot_BMI160::setStepPowerMode(model,Obmi160);
}

int8_t DFRobot_BMI160::setStepPowerMode(uint8_t model,struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  if (model == stepNormalPowerMode){
    dev->accelCfg.odr=BMI160_ACCEL_ODR_1600HZ;
    dev->accelCfg.power = BMI160_ACCEL_NORMAL_MODE;
    dev->gyroCfg.odr = BMI160_GYRO_ODR_3200HZ;
    dev->gyroCfg.power = BMI160_GYRO_NORMAL_MODE; 
  }else if(model == stepLowPowerMode){
    dev->accelCfg.odr=BMI160_ACCEL_ODR_50HZ;
    dev->accelCfg.power = BMI160_ACCEL_LOWPOWER_MODE;
    dev->gyroCfg.odr = BMI160_GYRO_ODR_50HZ;
    dev->gyroCfg.power = BMI160_GYRO_SUSPEND_MODE; 
  }else{
    dev->accelCfg.odr=BMI160_ACCEL_ODR_1600HZ;
    dev->accelCfg.power = BMI160_ACCEL_NORMAL_MODE;
    dev->gyroCfg.odr = BMI160_GYRO_ODR_3200HZ;
    dev->gyroCfg.power = BMI160_GYRO_NORMAL_MODE; 
  }
  dev->accelCfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
  dev->accelCfg.range = BMI160_ACCEL_RANGE_2G;
  dev->gyroCfg.range = BMI160_GYRO_RANGE_2000_DPS;
  dev->gyroCfg.bw = BMI160_BW_NORMAL_MODE;

  result = DFRobot_BMI160::setAccelConf(dev);
  if (result == BMI160_OK) {
    result = DFRobot_BMI160::setGyroConf(dev);
    if (result == BMI160_OK) {
      /* write power mode for accel and gyro */
      result = DFRobot_BMI160::setPowerMode(dev);
      if (result == BMI160_OK)
        result = DFRobot_BMI160::checkInvalidSettg(dev);
    }
  }

  return result;
}

int8_t DFRobot_BMI160::setStepCounter()
{
  uint8_t step_enable = 1;//enable the step counter
  return DFRobot_BMI160::setStepCounter(step_enable,Obmi160);
}

int8_t DFRobot_BMI160::setStepCounter(uint8_t step_cnt_enable, struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  uint8_t data = 0;

  /* Null-pointer check */
  if (dev == NULL){
    result = BMI160_E_NULL_PTR; 
  }

  if (result != BMI160_OK) {
    result = BMI160_E_NULL_PTR;
  } else {
    result = getRegs(BMI160_INT_STEP_CONFIG_1_ADDR, &data, 1, dev);
    if (result == BMI160_OK) {
      if (step_cnt_enable == BMI160_ENABLE){
        data |= (uint8_t)(step_cnt_enable << 3);
      }else{
        data &= ~BMI160_STEP_COUNT_EN_BIT_MASK;
      }
      result = setRegs(BMI160_INT_STEP_CONFIG_1_ADDR, &data, 1, dev);
    }
  }

  return result;
}

int8_t DFRobot_BMI160::readStepCounter(uint16_t *stepVal)
{
  return readStepCounter(stepVal,Obmi160);
}

int8_t DFRobot_BMI160::readStepCounter(uint16_t *stepVal, struct bmi160Dev *dev)
{
  int8_t result = BMI160_OK;
  uint8_t data[2] = {0, 0};
  uint16_t msb = 0;
  uint8_t lsb = 0;

  /* Null-pointer check */
  if (dev == NULL){
    result = BMI160_E_NULL_PTR;
  }

  if (result != BMI160_OK) {
    result = BMI160_E_NULL_PTR;
  } else {
    result = getRegs(BMI160_INT_STEP_CNT_0_ADDR, data, 2, dev);
    if (result == BMI160_OK) {
      lsb = data[0];
      msb = data[1] << 8;
      *stepVal = msb | lsb;
    }
  }

  return result; 
}

void DFRobot_BMI160::resetFIFO() {
  setRegister(BMI160_COMMAND_REG_ADDR, BMI160_FIFO_FLUSH_VALUE);
}

void DFRobot_BMI160::setFIFOHeaderModeEnabled(bool enabled) {
  setBits(BMI160_FIFO_CONFIG_1_ADDR, BMI160_FIFO_HEADER_ENABLE_BIT, 1, enabled ? 0x1 : 0);
}

bool DFRobot_BMI160::getFIFOHeaderModeEnabled() {
  uint8_t buffer;
  getBits(BMI160_FIFO_CONFIG_1_ADDR, BMI160_FIFO_HEADER_ENABLE_BIT, 1, &buffer);
  return !!buffer;
}

void DFRobot_BMI160::setGyroFIFOEnabled(bool enabled) {
  setBits(BMI160_FIFO_CONFIG_1_ADDR, BMI160_FIFO_GYRO_ENABLE_BIT, 1, enabled ? 0x1 : 0);
}

bool DFRobot_BMI160::getGyroFIFOEnabled() {
  uint8_t buffer;
  getBits(BMI160_FIFO_CONFIG_1_ADDR, BMI160_FIFO_GYRO_ENABLE_BIT, 1, &buffer);
  return !!buffer;  
}

void DFRobot_BMI160::setAccelFIFOEnabled(bool enabled) {
  setBits(BMI160_FIFO_CONFIG_1_ADDR, BMI160_FIFO_ACCEL_ENABLE_BIT, 1, enabled ? 0x1 : 0);  
}

bool DFRobot_BMI160::getAccelFIFOEnabled() {
  uint8_t buffer;
  getBits(BMI160_FIFO_CONFIG_1_ADDR, BMI160_FIFO_ACCEL_ENABLE_BIT, 1, &buffer);
  return !!buffer;  
}

int8_t DFRobot_BMI160::getFIFOLength(uint16_t* count) {
  uint8_t* buffer = new uint8_t[2];
  int8_t result = getRegs(BMI160_FIFO_LENGTH_ADDR, buffer, 2, Obmi160);
  if (result == BMI160_OK) {
    *count = (((int16_t)buffer[1]) << 8) | buffer[0];
  }
  return result;
}

int8_t DFRobot_BMI160::getFIFOBytes(uint8_t* data, uint16_t length) {
  int8_t result = BMI160_OK;
  if (length) {
    result = getRegs(BMI160_FIFO_DATA_ADDR, data, length, Obmi160);
  }
  return result;
}

bool DFRobot_BMI160::getGyroDirty() {
  uint8_t buffer;
  getBits(BMI160_STATUS_ADDR, BMI160_STATUS_DIRTY_GYRO, 1, &buffer);
  return buffer;
}

void DFRobot_BMI160::waitForGyroDirty() {
  do {
    if (!getGyroDirty()) {
      delayMicroseconds(150);
    }
  } while (!getGyroDirty());
}


/*!
  @file DFRobot_BMI160.h
  @brief Define the basic structure of class DFRobot_BMI160 
  @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author  DFRobot_haoJ(hao.jiang@dfrobot.com)
  @version V1.0
  @date 2017-12-01
  @url https://github.com/DFRobot/DFRobot_BMI160
*/

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

//Mask definitions
#define BMI160_ACCEL_BW_MASK                    UINT8_C(0x70)
#define BMI160_ACCEL_ODR_MASK                   UINT8_C(0x0F)
#define BMI160_ACCEL_UNDERSAMPLING_MASK         UINT8_C(0x80)
#define BMI160_ACCEL_RANGE_MASK                 UINT8_C(0x0F)
#define BMI160_GYRO_BW_MASK                     UINT8_C(0x30)
#define BMI160_GYRO_ODR_MASK                    UINT8_C(0x0F)
#define BMI160_GYRO_RANGE_MSK                   UINT8_C(0x07)

//Mask definitions for FIFO_CONFIG register
#define BMI160_FIFO_GYRO                        UINT8_C(0x80)
#define BMI160_FIFO_ACCEL                       UINT8_C(0x40)
#define BMI160_FIFO_AUX                         UINT8_C(0x20)
#define BMI160_FIFO_TAG_INT1                    UINT8_C(0x08)
#define BMI160_FIFO_TAG_INT2                    UINT8_C(0x04)
#define BMI160_FIFO_TIME                        UINT8_C(0x02)
#define BMI160_FIFO_HEADER                      UINT8_C(0x10)
#define BMI160_FIFO_CONFIG_1_MASK               UINT8_C(0xFE)

//Mask definition for FIFO Header Data Tag
#define BMI160_FIFO_TAG_INTR_MASK               UINT8_C(0xFC)

//Fifo byte counter mask definitions
#define BMI160_FIFO_BYTE_COUNTER_MASK           UINT8_C(0x07)

//Enable/disable bit value
#define BMI160_ENABLE                           UINT8_C(0x01)
#define BMI160_DISABLE                          UINT8_C(0x00)

//BMI160 Register map
#define BMI160_CHIP_ID_ADDR                     UINT8_C(0x00)
#define BMI160_ERROR_REG_ADDR                   UINT8_C(0x02)
#define BMI160_AUX_DATA_ADDR                    UINT8_C(0x04)
#define BMI160_GYRO_DATA_ADDR                   UINT8_C(0x0C)
#define BMI160_ACCEL_DATA_ADDR                  UINT8_C(0x12)
#define BMI160_SENSOR_TIME_ADDR                 UINT8_C(0x18)
#define BMI160_STATUS_ADDR                      UINT8_C(0x1B)
#define BMI160_INT_STATUS_ADDR                  UINT8_C(0x1C)
#define BMI160_TEMPERATURE_ADDR                 UINT8_C(0x20)
#define BMI160_FIFO_LENGTH_ADDR                 UINT8_C(0x22)
#define BMI160_FIFO_DATA_ADDR                   UINT8_C(0x24)
#define BMI160_ACCEL_CONFIG_ADDR                UINT8_C(0x40)
#define BMI160_ACCEL_RANGE_ADDR                 UINT8_C(0x41)
#define BMI160_GYRO_CONFIG_ADDR                 UINT8_C(0x42)
#define BMI160_GYRO_RANGE_ADDR                  UINT8_C(0x43)
#define BMI160_AUX_ODR_ADDR                     UINT8_C(0x44)
#define BMI160_FIFO_DOWN_ADDR                   UINT8_C(0x45)
#define BMI160_FIFO_CONFIG_0_ADDR               UINT8_C(0x46)
#define BMI160_FIFO_CONFIG_1_ADDR               UINT8_C(0x47)
#define BMI160_AUX_IF_0_ADDR                    UINT8_C(0x4B)
#define BMI160_AUX_IF_1_ADDR                    UINT8_C(0x4C)
#define BMI160_AUX_IF_2_ADDR                    UINT8_C(0x4D)
#define BMI160_AUX_IF_3_ADDR                    UINT8_C(0x4E)
#define BMI160_AUX_IF_4_ADDR                    UINT8_C(0x4F)
#define BMI160_INT_ENABLE_0_ADDR                UINT8_C(0x50)
#define BMI160_INT_ENABLE_1_ADDR                UINT8_C(0x51)
#define BMI160_INT_ENABLE_2_ADDR                UINT8_C(0x52)
#define BMI160_INT_OUT_CTRL_ADDR                UINT8_C(0x53)
#define BMI160_INT_LATCH_ADDR                   UINT8_C(0x54)
#define BMI160_INT_MAP_0_ADDR                   UINT8_C(0x55)
#define BMI160_INT_MAP_1_ADDR                   UINT8_C(0x56)
#define BMI160_INT_MAP_2_ADDR                   UINT8_C(0x57)
#define BMI160_INT_DATA_0_ADDR                  UINT8_C(0x58)
#define BMI160_INT_DATA_1_ADDR                  UINT8_C(0x59)
#define BMI160_INT_LOWHIGH_0_ADDR               UINT8_C(0x5A)
#define BMI160_INT_LOWHIGH_1_ADDR               UINT8_C(0x5B)
#define BMI160_INT_LOWHIGH_2_ADDR               UINT8_C(0x5C)
#define BMI160_INT_LOWHIGH_3_ADDR               UINT8_C(0x5D)
#define BMI160_INT_LOWHIGH_4_ADDR               UINT8_C(0x5E)
#define BMI160_INT_MOTION_0_ADDR                UINT8_C(0x5F)
#define BMI160_INT_MOTION_1_ADDR                UINT8_C(0x60)
#define BMI160_INT_MOTION_2_ADDR                UINT8_C(0x61)
#define BMI160_INT_MOTION_3_ADDR                UINT8_C(0x62)
#define BMI160_INT_TAP_0_ADDR                   UINT8_C(0x63)
#define BMI160_INT_TAP_1_ADDR                   UINT8_C(0x64)
#define BMI160_INT_ORIENT_0_ADDR                UINT8_C(0x65)
#define BMI160_INT_ORIENT_1_ADDR                UINT8_C(0x66)
#define BMI160_INT_FLAT_0_ADDR                  UINT8_C(0x67)
#define BMI160_INT_FLAT_1_ADDR                  UINT8_C(0x68)
#define BMI160_FOC_CONF_ADDR                    UINT8_C(0x69)
#define BMI160_CONF_ADDR                        UINT8_C(0x6A)
#define BMI160_IF_CONF_ADDR                     UINT8_C(0x6B)
#define BMI160_SELF_TEST_ADDR                   UINT8_C(0x6D)
#define BMI160_OFFSET_ADDR                      UINT8_C(0x71)
#define BMI160_OFFSET_CONF_ADDR                 UINT8_C(0x77)
#define BMI160_INT_STEP_CNT_0_ADDR              UINT8_C(0x78)
#define BMI160_INT_STEP_CONFIG_0_ADDR           UINT8_C(0x7A)
#define BMI160_INT_STEP_CONFIG_1_ADDR           UINT8_C(0x7B)
#define BMI160_COMMAND_REG_ADDR                 UINT8_C(0x7E)
#define BMI160_SPI_COMM_TEST_ADDR               UINT8_C(0x7F)
#define BMI160_INTL_PULLUP_CONF_ADDR            UINT8_C(0x85)

//Mag setup definitions
#define BMI160_MAG_SETUP_MODE                   UINT8_C(0x80)
#define BMI160_MAG_DATA_MODE_2                  UINT8_C(0x01)
#define BMI160_MAG_DATA_MODE_6                  UINT8_C(0x02)
#define BMI160_MAG_DATA_MODE_8                  UINT8_C(0x03)

//Pullup enable registers
#define BMI160_EN_PULL_UP_REG_1                 UINT8_C(0x37)
#define BMI160_EN_PULL_UP_REG_2                 UINT8_C(0x9A)
#define BMI160_EN_PULL_UP_REG_3                 UINT8_C(0xC0)
#define BMI160_EN_PULL_UP_REG_4                 UINT8_C(0x90)
#define BMI160_EN_PULL_UP_REG_5                 UINT8_C(0x80)

//If config mode definitons
#define BMI160_IF_CONF_MODE_PRI_AUTO_SEC_OFF    0 << 4
#define BMI160_IF_CONF_MODE_PRI_I2C_SEC_OIS     1 << 4
#define BMI160_IF_CONF_MODE_PRI_AUTO_SEC_MAG    2 << 4

//Error code definitions
#define BMI160_OK                               INT8_C(0)
#define BMI160_E_NULL_PTR                       INT8_C(-1)
#define BMI160_E_COM_FAIL                       INT8_C(-2)
#define BMI160_E_DEV_NOT_FOUND                  INT8_C(-3)
#define BMI160_E_OUT_OF_RANGE                   INT8_C(-4)
#define BMI160_E_INVALID_INPUT                  INT8_C(-5)
#define BMI160_E_ACCEL_ODR_BW_INVALID           INT8_C(-6)
#define BMI160_E_GYRO_ODR_BW_INVALID            INT8_C(-7)
#define BMI160_E_LWP_PRE_FLTR_INT_INVALID       INT8_C(-8)
#define BMI160_E_LWP_PRE_FLTR_INVALID           INT8_C(-9)
#define BMI160_E_AUX_NOT_FOUND                  INT8_C(-10)
#define BMI160_FOC_FAILURE                      INT8_C(-11)
#define BMI160_ERR_CHOOSE                       INT8_C(-12)

//name API warning codes
#define BMI160_W_GYRO_SELF_TEST_FAIL            INT8_C(1)
#define BMI160_W_ACCEl_SELF_TEST_FAIL           INT8_C(2)

//BMI160 unique chip identifier
#define BMI160_CHIP_ID                          UINT8_C(0xD1)

//Soft reset command
#define BMI160_SOFT_RESET_CMD                   UINT8_C(0xb6)
#define BMI160_SOFT_RESET_DELAY_MS              UINT8_C(15)

//Start FOC command
#define BMI160_START_FOC_CMD                    UINT8_C(0x03)

//NVM backup enabling command
#define BMI160_NVM_BACKUP_EN                    UINT8_C(0xA0)

//Delay in ms settings
#define BMI160_ACCEL_DELAY_MS                   UINT8_C(5)
#define BMI160_GYRO_DELAY_MS                    UINT8_C(81)
#define BMI160_ONE_MS_DELAY                     UINT8_C(1)
#define BMI160_AUX_COM_DELAY                    UINT8_C(10)
#define BMI160_GYRO_SELF_TEST_DELAY             UINT8_C(20)
#define BMI160_ACCEL_SELF_TEST_DELAY            UINT8_C(50)

//Self test configurations
#define BMI160_ACCEL_SELF_TEST_CONFIG           UINT8_C(0x2C)
#define BMI160_ACCEL_SELF_TEST_POSITIVE_EN      UINT8_C(0x0D)
#define BMI160_ACCEL_SELF_TEST_NEGATIVE_EN      UINT8_C(0x09)
#define BMI160_ACCEL_SELF_TEST_LIMIT            UINT16_C(8192)

//Power mode settings
//Accel power mode
#define BMI160_ACCEL_NORMAL_MODE          UINT8_C(0x11)
#define BMI160_ACCEL_LOWPOWER_MODE        UINT8_C(0x12)
#define BMI160_ACCEL_SUSPEND_MODE         UINT8_C(0x10)

//Gyro power mode
#define BMI160_GYRO_SUSPEND_MODE          UINT8_C(0x14)
#define BMI160_GYRO_NORMAL_MODE           UINT8_C(0x15)
#define BMI160_GYRO_FASTSTARTUP_MODE      UINT8_C(0x17)

//Aux power mode
#define BMI160_AUX_SUSPEND_MODE           UINT8_C(0x18)
#define BMI160_AUX_NORMAL_MODE            UINT8_C(0x19)
#define BMI160_AUX_LOWPOWER_MODE          UINT8_C(0x1A)

//Range settings
//Accel Range
#define BMI160_ACCEL_RANGE_2G             UINT8_C(0x03)
#define BMI160_ACCEL_RANGE_4G             UINT8_C(0x05)
#define BMI160_ACCEL_RANGE_8G             UINT8_C(0x08)
#define BMI160_ACCEL_RANGE_16G            UINT8_C(0x0C)

//Gyro Range
#define BMI160_GYRO_RANGE_2000_DPS        UINT8_C(0x00)
#define BMI160_GYRO_RANGE_1000_DPS        UINT8_C(0x01)
#define BMI160_GYRO_RANGE_500_DPS         UINT8_C(0x02)
#define BMI160_GYRO_RANGE_250_DPS         UINT8_C(0x03)
#define BMI160_GYRO_RANGE_125_DPS         UINT8_C(0x04)

//Bandwidth settings
//Accel Bandwidth
#define BMI160_ACCEL_BW_OSR4_AVG1         UINT8_C(0x00)
#define BMI160_ACCEL_BW_OSR2_AVG2         UINT8_C(0x01)
#define BMI160_ACCEL_BW_NORMAL_AVG4       UINT8_C(0x02)
#define BMI160_ACCEL_BW_RES_AVG8          UINT8_C(0x03)
#define BMI160_ACCEL_BW_RES_AVG16         UINT8_C(0x04)
#define BMI160_ACCEL_BW_RES_AVG32         UINT8_C(0x05)
#define BMI160_ACCEL_BW_RES_AVG64         UINT8_C(0x06)
#define BMI160_ACCEL_BW_RES_AVG128        UINT8_C(0x07)

//Filter mode definitions
#define BMI160_BW_OSR4_MODE               UINT8_C(0x00)
#define BMI160_BW_OSR2_MODE               UINT8_C(0x01)
#define BMI160_BW_NORMAL_MODE             UINT8_C(0x02)

//Output Data Rate settings
//Accel Output data rate
#define BMI160_ACCEL_ODR_RESERVED         UINT8_C(0x00)
#define BMI160_ACCEL_ODR_0_78HZ           UINT8_C(0x01)
#define BMI160_ACCEL_ODR_1_56HZ           UINT8_C(0x02)
#define BMI160_ACCEL_ODR_3_12HZ           UINT8_C(0x03)
#define BMI160_ACCEL_ODR_6_25HZ           UINT8_C(0x04)
#define BMI160_ACCEL_ODR_12_5HZ           UINT8_C(0x05)
#define BMI160_ACCEL_ODR_25HZ             UINT8_C(0x06)
#define BMI160_ACCEL_ODR_50HZ             UINT8_C(0x07)
#define BMI160_ACCEL_ODR_100HZ            UINT8_C(0x08)
#define BMI160_ACCEL_ODR_200HZ            UINT8_C(0x09)
#define BMI160_ACCEL_ODR_400HZ            UINT8_C(0x0A)
#define BMI160_ACCEL_ODR_800HZ            UINT8_C(0x0B)
#define BMI160_ACCEL_ODR_1600HZ           UINT8_C(0x0C)
#define BMI160_ACCEL_ODR_RESERVED0        UINT8_C(0x0D)
#define BMI160_ACCEL_ODR_RESERVED1        UINT8_C(0x0E)
#define BMI160_ACCEL_ODR_RESERVED2        UINT8_C(0x0F)

//Gyro Output data rate
#define BMI160_GYRO_ODR_RESERVED          UINT8_C(0x00)
#define BMI160_GYRO_ODR_25HZ              UINT8_C(0x06)
#define BMI160_GYRO_ODR_50HZ              UINT8_C(0x07)
#define BMI160_GYRO_ODR_100HZ             UINT8_C(0x08)
#define BMI160_GYRO_ODR_200HZ             UINT8_C(0x09)
#define BMI160_GYRO_ODR_400HZ             UINT8_C(0x0A)
#define BMI160_GYRO_ODR_800HZ             UINT8_C(0x0B)
#define BMI160_GYRO_ODR_1600HZ            UINT8_C(0x0C)
#define BMI160_GYRO_ODR_3200HZ            UINT8_C(0x0D)

//Auxiliary sensor Output data rate
typedef enum {
  BMI160_AUX_ODR_RESERVED,        
  BMI160_AUX_ODR_0_78HZ,       // 25/32 HZ   
  BMI160_AUX_ODR_1_56HZ,       // 25/16 HZ   
  BMI160_AUX_ODR_3_12HZ,       //  25/8 HZ   
  BMI160_AUX_ODR_6_25HZ,       //  25/4 HZ
  BMI160_AUX_ODR_12_5HZ,       //  25/2 HZ  
  BMI160_AUX_ODR_25HZ,             
  BMI160_AUX_ODR_50HZ,           
  BMI160_AUX_ODR_100HZ,             
  BMI160_AUX_ODR_200HZ,              
  BMI160_AUX_ODR_400HZ,           
  BMI160_AUX_ODR_800HZ,      
} BMI160MagRate;

//Maximum limits definition
#define BMI160_ACCEL_ODR_MAX              UINT8_C(15)
#define BMI160_ACCEL_BW_MAX               UINT8_C(2)
#define BMI160_ACCEL_RANGE_MAX            UINT8_C(12)
#define BMI160_GYRO_ODR_MAX               UINT8_C(13)
#define BMI160_GYRO_BW_MAX                UINT8_C(2)
#define BMI160_GYRO_RANGE_MAX             UINT8_C(4)

//FIFO_CONFIG Definitions
#define BMI160_FIFO_TIME_ENABLE           UINT8_C(0x02)
#define BMI160_FIFO_TAG_INT2_ENABLE       UINT8_C(0x04)
#define BMI160_FIFO_TAG_INT1_ENABLE       UINT8_C(0x08)
#define BMI160_FIFO_HEAD_ENABLE           UINT8_C(0x10)
#define BMI160_FIFO_M_ENABLE              UINT8_C(0x20)
#define BMI160_FIFO_A_ENABLE              UINT8_C(0x40)
#define BMI160_FIFO_M_A_ENABLE            UINT8_C(0x60)
#define BMI160_FIFO_G_ENABLE              UINT8_C(0x80)
#define BMI160_FIFO_M_G_ENABLE            UINT8_C(0xA0)
#define BMI160_FIFO_G_A_ENABLE            UINT8_C(0xC0)
#define BMI160_FIFO_M_G_A_ENABLE          UINT8_C(0xE0)

//Accel, gyro and aux. sensor length and also their combined
//length definitions in FIFO
#define BMI160_FIFO_G_LENGTH              UINT8_C(6)
#define BMI160_FIFO_A_LENGTH              UINT8_C(6)
#define BMI160_FIFO_M_LENGTH              UINT8_C(8)
#define BMI160_FIFO_GA_LENGTH             UINT8_C(12)
#define BMI160_FIFO_MA_LENGTH             UINT8_C(14)
#define BMI160_FIFO_MG_LENGTH             UINT8_C(14)
#define BMI160_FIFO_MGA_LENGTH            UINT8_C(20)

//FIFO Header Data definitions
#define BMI160_FIFO_HEAD_SKIP_FRAME       UINT8_C(0x40)
#define BMI160_FIFO_HEAD_SENSOR_TIME      UINT8_C(0x44)
#define BMI160_FIFO_HEAD_INPUT_CONFIG     UINT8_C(0x48)
#define BMI160_FIFO_HEAD_OVER_READ        UINT8_C(0x80)
#define BMI160_FIFO_HEAD_A                UINT8_C(0x84)
#define BMI160_FIFO_HEAD_G                UINT8_C(0x88)
#define BMI160_FIFO_HEAD_G_A              UINT8_C(0x8C)
#define BMI160_FIFO_HEAD_M                UINT8_C(0x90)
#define BMI160_FIFO_HEAD_M_A              UINT8_C(0x94)
#define BMI160_FIFO_HEAD_M_G              UINT8_C(0x98)
#define BMI160_FIFO_HEAD_M_G_A            UINT8_C(0x9C)

//FIFO flag definitions
#define BMI160_FIFO_HEADER_DATA_FRAME_FLAG_M            1 << 4 // 0b00 0 100  00
#define BMI160_FIFO_HEADER_DATA_FRAME_FLAG_G            1 << 3 // 0b00 0 010  00
#define BMI160_FIFO_HEADER_DATA_FRAME_FLAG_A            1 << 2 // 0b00 0 001  00
#define BMI160_FIFO_HEADER_DATA_FRAME_MASK_HAS_DATA \
       (BMI160_FIFO_HEADER_DATA_FRAME_FLAG_M |\
        BMI160_FIFO_HEADER_DATA_FRAME_FLAG_G |\
        BMI160_FIFO_HEADER_DATA_FRAME_FLAG_A)

//FIFO enable bits
#define BMI160_FIFO_HEADER_ENABLE_BIT     UINT8_C(4)
#define BMI160_FIFO_ACCEL_ENABLE_BIT      UINT8_C(6)
#define BMI160_FIFO_GYRO_ENABLE_BIT       UINT8_C(7)

//FIFO sensor time length definitions
#define BMI160_FIFO_SKIP_FRAME_LENGTH     UINT8_C(1)
#define BMI160_FIFO_INPUT_CONFIG_LENGTH   UINT8_C(1)
#define BMI160_FIFO_SENSOR_TIME_LENGTH    UINT8_C(3)

//FIFO DOWN selection
//Accel fifo down-sampling values
#define  BMI160_ACCEL_FIFO_DOWN_ZERO      UINT8_C(0x00)
#define  BMI160_ACCEL_FIFO_DOWN_ONE       UINT8_C(0x10)
#define  BMI160_ACCEL_FIFO_DOWN_TWO       UINT8_C(0x20)
#define  BMI160_ACCEL_FIFO_DOWN_THREE     UINT8_C(0x30)
#define  BMI160_ACCEL_FIFO_DOWN_FOUR      UINT8_C(0x40)
#define  BMI160_ACCEL_FIFO_DOWN_FIVE      UINT8_C(0x50)
#define  BMI160_ACCEL_FIFO_DOWN_SIX       UINT8_C(0x60)
#define  BMI160_ACCEL_FIFO_DOWN_SEVEN     UINT8_C(0x70)

//Gyro fifo down-smapling values
#define  BMI160_GYRO_FIFO_DOWN_ZERO       UINT8_C(0x00)
#define  BMI160_GYRO_FIFO_DOWN_ONE        UINT8_C(0x01)
#define  BMI160_GYRO_FIFO_DOWN_TWO        UINT8_C(0x02)
#define  BMI160_GYRO_FIFO_DOWN_THREE      UINT8_C(0x03)
#define  BMI160_GYRO_FIFO_DOWN_FOUR       UINT8_C(0x04)
#define  BMI160_GYRO_FIFO_DOWN_FIVE       UINT8_C(0x05)
#define  BMI160_GYRO_FIFO_DOWN_SIX        UINT8_C(0x06)
#define  BMI160_GYRO_FIFO_DOWN_SEVEN      UINT8_C(0x07)

//Accel Fifo filter enable
#define  BMI160_ACCEL_FIFO_FILT_EN        UINT8_C(0x80)

//Gyro Fifo filter enable
#define  BMI160_GYRO_FIFO_FILT_EN         UINT8_C(0x08)

//Definitions to check validity of FIFO frames
#define FIFO_CONFIG_MSB_CHECK             UINT8_C(0x80)
#define FIFO_CONFIG_LSB_CHECK             UINT8_C(0x00)

//BMI160 accel FOC configurations
#define BMI160_FOC_ACCEL_DISABLED         UINT8_C(0x00)
#define BMI160_FOC_ACCEL_POSITIVE_G       UINT8_C(0x01)
#define BMI160_FOC_ACCEL_NEGATIVE_G       UINT8_C(0x02)
#define BMI160_FOC_ACCEL_0G               UINT8_C(0x03)

//Array Parameter DefinItions
#define BMI160_SENSOR_TIME_LSB_BYTE       UINT8_C(0)
#define BMI160_SENSOR_TIME_XLSB_BYTE      UINT8_C(1)
#define BMI160_SENSOR_TIME_MSB_BYTE       UINT8_C(2)

//Interface settings
#define BMI160_SPI_INTF                   UINT8_C(1)
#define BMI160_I2C_INTF                   UINT8_C(0)
#define BMI160_SPI_RD_MASK                UINT8_C(0x80)
#define BMI160_SPI_WR_MASK                UINT8_C(0x7F)

//Sensor & time select definition
#define BMI160_ACCEL_SEL                  UINT8_C(0x01)
#define BMI160_GYRO_SEL                   UINT8_C(0x02)
#define BMI160_TIME_SEL                   UINT8_C(0x04)

//Sensor select mask
#define BMI160_SEN_SEL_MASK               UINT8_C(0x07)

//Error code mask
#define BMI160_ERR_REG_MASK               UINT8_C(0x0F)
#define BMI160_ERR_MASK_MAG_DRDY_ERR         0x10000000
#define BMI160_ERR_MASK_DROP_CMD_ERR         0x01000000
#define BMI160_ERR_MASK_I2C_FAIL             0x00100000
#define BMI160_ERR_MASK_ERROR_CODE           0x00011110
#define BMI160_ERR_MASK_CHIP_NOT_OPERABLE    0x00000001

//BMI160 I2C address
#define BMI160_I2C_ADDR                   UINT8_C(0x68)

//BMI160 secondary IF address
#define BMI160_AUX_BMM150_I2C_ADDR        UINT8_C(0x10)

//BMI160 Length definitions
#define BMI160_ONE                        UINT8_C(1)
#define BMI160_TWO                        UINT8_C(2)
#define BMI160_THREE                      UINT8_C(3)
#define BMI160_FOUR                       UINT8_C(4)
#define BMI160_FIVE                       UINT8_C(5)

//BMI160 fifo level Margin
#define BMI160_FIFO_LEVEL_MARGIN          UINT8_C(16)

//BMI160 fifo flush Command
#define BMI160_FIFO_FLUSH_VALUE           UINT8_C(0xB0)

//BMI160 offset values for xyz axes of accel
#define BMI160_ACCEL_MIN_OFFSET           INT8_C(-128)
#define BMI160_ACCEL_MAX_OFFSET           INT8_C(127)

//BMI160 offset values for xyz axes of gyro
#define BMI160_GYRO_MIN_OFFSET            INT16_C(-512)
#define BMI160_GYRO_MAX_OFFSET            INT16_C(511)

//BMI160 fifo full interrupt position and mask
#define BMI160_FIFO_FULL_INT_POS          UINT8_C(5)
#define BMI160_FIFO_FULL_INT_MSK          UINT8_C(0x20)
#define BMI160_FIFO_WTM_INT_POS           UINT8_C(6)
#define BMI160_FIFO_WTM_INT_MSK           UINT8_C(0x40)

#define BMI160_FIFO_FULL_INT_PIN1_POS     UINT8_C(5)
#define BMI160_FIFO_FULL_INT_PIN1_MSK     UINT8_C(0x20)
#define BMI160_FIFO_FULL_INT_PIN2_POS     UINT8_C(1)
#define BMI160_FIFO_FULL_INT_PIN2_MSK     UINT8_C(0x02)

#define BMI160_FIFO_WTM_INT_PIN1_POS      UINT8_C(6)
#define BMI160_FIFO_WTM_INT_PIN1_MSK      UINT8_C(0x40)
#define BMI160_FIFO_WTM_INT_PIN2_POS      UINT8_C(2)
#define BMI160_FIFO_WTM_INT_PIN2_MSK      UINT8_C(0x04)

#define BMI160_MANUAL_MODE_EN_POS         UINT8_C(7)
#define BMI160_MANUAL_MODE_EN_MSK         UINT8_C(0x80)
#define BMI160_AUX_READ_BURST_POS         UINT8_C(0)
#define BMI160_AUX_READ_BURST_MSK         UINT8_C(0x03)

#define BMI160_GYRO_SELF_TEST_POS         UINT8_C(4)
#define BMI160_GYRO_SELF_TEST_MSK         UINT8_C(0x10)
#define BMI160_GYRO_SELF_TEST_STATUS_POS  UINT8_C(1)
#define BMI160_GYRO_SELF_TEST_STATUS_MSK  UINT8_C(0x02)

#define BMI160_GYRO_FOC_EN_POS            UINT8_C(6)
#define BMI160_GYRO_FOC_EN_MSK            UINT8_C(0x40)

#define BMI160_ACCEL_FOC_X_CONF_POS       UINT8_C(4)
#define BMI160_ACCEL_FOC_X_CONF_MSK       UINT8_C(0x30)

#define BMI160_ACCEL_FOC_Y_CONF_POS       UINT8_C(2)
#define BMI160_ACCEL_FOC_Y_CONF_MSK       UINT8_C(0x0C)

#define BMI160_ACCEL_FOC_Z_CONF_MSK       UINT8_C(0x03)

#define BMI160_FOC_STATUS_POS             UINT8_C(3)
#define BMI160_FOC_STATUS_MSK             UINT8_C(0x08)

#define BMI160_GYRO_OFFSET_X_MSK          UINT8_C(0x03)

#define BMI160_GYRO_OFFSET_Y_POS          UINT8_C(2)
#define BMI160_GYRO_OFFSET_Y_MSK          UINT8_C(0x0C)

#define BMI160_GYRO_OFFSET_Z_POS          UINT8_C(4)
#define BMI160_GYRO_OFFSET_Z_MSK          UINT8_C(0x30)

#define BMI160_GYRO_OFFSET_EN_POS         UINT8_C(7)
#define BMI160_GYRO_OFFSET_EN_MSK         UINT8_C(0x80)

#define BMI160_ACCEL_OFFSET_EN_POS        UINT8_C(6)
#define BMI160_ACCEL_OFFSET_EN_MSK        UINT8_C(0x40)

#define BMI160_GYRO_OFFSET_POS            UINT16_C(8)
#define BMI160_GYRO_OFFSET_MSK            UINT16_C(0x0300)

#define BMI160_NVM_UPDATE_POS             UINT8_C(1)
#define BMI160_NVM_UPDATE_MSK             UINT8_C(0x02)

#define BMI160_NVM_STATUS_POS             UINT8_C(4)
#define BMI160_NVM_STATUS_MSK             UINT8_C(0x10)

//Dirty sensor defines
#define BMI160_STATUS_DIRTY_GYRO          UINT8_C(6)

//BIT SLICE GET AND SET FUNCTIONS
#define  BMI160_GET_BITS(regvar, bitname)\
    ((regvar & bitname##_MSK) >> bitname##_POS)
#define BMI160_SET_BITS(regvar, bitname, val)\
    ((regvar & ~bitname##_MSK) | \
    ((val<<bitname##_POS)&bitname##_MSK))

#define BMI160_SET_BITS_POS_0(reg_data, bitname, data) \
        ((reg_data & ~(bitname##_MSK)) | \
        (data & bitname##_MSK))

#define BMI160_GET_BITS_POS_0(reg_data, bitname)  (reg_data & (bitname##_MSK))

//UTILITY MACROS
#define BMI160_SET_LOW_BYTE     UINT16_C(0x00FF)
#define BMI160_SET_HIGH_BYTE    UINT16_C(0xFF00)

#define BMI160_GET_LSB(var) (uint8_t)(var & BMI160_SET_LOW_BYTE)
#define BMI160_GET_MSB(var) (uint8_t)((var & BMI160_SET_HIGH_BYTE) >> 8)

// Structure for holding FIFO usage data
struct bmi160FifoFrame {
  uint8_t *data;              // Data buffer of user defined length is to be mapped here
  uint16_t length;            // Number of bytes in FIFO to be read, Number of FIFO data bytes available
  uint8_t  fifoTimeEnable;     // FIFO time enable
  uint8_t  fifoHeaderEnable;   // Enabling of the FIFO header to stream in header mode
  uint8_t  fifoDataEnable;     // Streaming of the Accelerometer, Gyroscope sensor data or both in FIFO
  uint16_t accelByteStartIdx; // Will be equal to length when no more frames are there to parse
  uint16_t gyroByteStartIdx;  // Will be equal to length when no more frames are there to parse
  uint16_t auxByteStartIdx;   // Will be equal to length when no more frames are there to parse
  uint32_t sensorTime;        // Value of FIFO sensor time time
  uint8_t  skippedFrameCount;  // Value of Skipped frame counts
};

// Enumerator for selecting which BMI160 sensor to read
// 1 - read only the accelerometer
// 2 - read only the gyro
// 3 - read both
enum eBmi160SelectSensor {
  eBmi160AccelOnly = 1,
  eBmi160GyroOnly,
  eBmi160BothAccelAndGyro
};

//Structure to hold bmi160 sensor configuration
struct bmi160Cfg {
  uint8_t power;  // power mode
  uint8_t odr;    // output data rate
  uint8_t range;  // range
  uint8_t bw;     // bandwidth
};

//Structure to hold Aux config parameters
struct bmi160AuxCfg {
  uint8_t auxSensorEnable : 1;   // 1 - enable 0 - disable
  uint8_t manualEnable : 1;      // Aux manual/auto mode status
  uint8_t auxRdBurstLength : 2;  // Aux read burst length
  uint8_t auxOdr :4;             // Output data rate
  uint8_t auxI2cAddress;         // I2C address of auxiliary sensor
};

//type definitions
typedef int8_t (*bmi160ComFptrT)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef void (*bmi160DelayFptrT)(uint32_t period);

//Structure for storing bmi160 instance setings
struct bmi160Dev {
  uint8_t chipId;                 // Value read from Chip ID register
  uint8_t comAddress;             // I2C address or SPI select pin
  uint8_t interface;              // 0 - I2C,   1 - SPI Interface
  struct bmi160Cfg accelCfg;      // Structure to configure Accel sensor
  struct bmi160Cfg prevAccelCfg;  // Keep old accelerometer config seperate to avoid overwritting
  struct bmi160Cfg gyroCfg;       // Structure to configure Gyro sensor
  struct bmi160Cfg prevGyroCfg;   // Keep old gyro config seperate to avoid overwritting
  struct bmi160AuxCfg auxCfg;     // Structure to configure Aux sensor
  struct bmi160AuxCfg prevAuxCfg; // Keep old aux config seperate to avoid overwritting
  struct bmi160FifoFrame *fifo;   
  bmi160ComFptrT read;            // Read function pointer
  bmi160ComFptrT write;           // Write function pointer
  bmi160DelayFptrT delayMs;       // Delay function pointer
};

//Structure for storing gyro or accelerometer data
struct bmi160SensorData {
  int16_t x;           // X-axis sensor data
  int16_t y;           // Y-axis sensor data
  int16_t z;           // Z-axis sensor data
  uint32_t sensortime; // Sensor time
};

class DFRobot_BMI160{
public:
  DFRobot_BMI160();
  
  // Read the Chip ID
  // @return Chip ID
  uint8_t getChipID();

  // Initialize the BMI160
  // @param address BMI160 I2C address or BMI160 select pin
  // @n     0x68: connect SDIO to ground (default)
  // @n     0x69: set I2C address by parameter
  // @n      0-3: index in SPI select pin array
  // @param accelRate
  // @param accelRange
  // @param accelFilterMode
  // @param gyroRate
  // @param gyroRange
  // @param gyroFilterMode
  // @return BMI160_OK(0) means success
  int8_t setSensorConfig(uint8_t address, 
                     uint8_t accelRate, uint8_t accelRange, uint8_t accelFilterMode,
                     uint8_t gyroRate, uint8_t gyroRange, uint8_t gyroFilterMode);

  // Set the I2C address and initialize I2C
  // @param i2c_addr  bmi160 i2c addr
  // @n     0x68: connect SDIO to ground (default)
  // @n     0x69: set I2C address by parameter
  // @return BMI160_OK(0) means success
  int8_t I2cInit(int8_t i2c_addr = BMI160_I2C_ADDR);

  // Get the current sensor time
  // @param time storage object for getting the data in
  // @return BMI160_OK(0) means success
  int8_t getSensorTime(uint32_t* time);

  // Get the temperature data
  // @param output storage object for getting the data in
  // @return BMI160_OK(0) means success
  int8_t getTemperature(int16_t* output);
  
  // Select mode and save returned data to parameter data
  // @param type
  // @n     onlyAccel - only get the accel data.
  // @n     onlyGyro - only get the gyro data.
  // @n     bothAccelGyro - get both accel and gyro data.
  // @param data  save returned data to parameter data
  // @return BMI160_OK(0) means success
  int8_t getSensorData(uint8_t type,int16_t* data);
  
  // Get the accel data 
  // @param data pointer to store the accel data
  // @return BMI160_OK(0) means success
  int8_t getAccelData(int16_t* data);
  
  // Get the accel data 
  // @param data pointer to store the accel data
  // @param timestamp pointer to store the timestamp for accel
  // @return BMI160_OK(0) means success
  int8_t getAccelData(int16_t* data, uint32_t* timestamp);
  
  // Get the gyro data 
  // @param data pointer to store the gyro data
  // @return BMI160_OK(0) means success
  int8_t getGyroData(int16_t* data);
  
  // Get the gyro data 
  // @param data pointer to store the gyro data
  // @param timestamp pointer to store the timestamp for gryo
  // @return BMI160_OK(0) means success
  int8_t getGyroData(int16_t* data, uint32_t* timestamp);
  
  // Get the accel and gyro data 
  // @param data pointer to store the accel and gyro data
  // @return BMI160_OK(0) means success
  int8_t getAccelGyroData(int16_t* data);

  // Get the accel and gyro data 
  // @param data pointer to store the accel and gyro data
  // @param timestamp pointer to store the timestamp for accel and gyro
  // @return BMI160_OK(0) means success
  int8_t getAccelGyroData(int16_t* data, uint32_t* timestamp);

  // Reset bmi160 hardware
  // @return BMI160_OK(0) means success
  int8_t softReset();

  // Set a specified register in the BMI160
  // @param address register to set
  // @param data byte to set in the register
  // @return BMI160_OK(0) means success
  int8_t setRegister(uint8_t address, uint8_t data);

  // Read a specified register in the BMI160
  // @param address register to read
  // @param data storage object to capture read byte
  // @return BMI160_OK(0) means success
  int8_t getRegister(uint8_t address, uint8_t data);

  // Set specific bits in the target register
  // @param address register to set bits in
  // @param bitStart bit position in the register to start on
  // @param length number of bits to set
  // @param data byte containting the bits to set
  // @return BMI160_OK(0) means success
  int8_t setBits(uint8_t address, uint8_t bitStart, uint8_t length, uint8_t data);

  // Read specifi bits in the target register
  // @param address register to get bits from
  // @param bitStart bit position in the register to start on
  // @param length number of bits to get
  // @param data storage object to get the bits in
  // @return BMI160_OK(0) means success
  int8_t getBits(uint8_t address, uint8_t bitStart, uint8_t length, uint8_t* data);

  // Read the error code register
  // @param code storage object to get error code in
  // @return BMI160_OK(0) means success
  int8_t getErrorRegister(uint8_t* code);

  // Set magnetometer (aux) register value
  // @param address register to set
  // @param data to set in the register
  // @return BMI160_OK(0) means success
  int8_t setMagRegister(uint8_t address, uint8_t data);

  // Set mag device address
  // @param address to store in the mag address register
  void setMagDeviceAddress(uint8_t address);

  // Clear FIFO
  void resetFIFO();

  // Set FIFO Header-Mode enabled status
  // @param enabled New status
  void setFIFOHeaderModeEnabled(bool enabled);

  // Get FIFO Header-Mode enabled status
  // @return current FIFO Header-Mode enabled status
  bool getFIFOHeaderModeEnabled();

  // Set Gyro FIFO enabled status
  // @param enabled New status
  void setGyroFIFOEnabled(bool enabled);

  // Get Gyro FIFO enabled status
  // @param enabled New status
  bool getGyroFIFOEnabled();

  // Set Accel FIFO enabled status
  // @param enabled New status
  void setAccelFIFOEnabled(bool enabled);

  // Get Accel FIFO enabled status
  // @param enabled New status
  bool getAccelFIFOEnabled();

  // Get length of the FIFO buffer
  // @param count storage object to get the FIFO length in
  // @return BMI160_OK(0) means success
  int8_t getFIFOLength(uint16_t* count);

  // Get data from the FIFO buffer
  // @param data storage object to get the FIFO data in
  // @param length size of the buffer
  // @return BMI160_OK(0) means success
  int8_t getFIFOBytes(uint8_t* data, uint16_t length);

  // Get gyro dirty status
  // @return gyro dirty status
  bool getGyroDirty();

  // wait until gyro dirty status
  void waitForGyroDirty();

  uint8_t onlyAccel=1;
  uint8_t onlyGyro=2;
  uint8_t bothAccelGyro=3;
  uint8_t stepNormalPowerMode=0;
  uint8_t stepLowPowerMode=1;

  private:
    int8_t I2cInit(struct bmi160Dev *dev);
    int8_t SPIInit();
    int8_t SPIInit(struct bmi160Dev *dev);
    
    int8_t softReset(struct bmi160Dev *dev);
    void   defaultParamSettg(struct bmi160Dev *dev);
    
    int8_t setSensorConfig(struct bmi160Dev *dev);
    
    int8_t setAccelConf(struct bmi160Dev *dev);
    int8_t checkAccelConfig(uint8_t *data, struct bmi160Dev *dev);
    int8_t processAccelOdr(uint8_t *data, struct bmi160Dev *dev);
    int8_t processAccelBw(uint8_t *data, struct bmi160Dev *dev);
    int8_t processAccelRange(uint8_t *data, struct bmi160Dev *dev);

    int8_t setGyroConf(struct bmi160Dev *dev);
    int8_t checkGyroConfig(uint8_t *data, struct bmi160Dev *dev);
    int8_t processGyroOdr(uint8_t *data, struct bmi160Dev *dev);
    int8_t processGyroBw(uint8_t *data, struct bmi160Dev *dev);
    int8_t processGyroRange(uint8_t *data, struct bmi160Dev *dev);

    int8_t setPowerMode(struct bmi160Dev *dev);
    int8_t setAccelPwr(struct bmi160Dev *dev);
    int8_t processUnderSampling(uint8_t *data, struct bmi160Dev *dev);
    int8_t setGyroPwr(struct bmi160Dev *dev);

    int8_t checkInvalidSettg( struct bmi160Dev *dev);

    int8_t getSensorData(uint8_t select_sensor, struct bmi160SensorData *accel, struct bmi160SensorData *gyro,struct bmi160Dev *dev);
    int8_t getAccelData(uint8_t len, struct bmi160SensorData *accel, struct bmi160Dev *dev);
    int8_t getGyroData(uint8_t len, struct bmi160SensorData *gyro, struct bmi160Dev *dev);
    int8_t getAccelGyroData(uint8_t len, struct bmi160SensorData *accel, struct bmi160SensorData *gyro, struct bmi160Dev *dev);

    int8_t getRegs(uint8_t reg_addr, uint8_t * data, uint8_t len, struct bmi160Dev *dev);
    int8_t setRegs(uint8_t reg_addr, uint8_t * data, uint8_t len, struct bmi160Dev *dev);

    int8_t I2cGetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len);
    int8_t I2cSetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len);

    int8_t SPIGetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len);
    int8_t SPISetRegs(struct bmi160Dev *dev, uint8_t reg_addr, uint8_t *data, uint8_t len);

    struct bmi160Dev* Obmi160;
    struct bmi160SensorData* Oaccel;
    struct bmi160SensorData* Ogyro; 
};

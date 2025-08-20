/**
 * @file    ADXL375.h
 * @brief   Header file for ADXL375 SPI accelerometer driver
 * @author  Nathan Netzel
 * @date    Jul 28, 2025
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Nathan Netzel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the conditions of the MIT License.
 */

#ifndef INC_ADXL375_H_
#define INC_ADXL375_H_


#include "stm32f4xx_hal.h"


// --- Registers ---
#define ADXL_ANALOG_DEVICES_ID_REG			0x00
#define ADXL_THRESH_SHOCK					0x1D
#define ADXL_OFFSET_X						0x1E
#define ADXL_OFFSET_Y						0x1F
#define ADXL_OFFSET_Z						0x20
#define ADXL_DUR							0x21
#define ADXL_LATENT							0x22
#define ADXL_WINDOW							0x23
#define ADXL_THRESH_ACT						0x24
#define ADXL_THRESH_INACT					0x25
#define ADXL_TIME_INACT						0x26
#define ADXL_ACT_INACT_CTL					0x27
#define ADXL_SHOCK_AXES						0x2A
#define ADXL_ACT_SHOCK_STATUS				0x2B
#define ADXL_BW_RATE						0x2C
#define ADXL_POWER_CTL						0x2D
#define ADXL_INT_ENABLE						0x2E
#define ADXL_INT_MAP						0x2F
#define ADXL_INT_SOURCE						0x30
#define ADXL_DATA_FORMAT					0x31
#define ADXL_DATA_ADDRESS_INIT				0x32
#define ADXL_DATAX0							0x32
#define ADXL_DATAX1							0x33
#define ADXL_DATAY0							0x34
#define ADXL_DATAY1 						0x35
#define ADXL_DATAZ0							0x36
#define ADXL_DATAZ1							0x37
#define ADXL_FIFO_CTL						0x38
#define ADXL_FIFO_STATUS					0x39

// --- Device ID ---
#define ADXL_DEVICE_ID						0xE5

// --- Output Data Rates ---
#define ADXL_ODR_3200		0x0F
#define ADXL_ODR_1600		0x0E
#define ADXL_ODR_0800		0x0D
#define ADXL_ODR_0400		0x0C
#define ADXL_ODR_0200		0x0B
#define ADXL_ODR_0100		0x0A
#define ADXL_ODR_0050		0x09
#define ADXL_ODR_0025		0x08
#define ADXL_ODR_12_5		0x07
#define ADXL_ODR_6_25		0x06
#define ADXL_ODR_3_13		0x05
#define ADXL_ODR_1_56		0x04
#define ADXL_ODR_0_78		0x03
#define ADXL_ODR_0_39		0x02
#define ADXL_ODR_0_20		0x01
#define ADXL_ODR_0_10		0x00

// --- Interrupt Polarity ---
#define ADXL_INT_ACTIVEHIGH		0
#define ADXL_INT_ACTIVELOW  	1

// --- Operation Modes ---
#define ADXL_LOWPOWMODE			0x10
#define ADXL_MEASUREMODE 		0x08
#define ADXL_STANDBYMODE 		0x00

// --- Reset Command ---
#define ADXL_RESET			0x52

// --- Interrupt Map ---
#define ADXL_INT_DATA_READY		0x80
#define ADXL_INT1_DATA_READY	0x00

// --- SPI Commands ---
#define ADXL_Read_Command			0x80
#define ADXL_Read_Multiple_Command	0xC0
#define ADXL_Write_command			0x00

// --- SPI Timeout (ms) ---
#define ADXL_Timeout	10

// --- Error Status Enum ---
typedef enum {
    ADXL_NO_ERROR            = 0x00U, /**< No error */
    ADXL_ERROR               = 0x01U, /**< Generic error */
    ADXL_HAL_ERROR           = 0x02U, /**< HAL communication error */
    ADXL_INV_REG_DATA_READ   = 0x03U, /**< Invalid register data read */
    ADXL_ID_VERIFY_ERROR     = 0x04U  /**< Device ID verification failed */
} ADXL375_Error_TypeDef;

// --- Hardware Initialization Structure ---
typedef struct {
    SPI_HandleTypeDef *SPIhandler;  /**< Pointer to SPI handler */
    GPIO_TypeDef *CS_GPIOport;      /**< GPIO port for chip select */
    uint16_t CS_GPIOpin;            /**< GPIO pin number for chip select */
    uint8_t SPI_Timeout;            /**< SPI timeout in milliseconds */
} ADXL375_HW_InitTypeDef;

/**
 * @brief Union for accelerometer axis data
 *
 * Contains 16-bit signed values for X, Y, and Z axes or raw 6-byte buffer.
 * Low byte (x0) comes before high byte (x1) in the register map.
 */
typedef union {
    struct {
        int16_t DX;
        int16_t DY;
        int16_t DZ;
    } axes;
    uint8_t raw_data[6];
} ADXL375_Data_TypeDef;

// --- Function Prototypes ---

/**
 * @brief  Set the ADXL375 output data rate
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @param  ADXL375_Rate Data rate value (see datasheet)
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Set_Output_DataRate(ADXL375_HW_InitTypeDef *, uint8_t);

/**
 * @brief  Enable continuous measurement mode
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Set_Continuous_Measurement(ADXL375_HW_InitTypeDef *);

/**
 * @brief  Enable measurement via data-ready interrupt
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @param  ADXL375_Data Pointer to output data structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Set_Interrupt_Measurement(ADXL375_HW_InitTypeDef *, ADXL375_Data_TypeDef *);

/**
 * @brief  Verify device ID of ADXL375
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_ID_Verify(ADXL375_HW_InitTypeDef *);

/**
 * @brief  Read accelerometer data from sensor
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @param  ADXL375_Data Pointer to output data structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Data_Read(ADXL375_HW_InitTypeDef *, ADXL375_Data_TypeDef *);

/**
 * @brief  Read single register from ADXL375
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @param  ADXL375_Register_Addr Address of the register
 * @param  pData Pointer to variable to store result
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_SingleRegister_Read(ADXL375_HW_InitTypeDef *, uint8_t, uint8_t *);

/**
 * @brief  Write single register in ADXL375
 * @param  ADXL375_Handler Pointer to hardware init structure
 * @param  ADXL375_Register_Addr Address of the register
 * @param  ADXL375_Write_Data 8-bit data to write
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_SingleRegister_Write(ADXL375_HW_InitTypeDef *, uint8_t, uint8_t);

/**
 * @brief  Enable chip select (CS) for SPI communication
 * @param  CS_GPIOport GPIO port of CS
 * @param  CS_GPIOpin GPIO pin of CS
 */
void enableCS_ADXL375(GPIO_TypeDef *, uint16_t);

/**
 * @brief  Disable chip select (CS) for SPI communication
 * @param  CS_GPIOport GPIO port of CS
 * @param  CS_GPIOpin GPIO pin of CS
 */
void disableCS_ADXL375(GPIO_TypeDef *, uint16_t);


#endif /* INC_ADXL375_H_ */

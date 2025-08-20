/*
 * ADXL375.c
 *
 * Created on: Jul 28, 2025
 * Author: Nathan Netzel
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


#include <ADXL375.h>


/* Private SPI Handler */



/**
 * @brief  Read the ADXL375 ID register and check its content
 * @note   Returns ADXL_NO_ERROR only if content matches the expected value
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_ID_Verify(ADXL375_HW_InitTypeDef *ADXL375_Handler){
	uint8_t ID_Receive=0;

	ADXL375_Error_TypeDef Verify_Status = ADXL375_SingleRegister_Read(ADXL375_Handler, ADXL_ANALOG_DEVICES_ID_REG, &ID_Receive);
	if ( (ID_Receive==ADXL_DEVICE_ID) && (Verify_Status==ADXL_NO_ERROR) )
		return ADXL_NO_ERROR;
	else
		return ADXL_ID_VERIFY_ERROR;
}



/**
 * @brief  Set the ADXL375 output data rate
 * @note   Writes directly into the ADXL_BW_RATE register
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @param  ADXL375_Rate 8-bit value of the DataRate (see datasheet)
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Set_Output_DataRate(ADXL375_HW_InitTypeDef *ADXL375_Handler, uint8_t ADXL375_Rate){

	uint8_t ADXL_RateReg_Return = 0;
	ADXL375_SingleRegister_Write(ADXL375_Handler, ADXL_BW_RATE, ADXL375_Rate);
	ADXL375_SingleRegister_Read(ADXL375_Handler, ADXL_BW_RATE, &ADXL_RateReg_Return);

	if( ADXL_RateReg_Return != ADXL375_Rate)
		return ADXL_INV_REG_DATA_READ;
	else
		return ADXL_NO_ERROR;
}

/**
 * @brief  Set the ADXL375 to continuous measurement mode
 * @note   Writes 0x08 into the ADXL_POWER_CTL register
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Set_Continuous_Measurement(ADXL375_HW_InitTypeDef *ADXL375_Handler){

	uint8_t ADXL_MeasureReg_Return = 0;
	ADXL375_SingleRegister_Write(ADXL375_Handler, ADXL_POWER_CTL, ADXL_MEASUREMODE);
	ADXL375_SingleRegister_Read(ADXL375_Handler, ADXL_POWER_CTL, &ADXL_MeasureReg_Return);

	if(ADXL_MeasureReg_Return!=ADXL_MEASUREMODE)
		return ADXL_INV_REG_DATA_READ;
	else
		return ADXL_NO_ERROR;
}

/**
 * @brief  Set the ADXL375 interrupt configuration for data ready on INT1
 * @note   Writes to ADXL_INT_MAP and ADXL_INT_ENABLE and clears any pending interrupt
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @param  ADXL375_Data Pointer to the ADXL375_Data_TypeDef structure used to clear interrupt
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Set_Interrupt_Measurement(ADXL375_HW_InitTypeDef *ADXL375_Handler, ADXL375_Data_TypeDef *ADXL375_Data){
	uint8_t ADXL_IntMapReg_Return = 0;
	uint8_t ADXL_IntEnableReg_Return = 0;

	// Rout Data Ready to INT1
	ADXL375_SingleRegister_Write(ADXL375_Handler, ADXL_INT_MAP, ADXL_INT1_DATA_READY);
	ADXL375_SingleRegister_Read(ADXL375_Handler, ADXL_INT_MAP, &ADXL_IntMapReg_Return);

	// Enable Data Ready Interupt
	ADXL375_SingleRegister_Write(ADXL375_Handler, ADXL_INT_ENABLE, ADXL_INT_DATA_READY);
	ADXL375_SingleRegister_Read(ADXL375_Handler, ADXL_INT_ENABLE, &ADXL_IntEnableReg_Return);

	// Read Data to Clear Interrupt
	ADXL375_Data_Read(ADXL375_Handler, ADXL375_Data);

	if(ADXL_IntMapReg_Return!= ADXL_INT1_DATA_READY ||  ADXL_IntEnableReg_Return != ADXL_INT_DATA_READY)
		return ADXL_INV_REG_DATA_READ;
	else
		return ADXL_NO_ERROR;
}


/**
 * @brief  Acquire the X, Y and Z axis data from the sensor last conversion
 * @note   Performs a sequential read of 6 bytes starting at data address
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @param  ADXL375_Data Pointer to the ADXL375_Data_TypeDef structure
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_Data_Read(ADXL375_HW_InitTypeDef *ADXL375_Handler, ADXL375_Data_TypeDef *ADXL375_Data){

	uint8_t SPITransmitData_ADXL;

	SPITransmitData_ADXL = (ADXL_Read_Multiple_Command | ADXL_DATA_ADDRESS_INIT);

	enableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(ADXL375_Handler->SPIhandler, &SPITransmitData_ADXL, 1, ADXL375_Handler->SPI_Timeout) != HAL_OK)
	{
		disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
		return ADXL_HAL_ERROR;
	}
	if(HAL_SPI_Receive(ADXL375_Handler->SPIhandler, ADXL375_Data->raw_data, 6, ADXL375_Handler->SPI_Timeout) != HAL_OK)
	{
		disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
		return ADXL_HAL_ERROR;
	}
	disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);

	return ADXL_NO_ERROR;
}

/**
 * @brief  Perform a single register write operation
 * @note   None
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @param  ADXL375_Register_Addr Register address to write
 * @param  ADXL375_Write_Data 8-bit data to be written
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_SingleRegister_Write(ADXL375_HW_InitTypeDef *ADXL375_Handler, uint8_t ADXL375_Register_Addr, uint8_t ADXL375_Write_Data)
{
	uint8_t SPITransmitData_ADXL;
	SPITransmitData_ADXL = (ADXL_Write_command | ADXL375_Register_Addr);

	enableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(ADXL375_Handler->SPIhandler, &SPITransmitData_ADXL, 1, ADXL375_Handler->SPI_Timeout) != HAL_OK)
	{
		disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
		return ADXL_HAL_ERROR;
	}
	if(HAL_SPI_Transmit(ADXL375_Handler->SPIhandler, &ADXL375_Write_Data, 1, ADXL375_Handler->SPI_Timeout) != HAL_OK)
	{
		disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
		return ADXL_HAL_ERROR;
	}
	disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);

	return ADXL_NO_ERROR;
}

/**
 * @brief  Perform a single register read operation
 * @param  ADXL375_Handler Pointer to the ADXL375_HW_InitTypeDef structure
 * @param  ADXL375_Register_Addr 8-bit register address
 * @param  pData Pointer to variable to store read data
 * @retval ADXL375_Error_TypeDef
 */
ADXL375_Error_TypeDef ADXL375_SingleRegister_Read(ADXL375_HW_InitTypeDef *ADXL375_Handler, uint8_t ADXL375_Register_Addr, uint8_t *pData)
{
	uint8_t SPITransmitData_ADXL;
	SPITransmitData_ADXL = (ADXL_Read_Command | ADXL375_Register_Addr);

	enableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(ADXL375_Handler->SPIhandler, &SPITransmitData_ADXL, 1, ADXL375_Handler->SPI_Timeout) != HAL_OK)
	{
		disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
		return ADXL_HAL_ERROR;
	}
	if(HAL_SPI_Receive(ADXL375_Handler->SPIhandler, pData, 1, ADXL375_Handler->SPI_Timeout) != HAL_OK)
	{
		disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);
		return ADXL_HAL_ERROR;
	}
	disableCS_ADXL375(ADXL375_Handler->CS_GPIOport, ADXL375_Handler->CS_GPIOpin);

	return ADXL_NO_ERROR;
}

/**
 * @brief  Enables the chip select pin for the specified SPI Chip Select
 * @param  CS_GPIOport Chip select GPIO port address
 * @param  CS_GPIOpin Chip select GPIO pin number
 * @retval None
 */
void enableCS_ADXL375(GPIO_TypeDef *CS_GPIOport, uint16_t CS_GPIOpin){
  HAL_GPIO_WritePin(CS_GPIOport, CS_GPIOpin, GPIO_PIN_RESET);
}

/**
 * @brief  Disables the chip select pin for the specified SPI Chip Select
 * @param  CS_GPIOport Chip select GPIO port address
 * @param  CS_GPIOpin Chip select GPIO pin number
 * @retval None
 */
void disableCS_ADXL375(GPIO_TypeDef *CS_GPIOport, uint16_t CS_GPIOpin){
  HAL_GPIO_WritePin(CS_GPIOport, CS_GPIOpin, GPIO_PIN_SET);
  //CS_GPIOport->BSRR = CS_GPIOpin;
}

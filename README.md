# ADXL375 STM32 HAL Driver

C driver library for the ADXL375 high-g 3-axis accelerometer sensor.  
Designed for STM32 microcontrollers using the STM32 HAL library and SPI interface.

---

## **Features**

- Verify device ID  
- Configure output data rate and measurement range  
- Enable continuous measurement mode  
- Read acceleration data from X, Y, Z axes  
- SPI communication with chip select control  
- Basic error handling  

---

## **Requirements**

- STM32 MCU with STM32 HAL library  
- SPI hardware interface connected to ADXL375 sensor  
- Basic experience with embedded C and STM32CubeIDE or similar  

---

**Note:**  
This driver includes `#include "stm32h5xx_hal.h"` in the header files and is configured for the STM32H5 series.  
If you intend to reuse the code for other STM32 families, please adjust the HAL include and any hardware-specific settings accordingly.

## **Quick Start**

1. Add driver files to your project

Include `ADXL375.c` and `ADXL375.h` in your STM32 project source folder.

2. Configure your hardware

Set up SPI peripheral and GPIO pins for SPI and Chip Select (CS) according to your board schematic.

3. Initialize driver handle structure

```c
ADXL375_HW_InitTypeDef ADXL375_Handle = {
    .SPIhandler = &hspi1,             // Your SPI handle configured in your project
    .CS_GPIOport = GPIOA,             // GPIO port of CS pin
    .CS_GPIOpin = GPIO_PIN_4,         // GPIO pin of CS
    .SPI_Timeout = 100                // SPI timeout in milliseconds
};
```

4. Verify sensor connection

```c
if (ADXL375_ID_Verify(&ADXL375_Handle) != ADXL_NO_ERROR) {
    // Handle error: sensor not detected
}
```

5. Configure sensor settings

```c
ADXL375_Set_Output_DataRate(&ADXL375_Handle, ADXL_RATE_100HZ);
ADXL375_Set_Range(&ADXL375_Handle, ADXL_RANGE_200G);
ADXL375_Set_Continuous_Measurement(&ADXL375_Handle);
```

6. Read acceleration data

```c
ADXL375_Data_TypeDef accel_data;
if (ADXL375_Data_Read(&ADXL375_Handle, &accel_data) == ADXL_NO_ERROR) {
    int16_t x = accel_data.axes.DX;
    int16_t y = accel_data.axes.DY;
    int16_t z = accel_data.axes.DZ;
    // Process acceleration data
}
```

---

## **API Overview**

- `ADXL375_ID_Verify()` — Verify sensor presence  
- `ADXL375_Set_Output_DataRate()` — Set output data rate  
- `ADXL375_Set_Range()` — Set measurement range  
- `ADXL375_Set_Continuous_Measurement()` — Enable continuous measurement  
- `ADXL375_Data_Read()` — Read acceleration data X, Y, Z  
- `ADXL375_SingleRegister_Read()` and `Write()` — Low-level register access  

---

## **License**

This project is licensed under the MIT License. See the LICENSE file for details.

---

## **Author**

Nathan Netzel  
Electrical Engineering Student - Londrina State University


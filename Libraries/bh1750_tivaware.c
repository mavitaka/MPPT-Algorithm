//*****************************************************************************
//
// vector.c - Functions for performing vector operations.
//
// Copyright (c) 2012-2015 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.2.111 of the Tiva Firmware Development Package.
//
//*****************************************************************************

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "i2c_function.h"

//*****************************************************************************
//
//! \addtogroup vector_api
//! @{
//
//*****************************************************************************
uint16_t GetLightIntensity(void){
uint16_t Intensity_value;
 Intensity_value = readI2C0(Device_Address_H);
 Intensity_value <<= 8;
 Intensity_value |= readI2C0(Device_Address_H);
 Intensity_value = Intensity_value/1.2;
 return Intensity_value;
 }
 
//*****************************************************************************
//
//! Computes the dot product of two vectors.
//!
//! \param pfVectorIn1 is the first vector.
//! \param pfVectorIn2 is the second vector.
//!
//! This function computes the dot product of two 3-dimensional vector.
//!
//! \return Returns the dot product of the two vectors.
//
//*****************************************************************************
void initI2C0(void);
void writeI2C0(uint16_t device_address, uint8_t device_data);
uint8_t readI2C0(uint16_t device_address);
uint16_t GetLightIntensity(void);
uint16_t lux;
void main(void) {
	initI2C0();
	writeI2C0(Device_Address_H,Power_On);
	writeI2C0(Device_Address_H,Continuous_H_resolution_Mode);
	while(1)
	{
	lux = GetLightIntensity();
	SysCtlDelay(SysCtlClockGet()/6);
	}
}

//*****************************************************************************
//
//! Computes the cross product of two vectors.
//!
//! \param pfVectorOut is the output vector.
//! \param pfVectorIn1 is the first vector.
//! \param pfVectorIn2 is the second vector.
//!
//! This function computes the cross product of two 3-dimensional vectors.
//!
//! \return None.
//
//*****************************************************************************
void initI2C0(void)
{
	//This function is for eewiki and is to be updated to handle any port

	//enable I2C module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	//reset I2C module
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

	//enable GPIO peripheral that contains I2C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);

	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

	// Enable and initialize the I2C0 master module.  Use the system clock for
	// the I2C0 module.  The last parameter sets the I2C data transfer rate.
	// If false the data rate is set to 100kbps and if true the data rate will
	// be set to 400kbps.
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

	//clear I2C FIFOs
	HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

//*****************************************************************************
//
//! Scales a vector.
//!
//! \param pfVectorOut is the output vector.
//! \param pfVectorIn is the input vector.
//! \param fScale is the scale factor.
//!
//! This function scales a 3-dimensional vector by multiplying each of its
//! components by the scale factor.
//!
//! \return None.
//
//*****************************************************************************
uint8_t readI2C0(uint16_t device_address)
{
	//read from the specified slave device
	I2CMasterSlaveAddrSet(I2C0_BASE, device_address, true);
	//wait for MCU to finish transaction
	while(I2CMasterBusBusy(I2C0_BASE));
	//send control byte and read from the register from the MCU
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	//wait while checking for MCU to complete the transaction
	while(I2CMasterBusy(I2C0_BASE));
	//Get the data from the MCU register and return to caller
	return(I2CMasterDataGet(I2C0_BASE));
}

//*****************************************************************************
//
//! Adds two vectors.
//!
//! \param pfVectorOut is the output vector.
//! \param pfVectorIn1 is the first vector.
//! \param pfVectorIn2 is the second vector.
//!
//! This function adds two 3-dimensional vectors.
//!
//! \return None.
//
//*****************************************************************************
void writeI2C0(uint16_t device_address, uint8_t device_data)
{
	//specify that we want to communicate to device address with an intended write to bus
	I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);
	//wait for MCU to finish transaction
	while(I2CMasterBusBusy(I2C0_BASE));
	//specify data to be written to the above mentioned device_register
	I2CMasterDataPut(I2C0_BASE, device_data);
	//send control byte and register address byte to slave device
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	//wait for MCU & device to complete transaction
	while(I2CMasterBusy(I2C0_BASE));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

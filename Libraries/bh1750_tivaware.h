//*****************************************************************************
//
// vector.h - Prototypes for the vector functions.
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

#ifndef __I2C_FUNCTION_H__
#define __I2C_FUNCTION_H__

#define Device_Address_L 0x23 // Device address when address pin LOW
#define Device_Address_H 0x5C // Device address when address pin HIGH
//all command here taken from Data sheet OPECODE Table page 5
#define Power_Down 0x00

#define Power_On 0x01

#define Reset 0x07

#define Continuous_H_resolution_Mode  0x10

#define Continuous_H_resolution_Mode2  0x11

#define Continuous_L_resolution_Mode  0x13

#define OneTime_H_resolution_Mode  0x20

#define OneTime_H_resolution_Mode2  0x21

#define OneTime_L_resolution_Mode  0x23//As well as address value

//#define AddrPin 63 // Address pin enable

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Prototypes.
//
//*****************************************************************************
void initI2C0(void);
void writeI2C0(uint16_t device_address, uint8_t device_data);
uint8_t readI2C0(uint16_t device_address);
uint16_t GetLightIntensity(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __SENSORLIB_VECTOR_H__

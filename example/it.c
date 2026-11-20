/**
 ******************************************************************************
 * @file 	it.c
 * @author 	Can Gulmez
 * @brief 	The interrupt service routines of the application
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Can Gulmez.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "main.h"

void SysTick_Handler(void) 
{
   HAL_IncTick();
}

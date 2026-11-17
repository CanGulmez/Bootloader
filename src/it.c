/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	The interrupt requests of the bootloader
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

#include "main.h"

void SysTick_Handler(void) 
{
   HAL_IncTick();
}

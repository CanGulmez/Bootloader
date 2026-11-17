/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	The core implementation of the bootloader
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

void erase_sectors(void)
{
	HAL_StatusTypeDef status;
	FLASH_EraseInitTypeDef erase;
	uint32_t error;

	/* When erasing, the Flash MUST be unlocked. */
	status = HAL_FLASH_Unlock();
	if (status != HAL_OK)
		panic_hal(status, "Couldn't be unlocked the Flash");

	erase.TypeErase = FLASH_TYPEERASE_SECTORS;
	erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;	/* 2.7V to 3.6V */
	erase.Sector = FLASH_SECTOR_1;	/* start from Sector 1 */
	erase.NbSectors = 7;	/* erase Sector 1 through 7 */
	
	status = HAL_FLASHEx_Erase(&erase, &error);
	if (status != HAL_OK)
		panic_hal(status, "Failed to erase the Flash sectors");

	/* After erased, lock the Flash again. */
	status = HAL_FLASH_Lock();
	if (status != HAL_OK)
		panic_hal(status, "Couldn't be locked the Flash");
}

void write_flash(uint32_t address, uint8_t *data, uint32_t size)
{
	HAL_StatusTypeDef status;
	uint32_t i, word;

	/* When programming, the Flash MUST be unlocked. */
	status = HAL_FLASH_Unlock();
	if (status != HAL_OK)
		panic_hal(status, "Couldn't be unlocked the Flash");

	for (i = 0; i < size; i += 4)
	{
		/* Program the Flash word-by-word (32-bit). */
		word = *(uint32_t *)(data + i);
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, word);
		if (status != HAL_OK)
			panic_hal(status, "Failed to program the Flash");
	}

	/* After programmed, lock the Flash again. */
	status = HAL_FLASH_Lock();	
	if (status != HAL_OK)
		panic_hal(status, "Couldn't be locked the Flash");
}

void jump_to_app(void)
{
	uint32_t i, msp, reset_handler;

	/* Get the MSP (Main Stack Pointer) and reset handler from 
	   the first and second words of vector table. */
	msp = *(volatile uint32_t *)(APP_ADDRESS);
	reset_handler = *(volatile uint32_t *)(APP_ADDRESS + 4UL);

	/* Disable all interrupts and clear pending flags. */
	__disable_irq();
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFFUL;
		NVIC->ICPR[i] = 0xFFFFFFFFUL;
	}

	/* Set the Vector Table Offset Register (VTOR) to the application's. */
	SCB->VTOR = APP_ADDRESS;
	__DSB();	/* data synchronization barrier */
	__ISB();	/* instruction synchronization barrier */

	/* Set the Main Stack Pointer (MSP). */
	__set_MSP(msp);

	/* Create a function pointer to the application's reset handler. */
	void (*main_entry)(void) = (void (*)(void))reset_handler;
	
	/* Jump to reset handler. Reset handler will jump to application's
	   main() so that the bootloader passes the control to application. */
	main_entry();

	/* MUST never reach here. */
	while (true) {};
}

/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	The main implementation of the bootloader
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

int main(void)
{
	uint32_t appAddr = APP_ADDRESS;
	uint8_t chunk[BUF_SIZE] = {0};
	HAL_StatusTypeDef status;

	/* Intialize the HAL driver. */
	HAL_Init();
	
	/* Configure the required peripherals. */
	config_osc_clk();
	config_debug();

	println("\n(bl) Bootloader is started.");
	println("(bl) It is put in the sector 0 of Flash memory.");

	erase_sectors();
	println("(bl) Erased the sector 1-7 of Flash memory.");

	println("(bl) Waiting for the application (.bin) over UART...");
	while (true)
	{
		/* Receive the application firmware in 256 bytes chunks. */
		status = HAL_UART_Receive(&husart2, chunk, sizeof(chunk), TIMEOUT);
		if (status == HAL_OK)
		{
			/* Write the application firmware in order. */
			print("(bl) Writing Flash memory with application (%ld - ", appAddr);
			write_flash(appAddr, chunk, sizeof(chunk));
			appAddr += sizeof(chunk);
			println("%ld", appAddr);
		}
		else
		{
			panic("(bl) Couldn't get the application completely");
		}
	}
	println("(bl) Writing completed the application");
	println("(bl) Jumping into the application entry point...");
	jump_to_app();

	return EXIT_SUCCESS;
}

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
	uint8_t chunk[APP_CHUNK] = {0};
	HAL_StatusTypeDef status;
	bool succedded = false;
	uint32_t totalByte = 0;

	/* Intialize the HAL driver. */
	HAL_Init();
	
	/* Configure the required peripherals. */
	config_osc_clk();
	config_debug();

	println("\n(bl) Bootloader is started...");
	println("(bl) Sector 0 of Flash memory is reserved.");

	erase_sectors();
	println("(bl) Erased the sector 1-7 of Flash memory.");

	println("(bl) Waiting for the binary (%d ms timeout)...", TIMEOUT);
	while (true)
	{
		/* Receive the application firmware in 256 bytes chunks. */
		status = HAL_UART_Receive(&husart2, chunk, sizeof(chunk), TIMEOUT);
		if (status != HAL_OK)
			break;
		
		/* Write the application firmware in order. */
		write_flash(appAddr, chunk, sizeof(chunk));
		/* Update the memory address for the next chunk. */
		appAddr += sizeof(chunk);
		succedded = true;
		totalByte += sizeof(chunk);
	}
	if (!succedded)
		panic("(bl) Coudln't completed the writing process.");

	println("(bl) Completed the writing into Flash memory.");
	println("(bl) Total %ld bytes written (0x0%lx - 0x0%lx).",
		appAddr - APP_ADDRESS, APP_ADDRESS, appAddr);
	println("(bl) Jumping into the application entry point...");
	/* Jump the application entry point (its main). */
	jump_to_app();

	return EXIT_SUCCESS;
}

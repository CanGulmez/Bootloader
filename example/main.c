/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	The main implementation of the application
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

static int32_t i = 0;

int main(void)
{
	HAL_Init();

	config_osc_clk();
	config_debug();

	println("\n(app) The firmware is running...");
	println("(app) System clock is set to %ld MHz.", SystemCoreClock / 1000000);

	while (true)
	{
		println("(app) Hello from the application! (#%ld)", i++);
		HAL_Delay(1000);
	}

	return EXIT_SUCCESS;
}

/**
 ******************************************************************************
 * @file 	peripheral.c
 * @author 	Can Gulmez
 * @brief 	The peripheral configs of the application
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

UART_HandleTypeDef husart2	= {0};
GPIO_InitTypeDef igpio		= {0};
RCC_OscInitTypeDef iosc		= {0};
RCC_ClkInitTypeDef iclk		= {0};

void config_osc_clk(void)
{
	iosc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	iosc.HSIState = RCC_HSI_ON;
	iosc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	iosc.PLL.PLLState = RCC_PLL_OFF;
	HAL_RCC_OscConfig(&iosc);
	
	iclk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | 
					 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	iclk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	iclk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	iclk.APB1CLKDivider = RCC_HCLK_DIV1;
	iclk.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&iclk, FLASH_LATENCY_0);
}

void config_debug(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
	igpio.Mode = GPIO_MODE_AF_PP;
	igpio.Alternate = GPIO_AF7_USART2;
	igpio.Pull = GPIO_NOPULL;
	igpio.Speed = GPIO_SPEED_FREQ_LOW;
	igpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &igpio);

	husart2.Instance = USART2;
	husart2.Init.BaudRate = 115200;
	husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	husart2.Init.Mode = UART_MODE_TX;
	husart2.Init.OverSampling = UART_OVERSAMPLING_16;
	husart2.Init.Parity = UART_PARITY_NONE;
	husart2.Init.StopBits = UART_STOPBITS_1;
	husart2.Init.WordLength = UART_WORDLENGTH_8B;
	HAL_UART_Init(&husart2);
}
 
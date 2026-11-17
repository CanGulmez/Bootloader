/**
 * Main source of the firmware.
 */

#include "main.h"

static int32_t i = 0;

int main(void)
{
	HAL_Init();

	config_osc_clk();
	config_debug();

	println("\nThe firmware is running...");
	println("System clock is set to %ld HZ.", SystemCoreClock);

	while (1)
	{
		println("Hello from STM32! (#%ld)", i++);
		HAL_Delay(2000);
	}

	return EXIT_SUCCESS;
}

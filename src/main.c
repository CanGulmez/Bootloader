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

	print("\nThe firmware is running...");
	print("System clock is set to %ld HZ.", SystemCoreClock);

	panic(HAL_ERROR, "the error occured");

	while (1)
	{
		print("Hello from STM32! (#%ld)", i++);
		HAL_Delay(2000);
	}

	return EXIT_SUCCESS;
}

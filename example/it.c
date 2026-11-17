/**
 * Interrupt Service Routines (ISRs) of the firmware.
 */

#include "main.h"

void SysTick_Handler(void) 
{
   HAL_IncTick();
}

/**
 * Main header of the firmware.
 */

/* Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

#include "stm32f4xx_hal.h"

/* Global and general definitions */

#define BUF_SIZE		64

#define STATUS(status) 	((status == HAL_ERROR) ? "error" :		\
						 (status == HAL_BUSY) ? "busy" :		\
						 (status == HAL_TIMEOUT) ? "timeout" :	\
						  "undefined")

/*****************************************************************************/

/* Peripheral handlers */

extern RCC_OscInitTypeDef iosc;
extern RCC_ClkInitTypeDef iclk;
extern GPIO_InitTypeDef igpio;
extern UART_HandleTypeDef husart2;

/*****************************************************************************/

/**
 * Print the messages over the serial line.
 */
#define print(fmt, ...)														\
{																			\
	char buffer[BUF_SIZE];													\
																			\
	snprintf(buffer, BUF_SIZE, fmt "\r\n", ##__VA_ARGS__);					\
	HAL_UART_Transmit(&husart2, (uint8_t*)buffer, strlen(buffer), 			\
		HAL_MAX_DELAY);														\
}

/**
 * Print the panic messages over the serial line.
 */
#define panic(status, fmt, ...)												\
{																			\
	char buffer[BUF_SIZE];													\
																			\
	snprintf(buffer, BUF_SIZE, "*** [status: %s] " fmt " (%s::%d) ***\r\n", \
		STATUS(status), ##__VA_ARGS__, __FILE__, __LINE__);					\
	HAL_UART_Transmit(&husart2, (uint8_t*)buffer, strlen(buffer), 			\
		HAL_MAX_DELAY);														\
}

/*****************************************************************************/

/* Function prototypes */

int _fstat(int, struct stat *);
int _read(int, char *, int);
int _write(int, char *, int);
int _lseek(int, int, int);
int _close(int);
pid_t _getpid(void);
int _isatty(int);
int _kill(pid_t, int);

void config_osc_clk(void);
void config_debug(void);

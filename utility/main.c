/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	The main implementation of the uploader
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

int main(int argc, char *argv[])
{
	if (argc != 3)
		user_error("Usage: %s channel binary", argv[0]);

	/* Call the uploader program. */
	uploader(argv[1], argv[2]);

	return EXIT_SUCCESS;
}

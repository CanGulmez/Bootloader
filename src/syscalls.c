/**
 ******************************************************************************
 * @file 	syscalls.c
 * @author 	Can Gulmez
 * @brief 	The dumpy syscalls of the bootloader
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

int _fstat(int fd, struct stat *st)
{
	(void)fd;
	(void)st;

	return 0;
}

int _read(int fd, char *ptr, int len)
{
	(void)fd;
	(void)ptr;
	(void)len;

	return 0;
}

int _write(int fd, char *ptr, int len)
{
	(void)fd;
	(void)ptr;
	(void)len;

	return 0;
}

int _lseek(int fd, int ptr, int dir)
{
	(void)fd;
	(void)ptr;
	(void)dir;

	return 0;
}

int _close(int fd)
{
	(void)fd;

	return 0;
}

pid_t _getpid(void)
{
	return 0;
}

int _isatty(int fd)
{
	(void)fd;

	return 0;
}

int _kill(pid_t pid, int sig)
{
	(void)pid;
	(void)sig;

	return 0;
}

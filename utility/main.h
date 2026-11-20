/**
 ******************************************************************************
 * @file 	main.h
 * @author 	Can Gulmez
 * @brief 	The main definitions of the uploader
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

#ifndef UPLOADER_H
#define UPLOADER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Libraries */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <sys/times.h>

/* Global definitions */

#define BUF_SIZE		256
#define CHUNK_SIZE		1024	/* bytes */

/* Some useful macro functions */

#define syscall_error()												\
do {                                      							\
	fprintf(stderr, "\n*** %s (%s::%d) ***\n", strerror(errno),		\
		__FILE__, __LINE__);		   	 							\
	exit(EXIT_FAILURE);	/* exit with failure status */				\
} while (0)

#define user_error(fmt, ...) 										\
do {                            									\
	fprintf(stderr, "\n*** " fmt " (%s::%d) ***\n",					\
			  ##__VA_ARGS__, __FILE__, __LINE__);					\
	exit(EXIT_FAILURE);	/* exit with failure status */				\
} while (0)

/* Function prototypes */

void uploader(const char *channel, char *binary);

#ifdef __cplusplus
}
#endif

#endif /* UPLOADER_H */
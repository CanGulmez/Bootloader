/**
 ******************************************************************************
 * @file 	uploader.c
 * @author 	Can Gulmez
 * @brief 	The core implementation of the uploader
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

void set_channel_attrs(int cfd)
{
	struct termios tty;

	/* Get the current serial terminal attributes. */
	if (tcgetattr(cfd, &tty) == -1)
		syscall_error();

	cfmakeraw(&tty);				/* for raw binary transmission */
	cfsetispeed(&tty, B115200);		/* input baudrate: 115200 */
	cfsetospeed(&tty, B115200);		/* output baudrare: 115200 */
	tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSTOPB;			/* 1 stop bit */
    tty.c_cflag &= ~PARENB;			/* no parity */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;				/* 8 data bits */
    tty.c_cc[VMIN]  = 1;			/* read() blocks for at least 1 byte */
    tty.c_cc[VTIME] = 5;			/* ...but times out after 0.5 s */

	/* Set the new attributes. */
    if (tcsetattr(cfd, TCSANOW, &tty) != 0)
        syscall_error();

    tcflush(cfd, TCIOFLUSH);
}

void uploader(const char *channel, char *binary)
{
	int cfd, bfd;
	char *dot;
	struct stat st;
	uint8_t chunk[CHUNK_SIZE];
	ssize_t numRead, numWritten;
	size_t sent = 0;
	
	/* Open the serial channel and ... */
	cfd = open(channel, O_RDWR | O_NOCTTY);
	if (cfd == -1)
		syscall_error();
	set_channel_attrs(cfd);	/* ... set the attributes. */

	/* Open the binary file. */
	bfd = open(binary, O_RDONLY);
	if (bfd == -1)
		syscall_error();
	
	/* The binary file should be ended up '.bin' that it contains
	   just raw machine bytes instead of full program metadata. */
	dot = strrchr(binary, '.');
	if (dot == nullptr ||  strcmp(dot, ".bin") != 0)
		user_error("(uploader) The binary shoud be ended up '.bin'.");

	/* Get the size of the binary. */
	if (stat(binary, &st) == -1)
		syscall_error();
	printf("(uploader) The size of '%s' is %ld bytes.\n", binary, st.st_size);

	/* Read the raw binary contents from the binary file. */
	while ((numRead = read(bfd, chunk, CHUNK_SIZE)) > 0)
	{
		/* Write the raw bytes to the serial port. */
		numWritten = write(cfd, chunk, (size_t)numRead);
		if (numRead != numWritten)
			syscall_error();

		/* Also count the total written bytes. */
		sent += (size_t)numWritten;
		printf("(uploader) Sent %ld of %ld bytes...\n", sent, st.st_size);

		/* Give some time... */
		sleep(1);
	}
	printf("(uploader) Upload completed!\n");

	/* Close the streams. */
	if (close(cfd) == -1)
		syscall_error();
	if (close(bfd) == -1)
		syscall_error();
}

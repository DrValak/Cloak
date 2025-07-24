#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#define BUFFER_SIZE 1024;

typedef struct
{
	uint8_t data[BUFFER_SIZE]; // 1024 bytes
	int head;
	int tail;
	int full;
} CircularBuffer;

void startBuffer(CircularBuffer *buffer);
int addToBuffer(CircularBuffer *buffer, uint8_t insertion);
int removeToBuffer(CircularBuffer *buffer, uint8_t remove_value);
int showBuffer(CircularBuffer *buffer);

#endif 
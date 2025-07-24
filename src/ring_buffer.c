#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ring_buffer.h"

void startBuffer(CircularBuffer *buffer) {
	//All starts with 0
	buffer -> head = 0;
	buffer -> tail = 0;
	buffer -> full = 0;
}

int addToBuffer(CircularBuffer *buffer, uint8_t insertion) {

	if(buffer -> full){ printf("Buffer is full"); return 1; } // if it is full

	buffer -> data[buffer -> tail] = insertion;

	buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE; // increment tail in circular

	if (buffer -> tail == buffer -> head) {
		buffer -> full = 1; // buffer it's full
	};

	return 0;
}

int removeToBuffer(CircularBuffer *buffer, uint8_t *remove_value) {

	if (buffer -> head == buffer -> tail && buffer -> full == 0) { printf("Buffer is empty!"); return 1; }

	*remove_value = buffer -> data[buffer -> head]; // Store the removed value

	buffer -> head = (buffer -> head + 1) % BUFFER_SIZE; // Circular

	buffer -> full = 0;

	return 0;
}

int showBuffer(CircularBuffer *buffer) {


	int count = 0;

	if (buffer -> full) {
		count = BUFFER_SIZE;
	}

	else if (buffer -> tail >= buffer -> head) {
		count = buffer -> tail - buffer -> head;
	}

	else {
		count = BUFFER_SIZE - buffer -> head + buffer -> tail;
	}

	int idx = buffer -> head;
	for (int i = 0; i < count; i++) {
		printf("%02X\n",buffer -> data[idx]);
		idx = (idx + 1) % BUFFER_SIZE;
	}

	return 0;
}
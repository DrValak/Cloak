#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crypt.h"
#include "ring_buffer.h"
#define ENCRYPTION_KEY 0x3F


void encrypt(uint8_t *data, size_t length, uint8_t key) {

	for (size_t i = 0; i < length; i++) {
    data[i] ^= key;
	}

}

void decrypt(uint8_t *data, size_t length, uint8_t key) {

	for (size_t i = 0; i < length; i++) {
    	data[i] ^= key;
		}

}
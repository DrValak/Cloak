#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef CRYPT_H
#define CRYPT_H

void encrypt(uint8_t *data, size_t lenght, uint8_t key);
void decrypt(uint8_t *data, size_t lenght, uint8_t key);

#endif CRYPT_H
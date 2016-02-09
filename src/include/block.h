#ifndef BLOCK_H
#define BLOCK_H

#include "vec.h"
#include <stdint.h>

struct block
{
	uint8_t blockid;
	uint8_t data;
};

typedef struct block block_t;

block_t block_init(int8_t blockid, int8_t data);

block_t block_rotate(block_t block, vec3 dirs);
block_t block_flip(block_t block, vec3 dirs);

#endif /* BLOCK_H */

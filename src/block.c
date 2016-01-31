#include "block.h"

block_t block_init(int8_t blockid, int8_t data)
{
	block_t block;
	block.blockid = blockid;
	block.data    = data;

	return block;
}

block_t block_rotate(block_t block, vec3 dirs)
{
	switch (block.blockid)
	{
		/* Do this part when without means to commit suicide */
	}

	return block;
}

block_t block_flip(block_t block, vec3 dirs)
{
	switch (block.blockid)
	{
		/* Do this part when without means to commit suicide */
	}

	return block;
}

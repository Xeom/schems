#include "block.h"

/* I am so, so sorry. */

#define SWITCH_WOOD(blk, code)					\
case 17:case 162:								\
	dir = blk.data & 0x06 >> 2;				\
	blk.data -= dir << 2;						\
	code										\
	blk.data |= dir << 2;						\
	break;

#define WOOD_UPDOWN     0x00
#define WOOD_EASTWEST   0x01
#define WOOD_NORTHSOUTH 0x02
#define WOOD_WTF        0x03

#define SWITCH_TORCH(blk, code)				\
case 75:case 76:case 50:						\
	dir = blk.data;							\
	code										\
	blk.data = dir;							\
	break;

#define TORCH_DOWN  0x00
#define TORCH_EAST  0x01
#define TORCH_WEST  0x02
#define TORCH_SOUTH 0x03
#define TORCH_NORTH 0x04
#define TORCH_UP    0x05

#define SWITCH_STAIR(blk, code)									\
case  53:case  67:case 108:case 109:case 114:case 128:case 134:	\
case 135:case 136:case 156:case 163:case 164:case 180:case 203:	\
	upside_down = blk.data & 0x04 >> 2;							\
	dir         = blk.data & 0x03;									\
	code															\
	blk.data = upside_down << 2 || dir;							\
	break;

#define STAIR_EAST  0x00
#define STAIR_WEST  0x01
#define STAIR_SOUTH 0x02
#define STAIR_NORTH 0x03

#define SWITCH_PISTONS_N_SHIT(blk, code)					\
case 23:case 33:case 29:case 34:case 138:case 154:			\
	dir = blk.data & 0x07;									\
	blk.data -= dir;										\
	code													\
	blk.data += dir;										\
	break;

#define PISTON_DOWN  0x00
#define PISTON_UP    0x01
#define PISTON_NORTH 0x02
#define PISTON_SOUTH 0x03
#define PISTON_WEST  0x04
#define PISTON_EAST  0x05

#define SWITCH_LEVER(blk, code)	\
case 69:							\
	dir = blk.data & 0x7;			\
	blk.data -= dir;				\
	code							\
	blk.data += dir;				\
	break;

#define LEVER_UP_EW   0x00
#define LEVER_EAST    0x01
#define LEVER_WEST    0x02
#define LEVER_SOUTH   0x03
#define LEVER_NORTH   0x04
#define LEVER_DOWN_NS 0x05
#define LEVER_DOWN_EW 0x06
#define LEVER_UP_NS   0x07

block_t block_init(int8_t blockid, int8_t data)
{
	block_t block;
	block.blockid = blockid;
	block.data    = data;

	return block;
}

block_t block_rotate_x(block_t block)
{
	uint8_t dir, upside_down;

	switch (block.blockid)
	{
	SWITCH_WOOD(block,
	            if (dir == WOOD_UPDOWN)
		            dir = WOOD_NORTHSOUTH;
	            else if (dir == WOOD_NORTHSOUTH)
		            dir = WOOD_UPDOWN;
		)

	SWITCH_TORCH(block,
	             if (dir == TORCH_NORTH)
		             dir = TORCH_UP;
	             else if (dir == TORCH_UP)
		             dir = TORCH_SOUTH;
	             else if (dir == TORCH_SOUTH)
		             dir = TORCH_DOWN;
	             else if (dir == TORCH_DOWN)
		             dir = TORCH_NORTH;
		)

	SWITCH_PISTONS_N_SHIT(block,
	                      if      (dir == PISTON_DOWN)
		                      dir = PISTON_NORTH;
	                      else if (dir == PISTON_NORTH)
		                      dir = PISTON_UP;
	                      else if (dir == PISTON_UP)
		                      dir = PISTON_SOUTH;
	                      else if (dir == PISTON_SOUTH)
		                      dir = PISTON_DOWN;
		)

	SWITCH_STAIR(block,
	             upside_down = !upside_down;
	             if      ( upside_down && dir == STAIR_NORTH)
		             dir = STAIR_SOUTH;
	             else if (!upside_down && dir == STAIR_SOUTH)
		             dir = STAIR_NORTH;
		)

	SWITCH_LEVER(block,
	             if (dir == LEVER_UP_NS || dir == LEVER_UP_EW)
		             dir = LEVER_SOUTH;
	             else if (dir == LEVER_SOUTH)
		             dir = LEVER_DOWN_NS;
	             else if (dir == LEVER_DOWN_NS || dir == LEVER_DOWN_EW)
		             dir = LEVER_NORTH;
	             else if (dir == LEVER_NORTH)
		             dir = LEVER_UP_NS;
		)
	}

	return block;
}

block_t block_rotate(block_t block, vec3 dirs)
{
	while (--dirs.x)
	{
		block = block_rotate_x(block);
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

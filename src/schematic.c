/** schem/schematic.h: Schematic storage and manipulation.
 */

#include "schematic.h"

#include <stdlib.h>

#define SCHEM_YZX_LOOP(size, code)				\
	do {							\
		int x, y, z;					\
		for (y = 0; y < size.y; y++)			\
			for (z = 0; z < size.z; z++)		\
				for (x = 0; x < size.x; x++) {	\
					vec3 pos = {x, y, z};	\
					code			\
						}		\
	} while (0)
                    

#define YZX_INDEX(size, loc) (loc.x + size.x*(loc.z + size.z*(loc.y)))

struct schematic
{
	vec3 size;

	block_t blocks[];
};
typedef struct schematic schem;

schem *schem_init(vec3 size)
{
	schem *ret = (schem*) malloc(sizeof(schem) + sizeof(block_t) * vec3_vol(size));

	if (ret == NULL)
		return ret;

	ret->size = size;

	return ret;
}

schem *schem_copy(schem *schem)
{

	struct schematic *ret = schem_init(schem->size);

	if (ret == NULL)
		return ret;

	int i;
	for (i = 0; i < vec3_vol(ret->size); i++)
		ret->blocks[i] = schem->blocks[i];

	return ret;
}

void schem_kill(schem *schem)
{
	free(schem);
}


vec3 schem_size(schem *schem)
{
	return schem->size;
}

int schem_volume(schem *schem)
{
	return vec3_vol(schem->size);
}


block_t schem_get(schem *schem, vec3 coord)
{
	return schem->blocks[YZX_INDEX(schem->size, coord)];
}

int schem_set(schem *schem, vec3 coord, block_t block)
{
	return schem->blocks[YZX_INDEX(schem->size, coord)] = block;
}


schem *schem_resize(schem *schem, vec3 size)
{
	struct schematic *ret = schem_init(size);

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(size,
		       int src_index = YZX_INDEX(ret->size, pos);
		       int dst_index = YZX_INDEX(size, pos);
		       ret->blocks[dst_index] = schem->blocks[src_index];
		       );

	return ret;
}

schem *schem_stack(schem *schem, vec3 counts)
{
	vec3 nsize = vec3_mul(schem->size, counts);
	struct schematic *ret = schem_init(nsize);

	if (ret == NULL)
		return ret;

	// TODO
	return ret;
}

schem *schem_shift(schem *schem, vec3 offset)
{
	struct schematic *ret = schem_init(schem->size);

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(schem->size,
		       int src_index = YZX_INDEX(schem->size, pos);
		       int dst_index = YZX_INDEX(schem->size, vec3_add(pos, offset));
		       if (dst_index >= vec3_vol(schem->size))
			       ret->blocks[dst_index] = 0;
		       else
			       ret->blocks[dst_index] = schem->blocks[src_index];
		       );

	return ret;
}

schem *schem_stacking_resize(schem *schem, vec3 size) {

    
	return NULL; // TODO


}

schem *schem_flip(schem *schem, vec3 dirs)
{
	struct schematic *ret = schem_init(schem->size);

	if (ret == NULL)
		return ret;


	dirs = vec3_mod(dirs, (vec3) {2, 2, 2});
        dirs = vec3_mul(dirs, (vec3) {-1, -1, -1});

	SCHEM_YZX_LOOP(schem->size,
	               int src_index = YZX_INDEX(schem->size, pos);
	               int dst_index = YZX_INDEX(schem->size, vec3_mul(pos, dirs));
		       ret->blocks[dst_index] = schem->blocks[src_index];
	               );

	return ret;
}

int schem_fill(schem *schem, vec3 offset, vec3 size, block_t block)
{
	SCHEM_YZX_LOOP(size,
		       vec3 dst_pos = vec3_add(pos, offset);
		       schem->blocks[YZX_INDEX(schem->size, dst_pos)] = block;
		       );

	return 0;
}

int schem_insert(schem *into, vec3 offset, schem *from)
{
	return 0; // TODO
}


	int result = a.x * a.x;
	result    += a.y * a.y;
	result    += a.z * a.z;
	return result;block_t _block_rotate(block_t block, vec3 dirs)
{
	int8_t blockid = (int8_t)(block & 0xff00 >> 8);
	int8_t data    = (int8_t)(block & 0x000f);

	switch (blockid)
	{
		/* Do this part when without means to commit suicide */
	}

	return (blockid << 8) | data;
}

block_t block_rotate(block_t block, vec3 dirs)
{
	int8_t blockid = (int8_t)(block & 0xff00 >> 8);
	int8_t data    = (int8_t)(block & 0x000f);

	switch (blockid)
	{
		/* Do this part when without means to commit suicide */
	}

	return (blockid << 8) | data;
}



void 2d_rotate(int *x, int *y, int dir)
{
	switch (dir)
	{
	case 0:
		break;
	case 1:
		int xOrig = *x;
		*x = *y;
		*y = -xOrig;
		break;
	case 2:
		*x = -*x;
		*y = -*y;
		break;
	case 3:
		int xOrig = *x;
		*x = -*y;
		*y = xOrig;
		break;
	}
}

vec3 3d_rotate(vec3 vec, vec3 dirs)
{
	2d_rotate(&vec.z, &vec.y, dirs.x);
	2d_rotate(&vec.x, &vec.z, dirs.y);
	2d_rotate(&vec.x, &vec.y, dirs.z);
}

schem *schem_rotate(schem *schem, vec3 dirs)
{
	vec3 size = schem_size(schem);
	vec3 newsize = 3d_rotate(size, dirs);

	schem *newschem = schem_init(vec3_abs(newsize));

	vec3 incrx = 3d_rotate({1, 0, 0}, dirs);
	vec3 incry = 3d_rotate({0, 1, 0}, dirs);
	vec3 incrz = 3d_rotate({0, 0, 1}, dirs);

	vec3 cur = {0, 0, 0};

	for (int y = 0; y < size.y; ++y)
	{
		cur = vec3_add(cur, incry);
		cur.y = cur.y % size.y;
		for (int z = 0; z < size.z; ++z)
		{
			cur = vec3_add(cur, incz);
			cur.z = cur.z % size.z;
			for (int x = 0; x < size.x, ++x)
			{
				cur = vec3_add(cur, incx);
				cur.x = cur.x % size.x;

				vec3 index = vec3_sub(cur, vec3_min(size, {0, 0, 0}));
				newschem->blocks[YZX_INDEX(index)] =
					block_rotate(
						schem.blocks[YZX_INDEX((vec3){x, y, z})], dirs)
			}
		}
	}

	return newschem;
}

	

// Rotates by pi/2 n times on each axis.


schem *schem_rotate(schem *schem, vec3 dirs); // Rotates by pi/2 n times on each axis.


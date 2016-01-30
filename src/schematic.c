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


	dirs = vec3_mod(dirs, {2, 2, 2});
        dirs = vec3_mul(dirs, {-1, -1, -1});

	SCHEM_YZX_LOOP(schem->size,
	               int src_index = YZX_INDEX(schem->size, pos);
	               int dst_index = YZX_INDEX(schem->size, vec3_mul(pos, dirs));
		       ret->blocks[dst_index] = schem->blocks[src_index];
	               );

	return ret;
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
	/* /  1  0 \
	   \  0  1 /
	*/
	case 0:
		break;
	/* /  0  1 \
	   \ -1  0 /
	*/
	case 1:
		int xOrig = *x;
		*x = *y;
		*y = -xOrig;
		break;
	/* / -1  0 \
	   \  0 -1 /
	*/
	case 2:
		*x = -*x;
		*y = -*y;
		break;
	/* /  0 -1 \
	   \ -1  0 /
	*/
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
	/* The shape of the new schematic is obtained via rotation */
	vec3 newsize = 3d_rotate(size, dirs);
	/* Get rid of negatives */
	vec3 newsizeabs = vec3_abs(newsize);

	schem *newschem = schem_init(newsizeabs);

	/* These three variables represents what happens in the new
	   schematic when you move in the x y or z directions in the
	   old one.
	*/
	vec3 incrx = 3d_rotate({1, 0, 0}, dirs);
	vec3 incry = 3d_rotate({0, 1, 0}, dirs);
	vec3 incrz = 3d_rotate({0, 0, 1}, dirs);

	vec3 cur = {0, 0, 0};

	for (int y = 0; y < size.y; ++y)
	{
		cur = vec3_add(cur, incry);
		for (int z = 0; z < size.z; ++z)
		{
			cur = vec3_add(cur, incz);
			for (int x = 0; x < size.x, ++x)
			{
				cur = vec3_add(cur, incx);
				cur = vec3_mod(cur, newsize);
				/* If current index is negative make it positive */
				vec3 curpos = vec3_mod(cur, newsizeabs);
				/* Add the block to the new schematic */
				newschem->blocks[YZX_INDEX(newsizeabs, index)] =
					block_rotate(schem.blocks[YZX_INDEX(
						size, (vec3){x, y, z})], dirs);
			}
		}
	}

	return newschem;
}

void schem_fill(schem *schem, vec3 offset, vec3 size, block_t block)
{
	/* Get absolute values of input vectors */
	offset = vec3_abs(offset);
	size   = vec3_abs(size);

	/* Ensure the region to be filled is contained within the schematic */
	offset = vec3_min(schem.size, offset);
	size   = vec3_min(vec3_sub(schem.size, offset), size);

	/* Fill shit yo */
	YZX_LOOP(size,
		vec3 index = vec3_add(pos, offset);
		schem.blocks[index] = block_t;
		);
}

void schem_insert(schem *into, vec3 offset, schem *from)
{
	offset = vec3_abs(offset);
	offset = vec3_min(into->size, offset);

	vec3 wiggle = vec3_min(from->size, vec3_sub(into->size, offset));

	YZX_LOOP(wiggle,
			into->blocks[YZX_INDEX(into->size, pos)] =
				from->blocks[YZX_INDEX(from->size, vec3_sum(offset, pos))];
		);
}


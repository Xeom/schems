/** schem/schematic.h: Schematic storage and manipulation.
 */

#include "schematic.h"

#include <stdlib.h>

#define SCHEM_YZX_LOOP(size, code)				\
	do {										\
		int x, y, z;							\
		vec3 pos;								\
		for (y = 0; y < size.y; y++)			\
			for (z = 0; z < size.z; z++)		\
				for (x = 0; x < size.x; x++) {	\
					pos = vec3_init(x, y, z);	\
					code						\
						}						\
	} while (0)

#define YZX_INDEX(size, loc) (loc.x + size.x*(loc.z + size.z*(loc.y)))

struct schematic
{
	vec3 size;
	block_t *blocks;
};


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

	int i;

	if (ret == NULL)
		return ret;

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
	size_t src_index, dst_index;

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(size,
					   src_index = YZX_INDEX(ret->size, pos);
					   dst_index = YZX_INDEX(size, pos);
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

	/* TODO */
	return ret;
}

schem *schem_shift(schem *schem, vec3 offset)
{
	struct schematic *ret = schem_init(schem->size);
	size_t src_index, dst_index;

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(schem->size,
				   src_index = YZX_INDEX(schem->size, pos);
				   dst_index = YZX_INDEX(schem->size, vec3_add(pos, offset));
				   if ((int)dst_index >= vec3_vol(schem->size))
					   ret->blocks[dst_index] = 0;
				   else
					   ret->blocks[dst_index] = schem->blocks[src_index];
		);

	return ret;
}

schem *schem_stacking_resize(schem *schem, vec3 size) {


	return NULL; /* TODO */


}

schem *schem_flip(schem *schem, vec3 dirs)
{
	struct schematic *ret = schem_init(schem->size);
	size_t src_index, dst_index;
	if (ret == NULL)
		return ret;


	dirs = vec3_mod(dirs, vec3_init( 2,  2,  2));
	dirs = vec3_mul(dirs, vec3_init(-1, -1, -1));

	SCHEM_YZX_LOOP(schem->size,
				   src_index = YZX_INDEX(schem->size, pos);
				   dst_index = YZX_INDEX(schem->size, vec3_mul(pos, dirs));
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

void rotate_2d(int *x, int *y, int dir)
{
	int xOrig;
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
		xOrig = *x;
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
		xOrig = *x;
		*x = -*y;
		*y = xOrig;
		break;
	}
}

vec3 rotate_3d(vec3 vec, vec3 dirs)
{
	rotate_2d(&vec.z, &vec.y, dirs.x);
	rotate_2d(&vec.x, &vec.z, dirs.y);
	rotate_2d(&vec.x, &vec.y, dirs.z);

	return vec;
}

schem *schem_rotate(schem *schem, vec3 dirs)
{
	vec3 size = schem_size(schem);
	/* The shape of the new schematic is obtained via rotation */
	vec3 newsize = rotate_3d(size, dirs);
	/* Get rid of negatives */
	vec3 newsizeabs = vec3_abs(newsize);

	struct schematic *newschem = schem_init(newsizeabs);

	/* These three variables represents what happens in the new
	   schematic when you move in the x y or z directions in the
	   old one.
	*/
	vec3 newpos = vec3_init(0, 0, 0);
	vec3 oldpos = vec3_init(0, 0, 0);

	vec3 absnewpos;

	vec3 incrx = vec3_init(1, 0, 0);
	vec3 incry = vec3_init(0, 1, 0);
	vec3 incrz = vec3_init(0, 0, 1);

	incrx = rotate_3d(incrx, dirs);
	incry = rotate_3d(incry, dirs);
	incrz = rotate_3d(incrz, dirs);

	for (oldpos.y = 0; oldpos.y < size.y; ++(oldpos.y))
	{
		newpos = vec3_add(newpos, incry);
		for (oldpos.z = 0; oldpos.z < size.z; ++(oldpos.z))
		{

			newpos = vec3_add(newpos, incrz);
			for (oldpos.x = 0; oldpos.x < size.x; ++(oldpos.x))
			{
				newpos = vec3_add(newpos, incrx);
				newpos = vec3_mod(newpos, newsize);
				/* If current index is negative make it positive */
				absnewpos = vec3_mod(newpos, newsizeabs);

				/* Add the block to the new schematic */
				newschem->blocks[YZX_INDEX(newsizeabs, absnewpos)] =
					block_rotate(schem->blocks[YZX_INDEX(
						size, oldpos)], dirs);

			}
		}
	}

	return newschem;
}

void schem_fill(schem *schem, vec3 offset, vec3 size, block_t block)
{
	vec3 abspos;
 
	/* Get absolute values of input vectors */
	offset = vec3_abs(offset);
	size   = vec3_abs(size);

	/* Ensure the region to be filled is contained within the schematic */
	offset = vec3_min(schem->size, offset);
	size   = vec3_min(vec3_sub(schem->size, offset), size);

	/* Fill shit yo */
	SCHEM_YZX_LOOP(size,
				   abspos = vec3_add(pos, offset);
				   schem->blocks[YZX_INDEX(schem->size, abspos)] = block;
		);
}

void schem_insert(schem *into, vec3 offset, schem *from)
{
	vec3 wiggle;

	offset = vec3_abs(offset);
	offset = vec3_min(into->size, offset);

	wiggle = vec3_min(from->size, vec3_sub(into->size, offset));

	SCHEM_YZX_LOOP(wiggle,
				   into->blocks[YZX_INDEX(into->size, pos)] =
				   from->blocks[YZX_INDEX(from->size, vec3_add(offset, pos))];
		);
}


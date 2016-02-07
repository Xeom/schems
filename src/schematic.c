/** schem/schematic.h: Schematic storage and manipulation.
 */

#include "schematic.h"
#include <stdlib.h>
#include <string.h>

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

schem *schem_init(vec3 size)
{
	schem *ret = (schem*) malloc(sizeof(schem));
	ret->blocks = malloc( sizeof(block_t) * vec3_vol(size));

	if (ret == NULL)
		return ret;

	ret->size = size;

	return ret;
}

schem *schem_copy(schem *schem)
{
	struct schematic *ret;

	ret = schem_init(schem_size(schem));

	if (ret == NULL)
		return ret;

	schem_copy_blocks(ret, schem->blocks);

	return ret;
}

void schem_copy_blocks(schem *schem, block_t *blocks)
{
	size_t vol;

	vol = schem_vol(schem);

	memcpy(schem->blocks, blocks, sizeof(int8_t) * vol);
}

void schem_kill(schem *schem)
{
	free(schem->blocks);
	free(schem);
}


vec3 schem_size(schem *schem)
{
	return schem->size;
}

int schem_vol(schem *schem)
{
	return vec3_vol(schem->size);
}

block_t schem_get(schem *schem, vec3 coord)
{
	return schem_get_index(schem, YZX_INDEX(schem->size, coord));
}

void schem_set(schem *schem, vec3 coord, block_t block)
{
	schem_set_index(schem, YZX_INDEX(schem->size, coord), block);
}

block_t schem_get_index(schem *schem, size_t index)
{
	return schem->blocks[index];
}

void schem_set_index(schem *schem, size_t index, block_t block)
{
	schem->blocks[index] = block;
}

schem *schem_resize(schem *schem, vec3 size)
{
	vec3 wiggle;
	struct schematic *ret;

	wiggle = vec3_min(size, schem_size(schem));

	ret = schem_init(size);

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(wiggle,
				   schem_set(ret, pos, schem_get(schem, pos));
		);

	return ret;
}

schem *schem_stack(schem *schem, vec3 counts)
{
	vec3 newsize;

	newsize = vec3_mul(schem_size(schem), counts);

	return schem_stacking_resize(schem, newsize);
}


schem *schem_stacking_resize(schem *schem, vec3 newsize)
{
	vec3 size, src;
	struct schematic *ret;

	size    = schem_size(schem);

	ret = schem_init(newsize);

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(newsize,
				   src = vec3_mod(pos, size);
				   schem_set(ret, pos, schem_get(schem, src));
		);

	return ret;
}

schem *schem_shift(schem *schem, vec3 offset)
{
	vec3 size, wiggle, newpos;
	struct schematic *ret;

	size = schem_size(schem);

	ret = schem_init(size);

	if (ret == NULL)
		return ret;

	wiggle = vec3_sub(size, offset);

	SCHEM_YZX_LOOP(wiggle,
				   newpos = vec3_add(pos, offset);
				   schem_set(ret, newpos, schem_get(schem, pos));
		);

	return ret;
}

schem *schem_flip(schem *schem, vec3 dirs)
{
	vec3 offset, size, newpos;
	struct schematic *ret;

	size = schem_size(schem);

	ret = schem_init(schem_size(schem));

	if (ret == NULL)
		return ret;

	dirs   = vec3_mod(dirs, vec3_trip(2));
	offset = vec3_mul(dirs, vec3_sub(size, vec3_trip(1)));
	dirs   = vec3_add(vec3_scale(dirs, -2), vec3_trip(1));

	SCHEM_YZX_LOOP(schem->size,
				   newpos = vec3_add(offset, vec3_mul(pos, dirs));
				   schem_set(ret, newpos, schem_get(schem, pos));
		);

	return ret;
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
	rotate_2d(&vec.z,  &vec.y,  dirs.x);
	rotate_2d(&vec.x,  &vec.z,  dirs.y);
	rotate_2d(&vec.y,  &vec.x,  dirs.z);

	return vec;
}

schem *schem_rotate(schem *schem, vec3 dirs)
{
	vec3 size, newsize, offset, newpos;
	struct schematic *ret;

	dirs = vec3_mod(dirs, vec3_trip(4));

	size = schem_size(schem);

	/* The shape of the new schematic is obtained via rotation */
	newsize = rotate_3d(size, dirs);
	offset  = vec3_abs(vec3_min(vec3_add(newsize, vec3_trip(1)), vec3_trip(0)));
	newsize = vec3_abs(newsize);

	ret = schem_init(newsize);

	if (ret == NULL)
		return ret;

	SCHEM_YZX_LOOP(size,
				   newpos = vec3_add(offset, rotate_3d(pos, dirs));
				   schem_set(ret, newpos, block_rotate(schem_get(schem, pos), dirs));
		);

	return ret;
}

void schem_fill(schem *schem, vec3 offset, vec3 area, block_t block)
{
	vec3 offpos, size;

	size = schem_size(schem);

	/* Get absolute values of input vectors */
	offset = vec3_abs(offset);
	area   = vec3_abs(area);

	/* Ensure the region to be filled is contained within the schematic */
	offset = vec3_min(size, offset);
	area   = vec3_min(vec3_sub(size, offset), area);

	/* Fill shit yo */
	SCHEM_YZX_LOOP(area,
				   offpos = vec3_add(pos, offset);
				   schem_set(schem, offpos, block);
		);
}

void schem_insert(schem *into, vec3 offset, schem *from)
{
	vec3 wiggle, isize, fsize;

	isize = schem_size(into);
	fsize = schem_size(from);

	offset = vec3_abs(offset);
	offset = vec3_min(isize, offset);

	wiggle = vec3_min(fsize, vec3_sub(isize, offset));

	SCHEM_YZX_LOOP(wiggle,
				   schem_set(into, pos, schem_get(from, vec3_add(offset, pos)));
		);
}


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

schem *schem_rotate(schem *schem, vec3 dirs); // Rotates by pi/2 n times on each axis.


/** schem/schematic.h: Schematic storage and manipulation.
 */

#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include <stdint.h>
#include "vec.h"

typedef int16_t block_t;

struct schematic;
typedef struct schematic schem;

schem *schem_init(vec3 size);
schem *schem_copy(schem *schem);
void schem_kill(schem *schem);

vec3 schem_size(schem *schem);
int schem_volume(schem *schem);


schem *schem_from_file(const char *path);
int schem_to_file(schem *schem);

block_t schem_get(schem *schem, vec3 coord);
int schem_set(schem *schem, vec3 coord, block_t block);

schem *schem_resize(schem *schem, vec3 size);
schem *schem_stack(schem *schem, vec3 counts);
schem *schem_stacking_resize(schem *schem, vec3 size);

schem *schem_shift(schem *schem, vec3 offset);
schem *schem_flip(schem *schem, vec3 dirs); /* Flips n times on each axis. */
schem *schem_rotate(schem *schem, vec3 dirs); /* Rotates by pi/2 n times on each axis. */
schem *schem_fill(schem *schem, vec3 offset, vec3 size, block_t block);
schem *schem_insert(schem *into, vec3 offset, schem *from);

#endif /* SCHEMATIC_H */

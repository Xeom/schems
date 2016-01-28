/** schem/schematic.h: Schematic storage and manipulation.
 */

#ifndef SCHEMATIC_H
#define SCHEMATIC_H


struct vec3i
{
    int x, y, z;
};
typedef struct vec3i vec3;

// Place-wise ops
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, vec3 b);
vec3 vec3_div(vec3 a, vec3 b);

vec3 vec3_abs(vec3 a);
vec3 vec3_neg(vec3 a);
vec3 vec3_scale(vec3 a, int b);
// Place-wise min/max
vec3 vec3_min(vec3 a);
vec3 vec3_max(vec3 a);

// vec3 -> int
int vec3_sqr(vec3 a); // x^2 + y^2 + z^2
int vec3_dot(vec3 a, vec3 b);
int vec3_vol(vec3 a);


typedef int16_t block_t;

struct schematic;
typedef struct schematic schem;

schem *schem_init(int x, int y, int z);
schem *schem_copy(schem *schem);
void schem_kill(schem *schem);

vec3 schem_size(schem *schem);
int schem_volume(schem *schem);


schem *schem_from_file(const char *path);
int schem_to_file(schem *schem);

block_t schem_get(schem *schem, vec3 coord);
int schem_set(schem *schem, vec3 coord, block_t block);

int schem_resize(schem *schem, vec3 size);
int schem_shift(schem *schem, vec3 offset);
int schem_stack(schem *schem, vec3 counts);
int schem_flip(schem *schem, vec3 dirs); // Flips n times on each axis.
int schem_rotate(schem *schem, vec3 dirs); // Rotates by pi/2 n times on each axis.
int schem_fill(schem *schem, vec3 offset, vec3 size, block_t block);
int schem_insert(schem *into, vec3 offset, schem *from);
int schem_stacking_resize(schem *schem, vec3 size);

#endif // SCHEMATIC_H

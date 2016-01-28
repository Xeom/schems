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


typedef int16_t block_t;

struct schematic
{
    int width, height, length;

    block_t blocks[];
};

struct schematic *schem_init(int x, int y, int z);
struct schematic *schem_copy(struct schematic *schem);
void schem_kill(struct schematic *schem);

struct schematic *schem_from_file(const char *path);
int schem_to_file(struct schematic *schem);

block_t schem_get(struct schematic *schem, int 

#endif // SCHEMATIC_H

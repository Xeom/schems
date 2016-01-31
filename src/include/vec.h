#ifndef VEC_H
#define VEC_H

struct vec3i
{
    int x, y, z;
};
typedef struct vec3i vec3;

/* Because macros hate {} */
vec3 vec3_init(int x, int y, int z);
vec3 vec3_trip(int i);


/* Binary operators */

/* * Primary school maths class */
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, vec3 b);
vec3 vec3_div(vec3 a, vec3 b);
vec3 vec3_mod(vec3 a, vec3 b);

/* * Min/max */
vec3 vec3_min(vec3 a, vec3 b);
vec3 vec3_max(vec3 a, vec3 b);

/* Unary operators */

vec3 vec3_abs(vec3 a);
vec3 vec3_neg(vec3 a);
vec3 vec3_scale(vec3 a, int b);

/* vec3->int functions */

int vec3_sqr(vec3 a); /* x^2 + y^2 + z^2 */
int vec3_dot(vec3 a, vec3 b);
int vec3_vol(vec3 a);

int vec3_gt(vec3 a, vec3 b);
int vec3_lt(vec3 a, vec3 b);

#endif /* VEC_H */

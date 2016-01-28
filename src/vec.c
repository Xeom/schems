#include "include/vec.h"

#define min(a, b) ((a < b)? a : b)
#define max(a, b) ((a > b)? a : b)
#define abs(a) ((a >= 0)? a : -a)

/* Binary operators */

vec3 vec3_add(vec3 a, vec3 b)
{
	vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

vec3 vec3_sub(vec3 a, vec3 b)
{
	vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

vec3 vec3_mul(vec3 a, vec3 b)
{
	vec3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;

	return result;
}

vec3 vec3_div(vec3 a, vec3 b)
{
	vec3 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;

	return result;
}

vec3 vec3_min(vec3 a, vec3 b)
{
	vec3 result;
	result.x = min(a.x, b.x);
	result.y = min(a.y, b.y);
	result.z = min(a.z, b.z);

	return result;
}

vec3 vec3_max(vec3 a, vec3 b)
{
	vec3 result;
	result.x = max(a.x, b.x);
	result.y = max(a.y, b.y);
	result.z = max(a.z, b.z);

	return result;
}

/* Unary operators */

vec3 vec3_abs(vec3 a)
{
	vec3 result;
	result.x = abs(a.x);
	result.y = abs(a.y);
	result.z = abs(a.z);

	return result;
}

vec3 vec3_neg(vec3 a)
{
	vec3 result;
	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;

	return result;
}

vec3 vec3_scale(vec3 a, int b)
{
	vec3 result;
	result.x = b * a.x;
	result.y = b * a.y;
	result.z = b * a.z;

	return result;
}

/* vec3->int functions */

int vec3_sqr(vec3 a)
{
	int result = a.x * a.x;
	result    += a.y * a.y;
	result    += a.z * a.z;

	return result;
}

int vec3_dot(vec3 a, vec3 b)
{
	int result = a.x * b.x;
	result    += a.y * b.y;
	result    += a.z * b.z;

	return result;
}

int vec3_vol(vec3 a)
{
	return a.x * a.y * a.z;
}

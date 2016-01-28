/** schem/schematic.h: Schematic storage and manipulation.
 */

#ifndef SCHEMATIC_H
#define SCHEMATIC_H

typedef int16_t block_t;

struct schematic
{
    int width, height, length;

    block_t *blocks;
};


#endif // SCHEMATIC_H

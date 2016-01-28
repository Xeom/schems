/** schem/schematic.h: Schematic storage and manipulation.
 */

#ifndef SCHEMATIC_H
#define SCHEMATIC_H



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

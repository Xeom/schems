#include "schematic.h"

struct schematic
{
    int width, height, length;

    block_t blocks[];
};

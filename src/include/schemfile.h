#include "schematic.h"
#include <nbt/nbt.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef SCHEMFILE_H
#define SCHEMFILE_H

void   nbt_save_schem(nbt_node *nbt, schem *sch);
schem *nbt_load_schem(nbt_node *nbt);

void   file_save_schem(FILE *file, schem *sch);
schem *file_load_schem(FILE *file);

#endif /* SCHEMFILE_H */

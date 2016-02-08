#include "schemfile.h"

void nbt_save_schem(nbt_node *nbt, schem *sch)
{
	block_t block;
	nbt_node *bnode, *dnode;
	struct nbt_byte_array *barray, *darray;

	size_t vol, index;

	vol = (size_t)schem_vol(sch);

	nbt_find_by_name(nbt,  "Width")->payload.tag_short = schem_size(sch).x;
	nbt_find_by_name(nbt, "Height")->payload.tag_short = schem_size(sch).y;
	nbt_find_by_name(nbt, "Length")->payload.tag_short = schem_size(sch).z;

	bnode = nbt_find_by_name(nbt, "Blocks");
	dnode = nbt_find_by_name(nbt,   "Data");

	barray = &bnode->payload.tag_byte_array;
	darray = &dnode->payload.tag_byte_array;

	if (vol != (size_t)barray->length)
	{
		barray->length = vol;
		darray->length = vol;

		barray->data = realloc(barray->data, vol);
		darray->data = realloc(darray->data, vol);
	}

	for (index = 0; index < vol; ++index)
	{
		block = schem_get_index(sch, index);

		barray->data[index] = block.blockid;
		darray->data[index] = block.data;
	}
}

schem *nbt_load_schem(nbt_node *nbt)
{
	int8_t blockid, data;
	nbt_node *bnode, *dnode;
	size_t vol, index;
	schem *ret;

	ret = schem_init(
		vec3_init(nbt_find_by_name(nbt,  "Width")->payload.tag_short,
				  nbt_find_by_name(nbt, "Height")->payload.tag_short,
				  nbt_find_by_name(nbt, "Length")->payload.tag_short));

	vol = (size_t)schem_vol(ret);

	bnode = nbt_find_by_name(nbt, "Blocks");
	dnode = nbt_find_by_name(nbt,   "Data");

	for (index = 0; index < (size_t)vol; ++index)
	{
		blockid = bnode->payload.tag_byte_array.data[index];
		data    = dnode->payload.tag_byte_array.data[index];
		schem_set_index(ret, index, block_init(blockid, data));
	}

	return ret;
}

void file_save_schem(FILE *file, schem *sch)
{
	nbt_node *nbt = nbt_parse_file(file);
	nbt_save_schem(nbt, sch);
	nbt_dump_file(nbt, file, STRAT_INFLATE);
}

schem *file_load_schem(FILE *file)
{
	nbt_node *nbt = nbt_parse_file(file);
	schem    *sch = nbt_load_schem(nbt);

	return sch;
}

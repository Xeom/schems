#include "schemfile.h"

void nbt_save_schem(nbt_node *nbt, schem *sch)
{
	block_t block;
	nbt_node *bnode, *dnode;
	struct nbt_byte_array barray, darray;

	size_t vol, index;
	int8_t blockid, data;

	vol = (size_t)schem_volume(sch);

	bnode = nbt_find_by_name(nbt, "Blocks");
	dnode = nbt_find_by_name(nbt,   "Data");

	barray = bnode->payload.tag_byte_array;
	darray = dnode->payload.tag_byte_array;

	if (vol != (size_t)barray.length)
	{
		puts("Realloc");
		barray.length = vol;
		darray.length = vol;

		barray.data = realloc(barray.data, vol);
		darray.data = realloc(darray.data, vol);
	}

	for (index = 0; index < vol; ++index)
	{
		block = sch->blocks[index];
		blockid = (int8_t)((block & 0xff00) >> 8);
		data    = (int8_t)(block & 0x00ff);

		barray.data[index] = blockid;
		darray.data[index] = data;
	}
}

schem *nbt_load_schem(nbt_node *nbt)
{
	nbt_node *bnode, *dnode;

	size_t vol, index;
	int8_t blockid, data;

	schem *ret;

	ret = schem_init(
		vec3_init(nbt_find_by_name(nbt,  "Width")->payload.tag_short,
				  nbt_find_by_name(nbt, "Height")->payload.tag_short,
				  nbt_find_by_name(nbt, "Length")->payload.tag_short));

	vol = (size_t)schem_volume(ret);

	bnode = nbt_find_by_name(nbt, "Blocks");
	dnode = nbt_find_by_name(nbt,   "Data");

	for (index = 0; index < (size_t)vol; ++index)
	{
		blockid = bnode->payload.tag_byte_array.data[index];
		data    = dnode->payload.tag_byte_array.data[index];
		ret->blocks[index] = (int16_t)((blockid << 8) | data);
	}

	return ret;
}


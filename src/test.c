#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

#include "schematic.h"
#include "vec.h"
#include "schemfile.h"

typedef enum
{
	rotate,
	flip,
	stack,
	resize,
	fill,

	num_ops
} op;

#define is_op(v) (v < num_ops)

int main(int argc, char *argv[])
{

	nbt_node *nbt = nbt_parse_file(stdin);
	schem    *sch = nbt_load_schem(nbt);
	schem    *schnew;

	fputs("Doing the following shit\n", stderr);
	fputs("------------------------\n", stderr);

	struct option longopts[] =
	{
		{"rotate", required_argument, NULL, rotate},
		{"flip",   required_argument, NULL, flip},
		{"stack",  required_argument, NULL, stack},
		{"resize", required_argument, NULL, resize},
		{"fill",   required_argument, NULL, fill}
	};

	char opt;
	opterr = 0;
	while ((opt = getopt_long(argc, argv, "", longopts, NULL)) != -1)
	{

		vec3 vec;

		if (opt == stack)
			vec = vec3_init(1, 1, 1);

		else
			vec = vec3_init(0, 0, 0);

		if (is_op(opt))
		{
			int i;

			if (sscanf(optarg, "%*[^x]x%d", &i) == 1 || sscanf(optarg, "x%d", &i) == 1)
				vec.x = i;
			if (sscanf(optarg, "%*[^y]y%d", &i) == 1 || sscanf(optarg, "y%d", &i) == 1)
				vec.y = i;
			if (sscanf(optarg, "%*[^z]z%d", &i) == 1 || sscanf(optarg, "z%d", &i) == 1)
				vec.z = i;
		}

		if (opt == stack)
		{
			fprintf(stderr, "- Stacking { %d  %d  %d } times apparently\n", vec.x, vec.y, vec.z);
			schnew = schem_stack(sch, vec);
			schem_kill(sch);
			sch = schnew;
		}

		else if (opt == flip)
		{
			fprintf(stderr, "- Reflecting stuff by vector { %d  %d  %d }\n", vec.x, vec.y, vec.z);
			schnew = schem_flip(sch, vec);
			schem_kill(sch);
			sch = schnew;
		}

		else if (opt == rotate)
		{
			fprintf(stderr, "- Spinnin this shit by vector { %d  %d  %d }\n", vec.x, vec.y, vec.z);
			schnew = schem_rotate(sch, vec);
			schem_kill(sch);
			sch = schnew;
		}

//		else if (opt == fill)
//		{
			

		else if (opt == '?')
		{
			if (is_op(optopt))
				fprintf(stderr, "! Needs more fukkin arguments mate.\n");

			else
				fprintf(stderr, "! Fuck you think that op is?\n");

			abort();
		}

		else
		{
			fputs("! What the shit.", stderr);
			abort();
		}
	}
	fputs("------------------------\n", stderr);

	nbt_save_schem(nbt, sch);
	nbt_dump_file(nbt, stdout, STRAT_INFLATE);
	schem_kill(sch);
	nbt_free(nbt);
}

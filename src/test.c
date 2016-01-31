#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "schematic.h"
#include "vec.h"
#include "schemfile.h"

#define XYZ_SWITCH(chr, x, y, z)	\
char c = chr;						\
switch (c) {						\
	case 'x':case 'X': x break;	\
	case 'y':case 'Y': y break;	\
	case 'z':case 'Z': z break;	\
default:fprintf(stderr, "! '%c' isn't a fukkin direction. Try x, y or z.\n", c);abort();}


int main(int argc, char *argv[])
{

	nbt_node *nbt = nbt_parse_file(stdin);
	schem    *sch = nbt_load_schem(nbt);

	fputs("Doing the following shit\n", stderr);
	fputs("------------------------\n", stderr);

	char opt;
	opterr = 0;
	while ((opt = getopt(argc, argv, "f:r:")) != -1)
	{
		if (opt == 'f')
		{
			vec3 flip_vec = vec3_init(0, 0, 0);
			int i = -1;
			while (optarg[++i])
			{
				XYZ_SWITCH(optarg[i],
						   /* x */
						   fputs("- Flip on X axis\n", stderr);
						   flip_vec.x = 1;,
						   /* y */
						   fputs("- Flip on Y axis\n", stderr);
						   flip_vec.y = 1;,
						   /* z */
						   fputs("- Flip on Z axis\n", stderr);
						   flip_vec.z = 1;
					);
			}
			sch = schem_flip(sch, flip_vec);
		}

		else if (opt == 'r')
		{
			vec3 rot_vec = vec3_init(0, 0, 0);
			int i = -1;
			while (optarg[++i])
			{
				if (optarg[i] == 'c')
				{
					if (!optarg[++i])
						break;

					XYZ_SWITCH(optarg[i],
							   /* x */
							   fputs("- Rotate counter clockwise about X axis\n", stderr);
							   --(rot_vec.x);,
							   /* y */
							   fputs("- Rotate counter clockwise about Y axis\n", stderr);
							   --(rot_vec.y);,
							   /* z */
							   fputs("- Rotate counter clockwise about Z axis\n", stderr);
							   --(rot_vec.z);
						);
				}
				else
				{
					XYZ_SWITCH(optarg[i],
							   /* x */
							   fputs("- Rotate clockwise about X axis\n", stderr);
							   ++(rot_vec.x);,
							   /* y */
							   fputs("- Rotate clockwise about Y axis\n", stderr);
							   ++(rot_vec.y);,
							   /* z */
							   fputs("- Rotate clockwise about Z axis\n", stderr);
							   ++(rot_vec.z);
						);
				}
			}
			sch = schem_rotate(sch, rot_vec);
		}
		else if (opt == '?')
		{
			switch (optopt)
			{
			case 'r':
			case 'f':
				fprintf(stderr, "! You fucked up -%c you idiot. Prolly needs an option or smthn.\n", optopt);
				abort();
				break;
			default:
				fprintf(stderr, "! Yea I dunno what you think -%c is, but fuck off with it aight?\n", optopt);
				abort();
			}
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

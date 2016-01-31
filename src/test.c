#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "schematic.h"
#include "vec.h"
#include "schemfile.h"

int main(int argc, char *argv[])
{
	vec3 flip_vec = vec3_init(0, 0, 0);
	char opt;
	opterr = 0;
	int i;
	fputs("Doing the following shit\n", stderr);
	fputs("------------------------\n", stderr);
	while ((opt = getopt(argc, argv, "f:")) != -1)
	{
		switch (opt)
		{
		case 'f':
			i = -1;
			while (optarg[++i])
			{
				switch (optarg[i])
				{
				case 'x':
				case 'X':
					fputs("- Flip on X axis\n", stderr);
					flip_vec.x = 1;
					break;
				case 'y':
				case 'Y':
					fputs("- Flip on Y axis\n", stderr);
					flip_vec.y = 1;
					break;
				case 'z':
				case 'Z':
					fputs("- Flip on Z axis\n", stderr);
					flip_vec.z = 1;
					break;
				default:
					fprintf(stderr, "! '%c' isn't a fukkin direction. Try x, y or z.\n", optarg[i]);
					abort();
				}
			}
			break;
		case '?':
			switch (optopt)
			{
			case 'f':
				fprintf(stderr, "! You fucked up -%c you idiot. Prolly needs an option or smthn.\n", optopt);
				abort();
				break;
			default:
				fprintf(stderr, "! Yea I dunno what you think -%c is, but fuck off with it aight?\n", optopt);
				abort();
			}
			break;
		default:
			fputs("! What the shit.", stderr);
			abort();
		}
	}
	puts("------------------------");
	FILE *infile = fopen("out2.schematic", "r");
	nbt_node *nbt = nbt_parse_file(infile);
	fclose(infile);
	schem    *sch = nbt_load_schem(nbt);
	sch = schem_flip(sch, flip_vec);
	nbt_save_schem(nbt, sch);
	FILE *outfile = fopen("out2.schematic", "w");
	nbt_dump_file(nbt, outfile, STRAT_INFLATE);
	fclose(outfile);
	schem_kill(sch);
	nbt_free(nbt);
}

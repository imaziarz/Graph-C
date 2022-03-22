#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "graph.h"

char *usage =
	"Usage: %s --file data_file [--from k --to l --output output_file] [--grow n --gcol m --gfrom x --gto y --goutput data_output_file]";
int main(int argc, char **argv)
{
	int opt;
	char *inp = NULL;
	char *outn = NULL;
	int k = 0;
	int l = 0;
	int n = 10;
    int m = 10;
	double x = 0.01; 
	double y = 10;
	char *goutn = NULL;
	char *progname = argv[0];
	graph_t graph;
	int flag = 0; //flaga aby sprawdzić, czy podano choć jeden argument do znajdowania ścieżki.jeżeli nie-tylko generujemy graf

	const struct option long_options[] = {
		{"file", 1, NULL, 'f'},
		{"from", 1, NULL, 'k'},
		{"to", 1, NULL, 'l'},
		{"output", 1, NULL, 'o'},
		{"grow", 1, NULL, 'n'},
		{"gcol", 1, NULL, 'm'},
		{"gfrom", 1, NULL, 'x'},
		{"gto", 1, NULL, 'y'},
		{"goutput", 1, NULL, 'g'},
		{0,0,0,0}
	};
	int opt_indx = 0;
	while ((opt = getopt_long(argc, argv, "f:k:l:o:n:m:x:y:g:",long_options, &opt_indx)) != -1)
			{
				switch (opt)
				{
					case 'f':
						inp = optarg;
						flag = 1;
						break;
					case 'k':
						k = atoi(optarg);
						flag = 1;
						break;
					case 'l':
						l = atoi(optarg);
						flag = 1;
						break;
					case 'o':
                        outn = optarg;
						flag = 1;
                        break;
					case 'n':
                        n = atoi(optarg);
                        break;
					case 'm':
                        m = atoi(optarg);
                        break;
					case 'x':
                        x = atof(optarg);
                        break;
					case 'y':
						y = atof(optarg);
						break;
					case 'g':
						goutn = optarg;	
					default:
						fprintf(stderr, usage, progname);
						exit(EXIT_FAILURE);
				}
			}
	if (optind < argc)
	{
		fprintf(stderr, "Nieprawidłowe parametry!\n");
		for (; optind < argc; optind++)
			fprintf(stderr, "\t\"%s\"\n", argv[optind]);
		fprintf(stderr, "\n");
		fprintf(stderr, usage, progname);
		exit(EXIT_FAILURE);
	}
	if (flag == 1){
	if (inp != NULL)
	{
		FILE *in = fopen(inp, "r");
		if (in == NULL)
		{
			fprintf(stderr, "%s: nie udało się otworzyć plik z danymi: %s. Przerywam działanie...\n", argv[0], inp);
			exit(EXIT_FAILURE);
		}
		if (read_graph(in, graph))
		{
			exit(EXIT_FAILURE);
		}
		else 
			fclose(in);
	} else 
	{
		fprintf(stderr, "%s: nie został podany plik z danymi. Tworzę graf o rozmiarach %d, %d, z wartościami wag w zakresie [%f, %f].\n", argv[0], n, m, x, y);
		FILE *gout;
		if (goutn != NULL)
			gout = fopen(goutn, "w");
		else gout = stdout;
		generate_graph(graph, n, m, x, y);
		write_graph(graph, gout);
	}
	FILE *out;
	if (outn != NULL)
	{
		out = fopen(outn, "w");
		if (out == NULL){
			fprintf(stderr, "%s: nie udało się otworzyć plik do zapisu ścieżki: %s. Ścieżka będzie wypisana na stdout.", argv[0], outn);
			out = stdout;
		}
	}
	find_path(graph, k, l, out);
	}
	return 0;
}

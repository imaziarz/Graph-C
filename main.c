#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "graph.h"

char *usage =
	"Usage: %s [[--file data_file] [--from k] [--to l] [--output output_file]] [[--grow n] [--gcol m] [--gfrom x] [--gto y] [--gconnect s] [--goutput data_output_file]]\n"
	"	if any od first four parameters was given then\n"	
	"		if data_file is given then\n"
	"			reads graph points from data_file\n"
	"			finds path\n" 
	"				-from k (>= 0 and <= maximum node nr.) node (0 by default)\n"
	"				-to l (>= 0 and <= maximum node nr.) node (maximum node nr. by default)\n"
	"			writes the path to output_file (stdout by default)\n"
	"		else (data_file not given)\n"
	"			generate connected graph\n"
	"				-of n (> 0) rows (10 by default)\n"
	"				-of m (> 0) columns (10 by default)\n"
	"				-with edge weights within <x, y> range (<0.1, 10> by default) \n"
	"			write generated graph to data_output_file (stdout by default)\n"
	"			finds path\n" 
	"				-from k (>= 0 and <= maximum node nr.) node (0 by default)\n"
	"				-to l (>= 0 and <= maximum node nr.) node (maximum node nr. by default)\n"
	"			writes the path to output_file (stdout by default)\n"
	"		endif\n"
	"	else (none of first four parameters was given) then\n"
	"		generate graph\n"
	"			-of n (> 0) rows (10 by default)\n"
	"			-of m (> 0) columns (10 by default)\n"
	"			-with edge weights within <x, y> range (<0.1, 10> by default) \n"
	"			-connectivity defined by s: 0 - not connected, 1 - connected, 2 - random one (2 by default)\n"
	"		write generated graph to data_output_file (stdout by default)\n"
	"	endif\n";
int main(int argc, char **argv)
{
	int opt;
	char *inp = NULL;
	char *outn = NULL;
	int k = 0;
	int l = 0;
	int flagl = 0;
	int n = 10;
   	int m = 10;
	double x = 0.01; 
	double y = 10;
	int s = 2;
	char *goutn = NULL;
	char *progname = argv[0];
	graph_t graph = (graph_t) malloc (sizeof(graph_t));
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
		{"gconnect", 1, NULL, 's'},
		{"goutput", 1, NULL, 'g'},
		{0,0,0,0}
	};
	int opt_indx = 0;
	while ((opt = getopt_long(argc, argv, "f:k:l:o:n:m:x:y:s:g:",long_options, &opt_indx)) != -1)
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
						flagl = 1;
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
					case 's':
						s = atoi(optarg);
						break;
					case 'g':
						goutn = optarg;	
						break;
					default:
						fprintf(stderr, usage, progname, "\n");
						exit(EXIT_FAILURE);
				}
			}
	if (optind < argc)
	{
		fprintf(stderr, "Nieprawidłowe parametry!\n");
		for (; optind < argc; optind++)
			fprintf(stderr, "\t\"%s\"\n", argv[optind]);
		fprintf(stderr, "\n");
		fprintf(stderr, usage, progname, "\n");
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
		else{ 
			for (int i=0; i<graph->col*graph->col*graph->row*graph->row; i++)
				printf("waga: %lf\n", graph->weights[i]);
			fclose(in);
		}
	} else 
	{
		fprintf(stderr, "%s: nie został podany plik z danymi. Tworzę spójny graf o rozmiarach %d, %d, z wartościami wag w zakresie [%f, %f].\n", argv[0], n, m, x, y);
		FILE *gout;
		s = 1;
		if (goutn != NULL)
			gout = fopen(goutn, "w");
		else gout = stdout;
		generate_graph(graph, n, m, x, y, s);
		write_graph(graph, gout);
	}
	FILE *out;
	if (outn != NULL)
	{
		out = fopen(outn, "w");
		if (out == NULL){
			fprintf(stderr, "%s: nie udało się otworzyć plik do zapisu ścieżki: %s. Ścieżka będzie wypisana na stdout.\n", argv[0], outn);
			out = stdout;
		}
	}
	if (flagl == 0)
		l = graph->col * graph->row - 1;
	//find_path(graph, k, l, out);
	} else {
		FILE *gout;
		if (goutn != NULL)
			gout = fopen(goutn, "w");
		else gout = stdout;
		generate_graph(graph, n, m, x, y, s);
		write_graph(graph, gout);
	}
	return 0;
}

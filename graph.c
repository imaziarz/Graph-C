#include "graph.h"
#include <time.h>

typedef struct		//zostawiam aby sprawdzać narazie
{
	int row;
	int col;
	double *weights; //2d macierz sąsiedztwa, wymiary row*col * row*col, w postaci 1d
} *graph_t;

//3 poniższe funkcje są na razie robocze i nie działają
int read_graph(FILE* in, graph_t graph){
        int neighbor_index;
        char a, b, c;
        double weight;
        if (fscanf(in, "%d %d", &graph->row, &graph->col) != 2){
                fprintf(stderr, "Błędny format pliku z grafem. Przerywam działanie.\n");
                return 1;
        }
        int iter = graph->col * graph->row;
        graph->weights = (double*) malloc(iter * iter * sizeof(double));

        //początkowo ustawiam wszystkie wagi jako nieskończoność
        for (int i=0; i<iter*iter; i++){
                graph->weights[i] = 100.0; //tu ustawić inf
        }

        for (int i=0; i < iter; i++){
        //przejście do linii z wagami
                for (;;){
                fscanf(in, "%d", &neighbor_index);
                printf("neighbor index: %d\n", neighbor_index);
                /*b = fgetc(in) == ' ';
                printf("zmienna b: %c\n", b);
                c = fgetc(in) == ':';
                printf("zmienna c: %c\n", c);
                */
                fscanf(in, "%c", &b);
                fscanf(in, "%c", &c);
                fscanf(in, "%lf", &weight);
                printf("waga: %lf\n", weight);
                graph->weights[i * iter + neighbor_index] = weight;
                if (a = fgetc(in) == '\n')
                        break;
                else
                        continue;
        }
        }

        return 0;
}

void write_graph(graph_t graph, FILE *gout){
	fprintf(gout, "%d %d\n", graph->row, graph->col);
	for (int i = 0; i < (graph->col)*(graph->row); i++){
		fprintf(gout, "\t");
		for (int j = 0; j < (graph->col)*(graph->row); j++){
			if (graph->weights[i*(graph->col)*(graph->row) + j] != INFINITY)
				fprintf(gout, "%d :%lf\t", j, graph->weights[i*(graph->col)*(graph->row) + j]);
		}
		fprintf(gout, "\n");
	}
}

void generate_graph(graph_t graph, int n, int m, double x, double y, int s){
	graph->row = n;
	graph->col = m;
	        if (graph->row <= 0){
                printf("Liczba wierszy musi być dodatnia. Ustawiam wartość 10.\n");
                graph->row = 10;
		n = 10;
        }
        if (graph->col <= 0){
                printf("Liczba kolumn musi być dodatnia. Ustawiam wartość 10.\n");
                graph->col = 10;
		m = 10;
        }

	if (x <= 0 ){
		printf("Dolna granica przedziału losowanych wartości musi być dodatnia. Ustawiam wartość 0.01\n");
		x = 0.01;
	}
	if (y <= 0 ){
		printf("Górna granica przedziału losowanych wartości musi być dodatnia. Ustawiam wartość x + 1. \n");
		y = x + 1;
	}


	int temp = 0; //zmienna do kontrolowania, czy do macierzy wstawiane są wagi tylko po jednej stronie diagonali
	srand(time(NULL));
	for (int i=0; i<n*m; i++) //wstawiam wiersz po wierszu
		for (int j=0; j<n*m; j++){
			//wagi nie mogą być różne dla tych samych węzłów - trzeba je zduplikować
			//wstawienie wartości infinity na przekątnej macierzy sąsiedztwa (bo węzły nie są bezpośrednio połączone z samym sobą)
			graph->weights[i*(n*m+1)] = INFINITY;
			temp = i*(n*m+1); //przechowuje indeks węzła do którego ostatnio wstawiono INFINITY
			if ((i*n*m + j) > temp) //&& warunek, który nie pozwoli na wstawienie krawędzi do węzłów, które nie sąsiadują ze sobą
				graph->weights[i*n*m + j] = x + RAND()/RAND_MAX*(y-x); //przedział [x,y]
			else
				graph->weights[i*n*m + j] = INFINITY;
		}
}
void dijkstra(graph_t graph, int start, int *last, double *length)
{
	int col = graph->col;
	int row = graph->row;
	int q[row*col];
	int i, min_i;
	double tmp, min;
	for (i = 0; i < (row*col); i++){
		q[i]=i;
		length[i] = INFINITY;
		last[i] = -1;
	}
	length[start] = 0;
	do{
		min = INFINITY;
		min_i = INFINITY;
		for(i = 0; i < (row * col); i++){
			if ((q[i] == 0) && (length[i] < min)){
				min = length[i];
				min_i = i;
			}
		}
		if (min_i != INFINITY)
		{
			for( i = 0; i < (col*row); i++){
				if (graph->weights[min_i * col * row + i] > 0)
				{
					tmp = min + graph->weights[min_i * col * row + i];
					if (tmp < length[i]){
					length[i] = tmp;
					last[i] = min_i;
					}
				}
			}
			q[min_i] = 1;
		}
	} while (min_i < INFINITY);

}
int bfs(graph_t graph)
{
	int col = graph->col;
	int rows = graph->row;
	int n = 1;
	int row[n];
	int flag[col * rows];
	int i,j;
	int k = 0;
	row [k] = 0;
	for (i = 0; i < (col * rows); i++)
		flag[i] = 0;
	for (i = 0; i < n; i++){
		flag[row[k]]=1;
		for (j = 0; j < (col * rows); j++)
			if (graph->weights[row[k] * col * rows + j] != 0 && flag[j] == 0)
			{
				n+=1;
				row = malloc(n * sizeof(int));
				row[n-1] = j;
			}
		k++;
	}
	for (i=0; i < (col * rows); i++){
		if (flag[i] == 0)
			return 1;
	}
	return 0;
}
void find_path(graph_t graph, int k, int l, FILE *out)
{
	if (bfs(graph)) {
		fprintf(stderr, "Podany graf nie jest spójny. Przerywam działanie.");
		return EXIT_FAILURE;
	}
	double length[graph->col * graph->row];
	double last[graph->col * graph->row];
	dijkstra(graph, k, last, length);
	double path[];
	double weight[];
	fprintf(out, "Najkrótsza ścieżka: \n");
	int i = l;
	int k = 1;
	path = malloc(1 * sizeof(double));
	path[0] = l;
	while (last[i] != -1) {
		k++;
		path = malloc(k * sizeof(double));
		path[k - 1] = last[i];
		weight = malloc((k-1) * sizeof(double));
		weight[k - 2] = graph->weights[i * graph->col * graph->col + k];
		i = last[i];
	}
	for (int j = k - 1; j > 0; j--) {
		fprintf(out, "%f -%f- ", path[j], weight[j-1]);
	}
	fprintf(out, "%f\n", path[0]);
	fprintf(out, "Długość ścieżki równa %f", length[l]);

}

#include "graph.h"
#include <time.h>
#include <math.h>


//do funkcji trzeba przekazać zainicjalizowany graf z alokowaną pamięcią malloc (graph_t) sizeof(graph_t)

int read_graph(FILE* in, graph_t graph){
        int neighbor;
        int k; //zmienna określająca ile będzie iteracji w wierszu
        char b, c;
        double weight;
        if (fscanf(in, "%d %d", &graph->row, &graph->col) != 2){
                fprintf(stderr, "Błędny format pliku z grafem. Przerywam działanie.\n");
                return 1;
        }
        int n = graph->row;
        int m = graph->col;

        if (m <= 0 || n <= 0){
                fprintf(stderr, "Błędny format pliku z grafem. Liczba kolumn i wierszy w grafie musi być większa od 0. Przerywam działanie.\n");
                return 1;
        }

        int iter = n*m;
        graph->weights = (double*) malloc(iter * iter * sizeof(double));

        //początkowo ustawiam wszystkie wagi jako zero; oznacza to, że nie ma połączeń między węzłami
        for (int i=0; i<iter*iter; i++){
                graph->weights[i] = 0.0;
        }

        for (int i=0; i < iter; i++){ //tu iteruję po każdej linii

                if (n != 1 && m != 1){
			if (i==0 || i==iter-1 || i==m-1 || i==iter-m) //warunek na 2 sąsiadów
                        	k = 2;
                	else if ((i%m == 0 || i%m == m-1 || (i>0 && i<m) || (i>iter-m && i<iter))) //warunek na 3 sąsiadów
                        	k = 3;
                	else //w innym przypadku będzie 4 sąsiadów
                        	k = 4;
		}

		if (n==1 || m==1){
			if (i==0 || i==iter-1)
				k = 1;
			else
				k = 2;
		}

                for (int j=0; j<k; j++){ //iteracja od 2 do 4 razy - bo tyle sąsiadów może mieć węzeł
                        //oddzielny sposób na wczytywanie grafu z row lub col == 1; wtedy dopuszczam tylko jednego sąsiada
                fscanf(in, "%d", &neighbor);

                if (!(neighbor == i+1 || neighbor == i-1 || neighbor == i-m || neighbor == i+m)){
                       fprintf(stderr, "Błędny format pliku z grafem. Między węzłem %d a %d nie może istnieć połączenie. Przerywam działanie.\n", i, neighbor);
                        return 1;
                }
                fscanf(in, "%c", &b); //wczytanie spacji i dwukropka (bądź innych dwóch znaków oddzielających węzeł od wagi
                fscanf(in, "%c", &c);
                fscanf(in, "%lf", &weight);

                if (weight < 0){
                        printf("Waga krawędzi między węzłem %d i %d jest ujemna, równa %lf. Ustawiam wartość %lf.", i, neighbor, weight, fabs(weight));
                        weight = fabs(weight);
                }
                graph->weights[i * iter + neighbor] = weight; //ewentualna duplikacja wagi, gdyby do tej samej krawędzi zostały podane dwie różne
                graph->weights[neighbor * iter + i] = weight;
                }
        }
        return 0;
}


void write_graph(graph_t graph, FILE* gout){
        int iter = graph->row*graph->col; //zmienna do iterowania równa liczbie wszystkich węzłów
        fprintf(gout, "%d %d\n", graph->row, graph->col);

        for (int i=0; i<iter; i++){ //pętla zewnętrzna do przechodzenia po węzłach
                fprintf(gout, "\t");
                for (int j=0; j<iter; j++){ //pętla wewnętrzna do przechodzenia po sąsiadach
                        if (graph->weights[i*iter+j] > 0.0){ //jeśli jest połączenie
                                fprintf(gout, "%d :%lf ", j, graph->weights[i*iter+j]);
                        }
                }
                fprintf(gout, "\n");
        }
}

void generate_graph(graph_t graph, int n, int m, double x, double y, int s){
        // graf niespójny, gdy s==0; gdy s==2 następuje wylosowanie spójności
        if (n <= 0){
                printf("Liczba wierszy musi być dodatnia. Ustawiam wartość 10.\n");
                n = 10;
        }
        if (m <= 0){
                printf("Liczba kolumn musi być dodatnia. Ustawiam wartość 10.\n");
                m = 10;
        }
        if (x <= 0 ){
                printf("Dolna granica przedziału losowanych wartości musi być dodatnia. Ustawiam wartość 0.01\n");
                x = 0.01;
        }
        if (y <= 0 ){
                printf("Górna granica przedziału losowanych wartości musi być dodatnia. Ustawiam wartość %lf. \n", x + 1);
                y = x + 1;
        }
	if ((s!= 0) && (s!=1) && (s!=2)){
		printf("Podana nieprawidłowa wartość flagi spójności generowanego grafu. Ustawiam wartość 2 - losowo wybierana.");
		s = 2;
	}
        graph->row = n;
        graph->col = m;
        int iter = n*m;

        graph->weights = (double*) malloc(iter * iter * sizeof(double));

        srand(time(NULL));
	if (s == 2)
		s = (rand() % 2);
        for (int i=0; i<iter*iter; i++)
                graph->weights[i] = 0.0; //początkowe ustawienie wszystkich wag jako zero (nie ma połączeń)

        for (int i=0; i<iter; i++) //iteruję po elementach macierzy tylko po jednej przekątnej - nastąpi zduplikowanie wag na drugą przekątną
                for (int j=0+i; j<iter; j++){
                        if ((j==i+1 && (i+1)%m) || (j==i-1 && i%m) || (j==i-m && i>=m) || (j==i+m && i<iter-m)){
                                //powyższe warunki pozwalają na wstawienie krawędzi tylko między sąsiadami
                                if (s || ((double)rand()/RAND_MAX > 0.5)){ //jeżeli s==0, to wstawiam krawędź z 50% prawdopodobieństwem
                                        graph->weights[i*iter + j] = x + (double)rand()/RAND_MAX*(y-x);
                                        graph->weights[j*iter + i] = graph->weights[i*iter + j]; //duplikacja wagi
                        }
                }
                }
}


void dijkstra(graph_t graph, int start, int *last, double *length)
{
	int col = graph->col;
	int row = graph->row;
	int q[row*col];		//tabela, pokazująca, czy odpowiedni węzeł był odwiedzony
	int i, min_i;
	double tmp, min;
	for (i = 0; i < (row*col); i++){
		q[i]=0;
		length[i] = INFINITY;		//ustawiam koszty dojścia na nieskończoność
		last[i] = -1;			//poprzedniki na -1
	}
	length[start] = 0;
	do{				//wykonujemy pętlę aż indeks węzła o najmniejszym koszcie dojścia nie będzie pusty
		min = INFINITY;
		min_i = -1;
		for(i = 0; i < (row * col); i++){	//szukamy węzeł o najmniejszym koszcie dojścia
			if ((q[i] == 0) && (length[i] < min)){
				min = length[i];
				min_i = i;
			}
		}
		if (min_i != -1)
		{
			for( i = 0; i < (col*row); i++){		//przypisujemy dojścia do sąsiadów
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
	} while (min_i != -1);

}
int bfs(graph_t graph)
{
	int col = graph->col;
	int rows = graph->row;
	int n = 1;
	int *row;		//kolejka pryorytetowa
	row = malloc(1 * sizeof(int));
	int flag[col * rows];		//flaga odwiedzenia wierzchołka
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
				row = realloc(row, n * sizeof(int));
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
		fprintf(stderr, "Podany graf nie jest spójny. Przerywam działanie.\n");
		exit(1);
	}
	if (k < 0) {
		printf("Numer węzła początkowego musi być większy lub równy 0. Ustawiam wartość 0.\n");
		k = 0;
	} else if (k >= (graph->col * graph->row)) {
		printf("Numer węzła początkowego znajduje się za przedziałem grafu. Ustawiam wartość %d.\n", (graph->col * graph->row -1));
		k = graph->col * graph->row - 1;
	}
	if (l < 0) {
		printf("Numer węzła końcowego musi być większy lub równy 0. Ustawiam wartość 0.\n");
		l = 0;
	} else if (l >= (graph->col * graph->row)) {
		printf("Numer węzła początkowego znajduje się za przedziałem grafu. Ustawiam wartość %d.\n", (graph->col * graph->row - 1));
		l = graph->col * graph->row - 1;
	}
	double length[graph->col * graph->row];
	int last[graph->col * graph->row];
	dijkstra(graph, k, last, length);
	int *path;
	double *weight;
	weight = malloc(1 * sizeof(double));
	fprintf(out, "Najkrótsza ścieżka: \n");
	int i = l;
	int n = 1;
	path = malloc(1 * sizeof(int));
	path[0] = l;
	while (last[i] != -1) {
		n++;
		path = realloc(path, n * sizeof(int));
		path[n - 1] = last[i];
		weight = realloc(weight, (n-1) * sizeof(double));
		weight[n - 2] = graph->weights[i * graph->col * graph->row + last[i]];
		i = last[i];
	}
	for (int j = n - 1; j > 0; j--) {
		fprintf(out, "%d -%f- ", path[j], weight[j-1]);
	}
	fprintf(out, "%d\n", path[0]);
	fprintf(out, "Długość ścieżki równa %f", length[l]);

}

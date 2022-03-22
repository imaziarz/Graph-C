#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	int row;
	int col;
	double *weights;
} *graph_t;

int read_graph(FILE *in, graph_t graph);
void write_graph(graph_t graph, FILE *gout);
void generate_graph(graph_t graph, int n, int m, double x, double y, int s);
void dijkstra(graph_t graph, int start, int *last, double *length);
int bfs(graph_t graph);
void find_path(graph_t graph, int k, int l, FILE *out);

#endif

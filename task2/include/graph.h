#pragma once
#include "matrix.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct node;
typedef struct node node_t;

struct curve;
typedef struct curve curve_t;

struct pair;
typedef struct pair pair_t;

#include "queue.h"

struct node
{
	int size;
	int num;
	int n_curves;
	curve_t** out_curves;
};

struct curve
{
	node_t* parent;
	node_t* child;
};

struct pair
{
	int parent;
	int child;
};

void read_pair(pair_t** pair , int* counters);
node_t* create_node(int n , int n_curves_t);
curve_t* create_curve(node_t* parent_t , node_t* child_t);
void add_curve(node_t* node , curve_t* curve);
node_t** create_graph(int num_nodes , int num_curves , pair_t** pairs , int* counters);
void print_graph(node_t** nodes , int num_nodes);
void delete_graph(node_t** nodes , int num_nodes);
void BFS(node_t* start , matrix_t* matr , int num_nodes);
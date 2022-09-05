#include "../include/graph.h"

void read_pair(pair_t** pair)
{
	*pair = (pair_t*)calloc(1 , sizeof(pair_t));
	scanf("%d" , &((*pair)->parent));
	scanf("%d" , &((*pair)->child));
}

void print_pair(pair_t* pair)
{
	printf("%d %d" , pair->parent , pair->child);
}

node_t* create_node(int n , int n_curves_t)
{
	node_t* node = (node_t*)calloc(1 , sizeof(node_t));
	node->out_curves = (curve_t**)calloc(n_curves_t , sizeof(curve_t*));
	node->num = n;
	node->size = 0;
	node->n_curves = n_curves_t;
	return node;
}

void add_curve(node_t* node , curve_t* curve)
{
	node->out_curves[node->size] = curve;
	node->size++;
}

curve_t* create_curve(node_t* parent_t , node_t* child_t)
{
	curve_t* curve = (curve_t*)calloc(1 , sizeof(curve_t));
	curve->parent = parent_t;
	curve->child = child_t;
	return curve;
}

node_t** create_graph(int num_nodes , int num_curves , pair_t** pairs)
{
	node_t** nodes = (node_t**)calloc(num_nodes , sizeof(node_t*));
	int counter = 0;
	for (int i = 0 ; i < num_nodes ; ++i)
	{
		for (int j = 0 ; j < num_curves ; ++j)
		{
			if (pairs[j]->parent == i)
				counter++;
		}
		nodes[i] = create_node(i , counter);
		counter = 0;
	}
	for(int i = 0 ; i < num_curves ; ++i)
	{
		add_curve(nodes[pairs[i]->parent] , create_curve(nodes[pairs[i]->parent] , nodes[pairs[i]->child]));
	}
	return nodes;
}

void print_graph(node_t** nodes , int num_nodes)
{
	for (int i = 0 ; i < num_nodes ; ++i)
	{
		printf("%d\n" , nodes[i]->num);
		for (int j = 0 ; j < nodes[i]->size ; ++j)
		{
			printf("%d " , nodes[i]->out_curves[j]->child->num);
		}
		printf("\n\n");
	}
}

void delete_graph(node_t** nodes , int num_nodes)
{
	for (int i = 0 ; i < num_nodes ; ++i)
	{
		for (int j = 0 ; j < nodes[i]->size ; ++j)
		{
			free(nodes[i]->out_curves[j]);
		}
		free(nodes[i]->out_curves);
		free(nodes[i]);
	}
	free(nodes);
}

void BFS(node_t* start , matrix_t* matr , int num_nodes)
{
	int* marks = (int*)calloc(num_nodes , sizeof(int));
	queue_t* q = create_queue(num_nodes);
	push(q , start);
	printf("%d\n" , q->num);
	marks[start->num] = 1;
	matr->matr_buf[start->num][start->num] = 1;
	while(q->num != 0)
	{
		node_t* per = pop(q);
		for (int i = 0 ; i < per->size ; ++i)
		{
			if (marks[per->out_curves[i]->child->num] == 0)
			{
				marks[per->out_curves[i]->child->num] = 1;
				matr->matr_buf[per->out_curves[i]->child->num][start->num] = 1;
				push(q , per->out_curves[i]->child);
			}
		}
	}
	delete_queue(q);
	free(marks);
}
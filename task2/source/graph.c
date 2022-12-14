#include "../include/graph.h"

void read_pair(pair_t** pair , int* counters)
{
	*pair = (pair_t*)calloc(1 , sizeof(pair_t));
	scanf("%d %d" , &((*pair)->parent) , &((*pair)->child));
	counters[(*pair)->parent]++;
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

node_t** create_graph(int num_nodes , int num_curves , pair_t** pairs , int* counters)
{
	node_t** nodes = (node_t**)calloc(num_nodes , sizeof(node_t*));
	for (int i = 0 ; i < num_nodes ; ++i)
	{
		nodes[i] = create_node(i , counters[i]);
	}
	for(int i = 0 ; i < num_curves ; ++i)
	{
		add_curve(nodes[pairs[i]->parent] , create_curve(nodes[pairs[i]->parent] , nodes[pairs[i]->child]));
		free(pairs[i]);
	}
	free(pairs);
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
	marks[start->num] = 1;
	int count = 0;
	while(q->num != 0)
	{
		node_t* per = pop(q);
		matr->matr_buf[per->num][per->num] = '1';

		for (int i = 0 ; i < per->size ; ++i)
		{
			matr->matr_buf[per->out_curves[i]->child->num][per->num] = '1';
			if (marks[per->out_curves[i]->child->num] == 0)
			{
				marks[per->out_curves[i]->child->num] = 1;
				push(q , per->out_curves[i]->child);
			}

			for (int v = 0 ; v < num_nodes ; ++v)
			{
				if (matr->matr_buf[per->out_curves[i]->child->num][v] == '0')
				{
					if (matr->matr_buf[per->num][v] == '1')
					{
						matr->matr_buf[per->out_curves[i]->child->num][v] = '1';
					}
				}
				if (matr->matr_buf[v][per->num] == '0')
				{
					if (matr->matr_buf[v][per->out_curves[i]->child->num] == '1')
					{
						matr->matr_buf[v][per->num] = '1';
					}
				}

				if ((matr->matr_buf[per->num][v] == '0'))
				{
					if (matr->matr_buf[per->num][per->out_curves[i]->child->num] == '1')
					{
						if (matr->matr_buf[per->out_curves[i]->child->num][v] == '1')
						{
							matr->matr_buf[per->num][v] = '1';
						}
					}	
				}

				if ((matr->matr_buf[v][per->out_curves[i]->child->num] == '0'))
				{
					if (matr->matr_buf[per->num][per->out_curves[i]->child->num] == '1')
					{
						if (matr->matr_buf[v][per->num] == '1')
						{
							matr->matr_buf[v][per->out_curves[i]->child->num] = '1';
						}
					}	
				}

			}
		}
	}
	delete_queue(q);
	free(marks);
}
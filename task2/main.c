#include "include/matrix.h"
#include "include/graph.h"
#include "include/queue.h"

int main()
{
	int num_nodes = 0;
	int num_curves = 0;
	scanf("%d" , &num_nodes);
	scanf("%d" , &num_curves);
	pair_t** pairs = (pair_t**)calloc(num_curves , sizeof(pair_t*));
	for (int i = 0 ; i < num_curves ; ++i)
	{
		read_pair(pairs + i);
	}
	node_t** nodes = create_graph(num_nodes , num_curves , pairs);
	matrix_t* matr = create_matrix(num_nodes , num_nodes);
	for (int i = 0 ; i < num_nodes ; ++i)
	{
		BFS(nodes[i] , matr , num_nodes);
	}
	delete_graph(nodes , num_nodes);
	for (int i = 0 ; i < num_curves ; ++i)
	{
		free(pairs[i]);
	}
	free(pairs);
	print_matrix(matr);
	delete_matrix(matr);
	return 0;
}
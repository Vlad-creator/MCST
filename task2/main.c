#include "include/matrix.h"
#include "include/graph.h"

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
	print_graph(nodes , num_nodes);
	delete_graph(nodes , num_nodes);
	for (int i = 0 ; i < num_curves ; ++i)
	{
		free(pairs[i]);
	}
	free(pairs);
	return 0;
}
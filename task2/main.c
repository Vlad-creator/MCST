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
	int* global_marks = (int*)calloc(num_nodes , sizeof(int));
	for (int i = 0 ; i < num_nodes ; ++i)
	{
		BFS(nodes[num_nodes - i - 1] , matr , num_nodes , global_marks);
		global_marks[num_nodes - i - 1] = 1;
		/*for (int i = 0 ; i < num_nodes ; ++i)
		{
			printf("%d " , global_marks[i]);
		}
		printf("\n\n");
		print_matrix(matr);*/
	}
	delete_graph(nodes , num_nodes);
	print_matrix(matr);
	delete_matrix(matr);
	free(global_marks);
	return 0;
}
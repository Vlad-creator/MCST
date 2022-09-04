#include "../include/matrix.h"

struct matrix
{
	int** matr_buf;
	int num_str;
	int num_clm;
};

matrix_t* create_matrix(int n_str , int n_clm)
{
	matrix_t* matr = (matrix_t*)calloc(1 , sizeof(matrix_t));
	matr->matr_buf = (int**)calloc(n_str , sizeof(int*));
	for (int i = 0 ; i < n_str ; ++i)
	{
		matr->matr_buf[i] = (int*)calloc(n_clm , sizeof(int));
	}
	matr->num_str = n_str;
	matr->num_clm = n_clm;
	return matr;
}

void set_val(matrix_t* matr , int n_str , int n_clm , int val)
{
	matr->matr_buf[n_str][n_clm] = val;
}

void print_matrix(matrix_t* matr)
{
	for (int i = 0 ; i < matr->num_str ; ++i)
	{
		for (int j = 0 ; j < matr->num_clm ; ++j)
		{
			printf("%d " , matr->matr_buf[i][j]);
		}
		printf("\n");
	}
}

void delete_matrix(matrix_t* matr)
{
	for (int i = 0 ; i < matr->num_str ; ++i)
	{
		free(matr->matr_buf[i]);
	}
	free(matr->matr_buf);
	free(matr);
}

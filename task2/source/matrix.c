#include "../include/matrix.h"

matrix_t* create_matrix(int n_str , int n_clm)
{
	matrix_t* matr = (matrix_t*)calloc(1 , sizeof(matrix_t));
	matr->matr_buf = (char**)calloc(n_str , sizeof(char*));
	for (int i = 0 ; i < n_str ; ++i)
	{
		matr->matr_buf[i] = (char*)calloc(n_clm + 1 , sizeof(char));
		for (int j = 0 ; j < n_clm ; ++j)
		{
			matr->matr_buf[i][j] = '0';
		}
		matr->matr_buf[i][n_clm] = '\n';
	}
	matr->num_str = n_str;
	matr->num_clm = n_clm;
	return matr;
}

void set_val(matrix_t* matr , int n_str , int n_clm , char val)
{
	matr->matr_buf[n_str][n_clm] = val;
}

void print_matrix(matrix_t* matr)
{
	for (int i = 0 ; i < matr->num_str ; ++i)
	{
		fwrite(matr->matr_buf[i] , 1 , matr->num_clm + 1 , stdout);
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
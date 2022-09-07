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

int is_eq_matrix(matrix_t* matr1 , matrix_t* matr2 , int num)
{
	for (int i = 0 ; i < num ; ++i)
	{
		for (int j = 0 ; j < num ; ++j)
		{
			if (matr1->matr_buf[i][j] != matr2->matr_buf[i][j])
				return 0;
		}
	}
	return 1;
}

int convert_matrix(matrix_t* matr , int num)
{
	int res = 0;
	for (int i = 0 ; i < num ; ++i)
	{
		for (int j = 0 ; j < num ; ++j)
		{
			if (matr->matr_buf[j][i] == '1')
			{
				for (int k = 0 ; k < num ; ++k)
				{
					if ((matr->matr_buf[k][j] == '1') && (matr->matr_buf[k][i] == '0'))
					{
						matr->matr_buf[k][i] = matr->matr_buf[k][j];
						res = 1;
					}
				}
			}
			res = 0;
		}
	}
	return res;
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
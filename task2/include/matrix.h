#pragma once
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct matrix;
typedef struct matrix matrix_t;

struct matrix
{
	char** matr_buf;
	int num_str;
	int num_clm;
};

matrix_t* create_matrix(int n_str , int n_clm);
void set_val(matrix_t* matr , int n_str , int n_clm , char val);
void print_matrix(matrix_t* matr);
int is_eq_matrix(matrix_t* matr1 , matrix_t* matr2 , int num);
int convert_matrix(matrix_t* matr , int num);
void delete_matrix(matrix_t* matr);
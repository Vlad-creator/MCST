#pragma once
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct matrix;
typedef struct matrix matrix_t;

matrix_t* create_matrix(int n_str , int n_clm);
void set_val(matrix_t* matr , int n_str , int n_clm , int val);
void print_matrix(matrix_t* matr);
void delete_matrix(matrix_t* matr);

#pragma once
#include "graph.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct
{
	node_t** buf;
	int size;
	int num;
} queue_t;

queue_t* create_queue(int size);
void push(queue_t* q , node_t* val);
node_t* pop(queue_t* q);
void delete_queue(queue_t* q);


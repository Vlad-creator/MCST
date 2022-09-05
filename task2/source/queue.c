#include "../include/queue.h"

queue_t* create_queue(int size)
{
	queue_t* q = (queue_t*)calloc(1 , sizeof(queue_t));
	q->buf = (node_t**)calloc(size , sizeof(node_t*));
	q->size = size;
	q->num = 0;
	return q;
}

void push(queue_t* q , node_t* val)
{
	q->buf[q->num] = val;
	q->num++;
}

node_t* pop(queue_t* q)
{
	node_t* per = q->buf[0];
	for (int i = 0 ; i < q->num - 1 ; ++i)
	{
		q->buf[i] = q->buf[i + 1];
	}
	q->num = q->num - 1;
	return per;
}

void delete_queue(queue_t* q)
{
	free(q->buf);
	free(q);
}
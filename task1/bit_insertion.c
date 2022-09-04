#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

void cin_bin(uint64_t* buf);
void cout_bin(uint64_t buf);

int main()
{

	uint64_t M = 0;
	uint64_t N = 0;
	int i1 = 0;
	int i2 = 0;
	cin_bin(&N);
	cin_bin(&M);
	scanf("%d" , &i1);
	scanf("%d" , &i2);

	int dif = i2 - i1 + 1;

	M = M & (((uint64_t)1 << dif) - 1);
	if (M > N)
	{
		N = M;
	}
	else
	{
		uint64_t M1 = ~(((1 << dif) - 1) << i1);
		N &= M1;

		uint64_t M2 = M << i1;
		N |= M2;
	}

	cout_bin(N);
	printf("\n");
	return 0;
}


//Функция чтения входных данных
void cin_bin(uint64_t* buf)
{
	char* buf_str = (char*)calloc(64 , sizeof(char));
	char* end;
	scanf("%s" , buf_str);
	*buf = strtoul(buf_str , &end , 2);
	assert(end != NULL);
	free(buf_str);
}

//Функция вывода данных на экран в нужном формате
void cout_bin(uint64_t buf)
{
	int size = 0;
	int* buff = (int*)calloc(64 , sizeof(int));
	if (buf == 0)
	{
		printf("%d" , 0);
		return;
	}
	while(buf != 0)
	{
		buff[size] = buf % 2;
		buf = buf / 2;
		size++;
	}
	for (int i = 1 ; i <= size ; ++i)
	{
		printf("%d" , buff[size - i]);
	}
	free(buff);
}

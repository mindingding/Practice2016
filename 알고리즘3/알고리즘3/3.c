#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

int n;
double W;
double *w;
double *p;
int heap_limit;


typedef struct
{
	int level;
	int *inORnot;
	double weight;
	double profit;
	double boundary;
}basket;

typedef struct
{
	basket *s;
	int heap_size;
}heap;

void shiftdown(heap *H, int i)
{
	basket shiftkey;
	int parent;
	int found;
	int largerchild;

	shiftkey = H->s[i];
	parent = i;
	found = 0;
	while ((2 * parent <= H->heap_size) && !found)
	{
		if ((2 * parent < H->heap_size) && (H->s[2 * parent].boundary < H->s[2 * parent + 1].boundary))
			largerchild = 2 * parent + 1;
		else
			largerchild = 2 * parent;
		if (shiftkey.boundary < H->s[largerchild].boundary)
		{
			H->s[parent] = H->s[largerchild];
			parent = largerchild;
		}
		else
			found = 1;
	}
	H->s[parent] = shiftkey;
}

basket delete(heap *H)
{
	basket out;
	out = H->s[1];
	H->s[1] = H->s[H->heap_size];
	H->heap_size = H->heap_size - 1;
	shiftdown(H, 1);

	return out;
}

void insert(heap *H, basket *new)
{
	int parent;
	heap tempH;
	
	H->heap_size = H->heap_size + 1;

	if (H->heap_size == heap_limit) // 힙의 메모리할당량을 넘으면 2배로 다시 할당
	{
		heap_limit = heap_limit * 2;
		tempH.s = H->s;
		H->s = (basket *)malloc(sizeof(basket)*heap_limit);
		memcpy(H->s, tempH.s, sizeof(basket)*(heap_limit / 2));
		free(tempH.s);
	}
	H->s[H->heap_size].inORnot = (int*)malloc(sizeof(int)*n);
	memcpy(H->s[H->heap_size].inORnot, new->inORnot, sizeof(int)*n);
	H->s[H->heap_size].boundary = new->boundary;
	H->s[H->heap_size].level = new->level;
	H->s[H->heap_size].profit = new->profit;
	H->s[H->heap_size].weight = new->weight;

	parent = H->heap_size / 2;
	while (parent > 0)
	{
		shiftdown(H, parent);
		parent = parent / 2;
	}
}

double bound(basket u)
{
	int j, k;
	double total_weight;
	double result;

	if (u.weight >= W)
		return 0;
	else
	{
		result = u.profit;
		j = u.level + 1;
		total_weight = u.weight;
		while (j <= n && total_weight + w[j] <= W)
		{
			total_weight = total_weight + w[j];
			result = result + p[j];
			j++;
		}
		k = j;
		if (k <= n)
			result = result + (W - total_weight)*(p[k] / w[k]);
		return result;
	}
}

double knapsack(basket *maxnode) // maxnode에는 동적할당을 끝낸 node를 받는다.
{
	heap H;
	basket u, v;
	double max;
	H.heap_size = 0;
	heap_limit = 100; // 처음에 heap 멤모리 용량을 100으로 잡는다.
	H.s = (basket *)malloc(sizeof(basket)*heap_limit);
	// 끝
	v.level = 0;
	v.profit = 0;
	v.weight = 0;
	max = 0;
	v.boundary = bound(v);
	u.inORnot = (int *)malloc(sizeof(int)*n);
	memset(u.inORnot, 0, sizeof(int)*n);
	v.inORnot = (int *)malloc(sizeof(int)*n);
	memset(v.inORnot, 0, sizeof(int)*n);
	insert(&H, &v);
	free(v.inORnot);

	while (H.heap_size != 0)
	{
		v = delete(&H);
		if (v.boundary > max)
		{
			u.level = v.level + 1;
			u.weight = v.weight + w[u.level];
			u.profit = v.profit + p[u.level];
			memcpy(u.inORnot, v.inORnot, sizeof(int)*n);
			if (u.weight <= W && u.profit > max)
			{
				max = u.profit;
				u.inORnot[u.level - 1] = 1;
				memcpy(maxnode->inORnot, u.inORnot, sizeof(int)*n);
			}
			u.boundary = bound(u);
			if (u.boundary > max)
			{
				u.inORnot[u.level - 1] = 1;
				insert(&H, &u);
			}
			u.weight = v.weight;
			u.profit = v.profit;
			u.boundary = bound(u);
			if (u.boundary > max)
			{
				u.inORnot[u.level - 1] = 0;
				insert(&H, &u);
			}
		}
		free(v.inORnot);
	}
	free(H.s);
	free(u.inORnot);
	
	return max;
}



void main()
{
		FILE *fin, *fout;
		char s[100];
		int i, j;
		double max_profit;
		int *original;
		double totalW;
		double totalP;
		int totalS;
		int *sequence;
		basket max_basket;

		printf("input file name? ");
		scanf("%s", &s);

		fin = fopen(s, "r");
		fout = fopen("output.txt", "w");

		w = (double *)malloc(sizeof(double)*(n + 1));
		p = (double *)malloc(sizeof(double)*(n + 1));
		original = (int *)malloc(sizeof(int)*(n + 1));
		sequence = (int *)malloc(sizeof(int)*(n + 1));
		max_basket.inORnot = (int *)malloc(sizeof(int)*n);

		for (i = 0; i < n; i++)
			fscanf(fin, "%lf %lf", &p[i + 1], &w[i + 1]);

		for (i = 1; i <= n; i++)
			sequence[i] = i;

		for (i = 1; i < n; i++) // 입력받은 profit, weight를 profit/weight가 큰 순으로 정렬
		{
			for (j = n - 1; j >= i; j--)
			{
				if (p[j] / w[j] < p[j + 1] / w[j + 1])
				{
					totalP = p[j];
					totalW = w[j];
					p[j] = p[j + 1];
					w[j] = w[j + 1];
					p[j + 1] = totalP;
					w[j + 1] = totalW;
					totalS = sequence[j];
					sequence[j] = sequence[j + 1];
					sequence[j + 1] = totalS;
				}
			}
		}

		max_profit = knapsack(&max_basket);

		for (i = 0; i < n; i++)
			original[sequence[i + 1] - 1] = max_basket.inORnot[i];

		fprintf(fout, "%d", (int)max_profit);

		free(w);
		free(p);
		free(sequence);
		free(original);
		fclose(fin);
		fclose(fout);
		
}
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000
#pragma warning(disable : 4996)

void main() {

	FILE *fin;
	int N, B, C;
	int A[MAX], P[MAX];
	int i;
	int allNum = 0;
	int temp1 = 1, temp2 = 0;

	fin = fopen("input.txt", "r");

	fscanf(fin, "%d", &N);

	for (i = 0; i < N; i++) {
		fscanf(fin, "%d", &A[i]);
	}
	
	fscanf(fin, "%d %d", &B, &C);

	for (i = 0; i < N; i++) {

		temp1 = (A[i] - B) / C;

		if (temp1 == 0)
			temp1 = 1;
		else
			temp2 = 1;

		if ((A[i] - B - (C*temp1)) % C != 0)
			temp1 = temp1 + 1;

		P[i] = temp1 + temp2;

		temp2 = 0;

	}

	for (i = 0; i < N; i++) {
		allNum += P[i];
	}

	printf("%d\n", allNum);
}
	
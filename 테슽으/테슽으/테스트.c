#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 100
#pragma warning(disable : 4996)

typedef struct{
	int item;
}matrix;

matrix matrixs[MAX_TERMS];
int arr[MAX_TERMS] = { 0 };
int p[MAX_TERMS][MAX_TERMS] = { 0 };
int M[MAX_TERMS][MAX_TERMS] = { 0 };
int happy = 1;
void print_order(int i, int j);
void minmult(int n);
int mmul(int i, int j);
FILE* fout;

int main() {

	int n;
	int i, j;
	int result = 0;
	FILE* fin;
	char s[100];

	printf("input file name?\n");
	scanf("%s", &s);

	fin = fopen(s, "r");
	fscanf(fin, "%d", &n);

	fout = fopen("output.txt", "w");

	for (i = 0; i < n; i++) {
		fscanf(fin, "%d", &arr[i]);
	}
	/*
	for (i = 0; i < n; i++) {
		printf("%d", arr[i]);
	}
	puts("");
	*/
	minmult(n);
	/*
	for (i = 1; i < n; i++) {
		for (j = 1; j < n; j++) {
			printf("%3d(%d) ", M[i][j], p[i][j]);
		}
		puts("");
	}*/
	//printf("\n%d\n", result);

	print_order(1, n - 1);
	return 0;
}

void minmult(int n) {

	int i, j, k;
	int diagonal;
	int z = n;

	for (diagonal = 1; diagonal <= n; diagonal++) {

		j = diagonal;
		for (i = 1; i <= z; i++) {

			if (i == j)
				M[i][j] = 0;
			if (i<j)
				M[i][j] = mmul(i, j);

			j++;
		}
		z--;
	}
}

int mmul(int i, int j) {

	int k, temp;
	int result = -1;

	for (k = i; k < j; k++) {

		temp = M[i][k] + M[k + 1][j] + (arr[i - 1] * arr[k] * arr[j]);

		if (result > temp || result == -1) {
			result = temp;
			p[i][j] = k;
		}
	}
	return result;
}

void print_order(int i, int j) {

	int k = 0;
	//k = p[i][j];

	if (i == j) {
		fprintf(fout, "%d", happy++);
	}
	else {
		k = p[i][j];
		fprintf(fout, "(");
		print_order(i, k);
		print_order(k + 1, j);
		fprintf(fout, ")");
	}
}
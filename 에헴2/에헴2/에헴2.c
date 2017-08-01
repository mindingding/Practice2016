#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

#define MAX 30
#pragma warning(disable : 4996)

int sequence[MAX];
int A[MAX][MAX][MAX];
int P[MAX][MAX][MAX];
	
void main() {

	int i, j, v;
	int num, final;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &num);
	fscanf(fin, "%d", &final);

	for (i = 1; i <= num; i++) {
		fscanf(fin, "%d", &sequence[i]);
	}

	for (i = 1; i <= num; i++) {
		for (j = 1; j <= num; j++) {
			for (v = 0; v <= 30; v++) {
				A[v][i][j] = -1;
			}
		}
		A[sequence[i]][i][i] = sequence[i];
		P[sequence[i]][i][i] = i;
	}


	
}
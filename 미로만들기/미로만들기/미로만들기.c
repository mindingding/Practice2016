#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4996);
#define MAX 9999

FILE *fout;
int **arr;
char **touch;
int **length;
int n;
int count=0;
FILE *fout;
void dijstra(FILE *fin);
void print_path(int p, int q);

void main() {

	FILE *fin;
	
	int i, j;
	char s[100];
	printf("input file name?\n");
	scanf("%s", &s);

	fin = fopen(s, "r");
	fout = fopen("output.txt", "w");
	
	fscanf(fin, "%d", &n);

	arr = (int**)malloc(sizeof(int*)*n);
	for (i = 1; i <= n; i++) {
		arr[i] = (int*)malloc(sizeof(int)*n);
	}

	touch = (char**)malloc(sizeof(char*)*n);
	for (i = 1; i <= n; i++) {
		touch[i] = (char*)malloc(sizeof(char)*n);
	}

	length = (int**)malloc(sizeof(int*)*n);
	for (i = 1; i <= n; i++) {
		length[i] = (int*)malloc(sizeof(int)*n);
	}

	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			fscanf(fin, "%d", &arr[i][j]);
			length[i][j] = MAX;
		}
	}
	/*
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			printf("%2d", arr[i][j]);
		}puts("");
	}*/

	dijstra(fin);
	fprintf(fout, "%d\n", length[n][n]);
	//fprintf(fout, "path\n");
	print_path(n, n);

	return 0;
}

void dijstra(FILE *fin) {

	int i, j;
	int near;
	int min;
	int p, q;

	length[1][1] = 0;

	while (1) {

		min = MAX;

		for (i = 1; i <= n; i++) {
			for (j = 1; j <= n; j++) {
				if (length[i][j] >= 0 && length[i][j] <= min) {
					min = length[i][j];
					p = i; 
					q = j;
				}
			}
		}

		if (p == n && q == n) return;

		if (((p + 1 <= n ) && ( p + 1 > 0 )) && length[p + 1][q] == MAX) {
			if (arr[p + 1][q] == 1) //¾È±úµÎ‰Ò
				length[p + 1][q] = length[p][q];
			else if (arr[p + 1][q] == 0) //ºí·°±ú¾ßÇÔ
				length[p + 1][q] = length[p][q] + 1;

			touch[p + 1][q] = 'U';
		}

		if (((q + 1 <= n )&&( q+1 >0)) && length[p][q+1] == MAX) {
			if (arr[p][q+1] == 1) //¾È±úµÎ‰Ò
				length[p][q+1] = length[p][q];
			else if (arr[p][q+1] == 0) //ºí·°±ú¾ßÇÔ
				length[p][q+1] = length[p][q] + 1;

			touch[p][q+1] = 'L';
		}

		if ((( p - 1 > 0 && p - 1 <= n))&& length[p-1][q] == MAX) {
			if (arr[p - 1][q] == 1) //¾È±úµÎ‰Ò
				length[p - 1][q] = length[p][q];
			else if (arr[p - 1][q] == 0) //ºí·°±ú¾ßÇÔ
				length[p - 1][q] = length[p][q] + 1;

			touch[p - 1][q] = 'D';
		}

		if (((q - 1 > 0) &&( q-1 <= n)) && length[p][q -1] == MAX) {
			if (arr[p][q-1] == 1) //¾È±úµÎ‰Ò
				length[p][q-1] = length[p][q];
			else if (arr[p][q-1] == 0) //ºí·°±ú¾ßÇÔ
				length[p][q-1] = length[p][q] + 1;

			touch[p][q-1] = 'R';
		}
		//printf("a");
		length[p][q] = -1;
	}
}

void print_path(int p, int q) { //¸ÇÃ³À½¿¡ (n,n) µé¾î¿È

	if (p == 1 && q == 1) {
		fprintf(fout,"1 1\n");
		return;
	}
	if (touch[p][q] == 'U')
		print_path(p - 1, q);
	else if (touch[p][q] == 'D')
		print_path(p + 1, q);
	else if (touch[p][q] == 'R')
		print_path(p, q+1);
	else if (touch[p][q] == 'L')
		print_path(p, q-1);

	fprintf(fout, "%d %d\n", p, q);
}
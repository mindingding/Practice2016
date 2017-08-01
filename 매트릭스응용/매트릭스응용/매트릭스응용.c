#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

void main() {

	FILE *fin;
	int N;
	int size;
	char **arr, i, j;
	char temp1, temp2;
	int flag = 0;
	int t;

	fin = fopen("input.txt", "r");
	
	fscanf(fin, "%d", &N);
	fscanf(fin, "%d", &size);

	arr = (int**)malloc(sizeof(int*) * size);

	for (i = 0; i < size; i++) {
		arr[i] = (int*)malloc(sizeof(int)*size);
	}
	
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++)
				arr[i][j] = '0'; //배열 초기화
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%2c", arr[i][j]);
		}
		printf("\n");                         
	}
	while (!feof(fin)) {

		temp1 = fgetc(fin);
	
		if (temp1 == ',') {
			flag++;
			continue;
		}

		temp2 = fgetc(fin);

		if (flag == 0)
			arr[temp1][temp2] = 'A';
		else if (flag == 1)
			arr[atoi(temp1)][atoi(temp2)] = 'B';
		else if (flag == 2 )
			arr[atoi(temp1)][atoi(temp2)] = 'C';

		
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%2c", arr[i][j]);
		}
	}
}
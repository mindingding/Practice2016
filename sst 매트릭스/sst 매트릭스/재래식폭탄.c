#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable : 4996)

int get(int i, int j, char **arr, int num) {

	if (i < 0 || j < 0 || i >= num || j >= num)
		return 0;

	return 1;
}

void main() {

	FILE *fin;
	int N;
	int size;
	char **arr;
	int i, j;
	char temp1, temp2;
	char *result;
	int flag = 0;
	char num[80];
	int a = 0, b = 0;
	int tem = 0;

	fin = fopen("input.txt", "r");
	
	fscanf(fin, "%d", &N);
	fscanf(fin, "%d", &size);
	//printf("%d %d\n", N, size);
	arr = (char**)malloc(sizeof(char*) * size);

	for (i = 0; i < size; i++) {
		arr[i] = (char*)malloc(sizeof(char)*size);
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++)
			arr[i][j] = '0'; //배열 초기화
	}

	fscanf(fin, "%s", num);
	
	result = strtok(num, ",");
	while (result != NULL)
	{
		tem = strlen(result);

		for (i = 0; i < tem; i = i+2) {

			a = result[i] - '0';
			b = result[i + 1] - '0';
	
			if (flag == 0)
				arr[a][b] = 'A';
			else if (flag == 1)
				arr[a][b] = 'B';
			else
				arr[a][b] = 'C';
		}

		flag++;
		result = strtok(NULL, ",");
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {

			switch (arr[i][j])	 {

			case 'C':
				if (get(i - 3, j, arr, N) == 1 ) {
					arr[i - 3][j] = '1'; arr[i - 2][j] = '1'; arr[i - 1][j] = '1';
				}
				if (get(i, j - 3, arr, N) == 1 ) {
					arr[i][j - 3] = '1'; arr[i][j - 2] = '1'; arr[i][j - 1] = '1';
				}
				if (get(i + 3, j, arr, N) == 1 ){
					arr[i + 3][j] = '1'; arr[i + 2][j] = '1'; arr[i + 1][j] = '1';
				}
					if (get(i, j + 3, arr, N) == 1 ){
						arr[i][j + 3] = '1'; arr[i][j + 2] = '1'; arr[i][j + 1] = '1';
				}

			case 'B' :
				if (get(i - 2, j, arr, N) == 1 && arr[i - 2][j] == '0') { arr[i - 2][j] = '1'; arr[i - 1][j] = '1'; }
				if (get(i, j - 2, arr, N) == 1 && arr[i][j  - 2] == '0') { arr[i][j - 2] = '1'; arr[i][j - 1] = '1'; }
				if (get(i + 2, j, arr, N) == 1 && arr[i + 2][j] == '0') { arr[i + 2][j] = '1'; arr[i + 1][j] = '1'; }
				if (get(i, j + 2, arr, N) == 1 && arr[i][j + 2] == '0') {
					arr[i][j + 2] = '1'; arr[i][j + 1] = '1';
				}

			case 'A':
				if (get(i - 1, j, arr, N) == 1 && arr[i - 1][j] == '0') arr[i - 1][j] = '1';
				if (get(i, j - 1, arr, N) == 1 && arr[i][j - 1] == '0') arr[i][j - 1] = '1';
				if (get(i + 1, j, arr, N) == 1 && arr[i + 1][j] == '0') arr[i + 1][j] = '1';
				if (get(i, j + 1, arr, N) == 1 && arr[i][j + 1] == '0') arr[i][j + 1] = '1';
				}
		}
	}
	
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%2c", arr[i][j]);
		} puts("");
	}
	
}
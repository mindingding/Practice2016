#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable : 4996)
void main() {

	FILE *fin;
	int N;
	int **arr;
	int i, j;
	int temp[3];
	int *max_high;
	int *index;
	int cnt = 1;
	//가장 긴 길이는 음수 붙여서 가장 짧은길이 찾기~! --> DP

	fin = fopen("input.txt", "r");

	fscanf(fin, "%d", &N);

	arr = (int**)malloc(sizeof(int*)*N);
	for (i = 0; i < N; i++) 
		arr[i] = (int*)malloc(sizeof(int)*4);

	max_high = (int*)malloc(sizeof(int)*N);
	index = (int*)malloc(sizeof(int)*N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < 3; j++) {
			fscanf(fin, "%d", &arr[i][j]);
		}
		arr[i][3] = i+1;//원래 인덱스 저장 편의를 위해 +1
	}
	//넓이 기준 정렬
	for (i = 0; i < N; i++) {
		for (j = i+1; j < N; j++) {

			if (arr[i][0] > arr[j][0]) {
				*temp = arr[i];
				arr[i] = arr[j];
				arr[j] = *temp;
			}
		}
	}

	for (i = 0; i < N; i++)
		printf("%d %d %d\n", arr[i][0], arr[i][1], arr[i][2]);

	for (i = 0; i < N; i++)
		max_high[i] = 0;

		max_high[0] = arr[N- 1][1];
		index[0] = arr[N-1][3];
		//printf("a");


	for (i = 1; i < N; i++) {

			
			if (arr[N-i-1][2] < arr[N - i][2]) {
				max_high[i] = max_high[i - 1] + arr[N-i-1][1];
				index[i] = arr[N - i - 1][3]; cnt++;
			}
			else {
				max_high[i] = max_high[i-1];
				index[i] = 0;
			}
		
	}
	printf("%d\n", cnt);
	for (i = N-1; i >= 0; i--) {

		if (index[i] != 0){

			printf("%d\n", index[i]);
		}
	}
}
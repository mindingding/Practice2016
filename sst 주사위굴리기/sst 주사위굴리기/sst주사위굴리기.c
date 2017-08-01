#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

int main() {

	int N, M;
	int x, y;
	int K;
	int **arr;
	int *order;
	int i, j;
	int dice[7] = { 0 };
	int temp[7] = { 0 };

	int top = 1;
	int north = 2;
	int east = 3;
	int west = 4;
	int south = 5;
	int bottom = 6;

	scanf("%d %d %d %d %d", &N, &M, &x, &y, &K);
	
	arr = (int**)malloc(sizeof(int*)*N);
	for (i = 0; i < N; i++)
		arr[i] = (int*)malloc(sizeof(int)*M);

	for (i = 0; i < N; i++) {
		for (j = 0; j <M; j++) {
			scanf("%d", &arr[i][j]); //지도
		}
	}

	order = (int*)malloc(sizeof(int)*K);

	for (i = 0; i < K; i++)
		scanf("%d", &order[i]);

	//0이면, 주사위바닥면수 복사, 0이아니면 칸에 써잇는 수가 복사!, 칸에는 0이됨
	

	for (i = 0; i < K; i++) {

		for (j = 1; j <= 6; j++) {
			temp[j] = dice[j];
		}

		if (order[i] == 1) {

	
				y++;
				dice[top] = temp[west];
				dice[west] = temp[bottom];
				dice[east] = temp[top];
				dice[bottom] = temp[east];

				if (arr[x][y] == 0)
					arr[x][y] = dice[bottom];
				else {
					dice[bottom] = arr[x][y];
					arr[x][y] = 0;
					//printf("%d\n", dice[bottom]);
				}

				printf("%d\n", dice[top]);
			
			
		}
		else if (order[i] == 2) {

			
	
				y--;
				dice[top] = temp[east];
				dice[east] = temp[bottom];
				dice[bottom] = temp[west];
				dice[west] = temp[top];

				if (arr[x][y] == 0)
					arr[x][y] = dice[bottom];
				else {
					dice[bottom] = arr[x][y];
					arr[x][y] = 0;
				}

				printf("%d\n", dice[top]);
	
		
		}
		else if (order[i] == 3) {

				x--;

				dice[top] = temp[south];
				dice[south] = temp[bottom];
				dice[bottom] = temp[north];
				dice[north] = temp[top];

				if (arr[x][y] == 0)
					arr[x][y] = dice[bottom];
				else {
					dice[bottom] = arr[x][y];
					arr[x][y] = 0;
				}

				printf("%d\n", dice[top]);
			
		
		}

		else if (order[i] == 4) {

				x++;
				dice[top] = temp[north];
				dice[north] = temp[bottom];
				dice[bottom] = temp[south];
				dice[south] = temp[top];
				//printf("A");
				if (arr[x][y] == 0)
					arr[x][y] = dice[bottom];
				else {
					dice[bottom] = arr[x][y];
					arr[x][y] = 0;
				}

				printf("%d\n", dice[top]);
			
		}
	}

	return 0;
}
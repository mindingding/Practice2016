#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

int **arr;
int L;
int check(int a, int b);
void main() {

	FILE *fin;

	int turn;

	int i, j;
	int length = 0;
	int *t;
	char *dir;

	int east = 1;
	int west = 2;
	int north = 3;
	int south = 4;
	int comp = 0;
	int direction = 1; //처음에는 동쪽 보고 있으니까
	int temp = 0;
	int x = 0, y = 0;
	int time = 1;

	fin = fopen("input.txt", "r");

	fscanf(fin, "%d", &L);
	fscanf(fin, "%d", &turn);

	length = (2 * L) + 1;
	arr = (int**)malloc(sizeof(int*)*length);

	for (i = 0; i < length; i++)
		arr[i] = (int*)malloc(sizeof(int)*length);

	t = (int*)malloc(sizeof(int)*turn);
	dir = (char*)malloc(sizeof(char)*turn);

	for (i = 0; i < turn; i++)
		fscanf(fin, "%d %c", &t[i], &dir[i]);

	//for (i = 0; i < turn; i++) {
		//printf("%d %c\n", t[i], dir[i]);
	//}

	for (i = 0; i < length; i++) {
		for (j = 0; j < length; j++) {
			arr[i][j] = 0;
		}
	}
	//처음위치 : L,L
	//arr 0으로 초기화하고 
	x = L;
	y = L;
	arr[x][y] = 1;

	for (i = 0; i < turn; i++) {

		for (j = 0; j < t[i]; j++) {

			switch (direction) {
			case 1:
				comp = check(x, y + 1);
				if (comp == 0) {
					arr[x][y + 1] = 1;
					time++;
					y++;
				}
				else {
					printf("%d", time);
					return 0;
				}
				break;
			case 2:
				comp = check(x, y - 1);
				if (comp == 0) {
					arr[x][y - 1] = 1;
					time++;
					y--;
				}
				else {
					printf("%d", time);
					return 0;
				}
				break;
			case 3:
				comp = check(x-1, y);
				if (comp == 0) {
					arr[x-1][y] = 1;
					time++;
					x--;
				}
				else {
					printf("%d", time);
					return 0;
				}
				break;
			case 4:
				comp = check(x+1, y);
				if (comp == 0) {
					arr[x][y + 1] = 1;
					time++;
					x++;
				}
				else {
					printf("%d", time);
					return 0;
				}
				break;
			}
		}

		switch (dir[i]) {

		case 'L':
			if (direction == 1)
				temp = 3;
			else if (direction == 2)
				temp = 4;
			else if (direction == 3)
				temp = 2;
			else if (direction == 4)
				temp = 1;
			break;
		case 'R':
			if (direction == 1)
				temp = 4;
			else if (direction == 2)
				temp = 3;
			else if (direction == 3)
				temp = 1;
			else if (direction == 4)
				temp = 2;
			break;
		}

		direction = temp;

	}


}

int check(int a, int b) {

	if (a > (2 * L) || b > (2 * L) || a<0 || b<0 || arr[a][b] == 1)
		return 1;

	else return 0;
}
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4996)
#define NUM 10
#define INFINITE 9999

FILE *fout;
int PP[NUM][NUM][NUM];

void path(int LEFT, int RIGHT, int LR);

void main() {

	int number, first_location, i, j;
	int D[NUM]; //가로등 위치
	int W[NUM]; //1초에 전력소비량
	int EE[NUM][NUM][NUM]; //EE는 초기상태에서 L~R까지 다 끄고 현재 L에 있을 때까지 총 소비전력
	int DD[NUM][NUM][NUM]; //DD는 초기상태에서 L~R까지 다 끄고 현지 L에 있을 때까지 총 이동거리
	int LEFT, RIGHT;
	int temp1 = 0, temp2 = 0;
	
	FILE *fin = fopen("light_in.txt", "r");
	fout = fopen("light_out.txt", "w");

	fscanf(fin, "%d\n", &number);
	fscanf(fin, "%d\n", &first_location);
	
	for (i = 1; i <= number; i++) {
		fscanf(fin, "%d %d", &D[i], &W[i]);
		for (j = 1; j <= number; j++) {
			DD[i][j][1] = DD[i][j][2] = 0;

			if (i > first_location || j <= first_location)
				EE[i][j][1] = EE[i][j][2] = INFINITE;
		}
		EE[i][i][1] = EE[i][i][2] = INFINITE;
		DD[i][i][1] = DD[i][i][2] = PP[i][i][1] = PP[i][i][2] = 0;
	}
	printf("a");
	EE[first_location][first_location][1] = EE[first_location][first_location][2] = 0;

	for (RIGHT = first_location; RIGHT <= number; RIGHT++) {
		for (LEFT = first_location; LEFT >= 1; LEFT--){

			if ((RIGHT == first_location) && (LEFT = first_location))
				continue;

			temp1 = EE[LEFT + 1][RIGHT][1] + (D[LEFT + 1] - D[LEFT] + DD[LEFT + 1][RIGHT][1]) * W[LEFT];
			temp2 = EE[LEFT + 1][RIGHT][2] + (D[RIGHT] - D[LEFT] + DD[LEFT + 1][RIGHT][2]) * W[LEFT];

			if (temp1 < temp2) {

				EE[LEFT][RIGHT][1] = temp1;
				DD[LEFT][RIGHT][1] = DD[LEFT + 1][RIGHT][1] + D[LEFT + 1] - D[LEFT];
				PP[LEFT][RIGHT][1] = LEFT + 1;
			}
			else {
				EE[LEFT][RIGHT][1] = temp2;
				DD[LEFT][RIGHT][1] = DD[LEFT + 1][RIGHT][2] + D[RIGHT] - D[LEFT];
				PP[LEFT][RIGHT][1] = RIGHT;
			}

			temp1 = EE[LEFT][RIGHT - 1][1] + (DD[LEFT][RIGHT - 1][1] + D[RIGHT] - D[LEFT]) * W[RIGHT];
			temp2 = EE[LEFT][RIGHT - 1][2] + (DD[LEFT][RIGHT - 1][2] + D[RIGHT] - D[RIGHT - 1]) * W[RIGHT];

			if (temp1 < temp2) {
				EE[LEFT][RIGHT][2] = temp1;
				DD[LEFT][RIGHT][2] = DD[LEFT][RIGHT - 1][1] + D[RIGHT] - D[LEFT];
				PP[LEFT][RIGHT][2] = LEFT;
			}

			else {
				EE[LEFT][RIGHT][2] = temp2;
				DD[LEFT][RIGHT][2] = DD[LEFT][RIGHT - 1][2] + D[RIGHT] - D[RIGHT - 1];
				PP[LEFT][RIGHT][2] = RIGHT - 1;
			}
		}
	}

	if (EE[1][number][1] < EE[1][number][2]) {
		fprintf(fout, "%d\n", EE[1][number][1]);
	
		path(1, number, 1);
		fprintf(fout, "1\n");
	}
	else {
		fprintf(fout, "%d\n", EE[1][number][2]);
		path(1, number, 2);
		fprintf(fout, "%d\n", number);
	}
}

void path(int LEFT, int RIGHT, int LR) {

	if (LEFT == RIGHT) return;

	if (PP[LEFT][RIGHT][LR] == LEFT + 1 && LR == 1) path(LEFT + 1, RIGHT, 1);
	else if (PP[LEFT][RIGHT][LR] == RIGHT && LR == 1) path(LEFT + 1, RIGHT, 2);
	else if (PP[LEFT][RIGHT][LR] == LEFT && LR == 2) path(LEFT, RIGHT - 1, 1);
	else if (PP[LEFT][RIGHT][LR] == RIGHT - 1 && LR == 2) path(LEFT, RIGHT - 1, 2);

	fprintf(fout, "%d\n", PP[LEFT][RIGHT][LR]);
}
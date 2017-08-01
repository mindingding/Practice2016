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
	int W[NUM]; //초당 전력 소비량
	int EE[NUM][NUM][NUM]; 
	int DD[NUM][NUM][NUM];
	int LEFT, RIGHT;
	int temp1 = 0, temp2 = 0;
	char s[100];

	FILE *fin;

	printf("input file name?\n");
	scanf("%s", &s);

	fin = fopen(s, "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d\n", &number);
	fscanf(fin, "%d\n", &first_location);

	for (i = 1; i <= number; i++) {
		fscanf(fin, "%d %d", &D[i], &W[i]);
		for (j = 1; j <= number; j++) {
			DD[1][i][j] = DD[2][i][j] = 0;

			if (i > first_location || j <= first_location)
				EE[1][i][j] = EE[2][i][j] = INFINITE;
		}
		EE[1][i][i] = EE[2][i][i] = INFINITE;
		DD[1][i][i] = DD[2][i][i] = PP[1][i][i] = PP[2][i][i] = 0;
	}

	EE[1][first_location][first_location] = EE[2][first_location][first_location] = 0;

	for (RIGHT = first_location; RIGHT <= number; RIGHT++)
	{
		for (LEFT = first_location; LEFT >= 1; LEFT--)
		{

			if (RIGHT == first_location && LEFT == first_location) continue;

			temp1 = EE[1][LEFT + 1][RIGHT] + (D[LEFT + 1] - D[LEFT] + DD[1][LEFT + 1][RIGHT]) * W[LEFT];
			temp2 = EE[2][LEFT + 1][RIGHT] + (D[RIGHT] - D[LEFT] + DD[2][LEFT + 1][RIGHT]) * W[LEFT];

			if (temp1 < temp2) {

				EE[1][LEFT][RIGHT] = temp1;
				DD[1][LEFT][RIGHT] = DD[1][LEFT + 1][RIGHT] + D[LEFT + 1] - D[LEFT];
				PP[1][LEFT][RIGHT] = LEFT + 1;
			}
			else {
				EE[1][LEFT][RIGHT] = temp2;
				DD[1][LEFT][RIGHT] = DD[2][LEFT + 1][RIGHT] + D[RIGHT] - D[LEFT];
				PP[1][LEFT][RIGHT] = RIGHT;
			}

			temp1 = EE[1][LEFT][RIGHT - 1] + (DD[1][LEFT][RIGHT - 1] + D[RIGHT] - D[LEFT]) * W[RIGHT];
			temp2 = EE[2][LEFT][RIGHT - 1] + (DD[2][LEFT][RIGHT - 1] + D[RIGHT] - D[RIGHT - 1]) * W[RIGHT];

			if (temp1 < temp2) {
				EE[2][LEFT][RIGHT] = temp1;
				DD[2][LEFT][RIGHT] = DD[1][LEFT][RIGHT - 1] + D[RIGHT] - D[LEFT];
				PP[2][LEFT][RIGHT] = LEFT;
			}

			else {
				EE[2][LEFT][RIGHT] = temp2;
				DD[2][LEFT][RIGHT] = DD[2][LEFT][RIGHT - 1] + D[RIGHT] - D[RIGHT - 1];
				PP[2][LEFT][RIGHT] = RIGHT - 1;
			}
		}
	}

	if (EE[1][1][number] < EE[2][1][number]) {
		fprintf(fout, "%d\n", EE[1][1][number]);

		path(1, number, 1);
		fprintf(fout, "1\n");
	}
	else {
		fprintf(fout, "%d\n", EE[2][1][number]);
		path(1, number, 2);
		fprintf(fout, "%d\n", number);
	}

	//printf("complete\n");
}

void path(int LEFT, int RIGHT, int LR) {

	if (LEFT == RIGHT) return;

	if (PP[LR][LEFT][RIGHT] == LEFT + 1 && LR == 1) path(LEFT + 1, RIGHT, 1);
	else if (PP[LR][LEFT][RIGHT] == RIGHT && LR == 1) path(LEFT + 1, RIGHT, 2);
	else if (PP[LR][LEFT][RIGHT] == LEFT && LR == 2) path(LEFT, RIGHT - 1, 1);
	else if (PP[LR][LEFT][RIGHT] == RIGHT - 1 && LR == 2) path(LEFT, RIGHT - 1, 2);

	fprintf(fout, "%d\n", PP[LR][LEFT][RIGHT]);


}


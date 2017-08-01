#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
#define MAX 100000

void main() {

	FILE *fin;
	int i;
	int testcase;
	float *p;
	int yoon[MAX];
	int coin[MAX];
	float flag = 0;
	int n = 1, j, k;
	int temp;
	int min = 99999;
	int result[6] = { 0 };
	int a = 0, b = 0;

	fin = fopen("input.txt", "r");
	fscanf(fin, "%d", &testcase);

	for (k = 1; k <= testcase; k++) {

		p = (float*)malloc(sizeof(float)*testcase);
		fscanf(fin, "%f", &p[k]);

		while (1) {
			//printf("%f ", p[i]);
			flag = p[k] * n;
			if ((flag - (int)flag) == 0)
				b                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                reak;
			else
				n++;
		}
		n = 1;
		temp = flag;
		//printf("%d ", temp);

		for (i = 0; i <= temp; i++) { //1~5까지 비교

			if (i == 0)
				yoon[i] = 0;

			else {
				for (j = 1; j <= 5; j++) {

					if (i - j < 0)
						continue;

					if (yoon[i - j] < min) {

						min = yoon[i-j];
						coin[i] = j;
					}
				}

				yoon[i] = min + 1;
				min = MAX;
			}
		}

		printf("#testcase %d\n", k);
		for (i = 1; i <= temp; i++) {
			//printf("%d ", coin[i]);
		}
		puts("");
		for (i = 1; i <= temp; i++) {
			//printf("%d ", yoon[i]);
		}
		puts("");
		a = temp;

		for (i = 1; i <= temp / p[k]; i++) {

			b = coin[a];
			//printf("%d ", b);
			result[b]++;
			a = a - b;
		}

		for (i = 1; i <= 5; i++) {
			printf("%d ", result[i]);
		}
		for (i = 1; i <= 5; i++) {
			result[i] = 0;
		}
		puts("");
	}


}
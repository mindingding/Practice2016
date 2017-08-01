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
	float flag = 0;
	int n = 1;
	int temp;

	fin = fopen("input.txt", "r");
	fscanf(fin, "%d", &testcase);

	for (i = 1; i <= testcase; i++) {

		p = (float*)malloc(sizeof(float)*testcase);
		fscanf(fin, "%f", &p[i]);

		while (1) {
			//printf("%f ", p[i]);
			flag = p[i] * n;
			if ((flag - (int)flag) == 0)
				break;
			else
				n++;
		}
		n = 1;
		temp = flag;
		printf("%d ", temp);
	}
	
}
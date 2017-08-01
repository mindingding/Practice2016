// 돈까스 소스
#include <stdio.h>
#include <stdlib.h>

int *sequence;
int ***A;
int ***P;
int N;

void print_value(int,int,int,int);
int calculate(int, int, int);

FILE *input;
FILE *output;

void main() {
	int i, j, v, final;
	char fname[50];
	printf("input file name? ");
	gets(fname);
	if((input=fopen(fname, "r"))==NULL) {
		printf("파일 오류");
		exit(1);
	}

	if((output=fopen("output.txt", "w"))==NULL) {
		printf("파일 오류");
		exit(1);
	}

	fscanf(input, "%d", &N);
	fscanf(input, "%d", &final);

	sequence = (int*) malloc(sizeof(int*)*(N+1)); 
	A = (int***) malloc(sizeof(int**)*(N+1));
	P = (int***) malloc(sizeof(int**)*(N+1)); 
	
	for(i=1; i<=N; i++) {
		fscanf(input, "%d\n", &sequence[i]);
		A[i] = (int**) malloc(sizeof(int*)*(N+1));
		P[i] = (int**) malloc(sizeof(int*)*(N+1));
		for(j=1; j<=N; j++) {
			A[i][j] = (int*) malloc(sizeof(int)*31);
			P[i][j] = (int*) malloc(sizeof(int)*31);
			for(v=0; v<=30; v++) {						// 초기화
				A[i][j][v] = -1;
			}
		}
		A[i][i][sequence[i]] = sequence[i];				// 초기화
		P[i][i][sequence[i]] = i;						// 초기화
	}

	for(i = 1; i <= N; i++)								// 축소 연산
		for(j =  1; (j + i) <= N; j++)
			for(v = 0; v<= 30; v++)			
				calculate(j, j+i, v);					

	if(A[1][N][final]==-1) fprintf(output, "0");
	else print_value(1, N, final, 1);					// 출력
}

void print_value(int i,int j, int v, int x) {
	int u, k;
	if(i < j) {
		u = A[i][j][v];
		k = P[i][j][v];
		if(A[i][k][u] != -1 && A[k+1][j][u+v] != -1) {
			print_value(i, k, u, x);
			print_value(k+1, j, u+v, x+1);
			fprintf(output, "%d\n", x);
		} else {
			print_value(i, k, u+v, x);
			print_value(k+1, j, u, x+1);
			fprintf(output, "%d\n", x);
		}
	}
}

int calculate(int i, int j, int v) {
	int u, k;
	for(u=i; u<j; u++) {
		for(k=0; k<=(30-v); k++) {
			if((A[i][u][k] != -1 && A[u+1][j][k+v] != -1) || (A[i][u][k+v] != -1 && A[u+1][j][k] != -1)) {
				A[i][j][v] = k;
				P[i][j][v] = u;
				return 0;
			}
		}
	}
}

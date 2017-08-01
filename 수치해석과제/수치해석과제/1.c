//컴퓨터학부 2015117662 권민지
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n;
float dataset[2][100];

void linear_regression();
void quadratic_regression();

void main() {

	FILE *fin;
	FILE *fout;
	int count = 1;
	int i, j;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	while (!feof(fin)) {

		fscanf(fin, "%d", &n);

		for (i = 0; i < n; i++)
			fscanf(fin, "%f %f", &dataset[0][i], &dataset[1][i]); //x, y

		//for (i = 0; i < n; i++)
			//printf("%f %f\n", dataset[0][i], dataset[1][i]);

		printf("Test %d\n", count++);
		linear_regression();
		quadratic_regression();

	}

}

void linear_regression()
{
	float a0 = 0, a1 = 0;
	int i, j;
	float temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0;
	float temp5 = 0, temp6 = 0;

	for (i = 0; i < n; i++) //xi
		temp1 += dataset[0][i];

	for (i = 0; i < n; i++) //yi
		temp2 += dataset[1][i];

	for (i = 0; i < n; i++) //xi^2
		temp3 += pow(dataset[0][i],2);

	for (i = 0; i < n; i++) //xiyi
		temp4 += (dataset[0][i] * dataset[1][i]);

	temp5 = temp1 / n;
	temp6 = temp2 / n;

	a1 = ((n * temp4) - (temp1*temp2)) / ( (n*temp3) - pow(temp1,2) );
	a0 = temp6 - (a1*temp5);


	printf("Linear regression : y = %f + %fx\n", a0, a1);
}

void quadratic_regression()
{
	float a0 = 0, a1 = 0, a2 = 0;
	int i, j;
	float temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0;
	float temp5 = 0, temp6 = 0, temp7 = 0;
	float A[5][5] = { 0 };
	float x[10] = { 0 };
	float m = 0;
	int k;
	int order = 3;
	float sum = 0;

	for (i = 0; i < n; i++)
		temp1 += dataset[0][i];
	for (i = 0; i < n; i++)
		temp2 += pow(dataset[0][i], 2);
	for (i = 0; i < n; i++)
		temp3 += pow(dataset[0][i], 3);
	for (i = 0; i < n; i++)
		temp4 += pow(dataset[0][i], 4);

	for (i = 0; i < n; i++)
		temp5 += (pow(dataset[0][i], 2)*dataset[1][i]);
	for (i = 0; i < n; i++)
		temp6 += (dataset[0][i]*dataset[1][i]);
	for (i = 0; i < n; i++)
		temp7 += dataset[1][i];

	A[1][1] = temp4;
	A[1][2] = temp3;
	A[1][3] = temp2;
	A[2][1] = temp3;
	A[2][2] = temp2;
	A[2][3] = temp1;
	A[3][1] = temp2;
	A[3][2] = temp1;
	A[3][3] = n;

	A[1][4] = temp5;
	A[2][4] = temp6;
	A[3][4] = temp7;

	for (j = 1; j <= order; j++) 
	{
		for (i = 1; i <= order; i++)
		{
			if (i>j)
			{
				m = (A[i][j] / A[j][j]);
				for (k = 1; k <= order + 1; k++)
				{
					A[i][k] = (A[i][k] - (m*A[j][k]));
				}
			}
		}
	}

	x[order] = (A[order][order + 1] / A[order][order]);

	for (i = order - 1; i >= 1; i--)
	{
		sum = 0;
		for (j = i + 1; j <= order; j++)
		{
			sum = sum + (A[i][j] * x[j]);
		}
		x[i] = (A[i][order + 1] - sum) / A[i][i];
	}
	//printf("\nThe solution is: \n");

	//for (i = 1; i <= order; i++)
	//{
	//	printf(" x%d = %f\n", i, x[i]); /* x1, x2, x3 are the required solutions*/
	//}

  	printf("Quadratic regression : y = %f + %fx + %fx^2\n", x[3], x[2], x[1]);
}
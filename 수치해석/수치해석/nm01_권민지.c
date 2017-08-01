//컴퓨터학부 2015117662 권민지
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <math.h>

#define MAX_TERMS 100
#pragma warning(disable : 4996)


typedef struct{
	float coef;
	int expon;
}term;
//다항식을 저장할 구조체
term terms[MAX_TERMS];

float f(int start, int finish, float x);
void bisection(float *x, float xl, float xu, int *itr);
void newton(int start, int finish, float x0);

int main()
{
	int i, numA;
	int startP, finishP;
	float xl, xu;
	float x0;
	float temp;
	int itr = 0;
	float x, x1;

	//파일을 열고 읽어서 다항식을 만든다
	FILE *fin = fopen("in.txt", "r");

	fscanf(fin, "%f %f %f", &xl, &xu, &x0);
	fscanf(fin, "%d", &numA);

	startP = 0;
	finishP = numA - 1;

	for (i = startP; i <= finishP; i++)
		fscanf(fin, "%f %d", &terms[i].coef, &terms[i].expon);

	//다항식이 잘 저장되었는지 확인하기 위해 출력해봄
	//printf("P(x) = ");
	/*for (i = startP; i <= finishP; i++)
	{
	if (i == finishP && terms[i].expon == 0)
	printf("%.0f\n", terms[i].coef);
	else if (i == finishP && terms[i].expon != 0)
	printf("%.0fx^%d", terms[i].coef, terms[i].expon);
	else
	printf("%.0fx^%d + ", terms[i].coef, terms[i].expon);
	}
	*/

	//f(xl)*f(xu) > 0이면 xl과 xu사이에 근이 존재 하지 않으므로 실패메세지를 띄운다
	if (f(startP, finishP, xl) * f(startP, finishP, xu) > 0)
		printf("Bisection : Failed\n");

	else {
		//첫번째 bisection 실시
		bisection(&x, xl, xu, &itr);

		do
		{   //첫번째 실시된 bisection에 의해 나온 x값과 xl사이에 근이 있으면 xu = x
			if (f(startP, finishP, xl)*f(startP, finishP, x) < 0)
				xu = x;
			//f(x)=0 즉 중근일때
			else if (f(startP, finishP, xl)*f(startP, finishP, x) == 0) {
				xu = x;
				xl = x;
			}
			// x와 xu사이에 근이 있을 때
			else
				xl = x;

			bisection(&x1, xl, xu, &itr);
			//approximation error가  threshold이하 일 때 까지 bisection반복
			if (fabs(x1 - x) / x1 <= 0.00001)
			{
				printf("Bisection : %f ( %d iterations )\n", x1, itr);
				break;
			}
			x = x1;
		}

		while (itr < 10000000);

	}

	newton(startP, finishP, x0);

	return 0;
}

//함수 값을 계산하는 함수
float f(int start, int finish, float x)
{
	int i;
	float sum = 0;

	for (i = start; i <= finish; i++)
	{
		if (terms[i].expon == 0)
			sum = sum + terms[i].coef;

		else
			sum = sum + (terms[i].coef * (float)pow((double)x, (double)terms[i].expon));
	}

	return sum;
}

//bisetion 실시하는 함수
void bisection(float *x, float xl, float xu, int *itr) {

	*x = (xl + xu) / 2; //중간값을 찾고 실시할때마다 itr을 증가시킨다
	++(*itr);

	if (xl == xu)
		--(*itr);

}

//newton-raphson을 실시하는 함수
void newton(int start, int finish, float x0) {

	int cnt = 0;
	float fx1 = 0, fdx1 = 0; //fx1은 함수값 저장하는 곳, f'(x)의 함수값 저장
	int i;
	float temp = 0, temp2 = 0;

	do
	{
		cnt++;
		fx1 = fdx1 = 0;

		fx1 = f(start, finish, x0);

		//f'(x)의 함수값 계산
		for (i = start; i <= finish; i++)
		{
			fdx1 += terms[i].coef * (terms[i].expon*(float)pow((double)x0, (double)terms[i].expon - 1));
		}
		temp = temp2;
		temp2 = (x0 - (fx1 / fdx1));

		x0 = temp2;
		//approximation error가 threshold 이하 일때까지 반복
	} while ((fabs(temp - x0) / temp) >= 0.00001);

	printf("Newton-Raphson : %f ( %d iterations )\n", temp, cnt);

}


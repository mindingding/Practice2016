#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyScanner.h"
#define BUFFER 512

#pragma warning(disable : 4996)
typedef int Boolean;

int findTypeOfSymbol(char);
int checkTokenType(char token[BUFFER]);
Boolean checkReservedWord(char token[BUFFER]);
void getToken(int *i);

char line[BUFFER];
char token[BUFFER][BUFFER] = { 0, };
int type[BUFFER] = { 0, };
int line_cnt, buffer_length, cnt;

int main(int argc, char *argv[])
{
	FILE *sourceFile, *resultFile;
	int i, j, k = 0;//,cnt=0; // state:0,1~9 (1 start, 9 end, error:-1)

	//parameter üũ
	if (argc != 3){
		fprintf(stderr, "����� c�����̸�.c �����������̸�.txt �������� �Է��ؾߵ˴ϴ�.");
		return -1;
	}
	//���� ��Ʈ�� ����	
	if (!(sourceFile = fopen(argv[1], "r"))){
		fprintf(stderr, "source file open fail!");
		return -1;
	}
	if (!(resultFile = fopen(argv[2], "w"))){
		fprintf(stderr, "result file open fail");
		return -1;
	}
	//���پ� �о��
	//while( (buffer_length = fread(line, sizeof(char), BUFFER,fp)) != 0){
	while (fgets(line, BUFFER, sourceFile)){
		line_cnt++; //���� ���������� line_count ++
		i = 0;
		cnt = 0; //token������ 0���� ����

		printf("%d : %s", line_cnt, line);
		fprintf(resultFile, "%d : %s", line_cnt, line);
		buffer_length = strlen(line);
		line[buffer_length] = 0;

		//���� ����(line)���� �ѱ��ھ�(i++)�̾Ƴ��� ���̸� ���� ������ ���� token�� �̾Ƴ���.
		//token�� ������ cnt�� ����ȴ�.
		while (buffer_length > i)
			getToken(&i);

		//���ٿ��� �̾��� ��ū�� ���
		for (i = 0; i<cnt; i++)
		{
			if (token[i][0] != 0){
				printf("          %d:%-13s:\t%s\n", line_cnt, tokenType[type[i]], token[i]);
				fprintf(resultFile, "          %d:%-13s:\t%s\n", line_cnt, tokenType[type[i]], token[i]);
			}
		}
	}
	return 0;
}

int findTypeOfSymbol(char ch)
{
	if (ch == '+' || ch == '-' || ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}')
		return 0;
	else if (ch >= '0' && ch <= '9')
		return 1;
	else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 2;
	else if (ch == '/')
		return 3;
	else if (ch == '*')
		return 4;
	else if (ch == '<' || ch == '>')
		return 5;
	else if (ch == '=')
		return 6;
	else if (ch == '!')
		return 7;
	else if (ch == ' ' || ch == '\t' || ch == '\n')
		return 9;
	else
		return 8;
}

int checkTokenType(char token[BUFFER])
{
	int midState; //�Ķ���ͷ� ���� ��ū�� startState�κ��� finishState��������� �߰��� ��ģ �ϳ��� state�� Ȯ��
	midState = Transition[1][findTypeOfSymbol(token[0])];
	if (midState == 6) //����
		return 2;
	else if (midState == 5 && checkReservedWord(token)) //reserved
		return 0;
	else if (midState == 5) //id
		return 1;
	else if (midState == 11 || (midState == 5 && strlen(token)<2))
		return 3;
	else
		return 4; //�ϰ͵��ƴ�
}

Boolean checkReservedWord(char token[BUFFER])
{
	int i = 0;
	for (i = 0; i<RESERWEDWORD_NUM; i++){
		if (strcmp(reserved[i], token) == 0)
			return TRUE;
	}
	return FALSE;
}
void getToken(int *i){
	int j = 0, typeOfSymbol;
	static int state = 1, newState = 10, state_temp;
	static char temp[BUFFER] = { 0, };

	if (newState == 10 || newState == 11)
		state = 1;
	else //�������³� �߰�����
		state = state_temp;

	while ((!accept[state] && state != 11) && buffer_length >= *i){
		if (line[*i] == 0) //�ּ��� �������忡 ���ļ� �ִ°�츦 ����.
			state_temp = newState;
		typeOfSymbol = findTypeOfSymbol(line[*i]);
		newState = Transition[state][typeOfSymbol];
		if (advance[state][typeOfSymbol])
		{
			temp[j++] = line[*i];
			*i = *i + 1;
			//�ּ����� start�� ���ƿ��� ��� Ȥ�� start���� ������ input�� ���
			if (newState == 1){
				j = 0; //�����ؿ��� temp���빰�� ��������.	
			}
		}
		state = newState;
	}
	temp[j] = 0;
	if (accept[state] || state == 11){ //11�� error state				
		strcpy(&token[cnt][0], temp);
		//token�� �����԰� ���ÿ� �� ��ū���� type�� �ϴ��ϸ�Ī���� ������.
		if (state == 11)
			type[cnt] = 3; //ERROR
		else
			type[cnt] = checkTokenType(token[cnt]);
		cnt++;
	}
}

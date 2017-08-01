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

	//parameter 체크
	if (argc != 3){
		fprintf(stderr, "실행명 c파일이름.c 저장할파이이름.txt 형식으로 입력해야됩니다.");
		return -1;
	}
	//파일 스트림 형성	
	if (!(sourceFile = fopen(argv[1], "r"))){
		fprintf(stderr, "source file open fail!");
		return -1;
	}
	if (!(resultFile = fopen(argv[2], "w"))){
		fprintf(stderr, "result file open fail");
		return -1;
	}
	//한줄씩 읽어옴
	//while( (buffer_length = fread(line, sizeof(char), BUFFER,fp)) != 0){
	while (fgets(line, BUFFER, sourceFile)){
		line_cnt++; //한줄 읽을때마다 line_count ++
		i = 0;
		cnt = 0; //token개수를 0으로 세팅

		printf("%d : %s", line_cnt, line);
		fprintf(resultFile, "%d : %s", line_cnt, line);
		buffer_length = strlen(line);
		line[buffer_length] = 0;

		//읽은 한줄(line)에서 한글자씩(i++)뽑아내서 길이를 넘지 않을때 까지 token을 뽑아낸다.
		//token의 개수는 cnt에 저장된다.
		while (buffer_length > i)
			getToken(&i);

		//한줄에서 뽑아진 토큰들 출력
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
	int midState; //파라미터로 들어온 토큰이 startState로부터 finishState나오기까지 중간에 거친 하나의 state를 확인
	midState = Transition[1][findTypeOfSymbol(token[0])];
	if (midState == 6) //숫자
		return 2;
	else if (midState == 5 && checkReservedWord(token)) //reserved
		return 0;
	else if (midState == 5) //id
		return 1;
	else if (midState == 11 || (midState == 5 && strlen(token)<2))
		return 3;
	else
		return 4; //암것도아님
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
	else //에러상태나 중간상태
		state = state_temp;

	while ((!accept[state] && state != 11) && buffer_length >= *i){
		if (line[*i] == 0) //주석이 여러문장에 걸쳐서 있는경우를 위해.
			state_temp = newState;
		typeOfSymbol = findTypeOfSymbol(line[*i]);
		newState = Transition[state][typeOfSymbol];
		if (advance[state][typeOfSymbol])
		{
			temp[j++] = line[*i];
			*i = *i + 1;
			//주석에서 start로 돌아오는 경우 혹은 start에서 공백이 input인 경우
			if (newState == 1){
				j = 0; //저장해오던 temp내용물을 날려버림.	
			}
		}
		state = newState;
	}
	temp[j] = 0;
	if (accept[state] || state == 11){ //11은 error state				
		strcpy(&token[cnt][0], temp);
		//token을 저장함과 동시에 각 토큰마다 type을 일대일매칭으로 정해줌.
		if (state == 11)
			type[cnt] = 3; //ERROR
		else
			type[cnt] = checkTokenType(token[cnt]);
		cnt++;
	}
}

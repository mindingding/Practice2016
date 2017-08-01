#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define NO_KEYWORDS 6
#define ID_LENGTH 12
#pragma warning(disable : 4996)

enum tsymbol{
	tnull = -1, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19,
	s20, s21, snumber, sident, selse, sif, sint, sreturn, svoid, swhile
};
// 20개의 특수 심볼, 숫자, 문자열, 6개의 키워드 순

struct tokenType {
	int number;    // 입력된 숫자를 저장
	char sym[2];    // 특수 심볼 지정
	char id[ID_LENGTH];
	int num;       // 키워드 지정
} a;

char *keyword[NO_KEYWORDS] = {
	"else", "if", "int", "return", "void", "while" };

enum tsymbol tnum[NO_KEYWORDS] = {
	selse, sif, sint, sreturn, svoid, swhile };
// 키워드의 순서와 순서에 맞는 문자열 선택을 위한 코드

int superLetter(char ch) {
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}
// 대문자(1), 소문자(2)일 경우 1을 리턴

int superLetterOrDigit(char ch){
	if (isalnum(ch) || ch == '_') return 1;
	else return 0;
}
// 대문자(1), 소문자(2), 숫자(4)일 경우 1을 리턴

int getIntNum(char firstCharacter, FILE *fin) {
	int num = 0;
	char ch;

	if (firstCharacter != '0'){
		ch = firstCharacter;
		do{
			num = 10 * num + (int)(ch - '0');
			fscanf(fin, "%c", &ch);
		} while (isdigit(ch));
	} // 숫자 여부 확인 후 반환

	else { num = 0; }
	ungetc(ch, fin);
	return num;
}

void scanner() {

	struct tokenType token;
	struct tokenType temp;
	int i = 0, index = 0;
	int n = 0;
	int flag = 1;
	char ch, id[ID_LENGTH], line[500];
	FILE *fin;
	FILE *fout;
	token.number = tnull;

	fin = fopen("input.txt", "r"); 
	fout = fopen("output.txt", "w");


	do {

		fgets(line, sizeof(line), fin);

		printf("%3d : %s\n", flag, line);

		while (1) {
			
			if (line[n] == '\n')
				break;
			if ((isspace(line[n]))){
				n++;
				flag--;
			}

			// 입력 받는 값이 공백이 아니면 while 탈출

			else if (superLetter(line[n])){ // 대문자, 소문자일 경우
				i = 0;
				do{
					if (i < ID_LENGTH) {
						id[i++] = line[n];
					}
					n++;

				} while (superLetterOrDigit(line[n]));
				// 특수문자, 숫자 여부 판별

				id[i] = '\0';
				ungetc(line[n], fin); // 읽혀진 문자를 다시 스트림으로 반환
				index = 0;
				while (index < NO_KEYWORDS) {

					if (!strcmp(id, keyword[index])) break; // 키워드 검색
					if (index < NO_KEYWORDS) {      // 키워드의 위치와 문자열 복사
						token.number = tnum[index];
						strcpy(token.id, id);
						printf("\t%d : reserved word : %s\n", flag, id);
					}
					else{
						token.number = sident;
						strcpy(token.id, id);
						printf("\t%d : ID, name : %s\n", flag, id);
					}
					// 찾지 못하면 변수로서 문자열 복사
					index++;
				}
				/*
				if (temp.number <= 21) printf("\t%d : Symbol : %s\n", flag, id);
				else if (temp.number > 22 && temp.number < 29) printf("\t%d : Keyword : %s\n", flag, id);*/
			}

			else if (isdigit(line[n])){ // 숫자일 경우 - 숫자면 true, 아니면 false
				token.number = snumber;
				token.num = getIntNum(line[n], fin);
				printf("\t%d : NUM, val = %d", line[n]);
			}

			else switch (line[n]){
				// 심볼일 경우 enum에서의 위치와 해당 심볼의 문자열을 구조체 변수에 복사
			case '*':
				n++;
				if (line[n] == '/') {
					token.number = s21;
					strcpy(token.sym, "*/");
				}
				else{
					token.number = s3;
					strcpy(token.sym, "* ");
					ungetc(line[n], fin);
				} break;
			case '/':
				n++;
				if (line[n] == '*') {
					strcpy(token.sym, "/*");
					token.number = s20;
				}
				else{
					strcpy(token.sym, "/ ");
					token.number = s4;
					ungetc(line[n], fin);
				} break;
			case '<':
				n++;
				if (line[n] == '=') {
					strcpy(token.sym, "<=");
					token.number = s6;
				}
				else{
					strcpy(token.sym, "< ");
					token.number = s5;
					ungetc(line[n], fin);
				} break;
			case '>':
				n++;
				if (line[n] == '=') {
					strcpy(token.sym, ">=");
					token.number = s8;
				}
				else{
					strcpy(token.sym, "> ");
					token.number = s7;
					ungetc(line[n], fin);
				} break;
			case '=':
				n++;
				if (line[n] == '=') {
					strcpy(token.sym, "==");
					token.number = s9;
				}
				else{
					strcpy(token.sym, "= ");
					token.number = s11;
					ungetc(line[n], fin);
				} break;
			case '!':
				n++;
				if (line[n] == '=') {
					strcpy(token.sym, "!=");
					token.number = s12;
				}
				else printf("Error\r\n"); break;

			case '+': token.number = s1;    strcpy(token.sym, "+ ");    break;
			case '-': token.number = s2;    strcpy(token.sym, "- ");    break;
			case ';': token.number = s12;    strcpy(token.sym, "; ");    break;
			case ',': token.number = s13;    strcpy(token.sym, ", ");    break;
			case '(': token.number = s14;    strcpy(token.sym, "( ");    break;
			case ')': token.number = s15;    strcpy(token.sym, ") ");    break;
			case '[': token.number = s16;    strcpy(token.sym, "[ ");    break;
			case ']': token.number = s17;    strcpy(token.sym, "] ");    break;
			case '{': token.number = s18;    strcpy(token.sym, "{ ");    break;
			case '}': token.number = s19;    strcpy(token.sym, "} ");    break;
			case EOF: token.number = s21;    strcpy(token.sym, "!=");    break;

			default: {
				printf("Current character : %c", line[n]);
				printf("Error\r\n");
				break;
			}
					 printf("\t%d : %s\n", flag, token.sym);
			}

			temp = token;
			/*
			if (temp.number < 21) printf("\t%d : Symbol : %s\n", flag, temp.sym);
			else if (temp.number > 22 && temp.number < 29) printf("\t%d : Keyword : %s\n",flag, temp.id);
			else if (temp.number == 21) printf("\t%d : Num , %d \n", flag, temp.num);
			else if (temp.number == 22) printf("\t%d : ID , %s\n",flag, temp.id);
			else {}*/

			flag++; 
			n++;
		}

	} while (!feof(fin));
		
}

	void main() {
		
		scanner();

		printf("complete\n");
	}


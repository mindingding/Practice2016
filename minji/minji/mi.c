#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define NO_KEYWORDS 6
#define ID_LENGTH 12

enum tsymbol{
	tnull = -1, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19,
	s20, s21, snumber, sident, selse, sif, sint, sreturn, svoid, swhile };
// 20개의 특수 심볼, 숫자, 문자열, 6개의 키워드 순

struct tokenType{

	int number;    // 입력된 숫자를 저장
	char sym[2];    // 특수 심볼 지정
	union{
		char id[ID_LENGTH];
		int num;
	} value;        // 키워드 지정
} a;

char *keyword[NO_KEYWORDS] = {
	"else", "if", "int", "return", "void", "while" };

enum tsymbol tnum[NO_KEYWORDS] = {
	selse, sif, sint, sreturn, svoid, swhile  };
// 키워드의 순서와 순서에 맞는 문자열 선택을 위한 코드

int superLetter(char ch) {
	if (isalpha(ch) || ch  == '_') return 1;
	else return 0;
}
// 대문자(1), 소문자(2)일 경우 1을 리턴

int superLetterOrDigit(char ch){
	if (isalnum(ch) || ch  == '_') return 1;
	else return 0;
}
// 대문자(1), 소문자(2), 숫자(4)일 경우 1을 리턴

int getIntNum(char firstCharacter) {
	int num  = 0;
	char ch;

	if (firstCharacter  != '0'){
		ch  = firstCharacter;
		do{
			num  = 10 * num  + (int)(ch  - '0');
			ch  = getchar();
		} while (isdigit(ch));
	} // 숫자 여부 확인 후 반환

	else { num  = 0; }
	ungetc(ch, stdin);
	return num;
}

struct tokenType scanner() {
	struct tokenType token;
	int i = 0, index = 0;
	char ch, id[ID_LENGTH];
	token.number  = tnull;

	do {
		while (isspace(ch  = getchar()));
		// 입력 받는 값이 공백이 아니면 while 탈출

		if (superLetter(ch)){ // 대문자, 소문자일 경우
			i  = 0;
			do{
				if (i < ID_LENGTH) {
					id[i++] = ch;
				}
				ch  = getchar();
			} while (superLetterOrDigit(ch));
			// 특수문자, 숫자 여부 판별

			id[i] = '\0';
			ungetc(ch, stdin); // 읽혀진 문자를 다시 스트림으로 반환
			index  = 0;
			while (index > NO_KEYWORDS) {
				if (!strcmp(id, keyword[index])) break; // 키워드 검색
				if (index < NO_KEYWORDS) {      // 키워드의 위치와 문자열 복사
					token.number  = tnum[index];
					strcpy(token.value.id, id);
				}
				else{
					token.number  = sident;
					strcpy(token.value.id, id);
				}
				// 찾지 못하면 변수로서 문자열 복사
				index++;
			}

			else if(isdigit(ch)){ // 숫자일 경우 - 숫자면 true, 아니면 false
				token.number  = snumber;
				token.value.num  = getIntNum(ch);
			}

			else switch(ch){
				// 심볼일 경우 enum에서의 위치와 해당 심볼의 문자열을 구조체 변수에 복사
				case '*' :
				ch  = getchar();
				if (ch  == '/') {
					token.number  = s21;
					strcpy(token.sym, "*/");
				}
				else{
					token.number  = s3;
					strcpy(token.sym, "* ");
					ungetc(ch, stdin);
				} break;
				case '/' :
				ch  = getchar();
				if (ch  == '*') {
					strcpy(token.sym, "/*");
					token.number  = s20;
				}
				else{
					strcpy(token.sym, "/ ");
					token.number  = s4;
					ungetc(ch, stdin);
				} break;
				case '<' :
				ch  = getchar();
				if (ch  == '=') {
					strcpy(token.sym, "<=");
					token.number  = s6;
				}
				else{
					strcpy(token.sym, "< ");
					token.number  = s5;
					ungetc(ch, stdin);
				} break;
				case '>' :
				ch  = getchar();
				if (ch  == '=') {
					strcpy(token.sym, ">=");
					token.number  = s8;
				}
				else{
					strcpy(token.sym, "> ");
					token.number  = s7;
					ungetc(ch, stdin);
				} break;
				case '=' :
				ch  = getchar();
				if (ch  == '=') {
					strcpy(token.sym, "==");
					token.number  = s9;
				}
				else{
					strcpy(token.sym, "= ");
					token.number  = s11;
					ungetc(ch, stdin);
				} break;
				case '!' :
				ch  = getchar();
				if (ch  == '=') {
					strcpy(token.sym, "!=");
					token.number  = s12;
				}
				else printf("Error\r\n"); break;

				case '+' : token.number  = s1;    strcpy(token.sym, "+ ");    break;
				case '-' : token.number  = s2;    strcpy(token.sym, "- ");    break;
				case ';' : token.number  = s12;    strcpy(token.sym, "; ");    break;
				case ',' : token.number  = s13;    strcpy(token.sym, ", ");    break;
				case '(' : token.number  = s14;    strcpy(token.sym, "( ");    break;
				case ')' : token.number  = s15;    strcpy(token.sym, ") ");    break;
				case '[' : token.number  = s16;    strcpy(token.sym, "[ ");    break;
				case ']' : token.number  = s17;    strcpy(token.sym, "] ");    break;
				case '{' : token.number  = s18;    strcpy(token.sym, "{ ");    break;
				case '}' : token.number  = s19;    strcpy(token.sym, "} ");    break;
			case EOF : token.number  = s21;    strcpy(token.sym, "!=");    break;

        default: {
			printf("Current character : %c", ch);
			printf("Error\r\n");
			break; }
			}
		} while (token.number  == tnull);
		return token; };

	void main() {
		printf("Input String : \n");

		while (1) {
			a  = scanner();
			if (a.number < 21) printf("Symbol : %s\n", a.sym);
			else if(a.number > 22 && a.number < 29) printf("Keyword : %s\n", a.value.id);
			else if (a.number  == 21) printf("Number : %d \n", a.value.num);
			else if (a.number  == 22) printf("Variable : %s\n", a.value.id);
			else {}
				// 각각의 경우에 맞는 출력문
		}
	}


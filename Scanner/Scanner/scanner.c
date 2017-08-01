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
// 20���� Ư�� �ɺ�, ����, ���ڿ�, 6���� Ű���� ��

struct tokenType {
	int number;    // �Էµ� ���ڸ� ����
	char sym[2];    // Ư�� �ɺ� ����
	char id[ID_LENGTH];
	int num;       // Ű���� ����
} a;

char *keyword[NO_KEYWORDS] = {
	"else", "if", "int", "return", "void", "while" };

enum tsymbol tnum[NO_KEYWORDS] = {
	selse, sif, sint, sreturn, svoid, swhile };
// Ű������ ������ ������ �´� ���ڿ� ������ ���� �ڵ�

int superLetter(char ch) {
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}
// �빮��(1), �ҹ���(2)�� ��� 1�� ����

int superLetterOrDigit(char ch){
	if (isalnum(ch) || ch == '_') return 1;
	else return 0;
}
// �빮��(1), �ҹ���(2), ����(4)�� ��� 1�� ����

int getIntNum(char firstCharacter, FILE *fin) {
	int num = 0;
	char ch;

	if (firstCharacter != '0'){
		ch = firstCharacter;
		do{
			num = 10 * num + (int)(ch - '0');
			fscanf(fin, "%c", &ch);
		} while (isdigit(ch));
	} // ���� ���� Ȯ�� �� ��ȯ

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

			// �Է� �޴� ���� ������ �ƴϸ� while Ż��

			else if (superLetter(line[n])){ // �빮��, �ҹ����� ���
				i = 0;
				do{
					if (i < ID_LENGTH) {
						id[i++] = line[n];
					}
					n++;

				} while (superLetterOrDigit(line[n]));
				// Ư������, ���� ���� �Ǻ�

				id[i] = '\0';
				ungetc(line[n], fin); // ������ ���ڸ� �ٽ� ��Ʈ������ ��ȯ
				index = 0;
				while (index < NO_KEYWORDS) {

					if (!strcmp(id, keyword[index])) break; // Ű���� �˻�
					if (index < NO_KEYWORDS) {      // Ű������ ��ġ�� ���ڿ� ����
						token.number = tnum[index];
						strcpy(token.id, id);
						printf("\t%d : reserved word : %s\n", flag, id);
					}
					else{
						token.number = sident;
						strcpy(token.id, id);
						printf("\t%d : ID, name : %s\n", flag, id);
					}
					// ã�� ���ϸ� �����μ� ���ڿ� ����
					index++;
				}
				/*
				if (temp.number <= 21) printf("\t%d : Symbol : %s\n", flag, id);
				else if (temp.number > 22 && temp.number < 29) printf("\t%d : Keyword : %s\n", flag, id);*/
			}

			else if (isdigit(line[n])){ // ������ ��� - ���ڸ� true, �ƴϸ� false
				token.number = snumber;
				token.num = getIntNum(line[n], fin);
				printf("\t%d : NUM, val = %d", line[n]);
			}

			else switch (line[n]){
				// �ɺ��� ��� enum������ ��ġ�� �ش� �ɺ��� ���ڿ��� ����ü ������ ����
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESERWEDWORD_NUM 6
#define TRUE 1
#define FALSE 0

#define START 1
#define COMP 2
#define EQUAL 3
#define NOT 4
#define ID 5
#define NUM 6
#define COMMENT1 7
#define COMMENT2 8
#define COMMENT3 9
#define DONE 10
#define ERROR 11

int accept[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 };

int advance[12][10] =
{
	//1,2,3,4,5,6,7,8,9
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //nothing
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //start
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, //comp1
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //comp2
	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, //different_comp
	{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, //Identifier
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, //Number
	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, //Comment1
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //Comment2
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //Comment3
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //finish
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  //error
};

int Transition[12][10] =
{
	//1,2,3,4,5,6,7,8,9,110
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //noting
	{ DONE, NUM, ID, COMMENT1, DONE, COMP, COMP, NOT, ERROR, START }, //start
	{ DONE, DONE, DONE, DONE, DONE, DONE, EQUAL, DONE, DONE, DONE }, //comp
	{ DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE }, //equal
	{ ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, DONE, ERROR, ERROR, ERROR }, //diffent_comp
	{ DONE, DONE, ID, DONE, DONE, DONE, DONE, DONE, DONE, DONE }, //Identifier
	{ DONE, NUM, DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE }, //Number
	{ DONE, DONE, DONE, DONE, COMMENT3, DONE, DONE, DONE, DONE, DONE }, //Comment1
	{ COMMENT2, COMMENT2, COMMENT2, COMMENT2, COMMENT3, COMMENT2, COMMENT2, COMMENT2, COMMENT2, COMMENT2 }, //Comment2
	{ COMMENT2, COMMENT2, COMMENT2, START, COMMENT3, COMMENT2, COMMENT2, COMMENT2, COMMENT2, COMMENT2 }, //CommentEQUAL
	{ DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE, DONE }, //finish
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  //error
};

char* reserved[6] = { "else", "if", "int", "return", "void", "while" };
char* tokenType[5] = { "reserved word", "ID,name", "NUM, val", "ERROR", "" };
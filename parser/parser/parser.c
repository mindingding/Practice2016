//2015117662 ±Ç¹ÎÁö
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFLEN 256						
#define MAXCHILD 3	
#define MAXTOKEN 40

#define FALSE 0
#define TRUE 1
			

typedef enum { StmtK, ExpK, DecK } NodeKind;
typedef enum { IfK, WhileK, ReturnK, CallK, CompoundK } StmtKind;
typedef enum { OpK, IdK, ConstK, AssignK } ExpKind;
typedef enum { ScalarK, ArrK, FuncK } DecKind;

typedef enum { Void, Integer } ExpType;		

typedef enum
{
	ENDFILE, ERROR,
	ELSE, IF, INT, RETURN, VOID, WHILE,
	ID, NUM,
	PLUS, MINUS, TIMES, OVER, LT, LTEQ, BG, BGEQ, EQ, NEQ, ASSIGN, LPAREN, RPAREN, SEMICOL, COL, LBRACE, RBRACE, LBRACKET, RBRACKET
} TokenType;

typedef enum
{
	START, INCOMPAREL, INCOMPAREB, INCOMPAREE, INNEQ, INCOMMENTA, INCOMMENTB, INCOMMENTC, INID, INNUM, DONE
}
StateType;

typedef struct treeNode {

	struct treeNode *child[MAXCHILD];
	struct treeNode *sibling;
	int linenumber;
	NodeKind nodekind;
	union {
		StmtKind stmt;
		ExpKind exp;
		DecKind dec;
	} kind;

	TokenType op;
	int val;
	char *name;

	ExpType funcType;
	ExpType varType;
	ExpType expType;
} TreeNode;			

static struct
{
	char * str;
	TokenType tok;
}reservedWords[6] =
{
	{ "else", ELSE }, { "if", IF }, { "int", INT }, { "return", RETURN }, { "void", VOID }, { "while", WHILE }
};

static char lineBuf[BUFLEN];			
static int linepos = 0;				
static int bufsize = 0;					
static int indentno = 0;

#define INDENT indentno+=4
#define UNINDENT indentno-=4

int linenumber = 0;						
int Error = FALSE;
char tokenString[MAXTOKEN+ 1];
int count = 1;

FILE * source;						
FILE * output;

static char getNextChar(void); 
static void ungetNextChar(void); 
static TokenType reservedLookup(char *s);
void printToken(TokenType token, const char* tokenString); 
TreeNode *newStmtNode(StmtKind kind); 
TreeNode *newExpNode(ExpKind kind); 
TreeNode *newDecNode(DecKind kind); 
char * copyString(char * s);
static void printSpace(void);
char *typeName(ExpType type);
void printTree(TreeNode * tree);
TokenType getToken(void);

static TokenType token;
static ExpType type_specifier(void);
static TreeNode *declaration_list(void);
static TreeNode *declaration(void);
static TreeNode *var_declaration(void);
static TreeNode *fun_declaration(ExpType type, char * id);
static TreeNode *params(void);
static TreeNode *param_list(void);
static TreeNode *param(void);
static TreeNode *compound_stmt(void);
static TreeNode *local_declarations(void);
static TreeNode *statement_list(void);
static TreeNode *statement(void);
static TreeNode *expression_stmt(void);
static TreeNode *selection_stmt(void);
static TreeNode *iteration_stmt(void);
static TreeNode *return_stmt(void);
static TreeNode *expression(void);
static TreeNode *simple_expression(TreeNode *next);
static TreeNode *additive_expression(TreeNode *next);
static TreeNode *term(TreeNode *next);
static TreeNode *factor(TreeNode *next);
static TreeNode *call(void);
static TreeNode *args(void);
static TreeNode *arg_list(void);

TreeNode *parse(void);
static void syntaxError(char *msg);
static void match(TokenType expected);



static char getNextChar(void)
{
	//printf(" %d : getNextChar \n", count++);
	if (!(linepos<bufsize))
	{
		linenumber++;
		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else return EOF;
	}
	else return lineBuf[linepos++];

	
}

static TokenType reservedLookup(char *s)
{
	int i;
	printf(" %d : reservedLookup \n", count++);
	for (i = 0; i<6; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

void printToken(TokenType token, const char* tokenString)
{
	//printf(" %d : printToken \n", count++);
	switch (token)
	{
	case ELSE:
	case IF:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		break;
	case LT: 
		fprintf(output, "<\n");
		break;
	case LTEQ: 
		fprintf(output, "<=\n"); 
		break;
	case BG: 
		fprintf(output, ">\n");
		break;
	case BGEQ:
		fprintf(output, ">=\n"); 
		break;
	case ASSIGN: 
		fprintf(output, "=\n"); 
		break;
	case EQ: 
		fprintf(output, "==\n"); 
		break;
	case NEQ: 
		fprintf(output, "!=\n");
		break;
	case PLUS:
		fprintf(output, "+\n"); 
		break;
	case MINUS: 
		fprintf(output, "-\n"); 
		break;
	case TIMES:
		fprintf(output, "*\n"); 
		break;
	case OVER: 
		fprintf(output, "/\n"); 
		break;
	case LPAREN: 
		fprintf(output, "(\n");
		break;
	case RPAREN:
		fprintf(output, ")\n"); 
		break;
	case SEMICOL: 
		fprintf(output, ";\n"); 
		break;
	case COL: 
		fprintf(output, ",\n"); 
		break;
	case LBRACE: 
		fprintf(output, "{\n"); 
		break;
	case RBRACE: 
		fprintf(output, "}\n"); 
		break;
	case LBRACKET: 
		fprintf(output, "[\n"); 
		break;
	case RBRACKET: 
		fprintf(output, "]\n");
		break;
	case ENDFILE: 
		fprintf(output, "EOF\n");
		break;
	case NUM:
		fprintf(output, "NUM = %s\n", tokenString);
		break;
	case ID:
		fprintf(output, "ID = %s\n", tokenString);
		break;
	case ERROR:
		fprintf(output, "ERROR: %s\n", tokenString);
		break;
	}
}


static void ungetNextChar(void)
{
	//printf(" %d : ungetNextCharn \n", count++);
	linepos--;
}

TreeNode *newStmtNode(StmtKind kind)
{
	TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));

	int i;

	printf(" %d : newStmtNode \n", count++);
	for (i = 0; i < MAXCHILD; ++i) t->child[i] = NULL;
	t->sibling = NULL;
	t->linenumber = linenumber;
	t->nodekind = StmtK;
	t->kind.stmt = kind;

	return t;
}

TreeNode *newExpNode(ExpKind kind)
{
	TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;

	printf(" %d : newExpNode \n", count++);
	for (i = 0; i < MAXCHILD; ++i) t->child[i] = NULL;
	t->sibling = NULL;
	t->linenumber = linenumber;
	t->nodekind = ExpK;
	t->kind.exp = kind;
	
	return t;
}

TreeNode *newDecNode(DecKind kind)
{
	TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;

	printf(" %d : newDecNode \n", count++);
	for (i = 0; i < MAXCHILD; ++i) t->child[i] = NULL;
	t->sibling = NULL;
	t->linenumber = linenumber;
	t->nodekind = DecK;
	t->kind.dec = kind;
	
	return t;
}

char* copyString(char * s)
{
	int n;
	char * t;

	printf(" %d : copyStrine \n", count++);
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = (char*)malloc(n*sizeof(char));
	
	strcpy(t, s);
	return t;
}

static void printSpace(void)
{
	int i;
	//printf(" %d : printSpace \n", count++);
	for (i = 0; i<indentno; i++)
		fprintf(output, " ");
}

char *typeName(ExpType type)
{
	static char i[] = "int";
	static char v[] = "void";
	static char unknown[] = "unknown type";
	//printf(" %d : typeName \n", count++);
	switch (type)
	{
		case Integer: return i; break;
		case Void:    return v; break;
		default:      return unknown;
	}
}

void printTree(TreeNode * tree)
{
	int i;

	INDENT;
	//printf(" %d : printTree \n", count++);
	while (tree != NULL)
	{
		printSpace();

		if (tree->nodekind == DecK)
		{
			switch (tree->kind.dec)
			{
			case ScalarK:
				fprintf(output, "scalar: %s type: %s\n", tree->name, typeName(tree->varType));
				break;
			case ArrK:
				fprintf(output, "array: %s size: %d type: %s\n", tree->name, tree->val, typeName(tree->varType));
				break;
			case FuncK:
				fprintf(output, "function: %s() return type: %s \n", tree->name, typeName(tree->funcType));
				break;
			default:
				fprintf(output, "unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp)
			{
			case OpK:
				fprintf(output, "Op: ");
				printToken(tree->op, "\0");
				break;
			case IdK:
				fprintf(output, "Id: %s", tree->name);
				fprintf(output, "\n");
				break;
			case ConstK:
				fprintf(output, "const: %d\n", tree->val);
				break;
			case AssignK:
				fprintf(output, "assign: \n");
				break;
			default:
				fprintf(output, "unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt)
			{
			case CompoundK:
				fprintf(output, "Compound\n");
				break;
			case IfK:
				fprintf(output, "If\n");
				break;
			case WhileK:
				fprintf(output, "While\n");
				break;
			case ReturnK:
				fprintf(output, "Return\n");
				break;
			case CallK:
				fprintf(output, "Call %s\n", tree->name);
				break;
			default:
				fprintf(output, "unknown ExpNode kind\n");
				break;
			}
		}
		else
			fprintf(output, "Unknown node kind>\n");
		for (i = 0; i<MAXCHILD; ++i)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}

TokenType getToken(void)
{

	int tokenStringIndex = 0;		
	TokenType currentToken;		
	StateType state = START;		

	int save;
	//printf(" %d : getToken \n", count++);
	while (state != DONE)
	{
		char c = getNextChar();
		save = TRUE;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			else if (c == '/')
			{
				save = FALSE;
				state = INCOMMENTA;
			}
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;
			else if (c == '!')
			{
				state = INNEQ;
			}
			else if (c == '<')
			{
				save = FALSE;
				state = INCOMPAREL;
			}
			else if (c == '>')
			{
				save = FALSE;
				state = INCOMPAREB;
			}
			else if (c == '=')
			{
				save = FALSE;
				state = INCOMPAREE;
			}
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case '{':
					currentToken = LBRACE;
					break;
				case '}':
					currentToken = RBRACE;
					break;
				case '[':
					currentToken = LBRACKET;
					break;
				case ']':
					currentToken = RBRACKET;
					break;
				case ';':
					currentToken = SEMICOL;
					break;
				case ',':
					currentToken = COL;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INCOMMENTA:
			if (c == '*') { state = INCOMMENTB; save = FALSE; }
			else
			{
				ungetNextChar();
				save = TRUE;
				state = DONE;
				currentToken = OVER;
			}
			break;
		case INCOMMENTB:
			save = FALSE;
			if (c == '*') state = INCOMMENTC;
			else
				state = INCOMMENTB;
			break;
		case INCOMMENTC:
			save = FALSE;
			if (c == '*') state = INCOMMENTC;
			else if (c == '/')
				state = START;
			else
				state = INCOMMENTB;
			break;
		case INNEQ:
			state = DONE;
			if (c == '=')
				currentToken = NEQ;
			else
			{
				ungetNextChar();
				save = FALSE;
				currentToken = ERROR;
			}
			break;
		case INCOMPAREL:
			state = DONE;
			if (c == '=')
				currentToken = LTEQ;
			else
			{
				ungetNextChar();
				currentToken = LT;
			}
			break;
		case INCOMPAREB:
			state = DONE;
			if (c == '=')
				currentToken = BGEQ;
			else
			{
				ungetNextChar();
				currentToken = BG;
			}
			break;
		case INCOMPAREE:
			state = DONE;
			if (c == '=')
				currentToken = EQ;
			else
			{
				ungetNextChar();
				currentToken = ASSIGN;
			}
			break;
		case INNUM:
			if (!isdigit(c))
			{
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c))
			{
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
		default:
			fprintf(output, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKEN))
			tokenString[tokenStringIndex++] = c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
				currentToken = reservedLookup(tokenString);
		}
	}
	return currentToken;
}

static void syntaxError(char *msg)
{
	printf(" %d : syntaxError \n", count++);
	fprintf(output, "\n>>");
	fprintf(output, "Syntax error at line %d : %s", linenumber, msg);
	Error = TRUE;
}

static void match(TokenType expected)
{
	printf(" %d : match\n", count++);
	if (token == expected)
		token = getToken();
	else
	{
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
	}
}

static ExpType type_specifier(void)
{
	ExpType type;
	printf(" %d : type_specifier \n", count++);
	if (token == INT)
	{
		type = Integer;
		token = getToken();
	}
	else if (token == VOID)
	{
		type = Void;
		token = getToken();
	}
	else
	{
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
		token = getToken();
	}

	return type;
}

TreeNode *declaration_list(void)
{
	TreeNode *t = declaration();
	TreeNode *p = t;
	printf(" %d : declaration_list \n", count++);
	while (token != ENDFILE)
	{
		TreeNode *q;
		q = declaration();

		if ((q != NULL) && (p != NULL))
		{
			p->sibling = q;
			p = q;
		}
	}
	return t;
}

TreeNode *declaration(void)
{
	TreeNode *t = NULL;
	ExpType declare;
	char *idName;
	printf(" %d : declaration \n", count++);
	declare = type_specifier();
	idName = copyString(tokenString);

	match(ID);

	switch (token)
	{
	case SEMICOL:		
		t = newDecNode(ScalarK);
		if (t != NULL)
		{
			t->varType = declare;
			t->name = idName;
		}
		match(SEMICOL);
		break;
	case LBRACKET:		
		t = newDecNode(ArrK);
		if (t != NULL)
		{
			t->varType = declare;
			t->name = idName;
		}
		match(LBRACKET);
		if (t != NULL)
			t->val = atoi(tokenString);
		match(NUM);
		match(RBRACKET);
		match(SEMICOL);
		break;
	case LPAREN: t = fun_declaration(declare, idName); break;
	default:
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
		token = getToken();
		break;
	}
	return t;
}

TreeNode *var_declaration(void)
{
	TreeNode  *t = NULL;
	ExpType declare;
	char *idName;
	printf(" %d : var_declaration \n", count++);
	declare = type_specifier();

	idName = copyString(tokenString);
	match(ID);

	switch (token)
	{
	case SEMICOL:	
		t = newDecNode(ScalarK);
		if (t != NULL)
		{
			t->varType = declare;
			t->name = idName;
		}
		match(SEMICOL);
		break;
	case LBRACKET:		
		t = newDecNode(ArrK);
		if (t != NULL)
		{
			t->varType = declare;
			t->name = idName;
		}
		match(LBRACKET);
		if (t != NULL) t->val = atoi(tokenString);
		match(NUM);
		match(RBRACKET);
		match(SEMICOL);
		break;
	default:
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
		token = getToken();
		break;
	}
	return t;
}

TreeNode *fun_declaration(ExpType type, char * id)
{
	TreeNode  *t = NULL;
	ExpType declare;
	char *idName;

	declare = type;
	idName = id;
	printf(" %d : fun_declarationn \n", count++);
	switch (token)
	{
	case LPAREN:
		t = newDecNode(FuncK);
		if (t != NULL)
		{
			t->funcType = declare;
			t->name = idName;
		}
		match(LPAREN);
		if (t != NULL) t->child[0] = params();
		match(RPAREN);
		if (t != NULL) t->child[1] = compound_stmt();
		break;
	default:
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
		token = getToken();
		break;
	}
	return t;
}

TreeNode *params(void)
{
	TreeNode *t = NULL;
	printf(" %d : params \n", count++);
	if (token == VOID)
	{
		match(VOID);
		return NULL;
	}
	else
		t = param_list();
	return t;
}

TreeNode *param_list(void)
{
	
	TreeNode *tem = param();
	TreeNode *q;
	TreeNode *p = tem;
	printf(" %d : param_lists \n", count++);
	while ((token == COL) && (tem != NULL))
	{
		match(COL);
		q = param();
		if (q != NULL)
		{
			p->sibling = q;
			p = q;
		}
	}
	return tem;
}

TreeNode *param(void)
{
	TreeNode *tem;
	ExpType param;
	char *idName;
	printf(" %d : param \n", count++);
	param = type_specifier();
	idName = copyString(tokenString);
	match(ID);
	
	if (token == LBRACKET)
	{
		match(LBRACKET);
		match(RBRACKET);
		tem = newDecNode(ArrK);
	}
	else
		tem = newDecNode(ScalarK);

	if (tem != NULL)
	{
		tem->name = idName;
		tem->val = 0;
		tem->varType = param;
	}
	return tem;
}

TreeNode *compound_stmt(void)
{
	TreeNode *tem = NULL;

	match(LBRACE);
	printf(" %d : compound_stmt \n", count++);
	if ((token != RBRACE) && (tem = newStmtNode(CompoundK)))
	{
		if ((token == INT) || (token == VOID))
			tem->child[0] = local_declarations();
		if (token != RBRACE)
			tem->child[1] = statement_list();
	}

	match(RBRACE);
	return tem;
}

TreeNode *local_declarations(void)
{
	TreeNode *tem;
	TreeNode *p;
	TreeNode *q;
	printf(" %d : local_declarations \n", count++);
	if ((token == INT) || (token == VOID))
		tem = var_declaration();

	if (tem != NULL)
	{
		p = tem;

		while ((token == INT) || (token == VOID))
		{
			q = var_declaration();
			if (q != NULL)
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return tem;
}

TreeNode *statement_list(void)
{
	TreeNode *tem = statement();
	TreeNode *p = tem;
	TreeNode *q;
	printf(" %d : statement_lists \n", count++);
	while (token != RBRACE)
	{
		q = statement();
		if ((p != NULL) && (q != NULL))
		{
			p->sibling = q;
			p = q;
		}
	}
	return tem;
}

TreeNode *statement(void)
{
	TreeNode *tem = NULL;
	printf(" %d : statement \n", count++);
	switch (token)
	{
	case ID:
	case SEMICOL:
	case LPAREN:
	case NUM: tem = expression_stmt(); break;
	case LBRACE: tem = compound_stmt(); break;
	case IF: tem = selection_stmt(); break;
	case WHILE: tem = iteration_stmt(); break;
	case RETURN: tem = return_stmt(); break;
	default:
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
		token = getToken();
		break;
	}
	return tem;
}

TreeNode *expression_stmt(void)
{
	TreeNode *tem = NULL;
	printf(" %d : expression_stmt \n", count++);
	if (token == SEMICOL)
		match(SEMICOL);
	else if (token != RBRACE)
	{
		tem = expression();
		match(SEMICOL);
	}
	return tem;
}

TreeNode *selection_stmt(void)
{
	TreeNode *tem = newStmtNode(IfK);
	printf(" %d : selection_stmt \n", count++);
	match(IF);
	match(LPAREN);
	if (tem != NULL) tem->child[0] = expression();
	match(RPAREN);
	if (tem != NULL) tem->child[1] = statement();

	if (token == ELSE)
	{
		match(ELSE);
		if (tem != NULL)
			tem->child[2] = statement();
	}
	return tem;
}

TreeNode *iteration_stmt(void)
{
	TreeNode *tem = newStmtNode(WhileK);
	printf(" %d : iteration_stmt \n", count++);
	match(WHILE);
	match(LPAREN);
	tem->child[0] = expression();
	match(RPAREN);
	tem->child[1] = statement();

	return tem;
}

TreeNode *return_stmt(void)
{
	TreeNode *tem = newStmtNode(ReturnK);
	printf(" %d : return_stmt \n", count++);
	match(RETURN);
	if (token == SEMICOL)
		match(SEMICOL);
	else if (tem != NULL)
	{
		tem->child[0] = expression();
		match(SEMICOL);
	}
	return tem;
}

TreeNode *expression(void)
{
	TreeNode *tem = NULL;
	TreeNode *right = NULL;
	TreeNode *left = NULL;
	printf(" %d : expression \n", count++);
	if (token == ID)
	{
		left = call();
	}

	if (token == ASSIGN)
	{
		if (left != NULL)
		{
			match(ASSIGN);
			right = expression();
			tem = newExpNode(AssignK);
			if (tem != NULL)
			{
				tem->child[0] = left;
				tem->child[1] = right;
			}
		}
		else
		{
			syntaxError("unexpected token ");
			token = getToken();
		}
	}
	else
		tem = simple_expression(left);

	return tem;
}

TreeNode *simple_expression(TreeNode *next)
{
	TreeNode *tem = additive_expression(next);
	printf(" %d : simple_expression \n", count++);
	if ((token == LT) || (token == LTEQ) || (token == BG) || (token == BGEQ) || (token == EQ) || (token == NEQ))
	{
		TreeNode *p = newExpNode(OpK);

		if (p != NULL)
		{
			p->child[0] = tem;
			p->op = token;
			tem = p;
		}
		match(token);
		if (tem != NULL)
			tem->child[1] = additive_expression(NULL);

	}
	return tem;
}

TreeNode *additive_expression(TreeNode *next)
{
	TreeNode *tem = term(next);
	printf(" %d : additive_expression \n", count++);
	while ((token == PLUS) || (token == MINUS))
	{
		TreeNode *p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = tem;
			p->op = token;
			tem = p;
			match(token);
			tem->child[1] = term(NULL);
		}
	}
	return tem;
}

TreeNode *term(TreeNode *next)
{
	TreeNode *tem = factor(next);
	printf(" %d : term \n", count++);
	while ((token == TIMES) || (token == OVER))
	{
		TreeNode *p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = tem;
			p->op = token;
			tem = p;
			match(token);
			p->child[1] = factor(NULL);
		}
	}
	return tem;
}

TreeNode *factor(TreeNode *next)
{
	TreeNode *tem = NULL;
	printf(" %d : factor \n", count++);
	if (next != NULL)
		return next;

	switch (token)
	{
	case NUM:
		tem = newExpNode(ConstK);
		if ((tem != NULL) && (token == NUM))
			tem->val = atoi(tokenString);
		match(NUM);
		break;

	case ID:
		tem = call();
		break;

	case LPAREN:
		match(LPAREN);
		tem = expression();
		match(RPAREN);
		break;

	default:
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(output, "\n");
		token = getToken();
		break;
	}
	return tem;
}

TreeNode *call(void)
{
	TreeNode *tem = NULL;
	TreeNode *p = NULL;
	TreeNode *q = NULL;
	char *idName;
	printf(" %d : call \n", count++);
	if (token == ID)
		idName = copyString(tokenString);
	match(ID);

	if (token == LPAREN)
	{
		match(LPAREN);
		q = args();
		match(RPAREN);

		tem = newStmtNode(CallK);
		if (tem != NULL)
		{
			tem->child[0] = q;
			tem->name = idName;
		}
	}

	else
	{
		if (token == LBRACKET)
		{
			match(LBRACKET);
			p = expression();
			match(RBRACKET);
		}

		tem = newExpNode(IdK);
		if (tem != NULL)
		{
			tem->child[0] = p;
			tem->name = idName;
		}
	}

	return tem;
}

TreeNode *args(void)
{
	TreeNode *tem = NULL;
	printf(" %d : args \n", count++);
	if (token != RPAREN)
		tem = arg_list();
	return tem;
}

TreeNode *arg_list(void)
{
	TreeNode *tem = expression();
	TreeNode *p = tem;
	TreeNode *q;
	printf(" %d : arg_list \n", count++);
	while (token == COL)
	{
		match(COL);
		q = expression();
		if ((p != NULL) && (tem != NULL))
		{
			p->sibling = q;
			p = q;
		}
	}
	return tem;
}

TreeNode *parse(void)
{
	TreeNode *tem;
	printf(" %d : parse \n", count++);
	token = getToken();
	tem = declaration_list();
	
	if (token != ENDFILE)
		syntaxError("Error : Code ends before file\n");

	return tem;
}

int main(int argc, char *argv[])
{
	char input[20];
	TreeNode *ST;

	strcpy(input, argv[1]);

	source = fopen(input, "r");

	output = fopen(argv[2], "w");

	ST = parse();

	printTree(ST);

	printf("parse success\n");

	return 0;
}

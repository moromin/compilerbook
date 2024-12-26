#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************
 * tokenize.c *
 **************/
// Token
typedef enum {
	TK_RESERVED,
	TK_IDENT,
	TK_NUM,
	TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
	TokenKind kind;
	Token *next;
	int val;
	char *str;
	int len;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
char *strndup(char *p, int len);
bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize();

extern char *user_input; // Input program
extern Token *token; // Current token

/***********
 * parse.c *
 ***********/
typedef enum {
	ND_ADD,       // +
	ND_SUB,       // -
	ND_MUL,       // *
	ND_DIV,       // /
	ND_EQ,        // ==
	ND_NE,        // !=
	ND_LT,        // <
	ND_LE,        // <=
	ND_ASSIGN,    // =
	ND_RETURN,    // "return"
	ND_IF,		  // "if"
	ND_WHILE,     // "while"
	ND_EXPR_STMT, // Expression statement
	ND_VAR,       // Local variable
	ND_NUM,       // Integer
} NodeKind;

// Local variable
typedef struct Var Var;
struct Var {
	Var *next;
	char *name;
	int offset; // Offset from RBP
};

// AST node
typedef struct Node Node;
struct Node {
	NodeKind kind;
	Node *next;

	Node *lhs;
	Node *rhs;

	// "if" or "while" statement
	Node *cond;
	Node *then;
	Node *els;

	Var *var;  // Used for ND_VAR
	int val;   // Used for ND_NUM
};

typedef struct {
	Node *node;
	Var *locals;
	int stack_size;
} Program;

Program *program();

/*************
 * codegen.c *
 *************/
void codegen(Program *program);

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Type Type;

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
void error_tok(Token *tok, char *fmt, ...);
Token *consume(char *op);
char *strndup(char *p, int len);
Token *consume_ident();
void expect(char *op);
int expect_number();
char* expect_ident();
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
	ND_ADDR,      // &
	ND_DEREF,     // *
	ND_RETURN,    // "return"
	ND_IF,		  // "if"
	ND_WHILE,     // "while"
	ND_FOR,       // "for"
	ND_BLOCK,     // { ... }
	ND_FUNCALL,   // Function call
	ND_EXPR_STMT, // Expression statement
	ND_VAR,       // Local variable
	ND_NUM,       // Integer
} NodeKind;

// Local variable
typedef struct Var Var;
struct Var {
	char *name;
	int offset; // Offset from RBP
};

typedef struct VarList VarList;
struct VarList {
	VarList *next;
	Var *var;
};

// AST node
typedef struct Node Node;
struct Node {
	NodeKind kind;
	Node *next;
	Type *ty;
	Token *tok;

	Node *lhs;
	Node *rhs;

	// "if", "while" or "for" statement
	Node *cond;
	Node *then;
	Node *els;
	Node *init;
	Node *inc;

	// Block
	Node *body;

	// Function call
	char *funcname;
	Node *args;

	Var *var;  // Used for ND_VAR
	int val;   // Used for ND_NUM
};

typedef struct Function Function;
struct Function {
	Function *next;
	char *name;
	VarList *params;

	Node *node;
	VarList *locals;
	int stack_size;
};

Function *program();

/************
 * typing.c *
 ************/
typedef enum {
	TY_INT,
	TY_PTR
} TypeKind;

struct Type {
	TypeKind kind;
	Type *base;
};

void add_type(Function *prog);

/*************
 * codegen.c *
 *************/
void codegen(Function *program);

#include <assert.h>
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
	TK_STR,
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

	char *contents; // String literal including '\0'
	char cont_len;  // String literal
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
Token *peek(char *s);
Token *consume(char *op);
char *strndup(char *p, int len);
Token *consume_ident();
void expect(char *op);
int expect_number();
char *expect_ident();
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
	ND_SIZEOF,    // "sizeof"
	ND_BLOCK,     // { ... }
	ND_FUNCALL,   // Function call
	ND_EXPR_STMT, // Expression statement
	ND_VAR,       // Local variable
	ND_NUM,       // Integer
	ND_NULL,      // Empty statement
} NodeKind;

// Variable
typedef struct Var Var;
struct Var {
	char *name;    // Variable name
	Type *ty;      // Type
	bool is_local; // local or global

	// Local variable
	int offset;    // Offset from RBP

	// Global variable
	char *contents;
	int cont_len;
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

typedef struct {
	VarList *globals;
	Function *fns;
} Program;

Program *program();

/************
 * typing.c *
 ************/
typedef enum {
	TY_CHAR,
	TY_INT,
	TY_PTR,
	TY_ARRAY,
} TypeKind;

struct Type {
	TypeKind kind;
	Type *base;
	int array_size;
};

Type *char_type();
Type *int_type();
Type *pointer_to(Type *base);
Type *array_of(Type *base, int size);
int size_of(Type *ty);

void add_type(Program *prog);

/*************
 * codegen.c *
 *************/
void codegen(Program *prog);

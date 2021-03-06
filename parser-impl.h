#ifndef TG_INDICATOR_PARSER_IMPL_H
#define TG_INDICATOR_PARSER_IMPL_H

#include "lexer.h"

namespace tg {

/* ------ AST开始 ------ */

enum NodeType {
	NT_NONE = TK_ALL + 1, 
	NT_FORMULA,
	NT_STMT,
	NT_INT_EXPR,
	NT_DECIMAL_EXPR,
	NT_ID_EXPR,
	NT_FUNC_CALL,
	NT_EXPR_LIST,
	NT_BINARY_EXPR,
	NT_ALL
	
};

struct Value;

struct Node {
#ifndef NDEBUG
	enum NodeType type;
#endif
	void (*clean)(Node *node);
	Value *(*interp)(Node *node, void *parser); /* 运行当前节点 */
};

struct Stmt;
struct Expr;

struct Formula {
	struct Node node;
	Array stmts; // Stmt **
};

struct Stmt {
	struct Node node;
	String id;
	enum Token op; /* TK_COLON_EQ/TK_COLON */
	Node *expr;
	Value *value;
};

struct IntExpr {
	struct Node node;
	Value *value;
};

struct DecimalExpr {
	struct Node node;
	Value *value;
};

struct IdExpr {
	struct Node node;
	String val;
	Value *value;
};

struct ExprList {
	struct Node node;
	Array exprs; // Expr **
	Value **values;
};

struct FuncCall {
	struct Node node;
	String id;
	ExprList *args;
	Value *value;
};

struct BinaryExpr {
	struct Node node;
	Node *lhs;
	enum Token op;
	Node *rhs;
	Value *value;
};

/* ------ AST结束 ------ */

class Parser {
public:
	Lexer *lex;
	
	enum Token tok;
	char *tokval; /* 指向当前token的值(由于缓冲区在lex中,没有以字符'\0'结尾) */
	int toklen; /* 当前token的长度 */
	
	void *errdata;
	int (*handleError)(int lineno, int charpos, int error, const char *errmsg, void *);
	int errcount;
	bool isquit;
	
	Formula *ast;
	
	void *userdata;
	
#ifdef CONFIG_LOG_PARSER
	int interpDepth; /* 用于在LOG_INTERP时控制打印的前面的空白字符 */
#endif
};

}

#endif

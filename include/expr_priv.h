#ifndef EXPR_PRIV_H
#define EXPR_PRIV_H

#include "expr_pub.h"

struct char_expr {
	enum expr_type type;
	char c;
};

struct list_expr {
	enum expr_type type;
	struct expr *first;
	struct expr *rest;
};

struct star_expr {
	enum expr_type type;
	struct expr *sub;
};


//struct ed_resp* expr_ed( struct expr* e, const char* str, int* size );
int* expr_ed( const struct expr* e, const char* str, size_t* processed, int acc );
int* charexpr_ed( const struct char_expr* e, const char* str, size_t* processed, int acc );
//int* starexpr_ed( struct star_expr* e, const char* str, size_t* processed, int acc );
int* listexpr_ed( const struct list_expr* e, const char* str, size_t* processed, int acc );


void charexpr_free( struct char_expr* e );
//void starexpr_free( struct starexpr* e );
void listexpr_free( struct list_expr* e );

#endif

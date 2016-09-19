#ifndef EXPR_PUB_H
#define EXPR_PUB_H

enum expr_type { CHAR, STAR, LIST };

struct expr {
	enum expr_type type;
};

int edit_distance( struct expr* e, const char* str );

struct expr* parse( const char* expr_str );

void free_expr( struct expr* e );

#endif

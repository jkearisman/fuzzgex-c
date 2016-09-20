#ifndef EXPR_PUB_H
#define EXPR_PUB_H

enum expr_type { CHAR, STAR, LIST };

struct expr {
	enum expr_type type;
};


struct expr* parse( const char* expr_str );

int edit_distance( struct expr* e, const char* str );

void expr_free( struct expr* e );

void print_array( int* arr,  size_t size );

#endif

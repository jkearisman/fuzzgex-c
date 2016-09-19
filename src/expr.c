#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/expr_pub.h"
#include "../include/expr_priv.h"

int edit_ditance( struct expr* e, const char* str ) {
	return 0;
}

struct expr* parse( const char* expr_str ) {
	return NULL;
}

int* expr_ed( struct expr* e, const char* str, size_t* size ) {
	switch( e->type ) {
		case CHAR:
			return charexpr_ed( (struct char_expr*) e, str, size );
			break;
			/*
		case STAR:
			return starexpr_ed( (struct star_expr*) e );
			break;
		case LIST:
			return listexpr_ed( (struct list_expr*) e );
			break;
			*/
		default:
			return NULL;
			//Panic!
			break;
	}
}

int* charexpr_ed( struct char_expr *e, const char* str, size_t* size ) {
	assert( e->type == CHAR );
	*size = 0;
	int* ret = _charexpr_ed( e, str, size );
	ret[0] = 1;
	return ret;
}

int* _charexpr_ed( struct char_expr *e, const char* str, size_t* size ) {
	assert( e->type == CHAR );
	printf( "calling _charexpr_ed with str=%s, size=%lu\n", str, *size);
	// Trivial case. String is empty.
	// Size does not increase, since nothing is processed
	size_t my_size = (*size);
	if( *str == '\0' ) {
		int* ret = calloc( my_size+1, sizeof( *ret ) );
		return ret;
	}

	// No matter what, we will process at least 1 char
	(*size)++;

	// Matches the first character.
	if( *str == e->c ) {
		printf( "Matching char found\n" );
		int* ret = calloc( my_size + 2, sizeof( *ret ) );
		ret[my_size] = my_size;
		ret[my_size+1] = my_size;
		return ret;
	}

	//else
	int* res = _charexpr_ed( e, str+1, size );
	printf( "Character does not match. res[%lu] = %lu\n", my_size, my_size );
	res[my_size+1] = my_size+1;
	return res;

}


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/expr_pub.h"
#include "../include/expr_priv.h"


void print_array( int* arr, size_t size) {
	for( int i = 0; i < size; i++ ) {
		printf( "%d ", arr[i] );
	}
	printf( "\n" );
}


int edit_ditance( struct expr* e, const char* str ) {
	size_t processed = 0;
	int* ed_list = expr_ed( e, str, &processed, 0 );
	//TODO
	int ret = ed_list[0];
	free( ed_list );
	return ret;
}

struct expr* parse( const char* expr_str ) {
	return NULL;
}

int* expr_ed( const struct expr* e, const char* str, size_t* processed, int acc ) {
	switch( e->type ) {
		case CHAR:
			return charexpr_ed( (struct char_expr*) e, str, processed, acc );
			break;
			/*
		case STAR:
			return starexpr_ed( (struct star_expr*) e, str, processed, acc );
			break;
			*/
		case LIST:
			return listexpr_ed( (struct list_expr*) e , str, processed, acc );
			break;
		default:
			return NULL;
			//Panic!
			break;
	}
}

int* listexpr_ed( const struct list_expr *e, const char* str, size_t* processed, int acc ) {
	assert( e->type == LIST );
	*processed = 0;
	// Get the results from the first one
	int* first = expr_ed( e->first, str, processed, acc );
	print_array( first, (*processed)+1 );
	printf( "After processing first, *processed is %lu\n", *processed );

	size_t len_str = strlen( str );
	int tmp_buffer[len_str + 1];
	memset( tmp_buffer, 0xff, (len_str+1)*sizeof( *tmp_buffer ) );

	int max_processed = 0;
	for( int i = 0; i <= *processed; i++ ) {
		size_t rest_processed = 0;
		int* sub = expr_ed( e->rest, str+i, &rest_processed, acc + first[i] );
		if( i + rest_processed > max_processed ) {
			max_processed = i+rest_processed;
		}
		for( size_t j = 0; j <= rest_processed; j++ ) {
			if( tmp_buffer[i+j] == -1 || sub[j] < tmp_buffer[i+j] ) {
				tmp_buffer[i + j] = sub[j];
			}
		}
		free( sub );
	}
	free( first );
	*processed = max_processed;
	int* ret = calloc( max_processed+1, sizeof( *ret ) );
	for( size_t i = 0; i <= max_processed; i++ ) {
		ret[i] = tmp_buffer[i];
	}

	return ret;

}

int* charexpr_ed( const struct char_expr *e, const char* str, size_t* processed, int acc ) {
	assert( e->type == CHAR );
	*processed = 0;
	int i = *processed;
	int* ret = NULL;
	while( *str != '\0' && *str != e->c ) {
		str++;
		i++;
	}
	if( *str ) {
		ret = calloc( i + 2, sizeof( *ret ) );
		ret[i+1] = i + acc;
		*processed = i + 1;
	} else {
		ret = calloc( i + 1, sizeof( *ret ) );
		*processed = i;
	}
	for( i = i; i > 0; i-- ) {
		ret[i] = i+acc;
	}

	//Trivial case where we don't process a char always has cost of 1
	ret[0] = acc + 1;

	return ret;
}

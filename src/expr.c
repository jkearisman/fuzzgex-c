#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/expr_pub.h"
#include "../include/expr_priv.h"


/**
 * Prints the array, this is mostly used for debugging, and will be removed
 * in the final version
 */
void print_array( int* arr, size_t size) {
	for( int i = 0; i < size; i++ ) {
		printf( "%d ", arr[i] );
	}
	printf( "\n" );
}


/**
 * Calculate the edit distance for str to match the expression e
 * returns an int that corresponds with the fewest number of edits
 */
int edit_distance( struct expr* e, const char* str ) {
	size_t processed = 0;
	int* ed_list = expr_ed( e, str, &processed, 0 );
	size_t max_processed = processed;
	if( max_processed < 0 ) {
		return -1;
	}
	size_t len_str = strlen( str );
	size_t min_score = ed_list[0] + len_str;
	for( int i = 1; i <= max_processed; i++ ) {
		if( ed_list[i] + len_str - i < min_score ) {
			min_score = ed_list[i] + len_str - i;
		}
	}
	free( ed_list );
	return min_score;
}

/**
 * Parse a string into an expr struct
 * The result of this function must eventually be freed
 */
struct expr* parse( const char* expr_str ) {
	if( expr_str[0] == '\0' ) {
		//This isn't supported yet. Need to add emptyexpr
		assert( 0 );
		return NULL;
	}
	if( expr_str[1] == '\0' ) {
		struct char_expr* c = malloc( sizeof *c );
		c->type = CHAR;
		c->c = expr_str[0];
		return (struct expr*)c;
	}
	struct list_expr* l = malloc( sizeof *l );
	struct char_expr* c = malloc( sizeof *c );
	c->type = CHAR;
	c->c = expr_str[0];
	l->type = LIST;
	l->first = (struct expr*)c;
	l->rest = parse( expr_str + 1 );
	return (struct expr*)l;
}

/**
 * Calculate all possible edit distances of between a e and str.
 * The return value of this function is an int array.
 * Element i of the array is the cost of processing the first i chars.
 * the last possible value of i (i.e. the maximum number of processed chars) is
 *  stored in processed. (the output array length is *processed+1
 *
 * The acc parameter allows you to encode any previous costs, allowing
 *  recursive parsing of expressions
 *
 * The output of this function will need to be freed
 */
int* expr_ed( const struct expr* e, const char* str, size_t* processed, int acc ) {
	// the default expr struct contains only 1 field, but depending on the
	// value of that field, we cast it to a different kind of expression
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

	size_t len_str = strlen( str );

	// Where we store temporary results.
	int tmp_buffer[len_str + 1];
	memset( tmp_buffer, 0xff, (len_str+1)*sizeof( *tmp_buffer ) );

	int max_processed = 0;
	// based on all possible results of the first, process the rest
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

	// Move entries in tmp_buffer to a heap-allocated array
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

	//Iterate through the string until we see the end or our char
	// Keep track of how far we needed to go
	while( *str != '\0' && *str != e->c ) {
		str++;
		i++;
	}
	// Assuming we got to our char
	if( *str ) {
		ret = calloc( i + 2, sizeof( *ret ) );
		ret[i+1] = i + acc;
		*processed = i + 1;
	} else { // We got to the end of the string
		ret = calloc( i + 1, sizeof( *ret ) );
		*processed = i;
	}
	// processing i invalid chars requires i-1 deletions and 1 substitution
	for( i = i; i > 0; i-- ) {
		ret[i] = i+acc;
	}

	//Trivial case where we don't process a char always has cost of 1 (insertion)
	ret[0] = acc + 1;

	return ret;
}

/**
 * Free any memory allocated by the expr
 * Depending on the type of expr, this may need to do a recersive descent
 * Any pointers in the AST should be set to NULL after freeing
 */
void expr_free( struct expr* e ) {
	switch( e->type ) {
		case CHAR:
			charexpr_free( (struct char_expr*) e );
			break;
			/*
		case STAR:
			return starexpr_ed( (struct star_expr*) e, str, processed, acc );
			break;
			*/
		case LIST:
			listexpr_free( (struct list_expr*) e );
			break;
		default:
			return;
			//Panic!
	}
}

void charexpr_free( struct char_expr* e ) {
	assert( e->type == CHAR );
	free( e );
}

void listexpr_free( struct list_expr* e ) {
	assert( e->type == LIST );
	expr_free( e->first );
	e->first = NULL;
	expr_free( e->rest );
	e->rest = NULL;
	free( e );
}

void starexpr_free( struct star_expr* e ) {
	assert( e->type == STAR );
	expr_free( e->sub );
	e->sub = NULL;
	free( e );
}

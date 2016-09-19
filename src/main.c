#include <stdio.h>

#include "../include/expr_pub.h"
#include "../include/expr_priv.h"

void print_array( int* arr,  size_t size ) {
	for( int i = 0; i < size; i++ ) {
		printf( "%d ", arr[i] );
	}
	printf( "\n" );
}

int main( ) {
	struct char_expr c = { CHAR, 'a' };
	size_t size;
	int* res = charexpr_ed( &c, "bba", &size );
	print_array( res, size + 1);
	int* res2 = charexpr_ed( &c, "bbb", &size );
	print_array( res2, size + 1);
	int* res3 = charexpr_ed( &c, "aaa", &size );
	print_array( res3, size + 1);
}

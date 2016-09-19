#include <stdio.h>
#include <stdlib.h>


#include "../include/expr_pub.h"
#include "../include/expr_priv.h"


int main( ) {
	struct expr* tst = parse( "ab" );
	size_t size;
	int* res = expr_ed( tst, "ab", &size, 0 );
	print_array( res, size + 1);
	free( res );
}

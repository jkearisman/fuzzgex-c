#include <stdio.h>
#include <stdlib.h>


#include "../include/expr_pub.h"


int main( ) {
	struct expr* tst = parse( "abcd" );
	/*
	int res = edit_distance( tst, "bb" );
	printf( "Edit distance is %d\n", res );
	*/
	expr_free( tst );
}

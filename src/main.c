#include <stdio.h>
#include <stdlib.h>


#include "../include/expr_pub.h"
#include "../include/expr_priv.h"


int main( ) {
	struct char_expr a = { CHAR, 'a' };
	struct char_expr b = { CHAR, 'b' };
	struct list_expr l = { LIST, (struct expr*)&a, (struct expr*)&b };
	size_t size;
	int* res = expr_ed( (struct expr*) &l, "ab", &size, 0 );
	print_array( res, size + 1);
	free( res );
}

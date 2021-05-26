/*
EXP09-A. Treat relational and equality operators as if they were nonassociative

The relational and equality operators are left-associative, not non-associative as they often are in other languages. This allows a C++ programmer to write an expression (particularly an expression used as a condition) that can be easily misinterpreted.
*/

void f() { // diagnostic
	int a = 2;
	int b = 2;
	int c = 2;
	// ...
	if ( a < b < c ) // condition #1, misleading, likely bug
		;
	if ( a == b == c ) // condition #2, misleading, likely bug
		;
}

void g() { // no diagnostic
	int a = 2;
	int b = 2;
	int c = 2;
	if ( a < b && b < c ) // clearer, and probably what was intended
		;
	if ( a == b && a == c ) // ditto
		;
}

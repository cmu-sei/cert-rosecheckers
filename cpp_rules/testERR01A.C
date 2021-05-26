/*
 ERR01-A. Prefer special-purpose types for exceptions

Because an exception is caught by its type, rather than its value, it is best to throw objects of special-purpose types that describe the nature of the exception being thrown. Throwing objects of more general-purpose types, particularly predefined types, makes code hard to understand and maintain, and defeats much of the advantage of the C++ exception-handling mechanism.
*/

extern void doSomething();

struct X {};

void f() {
	try {
		doSomething();
	}
	catch( char const *msg ) // diagnostic
		{}
	catch( int x ) // diagnostic
		{}
	catch( int (*fp)() ) // diagnostic
		{}
	catch( X &xr ) // no diagnostic
		{}
	catch( X *xp ) // diagnostic
		{}
}

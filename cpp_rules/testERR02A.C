/*
ERR02-A. Throw anonymous temporaries and catch by reference

When an exception is thrown, the value of the object in the throw expression is used to initialize an anonymous temporary exception object that is in a "safe" location. It is this temporary object that is matched to the types declared in catch clauses and rethrown as the result of a throw; statement. In a way similar to initializing the formal argument of a function with the corresponding actual argument in a function call, the variable in a catch clause is initialized with the anonymous temporary exception object. These mechanics imply the advice given in the title of this recommendation.
*/

#include <exception>

struct StackUnderflow {
	void modify() {}
};

extern int h();

void f() {
	switch( h() ) {
	case 1:
		throw new StackUnderflow; // diagnostic
	case 2: {
		StackUnderflow su1;
		throw &su1; //diagnostic
		}
	case 3: {
		static StackUnderflow su2; // diagnostic
		throw &su2;
		}
	}
}

void g() {
	try {
		f();
	}
	catch( StackUnderflow su ) { // diagnostic
		su.modify(); // modify argument
		throw; // modifications lost
	}
	catch( std::exception e ) { // diagnostic
		throw e; // original type of exception lost
	}
	
}

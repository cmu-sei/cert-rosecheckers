/*
EXP03-A. Do not overload the & operator


The C++ standard [ISO/IEC 14882-2003] says in Section 5.3.1 paragraph 4 that

    The address of an object of incomplete type can be taken, but if the complete type of that object is a class type that declares operator&() as a member function, then the behavior is undefined (and no diagnostic is required).

Therefore, to avoid possible undefined behavior, the operator & should not be overloaded.
*/

class X {
	X *operator &(); // diagnostic
	bool operator &( const X & ); // no diagnostic
};

X *operator &( const X & ); // diagnostic
X *operator &( const X &, const X & ); // no diagnostic

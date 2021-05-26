/*
EXP36-C. Do not cast or delete pointers to incomplete classes

The use of incomplete class declarations (also known as "forward" declarations) is common. While it is possible to declare pointers and references to incomplete classes, because the class definition is not available it is not possible to access a member of the class, determine the size of the class object, and so on. However, it is possible to cast and delete a pointer to an incomplete class, but this is never a good idea.
*/
/*
class Body; // incomplete class declaration

class Handle {
  public:
    Handle();
    ~Handle()
	{ delete impl_; } // diagnostic:  deletion of pointer to incomplete class
  private:
    Handle( const Handle & );
    Handle &operator =( const Handle & );
    Body *impl_;
};

class B {
};
B *getMeSomeSortOfB();

class D; // incomplete declaration

void f() {
	B *bp = getMeSomeSortOfB();
	D *dp = (D *)bp; // diagnostic:  old-stlye cast: legal, but inadvisable
	dp = reinterpret_cast<D *>(bp); // diagnostic:  new-style cast: legal, but inadvisable
	delete bp; // no diagnostic
	delete dp; // diagnostic
}
*/
class C;
inline void g( C *cp ) {
	delete cp; // diagnostic
}

class C {};
inline void h( C *cp ) {
	delete cp; // no diagnostic
}
/*
class E {
  public:
	void mf1( E *dp ) {
		delete dp; // no diagnostic
	}
	void mf2();
};
void E::mf2() {
	delete this; // no diagnostic
}
*/

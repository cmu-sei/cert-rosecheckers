/*
EXP10-A. Prefer the prefix forms of ++ and --.

The unary ++ and ? operators may be applied in either a prefix or postfix form with somewhat different behavior. However, if the value of the unary expression is not used in an enclosing expression, either form will produce the same result (for the built-in ++ and --), or should produce the same result (for user-defined ++ and --).

However, for user-defined overloaded ++ and -- operators, it is typically possible to produce a prefix implementation that is more efficient than the corresponding postfix version. For this reason, it is considered good practice in C++ to prefer the prefix forms of these operators when either will do.
*/

struct Iter {
	Iter &operator ++();
	Iter operator ++( int );
	Iter &operator --();
	Iter operator --( int );
	bool operator !=( const Iter & );
	operator bool() const;
};

struct Container {
	typedef Iter iterator;
	iterator begin();
	iterator end();
};

void iterate1( Container &c ) {
    typedef Container::iterator I;
    for( I i(c.begin());
	i != c.end();
	i++ ) // diagnostic
        ;
    for( I i(c.begin());
	i != c.end();
	++i ) // no diagnostic
        ;
    I i(c.begin());
    while (i != c.end() ) {
        i++; // diagnostic
    }
    Iter j;
    for( ; true; j++ ) // diagnostic
        ;
    for( ; true; ++j ) // no diagnostic
        ;
    j++; // diagnostic
    ++j; // no dignostic
    Iter k = j++; // no dignostic
    k = j++; // no diagnostic
    j.operator --(); // no diagnostic
    j.operator --(0); // diagnostic
}

template <class Cont>
void iterate3( Cont &c ) {
    typedef typename Cont::iterator I;
    for( I i(c.begin()); i != c.end(); i++ ) // diagnostic
        ;
}

template <class Cont>
void iterate4( Cont &c ) {
    typedef typename Cont::iterator I;
    for( I i(c.begin()); i != c.end(); ++i ) // no diagnostic
        ;
}


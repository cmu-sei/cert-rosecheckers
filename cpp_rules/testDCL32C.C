/*
DCL32-C. Avoid runtime static initialization of objects with external linkage

Runtime static initializations of namespace scope objects take place before main is entered. The initialization order is well-defined within a particular translation unit, but undefined between translation units. This can give rise to subtle initialization order dependencies and transient, difficult-to-reproduce bugs. There is a similar problem with runtime static destruction, which occurs after return from main.
*/

extern char const *myGetenv( char const * );
extern int myStrcmp( char const *, char const * );

extern char const *const term;
bool isVT100 = myStrcmp( term, "VT100" ) == 0; // diagnostic

char const * const term = myGetenv( "TERM" ); // diagnostic

extern char const *termF();

bool isVT100F() {
    static bool is = myStrcmp( termF(), "VT100" ) == 0; // no diagnostic
    return is;
}

char const *termF() {
    static char const * const theTerm = myGetenv( "TERM" ); // no diagnostic
    return theTerm;
}

static char const * const termL = myGetenv( "TERM" ); // no diagnostic

static bool isVT100L = myStrcmp( termL, "VT100" ) == 0; // no diagnostic

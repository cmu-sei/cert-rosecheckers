/*
EXP04-A. Do not overload the comma operator
Added by Fred Long, last edited by Justin Pincar on Sep 12, 2007  (view change) show comment hide comment

The built in comma operator guarantees that the first operand is evaluated before the second operand. However, if the comma operator is overloaded, its operand evaluation depends on C++'s function parameter mechanism, which does not guarantee the order of evaluation. Therefore, to avoid possibly confusing behavior, the comma operator should not be overloaded.
*/

class X {
	void operator ,( const X & );
};

X operator ,( const X &, int );

/*
EXP02-A. Do not overload the logical AND and OR operators
Added by Fred Long, last edited by Justin Pincar on Sep 12, 2007  (view change) show comment hide comment

The logical AND and logical OR operators (&&, ||) exhibit "short circuit" operation. That is, the second operand is not evaluated if the result can be deduced solely by evaluating the first operand. However, short-circuit evaluation is not possible for overloaded versions of these operators. Therefore, to avoid possibly confusing behavior, these operators should not be overloaded.
*/

class X {
	X &operator ||( int );
};

bool operator &&( const X &, const X & );

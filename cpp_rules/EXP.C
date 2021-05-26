/*
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"

// Note:  As written, these tests catch template declarations only if instantiated.

//XXX other casting advice, not reint if const, etc.
bool EXP00_A( const SgNode *node ) { // Do not use C-style casts
	//XXX This doesn't work when the type being casted to is a class. X(12) or (X)12, for instance.
	//XXX ROSE seems to recognize only reinterpret_cast as new; others are old.
	return false; //XXX Disabled due to false positives
	bool result = false;
	if( const SgCastExp *cast = isSgCastExp( node ) ) {
		switch( cast->get_cast_type() ) {
		case SgCastExp::e_unknown:
		case SgCastExp::e_default:
			break;
		case SgCastExp::e_C_style_cast:
			// Note that a cast node might be an impicit promotion.
			if( !isCompilerGeneratedNode( node ) ) {
				diagnostic( "EXP00-A", node, "avoid old-style casts." );
				result = true;
			}
			break;
		case SgCastExp::e_const_cast:
		case SgCastExp::e_static_cast:
		case SgCastExp::e_dynamic_cast:
		case SgCastExp::e_reinterpret_cast:
			break;
		}
	}
	//XXX probably should check for "call" of a ctor here...
	return result;
}

bool EXP02_A( const SgNode *node ) { // Do not overload the logical AND and OR operators
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const std::string n = fd->get_name().getString();
		if( n == "operator&&" || n == "operator||" ) {
			diagnostic( "EXP02-A", fd, "overloaded && or || operator." );
			return true;
		}
	}
	return false;
}

bool EXP03_A( const SgNode *node ) { // Do not overload the & operator
	// Note this is for the unary & operator only, not binary.
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const std::string n = fd->get_name().getString();
		if( n == "operator&" ) {
			int args = argCount( fd );
			bool isMember = isSgMemberFunctionDeclaration( fd );
			if( (isMember && args == 0) || (!isMember && args == 1) ) {
				diagnostic( "EXP03-A", fd, "overloaded unary & operator." );
				return true;
			}
		}
	}
	return false;
}

bool EXP04_A( const SgNode *node ) { // Do not overload the comma operator
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const std::string n = fd->get_name().getString();
		if( n == "operator," ) {
			diagnostic( "EXP04-A", fd, "overloaded , operator." );
			return  true;
		}
	}
	return false;
}

bool EXP08_A( const SgNode *node ) { // A switch statement should have a default clause unless every enumeration value is tested
	// Synopsis:  If the switch expression has enum type, get the values of all the enumerators and ensure
	// that each value is used by a case label.  (Note that we are going by the enum and label values, not names.)
	// If that test fails, check for a default.

	bool result = false;

	if( const SgSwitchStatement *s = isSgSwitchStatement( node ) ) {
		const SgStatement *stat = s->get_item_selector();
		ROSE_ASSERT( stat );
		const SgExprStatement *estat = isSgExprStatement( stat );
		//XXXINFO it seems that expression statements (and returns?) no longer have a pointer to a wrapper class.
		//XXXINFO also, not all valid switch syntaxes are handled by ROSE, like "switch( x ) while( a ) case 1: a = 12;"
		const SgExpression *expr = estat->get_expression();
		expr = removeImplicitIntegralPromotions( expr );
		Type et( expr->get_type() );
		if( const SgEnumDeclaration *edecl = et.getEnumDeclaration() ) {
			// Get the set of enumerator values, and the set of case label values, and see if
			// the intersection is complete.  If not, check for a default label.
			std::vector<int> evalues; // values of enumerators
			getEnumeratorValues( edecl, evalues );
			const SgBasicBlock *body = s->get_body();
			std::vector<int> cvalues; // values of case labels
			bool defaultExists = getCaseValues( body, cvalues );
			std::sort( evalues.begin(), evalues.end() );
			std::sort( cvalues.begin(), cvalues.end() ); // this one can't have duplicates (if the compiler's working!)

			size_t original_esize = evalues.size();
			evalues.erase( std::unique( evalues.begin(), evalues.end() ), evalues.end() );
			if( original_esize != evalues.size() ) {
				// duplicate enumerator values
				result = true;
				diagnostic( "EXP08-A", node, "switching over an enum that has duplicate enumerator values." );
			}

			std::vector<int> difference; // temp for set operation results
			std::set_difference( cvalues.begin(), cvalues.end(),
						evalues.begin(), evalues.end(), back_inserter(difference) );
			if( !difference.empty() ) {
				// there are cases that do not correspond to enumerator
				result = true;
				diagnostic( "EXP08-A", node, "there are" +
					utostring( difference.size() ) + " cases for values that do not correspond to any enumerator value." );
			}
			if( evalues.size() > cvalues.size() ) {
				// fewer case labels than enumerators
				if( !defaultExists ) {
					result = true;
					diagnostic( "EXP08-A", node, "not all enumerator values have corresponding cases." );
				}
			}
			else if( evalues.size() == cvalues.size() ) {
				// same size, see if the values match up
				if( !std::equal( evalues.begin(), evalues.end(), cvalues.begin() ) ) {
					result = true;
					diagnostic( "EXP08-A", node, "case labels do not match enumerator values." );
				}
			}
		}
	}
	return result;
}

bool EXP09_A( const SgNode *node ) { // Treat relational and equality operators as if they were nonassociative
	// Here, we examine only predefined relational and equality operators, because who knows what an
	// overloaded operator might intend?
	if( const SgBinaryOp *bop = isSgBinaryOp( node ) ) {
		if( isEqRelop( bop ) ) {
			const SgExpression *lhs = bop->get_lhs_operand();
			lhs = removeImplicitIntegralOrFloatingPromotions( lhs );
			const SgExpression *rhs = bop->get_rhs_operand();
			rhs = removeImplicitIntegralOrFloatingPromotions( rhs );
			if( isEqRelop( lhs ) || isEqRelop( rhs ) ) {
				diagnostic( "EXP09-A", node, "associative treatment of equality or relational operators." );
				return true;
			}
		}
	}
	return false;
}

bool EXP10_A( const SgNode *node ) { // Prefer the prefix forms of ++ and --.
	return false; //XXXXXXXXXXXX this needs work
	const SgUnaryOp *uop = 0;
	if( (uop = isSgPlusPlusOp( node )) || (uop = isSgMinusMinusOp( node )) ) {
		// Predefined ++ or --.
	}
	else if( const SgFunctionCallExp *fcall = isSgFunctionCallExp( node ) ) {
		// Function call; see if user-defined ++ or --.
		bool postfix = false;
		const SgExpression *fexpr = fcall->get_function();
		const size_t numArgs = fcall->get_args()->get_expressions().size();
		if( const SgFunctionRefExp *fref = isSgFunctionRefExp( fexpr ) ) {
			const SgFunctionSymbol *fsym = fref->get_symbol();
			const SgFunctionDeclaration *fdecl = fsym->get_declaration();
			const std::string n = fdecl->get_name().getString();
			if( n == "operator++" || n == "operator--" ) {
				if( fref )
					postfix = (numArgs == 2);
				const SgMemberFunctionRefExp *mfref = isSgMemberFunctionRefExp( fexpr );
				if( !mfref ) {
					const SgBinaryOp *bop = 0;
					if( (bop = isSgArrowExp( fexpr )) || (bop = isSgDotExp( fexpr )) ) {
						const SgExpression *rhs = bop->get_rhs_operand();
						if( mfref = isSgMemberFunctionRefExp( rhs ) )
							postfix = numArgs == 1;
					}
				}
				if( postfix ) {
					// Postfix use of user-defined ++ or --.  Issue diagnostic if not part of a
					// larger expression.
					const SgNode *parent = fcall->get_parent();
					if( !isSgExpression( parent ) ) {
						diagnostic( "EXP10-A", node, "unnecessary use of postfix increment or decrement." );
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool EXP36_C( const SgNode *node ) { // Do not cast or delete pointers to incomplete classes
	//XXX problems with determingin whether a declaration is complete or incomplete.
	//XXX punt for now.
	return false; //XXX
	if( const SgCastExp *cexp = isSgCastExp( node ) ) {
		const SgExpression *expr = cexp->get_operand();
		if( expressionIsPointerToIncompleteClass( expr ) ) {
			diagnostic( "EXP36-C", node, "cast involving pointer to incomplete class." );
			return true;
		}
	}
	else if( const SgDeleteExp *dexp = isSgDeleteExp( node ) ) {
		if( expressionIsPointerToIncompleteClass( dexp->get_variable() ) ) {
			//if( isSgThisExp(dexp->get_variable()) ) return false; //XXX hack
			diagnostic( "EXP36-C", node, "deletion of pointer to incomplete class." );
			return true;
		}
	}
	return false;
}

void handleCallToVirtualMemberOfThisClass( const SgFunctionCallExp *fcall, const SgClassDefinition *cdtorClassDef ) {
	const SgExpression *function = fcall->get_function();
	const SgMemberFunctionRefExp *mfre = 0;
	const SgBinaryOp *bop = 0;
	if( mfre = isSgMemberFunctionRefExp( function ) )
		/* nothing */;
	else if( (bop = isSgArrowExp( function )) || (bop = isSgDotExp( function )) ) {
		const SgExpression *lhs = bop->get_lhs_operand();
		if( isSgThisExp( lhs ) ) {
			const SgExpression *rhs = bop->get_rhs_operand();
			mfre = isSgMemberFunctionRefExp( rhs );
		}
	}
	//if( SgPointerDerefExp *ptrderef = isSgPointerDerefExp( function ) )
	//	std::cout << "PTR DEREF" << fcall->get_file_info()->get_line() << std::endl;
	if( mfre ) {
		const SgMemberFunctionSymbol *mfs = mfre->get_symbol();
		const SgMemberFunctionDeclaration *mfd = mfs->get_declaration();
		const SgClassDefinition *functionCallClassDef = mfd->get_class_scope();
		if( bool isVirtual = mfd->get_functionModifier().isVirtual() ) {
			//XXX not quite right:  non-virtual call of virtual incorrectly flagged.
			//XXX How to find out if the function name is qualified?  These do nothing:
			//int vc = mfre->get_virtual_call();
			//int nq = mfre->get_need_qualifier();
			// XXXcan't seem to find a use of SgQualifiedName anywhere either...
			if( functionCallClassDef == cdtorClassDef ) {
				diagnostic( "EXP38-C", fcall, "calling own virtual in constructor or destructor" );
			}
			else if( isBaseOf( cdtorClassDef, functionCallClassDef ) ) {
				diagnostic( "EXP38-C", fcall, "calling base class virtual in constructor or destructor" );
			}
		}
	}
}

class FunctionBodyTraversal : public AstSimpleProcessing {
  public:
	FunctionBodyTraversal( const SgClassDefinition *classdef )
		: classdef_(classdef) {}
	virtual void visit( SgNode *node ); // base class abstract takes ptr to non-const...
  private:
	const SgClassDefinition *classdef_;
};

void FunctionBodyTraversal::visit( SgNode *node ) {
	if( const SgFunctionCallExp *fcall = isSgFunctionCallExp( const_cast<const SgNode *>(node) ) )
		handleCallToVirtualMemberOfThisClass( fcall, classdef_ );
}

bool EXP38_C( const SgNode *node ) { // Avoid calling your own virtual functions in constructors and destructors.
	if( const SgFunctionDefinition *fdef = isSgFunctionDefinition( node ) ) {
		const SgFunctionDeclaration *fdec = fdef->get_declaration();
		if( fdec->get_specialFunctionModifier().isConstructor() || fdec->get_specialFunctionModifier().isDestructor() ) {
			if( const SgMemberFunctionDeclaration *mfdec = isSgMemberFunctionDeclaration( fdec ) ) {
				// get the type of the class to which ctor or dtor belongs
				const SgClassDefinition *cdef = mfdec->get_class_scope();
				const SgClassDeclaration *cdecl = cdef->get_declaration();
				//SgClassType *ctype = cdecl->get_type();
				const SgBasicBlock *body = fdef->get_body();
				//SgStatementPtrList &stats = body->get_statements();
				FunctionBodyTraversal bt( cdef );
				bt.traverse( const_cast<SgBasicBlock *>(body), postorder );
			}
		}
	}
	return false;
}

//char const * const memcmpIdentifier = "_memcmp___";
//char const * const memcpyIdentifier = "_memcpy___";

bool EXP39_C( const SgNode *node ) { // Don't bitwise copy class objects
	//if( SgFunctionRefExp *sgfrexp = isSgFunctionRefExp( node ) ) {
	//	std::string sgfrexpName = sgfrexp->get_symbol()->get_declaration()->get_mangled_name().getString();
	//	if( sgfrexpName.find( memcpyIdentifier, 0, strlen( memcpyIdentifier ) ) != std::string::npos
	//		|| sgfrexpName.find( memcmpIdentifier, 0, strlen( memcmpIdentifier ) ) != std::string::npos ) {
	//	}
	//}
	return false;
}

bool EXP( SgProject *project ) {
	bool violation = false;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( project, V_SgNode ); //XXX should use visit
	for( Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		//violation |= EXP00_A( *i ); //XXX needs fixing
		violation |= EXP02_A( *i );
		violation |= EXP03_A( *i );
		violation |= EXP04_A( *i );
		violation |= EXP08_A( *i );
		violation |= EXP09_A( *i );
		violation |= EXP10_A( *i );
		violation |= EXP36_C( *i );
		violation |= EXP38_C( *i );
		violation |= EXP39_C( *i );
	}
	return violation;
}

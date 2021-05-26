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
#include "rose.h"
#include "utilities.h"

enum Exp { ExpAppropriate = 0, ExpPointer, ExpStd, ExpOtherBad };

Exp isNotAppropriateExceptionType( Type t ) {
	if( const SgClassDeclaration *classdecl = t.getClassDeclaration() ) {
		// only type thrown from std namespace should be derived from std::exception
		if( isClassDeclaredInStdNamespace( classdecl ) ) {
			if( !isStdExceptionOrTypeDerivedFromStdException( classdecl ) )
				return ExpStd;
		}
	}
	else if( t.isPointer() ) { // it's a pointer to something: bad
		return ExpPointer;
	}
	else { // it's not a class or a pointer, violation of rule
		return ExpOtherBad;
	}
	return ExpAppropriate;
}

bool ERR01_A( const SgNode *node ) { // Prefer special-purpose types for exceptions
	bool result = false;
	if( const SgThrowOp *throwop = isSgThrowOp( node ) ) {
		if( throwop->get_throwKind() == SgThrowOp::throw_expression ) { // not a rethrow or exception-specification
			const SgExpression *expr = throwop->get_operand();
			Type t( expr->get_type() );
			switch( isNotAppropriateExceptionType( t ) ) {
			case ExpAppropriate:
				break;
			case ExpPointer:
				result = true;
				diagnostic( "ERR01-A", node, "should throw a special-purpose exception object, not a pointer." );
				break;
			case ExpStd:
				result = true;
				diagnostic( "ERR01-A", node, "do not throw standard types (except standard exceptions)." );
				break;
			case ExpOtherBad:
				result = true;
				diagnostic( "ERR01-A", node, "should throw a special-purpose exception object." );
				break;
			}
		}
	}
	return result;
}

bool doTryCatch( const SgTryStmt *trystmt ) {
	bool result = false;
	//	const SgStatement *trybody = trystmt->get_body();
	//	const SgStatementPtrList &stats = trybody->get_statements();
	const SgCatchStatementSeq *catchseq = trystmt->get_catch_statement_seq_root();
	const SgStatementPtrList &clauses = catchseq->get_catch_statement_seq();
	size_t clauseNumber = 0;
	size_t ellipsisClause = 0;
	for( SgStatementPtrList::const_iterator i = clauses.begin(); i != clauses.end(); ++i ) {
		if( const SgCatchOptionStmt *clause = isSgCatchOptionStmt( *i ) ) {
			++clauseNumber;
			const SgVariableDeclaration *decl = clause->get_condition();
			const SgInitializedNamePtrList &vars = decl->get_variables();
			const SgInitializedName *arg = vars.front();
			Type argType( arg->get_type() );
			const std::string argName = arg->get_name().getString();
			// const SgBasicBlock *body = clause->get_body();
			if( argType.isEllipsis() ) {
				if( ellipsisClause ) {
					// Note:  The compiler seems to catch this one, typically.
					result = true;
					diagnostic( "ERR02-A", clause, "repeated ellipsis catch-clause." );
				}
				ellipsisClause = clauseNumber;
			}
			else {
				if( !argType.isReference() ) {
					result = true;
					diagnostic( "ERR02-A", clause, "should catch by reference." );
				}
				else {
					// even if it is a reference, it may be a ref to an inappropriate type
					switch( isNotAppropriateExceptionType( argType.dereference() ) ) {
					case ExpAppropriate:
						break;
					case ExpPointer:
						result = true;
						diagnostic( "ERR02-A", clause, "should catch objects, not pointers." );
						break;
					case ExpStd:
						result = true;
						diagnostic( "ERR02-A", clause, "do not catch standard types (except standard exceptions)." );
						break;
					case ExpOtherBad:
						result = true;
						diagnostic( "ERR02-A", clause, "should catch a special-purpose exception object." );
						break;
					}
				}
			}
		}
	}
	if( ellipsisClause && (ellipsisClause != clauseNumber) ) {
		// Note:  The compiler seems to catch this one, typically.
		result = true;
		diagnostic( "ERR02-A", catchseq, "ellipsis should be last catch-clause." );
	}
	return result;
}

bool ERR02_A( const SgNode *node ) { // Throw anonymous temporaries and catch by reference
	if( const SgThrowOp *throwop = isSgThrowOp( node ) ) {
		if( throwop->get_throwKind() == SgThrowOp::throw_expression ) { // not a rethrow or exception-specification
			const SgExpression *expr = throwop->get_operand();
			ROSE_ASSERT( expr );
			//??? Don't know how torecognize anonymous temporaries!
		}
	}
	else if( const SgTryStmt *trystmt = isSgTryStmt( node ) )
		return doTryCatch( trystmt );
	else
		return false;
}

bool ERR( SgProject *project ) {
	bool violation = false;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( project, V_SgNode ); //XXX should use visit
	for( Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		violation |= ERR01_A( *i );
		violation |= ERR02_A( *i );
	}
	return violation;
}

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
#include <map>
#include <algorithm>
#include <functional>
#include "rose.h"
#include "utilities.h"
#include <iostream>

std::ostream &diagnostic( const char *rule, const SgNode *node, const std::string &message, std::ostream &os ) {
	int line = node->get_file_info()->get_line();
	if( line != 0 ) // don't flag compiler-generated code
		os << rule << ". " << node->get_file_info()->get_filenameString()
			<< ", line " << line << ": " << message << std::endl;
	return os;
}

std::string utostring( unsigned int n ) {
	std::string temp;
	if( n == 0 )
		temp = '0';
	else {
		do {
			switch( n % 10 ) {
			case 0: temp += '0'; break;
			case 1: temp += '1'; break;
			case 2: temp += '2'; break;
			case 3: temp += '3'; break;
			case 4: temp += '4'; break;
			case 5: temp += '5'; break;
			case 6: temp += '6'; break;
			case 7: temp += '7'; break;
			case 8: temp += '8'; break;
			case 9: temp += '9'; break;
			}
		} while( n /= 10 );
		std::reverse( temp.begin(), temp.end() );
	}
	return temp;
}

const SgType *getArgType( const SgFunctionDeclaration *fdec, int n ) { // note: fisrt arg is arg 1, not arg 0
	const SgFunctionParameterList *plist = fdec->get_parameterList();
	const SgInitializedNamePtrList &parms = plist->get_args();
	if( parms.size() < n || n <= 0 )
		return 0;
	SgInitializedNamePtrList::const_iterator i = parms.begin();
	std::advance( i, n-1 );
	const SgInitializedName *arg = *i;
	const SgType *argType = arg->get_type();
	return argType;
}

int argCount( const SgFunctionDeclaration *fdec ) {
	const SgFunctionParameterList *plist = fdec->get_parameterList();
	const SgInitializedNamePtrList &parms = plist->get_args();
	return parms.size();
}

bool getMemberInfo( const SgDeclarationStatementPtrList &members, MemberNameInfoList &info ) {
	info.clear();
	for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) { // for each member
		if( const SgVariableDeclaration *vdec = isSgVariableDeclaration( *i ) ) { // there may be several names
			const SgInitializedNamePtrList &vars = vdec->get_variables();
			for( SgInitializedNamePtrList::const_iterator i = vars.begin(); i != vars.end(); ++i ) {
				const std::string id( (*i)->get_name().getString() );
				info.push_back( MemberNameInfo(*i,id,(*i)->get_file_info(),variableName) );
			}
		}
		else if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration( *i ) ) {
			const std::string id( fdec->get_name().getString() );
			info.push_back( MemberNameInfo(*i,id,fdec->get_file_info(),functionName) );
		}
		else if( const SgEnumDeclaration *edec = isSgEnumDeclaration( *i ) ) {
			const std::string id( edec->get_name().getString() );
			info.push_back( MemberNameInfo(*i,id,edec->get_file_info(),enumName) );
			const SgInitializedNamePtrList &enumerators = edec->get_enumerators();
			for( SgInitializedNamePtrList::const_iterator i = enumerators.begin(); i != enumerators.end(); ++i ) {
				const std::string id( (*i)->get_name().getString() );
				info.push_back( MemberNameInfo(*i,id,(*i)->get_file_info(),enumeratorName) );
			}
		}
		else if( const SgTemplateDeclaration *tdec = isSgTemplateDeclaration( *i ) ) {
			const std::string id( tdec->get_name().getString() );
			info.push_back( MemberNameInfo(*i,id,tdec->get_file_info(),templateName) );	
		}
		else if( const SgTypedefDeclaration *tddec = isSgTypedefDeclaration( *i ) ) {
			const std::string id( tddec->get_name().getString() );
			info.push_back( MemberNameInfo(*i,id,tddec->get_file_info(),typedefName) );
		}
		else if( const SgUsingDeclarationStatement *udecl = isSgUsingDeclarationStatement( *i ) ) {
			if( const SgInitializedName *in = udecl->get_initializedName() ) {
				const std::string id( in->get_name().getString() );
				info.push_back( MemberNameInfo(*i,id, in->get_file_info(),usingdeclarationName) );
			}
		}
	}
	return !info.empty();
}

bool isSingleArgFunctionDeclaration( const SgNode *node ) {
	// Note: result is single arg function or function that has defaults for trailing arguments
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) {
		const SgFunctionParameterList *plist = fdec->get_parameterList();
		const SgInitializedNamePtrList &parms = plist->get_args();
		switch( parms.size() ) {
		case 0: // no args
			return false;
		case 1:
			return true;
		default: // check to see if 2nd arg on have default inits
			SgInitializedNamePtrList::const_iterator i = parms.begin();
			return (*++i)->get_initptr() != 0; // need to check just 2nd arg...
		}
	}
	return false;
}

bool isZeroArgFunctionDeclaration( const SgNode *node ) {
	// Note: result is single arg function or function that has defaults for trailing arguments
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) {
		const SgFunctionParameterList *plist = fdec->get_parameterList();
		const SgInitializedNamePtrList &parms = plist->get_args();
		switch( parms.size() ) {
		case 0: // no args
			return true;
		default: // check to see if 2nd arg on have default inits
			SgInitializedNamePtrList::const_iterator i = parms.begin();
			return (*i)->get_initptr() != 0; // need to check just 1st arg...
		}
	}
	return false;
}

bool isSingleArgMemberTemplateFunctionDeclaration( const SgNode *node ) {
	//XXX right now, templates are stored internally as a string.  I just don't want to go there...
	return false;
}

bool isSingleArgCtorDeclaration( const SgNode *node ) {
	if( isSingleArgFunctionDeclaration( node ) ) {
		if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) // could static_cast here, but...
			return fdec->get_specialFunctionModifier().isConstructor();
	}
	else if( isSingleArgMemberTemplateFunctionDeclaration( node ) ) {
		//XXX see above re template implementation
	}
	return false;
}

bool isDefaultCtorDeclaration( const SgNode *node ) {
	if( isZeroArgFunctionDeclaration( node ) ) {
		if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) // could static_cast here, but...
			return fdec->get_specialFunctionModifier().isConstructor();
	}
	return false;
}

inline bool isSameClassDeclaration( const SgClassDeclaration *a, const SgClassDeclaration *b )
	{ return a == b; } // pull out test, in case it's wrong!

bool isReferenceToThisClass( const SgClassType *thisClassType, const SgType *argType ) {
	// see if it's a reference or reference to const to this class type
	Type t( argType );
	if( t.isReference() ) {
		t = t.dereference();
		if( const SgClassDeclaration *argClassDeclaration = t.getClassDeclaration() ) {
			const SgClassDeclaration *thisClassDeclaration = Type(thisClassType).getClassDeclaration();
			return isSameClassDeclaration( argClassDeclaration, thisClassDeclaration );
		}
	}
	return false;
}

bool isCopyCtorDeclaration( const SgNode *node ) {
	if( isSingleArgCtorDeclaration( node ) ) {
		if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) {
			const SgFunctionParameterList *plist = fdec->get_parameterList();
			const SgInitializedNamePtrList &parms = plist->get_args();
			if( const SgMemberFunctionDeclaration *mfdec = isSgMemberFunctionDeclaration( fdec ) ) {
				// get the type of the class to which ctor belongs
				const SgClassDefinition *cdef = mfdec->get_class_scope();
				const SgClassDeclaration *cdecl = cdef->get_declaration();
				const SgClassType *ctype = cdecl->get_type();

				// get the type of the first argument to the ctor
				const SgInitializedName *arg1 = parms.front();
				const SgType *arg1type = arg1->get_type();

				return isReferenceToThisClass( ctype, arg1type );
			}
		}
	}
	return false;
}

bool isCopyAssignmentDeclaration( const SgNode *node ) {
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) {
		const std::string n = fdec->get_name().getString();
		if( n == "operator=" ) {
			if( const SgMemberFunctionDeclaration *mfdec = isSgMemberFunctionDeclaration( fdec ) ) {
				// get the type of the class to which function belongs
				const SgClassDefinition *cdef = mfdec->get_class_scope();
				const SgClassDeclaration *cdecl = cdef->get_declaration();
				const SgClassType *ctype = cdecl->get_type();
	
				// get the type of the first argument to the ctor
				const SgType *arg1type = getArgType( fdec, 1 );
				return isReferenceToThisClass( ctype, arg1type );
			}
		}
	}
	return false;
}

bool isTemplateCopyLikeCtorDeclaration( const SgNode *node ) {
	return false; //XXXXXXXXXXXXXXXXXXXXXx
}

bool isMemberDeclaration( const SgNode *node ) {
	if( node )
		do {
			if( isSgClassDefinition( node ) )
				return true;
		} while( node = node->get_parent() );
	return false;
}

bool isLocalDeclaration( const SgNode *node ) {
	if( node )
		do {
			if( isSgFunctionDefinition( node ) )
				return true;
		} while( node = node->get_parent() );
	return false;
}

bool isMemberStatement( const SgNode *node ) { //XXXXXXXXXXXXXX This doesn't work yet...
	if( const SgStatement *stat = isSgStatement( node ) ) {
		if( const SgScopeStatement *scope = stat->get_scope() ) {
			std::cout << "\tSCOPE:  " << scope->get_qualified_name().getString();
			if( const SgSymbolTable *tab = scope->get_symbol_table() ) {
			//	std::cout << "\ttable name: " << tab->get_name().getString();
			}
		}
	}
	return false;
}

const SgExpression *removeImplicitIntegralPromotions( const SgExpression *e ) {
	Type t( e->get_type() );
	if( t.isInt() || t.isUnsignedInt() ) {
		if( const SgCastExp *cast = isSgCastExp( e ) ) {
			if( isCompilerGeneratedNode( cast ) ) { // implicit promotions seem to be implemented as casts
				e = cast->get_operand();
			}
		}
	}
	return e;
}

const SgExpression *removeImplicitIntegralOrFloatingPromotions( const SgExpression *e ) {
	Type t( e->get_type() );
	if( t.isInt() || t.isUnsignedInt() || t.isFloat() || t.isDouble() ) {
		if( const SgCastExp *cast = isSgCastExp( e ) ) {
			if( isCompilerGeneratedNode( cast ) ) {
				e = cast->get_operand();
			}
		}
	}
	return e;
}

void getEnumeratorValues( const SgEnumDeclaration *edecl, std::vector<int> &values ) {
	values.clear();
	const SgInitializedNamePtrList &enums = edecl->get_enumerators();
	int enumeratorValue = -1; // so first value will default to 0
	for( SgInitializedNamePtrList::const_iterator i = enums.begin(); i != enums.end(); ++i ) {
		const SgInitializer *init = (*i)->get_initializer();
		if( !init )
			++enumeratorValue;
		else {
			const SgAssignInitializer *ainit = isSgAssignInitializer( init );
			const SgExpression *init_i = ainit->get_operand_i();
			const SgIntVal *intval = isSgIntVal( init_i );
			enumeratorValue = intval->get_value();
		}
		values.push_back( enumeratorValue );
	}
}

// Note that return value indicates whether there was a default case.
bool getCaseValues( const SgBasicBlock *body, std::vector<int> &values ) {
	bool sawDefault = false;
	values.clear();
	const SgStatementPtrList &stats = body->get_statements();
	for( SgStatementPtrList::const_iterator i = stats.begin(); i != stats.end(); ++i ) {
		if( const SgCaseOptionStmt *caseopt = isSgCaseOptionStmt( *i ) ) {
			const SgExpression *key = caseopt->get_key();
			const SgIntVal *keyval = isSgIntVal( key );
			values.push_back( keyval->get_value() );
		}
		else if( const SgDefaultOptionStmt *defaultopt = isSgDefaultOptionStmt( *i ) ) {
			sawDefault = true;
		}
	}
	return sawDefault;
}

bool isEqRelop( const SgNode *e ) {
	return
		isSgEqualityOp( e ) ||
		isSgGreaterOrEqualOp( e ) ||
		isSgGreaterThanOp( e ) ||
		isSgLessOrEqualOp( e ) ||
		isSgLessThanOp( e ) ||
		isSgNotEqualOp( e );
}

bool expressionIsPointerToIncompleteClass( const SgExpression *expr ) {
	//std::cout << "isExpressionToIncompleteClass: line" << expr->get_file_info()->get_line() << std::endl; //XXX
	Type et( expr->get_type() );
	if( et.isReference() ) { // a reference to pointer to incomplete counts, too
		//std::cout << "it's a reference" << std::endl; //XXX
		et = et.dereference();
	}
	if( et.isPointer() ) {
		//std::cout << "it's a pointer" << std::endl; //XXX
		et = et.dereference();
		//const SgClassDeclaration *cdecl = et.getClassDeclaration(); //XXX
		const SgClassDefinition *cdef = et.getClassDefinition();
		//bool forward = cdecl->isForward(); //XXX apparently, this doesn't mean incomplete class decl
		//const SgDeclarationStatement *fnd = cdecl->get_firstNondefiningDeclaration();
		////const SgDeclarationStatement *dd = cdecl->get_definingDeclaration();
		//std::cout << "declaration: " << cdecl->get_file_info()->get_line() << std::endl;
		//std::cout << "first: " << fnd->get_file_info()->get_line() << std::endl;
		//std::cout << "defining: " << dd->get_file_info()->get_line() << std::endl;
		//if( cdef )
		//	std::cout << "there's a definition" << std::endl;
		//if( cdecl->isForward() )
		//	std::cout << "forward" << std::endl;
		return cdef == 0;
	}
	return false;
}

class IsSameClassDefinitionAs {
  public:
	IsSameClassDefinitionAs( const SgClassDefinition *b )
		: b_(b) {}
	bool operator ()( const SgNode *a ) const {
		if( isSgClassDefinition( a ) )
			return a == b_;
		else
			return false;
	}
  private:
	const SgClassDefinition *b_;
};

bool isBaseOf( const SgClassDefinition *cdef, const SgClassDefinition *potentialBase ) {
	IsSameClassDefinitionAs op( potentialBase );
	return forEachBaseClassDepthFirstShortCircuit( cdef, op );
}

const char *const nameOfStandardNamespaceScope = "::std";
const char *const nameOfStdExceptionClass = "exception";

bool isClassDeclaredInStdNamespace( const SgClassDeclaration *decl ) {
	if( const SgScopeStatement *scope = decl->get_scope() ) {
		const std::string scopeName( scope->get_qualified_name().getString() );
		return scopeName == nameOfStandardNamespaceScope; // this check seems fragile...
	}
	return false;
}

bool isStdExceptionOrTypeDerivedFromStdException( const SgClassDeclaration *cdecl ) {
	const std::string className( cdecl->get_name().getString() );
	if( className == nameOfStdExceptionClass ) {
		if( isClassDeclaredInStdNamespace( cdecl ) )
			return true;
	}

	if( const SgClassDefinition *cdef = cdecl->get_definition() ) {
		const SgBaseClassPtrList &bases = cdef->get_inheritances();
		for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) {
			if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
				if( isStdExceptionOrTypeDerivedFromStdException( baseDecl ) )
					return true;
			}
		}
	}
	return false;
}

const SgFunctionSymbol *isCallOfFunctionNamed( const SgNode *node, const std::string &name ) { 
	if( const SgFunctionRefExp *f = isSgFunctionRefExp(node) ) {
		ROSE_ASSERT( f ); //XXX
		const SgFunctionSymbol *sym = f->get_symbol();
		if( sym->get_name().getString() == name ) //XXX what about qualified names?
			return sym;
	}
	return 0;
}

const SgFunctionSymbol *isCallOfFunctionWithMangledName( const SgNode *node, const std::string &mn ) {
	if( const SgFunctionRefExp *f = isSgFunctionRefExp(node) ) {
		ROSE_ASSERT( f ); //XXX
		const std::string fmn = f->get_symbol()->get_declaration()->get_mangled_name().getString();
		if( std::search( fmn.begin(), fmn.end(), mn.begin(), mn.end() ) != fmn.end() )
			return f->get_symbol();
	}
	return 0;
}

std::pair<const SgNode *,size_t> findParentNodeOfType( const SgNode *start, int nodeType ) { //XXX and do a version with a vector of nodeTypes, or just overload
	size_t depth = 0;
	const SgNode *parent = start->get_parent();
	for( ; parent; parent = parent->get_parent() ) {
		++depth;
		if( parent->variantT() == nodeType )
			break;
	}
	return std::make_pair(parent,depth);
}

bool isConstructorDeclaration( const SgNode* node ) {
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) )
		return fdec->get_specialFunctionModifier().isConstructor();
	return false;
}

bool isConstructorDefinition( const SgNode* node ) {
	if( const SgFunctionDefinition *fdef = isSgFunctionDefinition(node) ) {
		const SgFunctionDeclaration *fdecl = fdef->get_declaration();
		return fdecl->get_specialFunctionModifier().isConstructor();
	}
	return false;
}

bool isDestructorDeclaration( const SgNode* node ) {
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) )
		return fdec->get_specialFunctionModifier().isDestructor();
	return false;
}

bool isDestructorDefinition( const SgNode* node ) {
	if( const SgFunctionDefinition *fdef = isSgFunctionDefinition(node) ) {
		const SgFunctionDeclaration *fdecl = fdef->get_declaration();
		return fdecl->get_specialFunctionModifier().isDestructor();
	}
	return false;
}

bool isCallOfVirtualFunction( const SgNode *n ) {
	if( const SgMemberFunctionRefExp *call = isSgMemberFunctionRefExp(n) ) {
		const SgMemberFunctionDeclaration *decl = call->get_symbol()->get_declaration();
		const SgFunctionModifier modifier = decl->get_functionModifier();
		return modifier.isVirtual();
	}
	return false;
}

const SgNode *getForStatmentTest( const SgNode *node ) {
	if( const SgForStatement *forStatement = isSgForStatement(node) )
		return forStatement->get_test();
	else
		return 0;
}

size_t countClassDefinitionDataProtection( const SgNode *n, ProtectionLevel p ) {
	int pub = 0, prot = 0, priv = 0;
	if( const SgClassDefinition *classdef = isSgClassDefinition(n) ) {
		const SgDeclarationStatementPtrList &members = classdef->get_members();
		for( SgDeclarationStatementPtrList::const_iterator member = members.begin(); member != members.end(); ++member ) {
			if( const SgVariableDeclaration *vardecl = isSgVariableDeclaration(*member) ) {
				const SgAccessModifier &mod = const_cast<SgVariableDeclaration *>(vardecl)->get_declarationModifier().get_accessModifier();
				if( mod.isPublic() )
					++pub;
				else if( mod.isProtected() )
					++prot;
				else if( mod.isPrivate() )
					++priv;
			}
		}
	}
	switch( p ) {
	case PUBLIC: return pub;
	case PROTECTED: return prot;
	case PRIVATE: return priv;
	}
	return 0; // This should not happen, but it does circumvent a warning.
}

bool isCompilerGeneratedNode( const SgNode *node ) {
	if( const Sg_File_Info *info = node->get_file_info() )
		if( info->isCompilerGenerated() )
			return true;
	return false;
}

//
// Class member analysis.
// This is just a big collection of data about a class, including information about its
// own members as well as information inherited from base classes.  It is used to compose
// more specific questions about a class, and is typically not used directly.
//

class ClassMemberAnalysis;
const ClassMemberAnalysis &classMemberAnalysis( const SgClassDefinition *cdef );

//
//	Issue with all these "compiler-generated" queries is that the functions do not
//	seem to appear in the class definition.  Therefore we have to determine whether
//	they will be generated by other means (like applying the language rules).
//

class ClassMemberAnalysis {
  public:
	ClassMemberAnalysis();
	size_t countCtors() const
		{ return ctors_; }
	bool hasDefaultCtor() const
		{ return defaultCtors_ > 0; }
	size_t countDefaultCtors() const
		{ return defaultCtors_; }
	bool hasImplicitDefaultCtor() const
		{ return ctors_ == 0; }
	bool hasTrivialCtor() const {
		return hasImplicitDefaultCtor() && virtualMemberFunctions_ == 0
			&& !hasVirtualBase_ && !memberHasNonTrivialCtor_;
	}
	bool hasCopyCtor() const
		{ return copyCtors_ > 0; }
	bool countCopyCtors() const
		{ return copyCtors_; }
	bool hasImplicitCopyCtor() const
		{ return copyCtors_ == 0; }
	bool hasTrivialCopyCtor() const {
		return hasImplicitCopyCtor() && virtualMemberFunctions_ == 0 && !hasVirtualBase_
			&& !memberHasNonTrivialCopyCtor_ && !baseHasNonTrivialCopyCtor_;
	}
	bool hasCopyAssign() const
		{ return copyAssigns_ > 0; }
	bool countCopyAssigns() const
		{ return copyAssigns_; }
	bool hasImplicitCopyAssign() const
		{ return !hasCopyAssign(); }
	bool hasTrivialCopyAssign() const {
		return hasImplicitCopyAssign() && virtualMemberFunctions_ == 0 && !hasVirtualBase_
			&& !memberHasNonTrivialCopyAssign_ && !baseHasNonTrivialCopyAssign_;
	}
	bool hasDtor() const
		{ return dtor_; }
	bool hasImplicitDtor() const
		{ return !dtor_; }
	bool hasTrivialDtor() const
		{ return hasImplicitDtor() && !memberHasNonTrivialDtor_ && !baseHasNonTrivialDtor_; }
	bool hasVirtualFunction() const
		{ return virtualMemberFunctions_ > 0; }
	size_t countVirtualFunctions() const
		{ return virtualMemberFunctions_; }
	bool inheritsVirtualFunction() const
		{ return inheritsVirtualFunction_; }
	bool hasVirtualDtor() const
		{ return hasVirtualDtor_; }
	bool inheritsVirtualDtor() const
		{ return inheritsVirtualDtor_; }
	bool isPODClass() const { // section 9 intro of standard
		//XXX still have to check for arrays of non-POD!!!
		return isAggregate() && !hasReferenceMember_ && !hasPCMMember_
			&& !hasNonPODMember_ && !hasCopyAssign() && !hasDtor();
	}
	bool isAggregate() const { // Section 8.5.1.
		return ctors_ == 0 && privateNonStaticDataMembers_ == 0 && protectedNonStaticDataMembers_ == 0
			&& immediateBaseClasses_ == 0 && virtualMemberFunctions_ == 0;
	}
	bool hasPointerMember() const
		{ return hasPointerMember_; }
	bool hasMemberNew() const
		{ return operatorNews_ > 0; }
	size_t countMemberNews() const
		{ return operatorNews_; }
	bool hasMemberDelete() const
		{ return operatorDeletes_ > 0; }
	size_t countMemberDeletes() const
		{ return operatorDeletes_; }
	bool hasMemberArrayNew() const
		{ return operatorArrayNews_ > 0; }
	size_t countMemberArrayNews() const
		{ return operatorArrayNews_; }
	bool hasMemberArrayDelete() const
		{ return operatorArrayDeletes_ > 0; }
	size_t countMemberArrayDeletes() const
		{ return operatorArrayDeletes_; }
	bool inheritsMemberNew() const
		{ return inheritedOperatorNews_ > 0; }
	bool inheritsMemberDelete() const
		{ return inheritedOperatorDeletes_ > 0; }
	bool inheritsMemberArrayNew() const
		{ return inheritedOperatorArrayNews_ > 0; }
	bool inheritsMemberArrayDelete() const
		{ return inheritedOperatorArrayDeletes_ > 0; }
	bool hasUsualOperatorNew() const
		{ return hasUsualOperatorNew_; }
	bool hasUsualOperatorDelete() const
		{ return has1ArgUsualOperatorDelete_ || has2ArgUsualOperatorDelete_; }
	bool hasUsualArrayNew() const
		{ return hasUsualArrayNew_; }
	bool hasUsualArrayDelete() const
		{ return hasUsualArrayDelete_; }
	void operator ()( const SgNode *node );
  private:
	friend class SuperClassMemberAnalysis;
	size_t ctors_;
	size_t defaultCtors_;
	size_t copyCtors_;
	size_t copyAssigns_;
	size_t immediateBaseClasses_;
	size_t publicNonStaticDataMembers_;
	size_t protectedNonStaticDataMembers_;
	size_t privateNonStaticDataMembers_;
	size_t publicStaticDataMembers_;
	size_t protectedStaticDataMembers_;
	size_t privateStaticDataMembers_;
	size_t virtualMemberFunctions_;

	size_t operatorNews_;
	size_t operatorDeletes_;
	size_t operatorArrayNews_;
	size_t operatorArrayDeletes_;
	size_t inheritedOperatorNews_; // "inherited" counters count hidden operators new, delete, etc. too
	size_t inheritedOperatorDeletes_;
	size_t inheritedOperatorArrayNews_;
	size_t inheritedOperatorArrayDeletes_;

	bool dtor_;
	bool inheritsVirtualFunction_;
	bool hasVirtualDtor_;
	bool inheritsVirtualDtor_;
	bool memberHasNonTrivialCtor_;
	bool memberHasNonTrivialCopyCtor_;
	bool memberHasNonTrivialCopyAssign_;
	bool memberHasNonTrivialDtor_;
	bool hasVirtualBase_;
	bool baseHasNonTrivialCtor_;
	bool baseHasNonTrivialCopyCtor_;
	bool baseHasNonTrivialCopyAssign_;
	bool baseHasNonTrivialDtor_;
	bool hasPointerMember_;
	bool hasReferenceMember_;
	bool hasPCMMember_;
	bool hasNonPODMember_;
	bool hasNonStaticArrayMember_;

	bool hasUsualOperatorNew_;
	bool has1ArgUsualOperatorDelete_; // Note: could have both forms; in that case, 1-arg is usual
	bool has2ArgUsualOperatorDelete_;
	bool hasUsualArrayNew_;
	bool hasUsualArrayDelete_;

	bool inheritsUsualOperatorNew_;
	bool inherits1ArgUsualOperatorDelete_; // Note: could have both forms; in that case, 1-arg is usual
	bool inherits2ArgUsualOperatorDelete_;
	bool inheritsUsualArrayNew_;
	bool inheritsUsualArrayDelete_;
};

ClassMemberAnalysis::ClassMemberAnalysis()
	: ctors_(0),
	defaultCtors_(0),
	copyCtors_(0),
	copyAssigns_(0),
	immediateBaseClasses_(0),
	publicNonStaticDataMembers_(0),
	protectedNonStaticDataMembers_(0),
	privateNonStaticDataMembers_(0),
	publicStaticDataMembers_(0),
	protectedStaticDataMembers_(0),
	privateStaticDataMembers_(0),
	virtualMemberFunctions_(0),
	operatorNews_(0),
	operatorDeletes_(0),
	operatorArrayNews_(0),
	operatorArrayDeletes_(0),
	inheritedOperatorNews_(0),
	inheritedOperatorDeletes_(0),
	inheritedOperatorArrayNews_(0),
	inheritedOperatorArrayDeletes_(0),
	dtor_(false),
	inheritsVirtualFunction_(false),
	hasVirtualDtor_(false),
	inheritsVirtualDtor_(false),
	memberHasNonTrivialCtor_(false),
	memberHasNonTrivialCopyCtor_(false),
	memberHasNonTrivialCopyAssign_(false),
	memberHasNonTrivialDtor_(false),
	hasVirtualBase_(false),
	baseHasNonTrivialCtor_(false),
	baseHasNonTrivialCopyCtor_(false),
	baseHasNonTrivialCopyAssign_(false),
	baseHasNonTrivialDtor_(false),
	hasPointerMember_(false),
	hasReferenceMember_(false),
	hasPCMMember_(false),
	hasNonPODMember_(false),
	hasNonStaticArrayMember_(false),
	hasUsualOperatorNew_(false),
	has1ArgUsualOperatorDelete_(false),
	has2ArgUsualOperatorDelete_(false),
	hasUsualArrayNew_(false),
	hasUsualArrayDelete_(false),
	inheritsUsualOperatorNew_(false),
	inherits1ArgUsualOperatorDelete_(false),
	inherits2ArgUsualOperatorDelete_(false),
	inheritsUsualArrayNew_(false),
	inheritsUsualArrayDelete_(false)
	{}

void ClassMemberAnalysis::operator ()( const SgNode *node ) {
	if( const SgMemberFunctionDeclaration *mf = isSgMemberFunctionDeclaration( node ) ) {
		if( isConstructorDeclaration( mf ) ) {
			++ctors_;
			if( isDefaultCtorDeclaration( mf ) )
				++defaultCtors_;
			else if( isCopyCtorDeclaration( mf ) )
				++copyCtors_;
		}
		else if( isDestructorDeclaration( mf ) ) {
			dtor_ = true;
			if( isVirtualFunctionDeclaration( mf ) ) {
				++virtualMemberFunctions_;
				hasVirtualDtor_ = true;
			}
		}
		else if( isCopyAssignmentDeclaration( mf ) ) {
			++copyAssigns_;
			if( isVirtualFunctionDeclaration( mf ) ) // Yep.  You can write a virtual copy assign.  (But don't.)
				++virtualMemberFunctions_;
		}
		else if( isVirtualFunctionDeclaration( mf ) ) {
			++virtualMemberFunctions_;
		}
		else {
			const std::string n = mf->get_name().getString();
			if( n == "operator new" ) {
				++operatorNews_;
				if( argCount( mf ) == 1 )
					hasUsualOperatorNew_ = true; // note: first arg must be size_t
			}
			else if( n == "operator delete" ) {
				++operatorDeletes_;
				int args = argCount( mf );
				if( args == 1 )
					has1ArgUsualOperatorDelete_ = true; // note: first arg must be void *
				else if( args == 2 && Type(getArgType( mf, 2 )).isSize_t() )
					has2ArgUsualOperatorDelete_ = true;
			}
			else if( n == "operator new[]" ) {
				++operatorArrayNews_;
				if( argCount( mf ) == 1 )
					hasUsualArrayNew_ = true; // note: first arg must be size_t
			}
			else if( n == "operator delete[]" ) {
				++operatorArrayDeletes_;
				if( argCount( mf ) == 1 )
					hasUsualArrayDelete_ = true; // note: first arg must be void *
			}
		}
	}
	else if( const SgVariableDeclaration *v = isSgVariableDeclaration( node ) ) { // check non-static data members
		const SgDeclarationModifier &dmod = v->get_declarationModifier();
		const SgStorageModifier &smod = const_cast<SgDeclarationModifier &>(dmod).get_storageModifier();
		if( smod.isDefault() || smod.isUnspecified() || smod.isMutable() ) { // non-static data member
			const SgInitializedNamePtrList &vars = v->get_variables();
			for( SgInitializedNamePtrList::const_iterator i = vars.begin(); i != vars.end(); ++i ) {
				Type t( (*i)->get_type() );
				if( const SgClassDefinition *cdef = t.getClassDefinition() ) { // member has class type
					const ClassMemberAnalysis &mop = classMemberAnalysis( cdef ); // recurse here
					memberHasNonTrivialCtor_ |= !mop.hasTrivialCtor(); // merge from member
					memberHasNonTrivialCopyCtor_ |= !mop.hasTrivialCopyCtor();
					memberHasNonTrivialCopyAssign_ |= !mop.hasTrivialCopyAssign();
					memberHasNonTrivialDtor_ |= !mop.hasTrivialDtor();
					hasNonPODMember_ |= !mop.isPODClass();
				}
				else if( t.isPointer() ) {
					hasPointerMember_ = true;
				}
				else if( t.isReference() ) {
					hasReferenceMember_ = true;
				}
				else if( t.isMemberPointer() ) {
					hasPCMMember_ = true;
				}
				else if( t.isArray() ) {
					hasNonStaticArrayMember_ = true;
				}

				const SgAccessModifier &amod = const_cast<SgDeclarationModifier &>(dmod).get_accessModifier();
				if( amod.isPublic() )
					++publicNonStaticDataMembers_;
				else if( amod.isProtected() )
					++protectedNonStaticDataMembers_;
				else if( amod.isPrivate() )
					++privateNonStaticDataMembers_;
			}
		}
		else if( smod.isStatic() ) { // static data members
			const SgAccessModifier &amod = const_cast<SgDeclarationModifier &>(dmod).get_accessModifier();
			if( amod.isPublic() )
				++publicStaticDataMembers_;
			else if( amod.isProtected() )
				++protectedStaticDataMembers_;
			else if( amod.isPrivate() )
				++privateStaticDataMembers_;
		}
	}
}

class SuperClassMemberAnalysis {
  public:
	SuperClassMemberAnalysis( ClassMemberAnalysis &dop )
		: dop_(dop) {}
	void operator ()( const SgNode *node );
  private:
	SuperClassMemberAnalysis( const SuperClassMemberAnalysis & );
	SuperClassMemberAnalysis &operator =( const SuperClassMemberAnalysis & );
	ClassMemberAnalysis &dop_;
};

void SuperClassMemberAnalysis::operator ()( const SgNode *node ) {
	if( const SgClassDefinition *base = isSgClassDefinition( node ) ) {
		const ClassMemberAnalysis &bop = classMemberAnalysis( base ); // recurse here
		dop_.baseHasNonTrivialCtor_ |= !bop.hasTrivialCtor(); // merge from base
		dop_.baseHasNonTrivialCopyCtor_ |= !bop.hasTrivialCopyCtor();
		dop_.baseHasNonTrivialCopyAssign_ |= !bop.hasTrivialCopyAssign();
		dop_.baseHasNonTrivialDtor_ |= !bop.hasTrivialDtor();
		dop_.hasVirtualBase_ |= bop.hasVirtualBase_;
		dop_.inheritsVirtualFunction_ |= bop.hasVirtualFunction() || bop.inheritsVirtualFunction();
		dop_.inheritsVirtualDtor_ |= bop.hasVirtualDtor() || bop.inheritsVirtualDtor();
		dop_.inheritedOperatorNews_ = bop.operatorNews_ + bop.inheritedOperatorNews_;
		dop_.inheritedOperatorDeletes_ = bop.operatorDeletes_ + bop.inheritedOperatorDeletes_;
		dop_.inheritedOperatorArrayNews_ = bop.operatorArrayNews_ + bop.inheritedOperatorArrayNews_;
		dop_.inheritedOperatorArrayDeletes_ = bop.operatorArrayDeletes_ + bop.inheritedOperatorArrayDeletes_;
		dop_.inheritsUsualOperatorNew_ |= bop.hasUsualOperatorNew_ || bop.inheritsUsualOperatorNew_;
		dop_.inherits1ArgUsualOperatorDelete_ |= bop.has1ArgUsualOperatorDelete_ || bop.inherits1ArgUsualOperatorDelete_;
		dop_.inherits2ArgUsualOperatorDelete_ |= bop.has2ArgUsualOperatorDelete_ || bop.inherits2ArgUsualOperatorDelete_;
		dop_.inheritsUsualArrayNew_ |= bop.hasUsualArrayNew_ || bop.inheritsUsualArrayNew_;
		dop_.inheritsUsualArrayDelete_ |= bop.hasUsualArrayDelete_ || bop.inheritsUsualArrayDelete_;
	}
	else if( const SgBaseClassModifier *mod = isSgBaseClassModifier( node ) ) {
		dop_.hasVirtualBase_ |= mod->isVirtual();
		++dop_.immediateBaseClasses_;
	}
}

const ClassMemberAnalysis &classMemberAnalysis( const SgClassDefinition *cdef ) {
	typedef std::map<const SgClassDefinition *, ClassMemberAnalysis> Map; // store analysis by value; avoid heap
	typedef Map::iterator I;
	static Map doneOnes; // static is used to cache results to avoid redundant analysis of same class
	I i = doneOnes.find( cdef );
	if( i != doneOnes.end() )
		return i->second;
	else {
		ClassMemberAnalysis op;
		forEachClassMember( cdef, op );
		SuperClassMemberAnalysis bop( op );
		forEachImmediateBaseClass( cdef, bop );
		std::pair<I,bool> result = doneOnes.insert( std::make_pair( cdef, op ) );
		ROSE_ASSERT( result.second ); // insertion should not fail, due to check above
		return result.first->second;
	}
}

//
// The functions below are thin layers over the class member analysis mechanism
// that give a more "user-friendly" interface.
//

bool hasExplicitCopyCtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasCopyCtor();
}

bool hasExplicitCopyAssignment( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasCopyAssign();
}

bool hasExplicitDtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasDtor();
}

bool hasCompilerGeneratedDefaultCtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasImplicitDefaultCtor();
}

bool hasTrivialCtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasTrivialCtor();
}

bool hasTrivialCopyCtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasTrivialCopyCtor();
}

bool hasTrivialDtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasTrivialDtor();
}

bool hasCompilerGeneratedDtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasImplicitDtor();
}

bool hasCompilerGeneratedCopyCtor( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasImplicitCopyCtor();
}

bool hasCompilerGeneratedCopyAssignment( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasImplicitCopyAssign();
}

bool hasTrivialCopyAssignment( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasTrivialCopyAssign();
}

bool hasVirtualFunction( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasVirtualFunction();
}

bool isPODClass( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.isPODClass();
}

bool hasPointerMember( const SgClassDefinition *cdef ) {
	const ClassMemberAnalysis &op = classMemberAnalysis( cdef );
	return op.hasPointerMember();
}

bool isVirtualFunctionDeclaration( const SgNode *node ) {
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const SgFunctionModifier mod = fd->get_functionModifier();
		return mod.isVirtual();
	}
	return false;
}

bool isPureVirtualFunctionDeclaration( const SgNode *node ) {
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const SgFunctionModifier mod = fd->get_functionModifier();
		return mod.isPureVirtual();
	}
	return false;
}

bool isInlineFunctionDeclaration( const SgNode *node ) {
	if( const SgFunctionDeclaration *fdecl = isSgFunctionDeclaration( node ) ) {
		const SgFunctionModifier mod = fdecl->get_functionModifier();
		return mod.isInline();
	}
	return false;
}

bool isPublicVirtualMemberFunctionDeclaration( const SgNode* node ) {
	if( const SgMemberFunctionDeclaration *mf = isSgMemberFunctionDeclaration( node ) ) {
		const SgFunctionModifier &fm = mf->get_functionModifier();
		const SgAccessModifier &am =
			const_cast<SgMemberFunctionDeclaration *>(mf)->get_declarationModifier().get_accessModifier();
		return fm.isVirtual() && am.isPublic();
	}
	return false;
}

size_t CountLinesInFunction( const SgFunctionDeclaration *funcDecl ) {
	if( const SgFunctionDefinition *funcDef = funcDecl->get_definition() ) {
		const Sg_File_Info *start = funcDef->get_body()->get_startOfConstruct();
		const Sg_File_Info *end = funcDef->get_body()->get_endOfConstruct();
		int lineS = start->get_line();
		int lineE = end->get_line();
		int loc_actual = lineE-lineS;
		return loc_actual > 0 ? loc_actual : 0;
	}
	else
		return 0;
}

bool isCppFile( const SgNode *node ) {
	if( const SgFile *file = isSgFile( node ) )
		if( !file->get_C_only() && !file->get_C99_only() )
			return true;
	return false;
}

bool switchHasDefault( const SgSwitchStatement *theSwitch ) {
	if( const SgBasicBlock *block = theSwitch->get_body() ) {
		const SgStatementPtrList &stmts = block->get_statements();
		for( SgStatementPtrList::const_iterator i = stmts.begin(); i != stmts.end(); ++i )
			if( isSgDefaultOptionStmt( *i ) )
				return true;
	}
	return false;
}

bool isOperatorDeclaration( const SgNode *node ) {
	const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node );
	return fd && fd->get_specialFunctionModifier().isOperator();
}

bool isOperatorDefinition( const SgNode *node ) {
	const SgFunctionDefinition *fd = isSgFunctionDefinition( node );
	return fd && fd->get_declaration()->get_specialFunctionModifier().isOperator();
}

bool isMemberOperatorDefinition( const SgNode *node ) {
	const SgFunctionDefinition *fd = isSgFunctionDefinition( node );
	return isSgMemberFunctionDeclaration(fd->get_declaration()) && isOperatorDefinition( node );
}

bool isConversionOperatorDeclaration( const SgNode *node ) {
	const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node );
	return fd && fd->get_specialFunctionModifier().isConversion();
}

bool isMemberFunctionReturningPtrOrRef( const SgFunctionDefinition *fd ) {
	if( isSgMemberFunctionDeclaration( fd->get_declaration() ) ) {
		const SgFunctionType *ft = fd->get_declaration()->get_type();
		Type rt( ft->get_return_type() );
		if( rt.isPointer() || rt.isReference() )
			if( !rt.isConst() )
				return true;
	}
	return false;
}

// should pass two more arguments to this function: 1) a flag to tell it whether or not to stop recursion on a path where
// the name is found, and 2) a predicate to be applied to the member declaration before adding to the result list
// also, a more general form of the function that doesn't take a name, just a predicate.
//XXX actually, this function needs to be redesigned or removed...
bool findInheritedMembersNamed( const SgClassDefinition *cdef, const std::string &id, SgDeclarationStatementPtrList &dlist ) {
	dlist.clear(); // empty it, just to make sure...
	const SgBaseClassPtrList &bases = cdef->get_inheritances();
	// Recursively look up the class lattice for members with the same identifier.
	for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) { // for each base class...
		const bool baseIsVirtual = (*i)->get_baseClassModifier().isVirtual();
		if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
			if( const SgClassDefinition *baseDef = baseDecl->get_definition() ) { // get the base class definition...
				SgDeclarationStatementPtrList temp;
				const SgDeclarationStatementPtrList &baseMembers = baseDef->get_members();
				for( SgDeclarationStatementPtrList::const_iterator i = baseMembers.begin(); i != baseMembers.end(); ++i ) { // for each member declaration of the base class...
					//std::cout << (*i)->get_mangled_name().getString() << std::endl;
					if( const SgVariableDeclaration *vdec = isSgVariableDeclaration( *i ) ) {
						const SgInitializedNamePtrList &vars = vdec->get_variables();
						for( SgInitializedNamePtrList::const_iterator j = vars.begin(); j != vars.end(); ++j ) {
							const std::string did = (*j)->get_name().getString();
							if( did == id )  {//XXX check predicate here...
								std::cout << id << " is same Identifier as variable" << std::endl;
								temp.push_back( *i );
							}
							else {
								std::cout << id << " is different from data member id " << did << std::endl;
							}
						}
					}
					else if( SgFunctionDeclaration *fdec = isSgFunctionDeclaration( *i ) ) {
						const std::string fid = fdec->get_name().getString();
						if( fid == id ) {
							std::cout << id << " is same id as function: " << std::endl;
							temp.push_back( fdec );
						}
						else {
							std::cout << id << " is different from function id " << fid << std::endl;
						}
					}
					else {
						std::cout << "Member is not a variable or function declaration in base." << std::endl;
					}
				}
				//XXX if member found, don't recurse (note must recurse if checking overriding instead of hiding)
				if( temp.empty() ) { //XXX use recursion flag here...
					if( findInheritedMembersNamed( baseDef, id, temp ) )
					  dlist.insert( dlist.end(), temp.begin(), temp.end() );
				}
			}
		}
	}
	return !dlist.empty();
}

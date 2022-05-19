/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Third-Party Software subject to its own license.// 
// DM21-0505</legal>// 
 */

/*
 * \file utilities_cpp.h
 */

#ifndef UTILITIES_CPP_H
#define UTILITIES_CPP_H

#include "rose.h"
#include "type.h"

#include <list>
#include <string>
#include <iosfwd>

std::string utostring( unsigned int );

//
// Questions about class definitions.
//
bool hasExplicitCopyCtor( const SgClassDefinition *cdef ); // Here, "explicit" means user-defined rather than compiler generated.
bool hasExplicitCopyAssignment( const SgClassDefinition *cdef );
bool hasExplicitDtor( const SgClassDefinition *cdef );
bool hasCompilerGeneratedDefaultCtor( const SgClassDefinition *cdef );
bool hasTrivialCtor( const SgClassDefinition *cdef );
bool hasTrivialCopyCtor( const SgClassDefinition *cdef );
bool hasTrivialDtor( const SgClassDefinition *cdef );
bool hasCompilerGeneratedDtor( const SgClassDefinition *cdef );
bool hasCompilerGeneratedCopyCtor( const SgClassDefinition *cdef );
bool hasCompilerGeneratedCopyAssignment( const SgClassDefinition *cdef );
bool hasTrivialCopyAssignment( const SgClassDefinition *cdef );
bool hasVirtualFunction( const SgClassDefinition *cdef );
bool hasPointerMember( const SgClassDefinition *cdef );
bool isPODClass( const SgClassDefinition *classdef );

//
// Questions about class members.
// Note that these don't require a more specific type of argument than pointer to SgNode.
//
bool isConstructorDeclaration( const SgNode* node );
bool isConstructorDefinition( const SgNode* node );
bool isDestructorDeclaration( const SgNode* node );
bool isDestructorDefinition( const SgNode* node );
bool isSingleArgCtorDeclaration( const SgNode *node );
bool isCopyCtorDeclaration( const SgNode *node );
bool isCopyAssignmentDeclaration( const SgNode * );
bool isTemplateCopyLikeCtorDeclaration( const SgNode *node ); // NOT IMPLEMENTED
bool isMemberDeclaration( const SgNode *node );
bool isOperatorDeclaration( const SgNode *node );
bool isOperatorDefinition( const SgNode *node );
bool isMemberOperatorDefinition( const SgNode *node );
bool isConversionOperatorDeclaration( const SgNode *node );

//
// Other questions related to classes.
//
bool isBaseOf( const SgClassDefinition *cdef, const SgClassDefinition *potentialBase );
bool isClassDeclaredInStdNamespace( const SgClassDeclaration *decl );
bool expressionIsPointerToIncompleteClass( const SgExpression *expr );
bool isReferenceToThisClass( const SgClassType *classType, const SgType *type );

enum ProtectionLevel { PUBLIC, PROTECTED, PRIVATE };
size_t countClassDefinitionDataProtection( const SgNode *n, ProtectionLevel p );

//XXX I'm not sure if this is still used; it needs work, in any case.
bool findInheritedMembersNamed( const SgClassDefinition *cdef, const std::string &id, const SgDeclarationStatementPtrList &dlist );

//
// Questions about switch statements
//
bool switchHasDefault( const SgSwitchStatement *theSwitch );
bool isMemberStatement( const SgNode *node ); //XXXXXXXXXXXXXX This doesn't work yet...
bool isEqRelop( const SgNode *e );

bool isStdExceptionOrTypeDerivedFromStdException( const SgClassDeclaration *decl );

//
// Questions having to do with function declarations.
//
const SgType *getArgType( const SgFunctionDeclaration *fdec, unsigned int n ); // note: first arg is arg 1, not arg 0.
int argCount( const SgFunctionDeclaration *fdec );
bool isVirtualFunctionDeclaration( const SgNode *node );
bool isPureVirtualFunctionDeclaration( const SgNode *node );
bool isInlineFunctionDeclaration( const SgNode *node );
bool isPublicVirtualMemberFunctionDeclaration( const SgNode* node );
bool isMemberFunctionReturningPtrOrRef( const SgFunctionDefinition *fd );
bool isSingleArgFunctionDeclaration( const SgNode *node );
bool isSingleArgMemberTemplateFunctionDeclaration( const SgNode *node ); // NOT IMPLEMENTED

//
// Questions about function calls.
//
bool isCallOfVirtualFunction( const SgNode *n );
const SgFunctionSymbol *isCallOfFunctionNamed( const SgNode *node, const std::string &name );
const SgFunctionSymbol *isCallOfFunctionWithMangledName( const SgNode *node, const std::string &mn );

// The two function templates below simply execute the same operations as the two non-template
// functions above, but will accept a sequence of names instead of a single name.
template <typename In>
const SgFunctionSymbol *isCallOfFunctionNamedAnyOf( const SgNode *node, In b, In e ) {
	while( b != e )
		if( SgFunctionSymbol *f = isCallOfFunctionNamed( *b++ ) )
			return f;
	return 0;
}

template <typename In>
const SgFunctionSymbol *isCallOfFunctionMangledNamedAnyOf( const SgNode *node, In b, In e ) {
	while( b != e )
		if( SgFunctionSymbol *f = isCallOfFunctionWithMangledName( *b++ ) )
			return f;
	return 0;
}

//
// Miscellaneous questions and manipulations.
//
bool isCompilerGeneratedNode( const SgNode *node );
bool isLocalDeclaration( const SgNode *node );
const SgExpression *removeImplicitIntegralPromotions( const SgExpression *e );
const SgExpression *removeImplicitIntegralOrFloatingPromotions( const SgExpression *e );
bool isCppFile( const SgNode *node );

//
// Class member traversals
//

// Traverse the class members in order of declaration, invoking a function object (or function) on each
// member declaration.
template <typename Op>
void forEachClassMember( const SgClassDefinition *classdef, Op &op ) { // like std::for_each, but op passed by reference, no return
	const SgDeclarationStatementPtrList &members = classdef->get_members();
	for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i )
		op( *i );
}

// Same as above, but stop (short circuit) the traversal if op returns true.
template <typename Op>
bool forEachClassMemberShortCircuit( const SgClassDefinition *classdef, Op &op ) {
	const SgDeclarationStatementPtrList &members = classdef->get_members();
	for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i )
		if( op( *i ) )
			return true;
	return false;
}

//
// Base class traversals
//
// These base class traversals may look a little funny:
// A base class list consists of both a list of classes and a list of how those classes are inherited (access, virtual).
// These traversals invoke op on both aspects of the base class.  If op is not interested in one or the other, it can just
// ignore it.

// Call op just on the immediate base classes; that is, the base classes that appear in the class
// definition's base class list.
template <typename Op>
void forEachImmediateBaseClass( const SgClassDefinition *cdef, Op &op ) {
	const SgBaseClassPtrList &bases = cdef->get_inheritances();
	for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) { // for each base class...
		const SgBaseClassModifier *mod = (*i)->get_baseClassModifier(); // ...get the base class modifier...
		op( mod ); // ...and invoke op on it, then...
		if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
			if( const SgClassDefinition *baseDef = baseDecl->get_definition() ) // ...get the base class definition...
				op( baseDef ); // ...and invoke op on it.
		}
	}
}

//XXXXXXXXXXXXXX have to fix these below to do as above!
template <typename Op>
bool forEachImmediateBaseClassShortCircuit( const SgClassDefinition *cdef, Op &op ) {
	const SgBaseClassPtrList &bases = cdef->get_inheritances();
	for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) { // for each base class...
		if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
			if( const SgClassDefinition *baseDef = baseDecl->get_definition() ) // get the base class definition...
				if( op( baseDef ) )
					return true;
		}
	}
	return false;
}

// Traverse the entire base class lattice in depth-first order.
template <typename Op>
void forEachBaseClassDepthFirst( const SgClassDefinition *cdef, Op &op ) {
	const SgBaseClassPtrList &bases = cdef->get_inheritances();
	for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) { // for each base class...
		if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
			if( const SgClassDefinition *baseDef = baseDecl->get_definition() ) { // get the base class definition...
				op( baseDef );
				forEachBaseClassDepthFirst( baseDef, op ); // depth first
			}
		}
	}
}

// Same as above but short circuit on success.
template <typename Op>
bool forEachBaseClassDepthFirstShortCircuit( const SgClassDefinition *cdef, Op &op ) {
	const SgBaseClassPtrList &bases = cdef->get_inheritances();
	for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) { // for each base class...
		if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
			if( const SgClassDefinition *baseDef = baseDecl->get_definition() ) { // get the base class definition...
				if( op( baseDef ) )
					return true;
				else if( forEachBaseClassDepthFirstShortCircuit( baseDef, op ) ) // depth first
					return true;
			}
		}
	}
	return false;
}

//XXX to do:  breadth-first lattice traversals.

//
// This function was an early attempt.  It is likely to be replaced by something more flexible in the future,
//
template <typename Pred>
bool searchInheritedMembers( const SgClassDefinition *cdef, SgDeclarationStatementPtrList &dlist, Pred pred, bool deepFind = false ) {
	dlist.clear(); // empty it, just to make sure...
	const SgBaseClassPtrList &bases = cdef->get_inheritances();
	// Recursively look up the class lattice for members with the same identifier.
	for( SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i ) { // for each base class...
		if( const SgClassDeclaration *baseDecl = (*i)->get_base_class() ) {
			if( const SgClassDefinition *baseDef = baseDecl->get_definition() ) { // get the base class definition...
				SgDeclarationStatementPtrList temp;
				const SgDeclarationStatementPtrList &baseMembers = baseDef->get_members();
				for( SgDeclarationStatementPtrList::const_iterator i = baseMembers.begin(); i != baseMembers.end(); ++i ) {
					if( pred( *i ) ) temp.push_back( *i );
				}
				// look in bases of bases (depth first) if member not found or deepFind is on
				if( deepFind || temp.empty() )
					searchInheritedMembers( baseDef, temp, pred );
				dlist.insert( dlist.end(), temp.begin(), temp.end() );
			}
		}
	}
	return !dlist.empty();
}

enum MemberType {
	variableName, functionName, enumName, enumeratorName,
	templateName, typedefName, usingdeclarationName
};

struct MemberNameInfo {
	MemberNameInfo( SgNode *node, const std::string &id, Sg_File_Info *fileInfo, MemberType type )
		: id_(id), fileInfo_(fileInfo), type_(type), node_(node) {}
	std::string id_;
	Sg_File_Info *fileInfo_;
	MemberType type_;
	SgNode *node_;
};
typedef std::list<MemberNameInfo> MemberNameInfoList;
typedef MemberNameInfoList::iterator MemberNameInfoListIterator;

bool getMemberInfo( const SgDeclarationStatementPtrList &members, MemberNameInfoList &info );

class IsFunctionDeclarationNamed : public std::unary_function<SgFunctionDeclaration *,bool> {
  public:
	IsFunctionDeclarationNamed( const std::string &id )
		: id_(id) {}
	bool operator ()( SgNode *node ) const {
		if( SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
			std::string fid = fd->get_name().getString();
			return fid == id_;
		}
		return false;
	}
  private:
	std::string id_;
};

class IsNonVirtualFunctionDeclarationNamed : public std::unary_function<SgFunctionDeclaration *,bool> {
  public:
	IsNonVirtualFunctionDeclarationNamed( const std::string &id )
		: id_(id) {}
	bool operator ()( SgNode *node ) const {
		if( IsFunctionDeclarationNamed(id_)(node) ) {
			return !static_cast<SgFunctionDeclaration *>(node)->get_functionModifier().isVirtual();
		}
		return false;
	}
  private:
	std::string id_;
};

std::pair<const SgNode *,size_t> findParentNodeOfType( const SgNode *start, int nodeType ); //XXX and do a version with a vector of nodeTypes, or just overload
const SgNode *getForStatmentTest( const SgNode *node );

#endif

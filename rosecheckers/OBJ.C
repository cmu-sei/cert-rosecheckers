/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.// 
// DM21-0505</legal>// 
 */

/**
 * \file OBJ.C
 */

#include "rose.h"
#include "utilities.h"
#include <list>
#include <string>
#include "rose.h"
#include "utilities_cpp.h"

class IsPublicData {
public:
  IsPublicData()
    : foundPublic_(false), lastPublicVarDecl_(0) {}
  bool foundPublic() const
  { return foundPublic_; }
  const SgVariableDeclaration *publicDeclaration() const
  { return lastPublicVarDecl_; }
  bool operator ()( const SgDeclarationStatement *ds ) {
    if( const SgVariableDeclaration *vardecl = isSgVariableDeclaration( ds ) ) {
      const SgAccessModifier &mod
        = const_cast<SgVariableDeclaration *>(vardecl)->get_declarationModifier().get_accessModifier();
      if( mod.isPublic() ) {
        lastPublicVarDecl_ = vardecl;
        foundPublic_ = true;
        return true;
      }
    }
    return false;
  }
private:
  bool foundPublic_;
  const SgVariableDeclaration *lastPublicVarDecl_;
};

/* OBJ00-CPP. Declare data members private */
bool OBJ00_CPP( const SgNode *node ) {
  // This works for unions too...
  if( const SgClassDefinition *classdef = isSgClassDefinition( node ) ) {
    if( !isPODClass( classdef ) ) {
      IsPublicData op;
      forEachClassMemberShortCircuit( classdef, op );
      if( op.foundPublic() ) {
        print_error(op.publicDeclaration(), "OBJ00-CPP", "Public data member", true);
        return true;
      }
    }
  }
  return false;
}

class ConvOpCount {
public:
  ConvOpCount()
    : count_(0) {}
  size_t count() const
  { return count_; }
  bool operator ()( const SgNode *node ) {
    if( const SgFunctionDeclaration *fdecl = isSgFunctionDeclaration( node ) ) {
      if( fdecl->get_specialFunctionModifier().isConversion() ) {
        ++count_;
        return true;
      }
    }
    return false;
  }
private:
  size_t count_;
};

/* OBJ01-CPP. Be careful with the definition of conversion operators */
bool OBJ01_CPP( const SgNode *node ) {
  // I interpret "being careful" to not having more than one conversion function (though no such functions is better!)
  if( const SgClassDefinition *classdef = isSgClassDefinition( node ) ) {
    ConvOpCount op;
    forEachClassMember( classdef, op );
    if( op.count() > 1 ) {
      print_error( classdef,  "OBJ01-CPP", ("Class has " + utostring(op.count()) + " conversion functions.").c_str() , true);
      return true;
    }
  }
  return false;
}

/* OBJ02-CPP. Do not hide inherited non-virtual member functions */
bool OBJ02_CPP( const SgNode *node ) {
  bool result = false;
  if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
    SgDeclarationStatementPtrList dlist;
    const SgDeclarationStatementPtrList &members = cdef->get_members();
    MemberNameInfoList names;
    getMemberInfo( members, names );
    for( MemberNameInfoListIterator i = names.begin(); i != names.end(); ++i ) { // for each name
      if( isCopyAssignmentDeclaration( i->node_ ) ) // Note copy assign is not inherited!
        continue;
      if( searchInheritedMembers( cdef, dlist, IsNonVirtualFunctionDeclarationNamed(i->id_) ) ) {
        print_error( i->node_,  "OBJ02-CPP",  (std::string( i->id_ ) + " hides inherited nonvirtual function.").c_str() , true);
        result = true;
      }
    }
  }
  return result;
}

/* OBJ03-CPP. Prefer not to overload virtual functions */
bool OBJ03_CPP( const SgNode *node ) {
  bool result = false;
  if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
    SgDeclarationStatementPtrList dlist;
    const SgDeclarationStatementPtrList &members = cdef->get_members();
    for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) { // n**2 in members
      if( const SgFunctionDeclaration *fdec1 = isSgFunctionDeclaration( *i ) ) {
        if( fdec1->get_functionModifier().isVirtual() ) {
          const std::string name1 = fdec1->get_name().getString();
          for( SgDeclarationStatementPtrList::const_iterator j = members.begin(); j != members.end(); ++j ) {
            if( *j == *i ) // don't compare with self
              continue;
            if( const SgFunctionDeclaration *fdec2 = isSgFunctionDeclaration( *j ) ) {
              const std::string name2 = fdec2->get_name().getString();
              if( name1 == name2 ) {
                result = true;
                print_error( fdec1,  "OBJ03-CPP",  ("Overloaded virtual function " + name1 + " on line " + utostring( fdec2->get_file_info()->get_line() )).c_str() , true);
              }
            }
          }
        }
      }
    }
  }
  return result;
}

/* OBJ04-CPP. Prefer not to give virtual functions default argument initializers */
bool OBJ04_CPP( const SgNode *node ) {
  // Note member templates can't be virtual.
  bool result = false;
  if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
    SgDeclarationStatementPtrList dlist;
    const SgDeclarationStatementPtrList &members = cdef->get_members();
    for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) { // n**2 in members
      if( SgFunctionDeclaration *fdec = isSgFunctionDeclaration( *i ) ) {
        if( fdec->get_functionModifier().isVirtual() ) {
          const SgInitializedNamePtrList &args = fdec->get_args();
          for( SgInitializedNamePtrList::const_iterator i = args.begin(); i != args.end(); ++i ) {
            if( (*i)->get_initializer() ) {
              result = true;
              print_error( fdec,  "OBJ04-CPP",  (fdec->get_name().getString() + " is a virtual function with default initializer.").c_str() , true);
              break; // avoid multiple reports for multiple initializers
            }
          }
        }
      }
    }
  }
  return result;
}

void handleCallToVirtualMemberOfThisClass( const SgFunctionCallExp *fcall, const SgClassDefinition *cdtorClassDef ) {
  const SgExpression *function = fcall->get_function();
  const SgMemberFunctionRefExp *mfre = 0;
  const SgBinaryOp *bop = 0;
  if( (mfre = isSgMemberFunctionRefExp( function )) != 0 )
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
    const SgClassDefinition *functionCallClassDef = isSgClassDefinition( mfd->get_class_scope());
    bool isVirtual = mfd->get_functionModifier().isVirtual();
    if(isVirtual) {
      //XXX not quite right:  non-virtual call of virtual incorrectly flagged.
      //XXX How to find out if the function name is qualified?  These do nothing:
      //int vc = mfre->get_virtual_call();
      //int nq = mfre->get_need_qualifier();
      // XXXcan't seem to find a use of SgQualifiedName anywhere either...
      if( functionCallClassDef == cdtorClassDef ) {
        print_error(fcall, "OBJ30-CPP", "Calling base class virtual in constructor or destructor", false);
      }
      else if( isBaseOf( cdtorClassDef, functionCallClassDef ) ) {
        print_error(fcall, "OBJ30-CPP", "Calling base class virtual in constructor or destructor", false);
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

bool OBJ30_CPP( const SgNode *node ) { // Avoid calling your own virtual functions in constructors and destructors.
  if( const SgFunctionDefinition *fdef = isSgFunctionDefinition( node ) ) {
    const SgFunctionDeclaration *fdec = fdef->get_declaration();
    if( fdec->get_specialFunctionModifier().isConstructor() || fdec->get_specialFunctionModifier().isDestructor() ) {
      if( const SgMemberFunctionDeclaration *mfdec = isSgMemberFunctionDeclaration( fdec ) ) {
        // get the type of the class to which ctor or dtor belongs
        const SgClassDefinition *cdef = isSgClassDefinition( mfdec->get_class_scope());
        //const SgClassDeclaration *cdecl = cdef->get_declaration();
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

/* OBJ32-CPP. Ensure that single-argument constructors are marked "explicit" */
bool OBJ32_CPP( const SgNode *node ) {
  //XXX Check for templates and member templates as well!
  // Note:  skip check for copy ctors and member template copy-like ctors.
  if( !isCopyCtorDeclaration( node ) && !isTemplateCopyLikeCtorDeclaration( node ) && isSingleArgCtorDeclaration( node ) ) {
    const SgFunctionDeclaration *fdecl = isSgFunctionDeclaration( node );
    const SgFunctionModifier &mod = fdecl->get_functionModifier();
    if( !mod.isExplicit() ) {
      //XXX returns 0 for instantiated template
      print_error( fdecl,  "OBJ32-CPP",  (fdecl->get_name().getString() + " is a non-explicit single-argument constructor.").c_str() , false);
      return true;
    }
  }
  return false;
}


/***************************
 * Violation checking code *
 ***************************/

/// C++ checkers

bool OBJ_CPP(const SgNode *node) {
  bool violation = false;
  violation |= OBJ00_CPP(node);
  violation |= OBJ01_CPP(node);
  violation |= OBJ02_CPP(node);
  violation |= OBJ03_CPP(node);
  violation |= OBJ04_CPP(node);
  violation |= OBJ30_CPP(node);
  violation |= OBJ32_CPP(node);
  return violation;
}

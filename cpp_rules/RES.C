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
#include <cstring>
#include "rose.h"
#include "utilities.h"

bool RES35_C( const SgNode *node ) { // Declare a copy constructor, a copy assignment operator, and a destructor in a class that manages resources
	bool ret = false;
	if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
		// Skip the check if this is a POD (like a C struct).
		if( !isPODClass( cdef ) ) {
			// First, see which of these three functions the class has.
			size_t count = hasExplicitCopyCtor( cdef ) + hasExplicitCopyAssignment( cdef ) + hasExplicitDtor( cdef );
			if( count > 0 && count < 3 ) {
				diagnostic( "RES35-C", node, 
					"if any of copy constructor, copy assignment, and destructor are declared, all three should be." );
				ret = true;
			}
			//XXX more...how do we know if a class manages resources?  Punt and just check for a pointer member?
			if( hasPointerMember( cdef ) && count < 3 ) { //XXX should omit this check for unions
				diagnostic( "RES35-C", node,
					"a class with a pointer data member should probably "
					"define a copy constructor, copy assignment, and destructor." );
				ret = true;
			}
		}
	}
	return ret;
}
/*=========================
class PrintMember {
  public:
	void operator ()( const SgNode *node ) const {
		if( const SgDeclarationStatement *ds = isSgDeclarationStatement( node ) )
			std::cout << ds->get_mangled_name().getString();
		else
			std::cout << "not a declaration statement";
		if( isCompilerGenerated( node ) )
			std::cout << " is compiler-generated.";
		std::cout << std::endl;
	}
};
=============================*/
bool RESMISC_A( const SgNode *node ) { // misc things that haven't found a home yet...
	if( const SgClassDefinition *cd = isSgClassDefinition( node ) ) {
		std::cout << "==============================" << std::endl;
		if( hasCompilerGeneratedDefaultCtor( cd ) )
			std::cout << "\thas compiler generated default ctor" << std::endl;
		if( hasTrivialCtor( cd ) )
			std::cout << "\thas trivial ctor" << std::endl;
		if( hasTrivialDtor( cd ) )
			std::cout << "\thas trivial dtor" << std::endl;
		if( hasCompilerGeneratedDtor( cd ) )
			std::cout << "\thas compiler generated dtor" << std::endl;
		if( hasCompilerGeneratedCopyCtor( cd ) )
			std::cout << "\thas compiler generated copy ctor" << std::endl;
		if( hasCompilerGeneratedCopyAssignment( cd ) )
			std::cout << "\thas compiler generated copy assign" << std::endl;
		if( hasVirtualFunction( cd ) )
			std::cout << "\thas virtual function" << std::endl;
	}
	return false;
}

bool RES( SgProject *project ) {
	bool violation = false;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( project, V_SgNode );
	for( Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		violation |= RES35_C( *i );
		//violation |= RESMISC_A( *i );
	}
	return violation;
}

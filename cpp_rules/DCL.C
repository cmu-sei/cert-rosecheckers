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

namespace {
// A list of all reserved names.  This is not so good.
// Maybe better to do a test on the namespace in which the declaration occurs?
std::string reservedIds [] = {
"_Atomic_word",
"__builtin_acosf",
"__builtin_acosl",
"__builtin_asinf",
"__builtin_asinl",
"__builtin_atanf",
"__builtin_atanl",
"__builtin_atan2f",
"__builtin_atan2l",
"__builtin_ceilf",
"__builtin_ceill",
"__builtin_coshf",
"__builtin_coshl",
"__builtin_floorf",
"__builtin_floorl",
"__builtin_fmodf",
"__builtin_fmodl",
"__builtin_frexpf",
"__builtin_frexpl",
"__builtin_ldexpf",
"__builtin_ldexpl",
"__builtin_log10f",
"__builtin_log10l",
"__builtin_modff",
"__builtin_modfl",
"__builtin_powf",
"__builtin_powl",
"__builtin_sinhf",
"__builtin_sinhl",
"__builtin_tanf",
"__builtin_tanl",
"__builtin_tanhf",
"__builtin_tanhl",
"__builtin_strchr",
"__builtin_strrchr",
"__builtin_strpbrk",
"__builtin_strstr",
"__builtin_nansf",
"__builtin_nans",
"__builtin_nansl",
"__builtin_fabs",
"__builtin_fabsf",
"__builtin_fabsl",
"__builtin_cosf",
"__builtin_cosl",
"__builtin_sinf",
"__builtin_sinl",
"__builtin_sqrtf",
"__builtin_sqrtl",
"__builtin_powil",
"__builtin_powi",
"__builtin_powif",
"__atomic_add",
"__exchange_and_add",
"__verbose_terminate_handler"
};
const size_t reservedIdsLen = sizeof(reservedIds)/sizeof(reservedIds[0]);
}

bool isNameReservedForImplementation( const std::string &id ) {
	if( id.size() <= 1 )
		return false;
	else if( (id[0] == '_' && isupper(id[1])) || (id.find("__") != std::string::npos) )
		return std::find( reservedIds, reservedIds+reservedIdsLen, id) == reservedIds+reservedIdsLen;
	else
		return false;
}

bool DCL04_A( const SgNode *node ) { // Declare no more than one variable per declaration
	// NOTE: currently, each SgVariableDeclaration holds only a single SgInitializedName.  Therefore,
	// we're going to use a "heuristic" to decide whether this rule is violated...
	// Later, we can just test to see if the initialized name pointer list has size > 1, as below.
	//if( SgVariableDeclaration *vdec = isSgVariableDeclaration( node ) ) {
	//	SgInitializedNamePtrList &vars = vdec->get_variables();
	//	if( vars.size() > 1 ) {
	//		std::cout << "DCL04-A. line " << vdec->get_file_info()->get_line()
	//			<< ": more than one variable declared in declaration." << std::endl;
	//		return true;
	//	}
	//}
	return false;
}

bool DCL30_C( const SgNode *node ) { // Do not use names reserved for the implementation
	return false; //XXX Disabled due to false positives.  Hard to configure implementation-specific names.
	// Don't bother it it's not a declaration of some sort.
	if( !isSgDeclarationStatement( node ) )
		return false;

	// Don't issue diagnostic for local or member names.
	if( isLocalDeclaration( node ) || isMemberDeclaration( node ) )
		return false;

	bool result = false;
	std::string id;
	if( const SgVariableDeclaration *vdec = isSgVariableDeclaration( node ) ) { // there may be several names (eventually)
		const SgInitializedNamePtrList &vars = vdec->get_variables();
		for( SgInitializedNamePtrList::const_iterator i = vars.begin(); i != vars.end(); ++i ) {
			id = (*i)->get_name().getString();
			if( result = isNameReservedForImplementation( id ) )
				break;
		}
	}
	else if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration( node ) ) {
		id = fdec->get_name().getString();
		result = isNameReservedForImplementation( id );
	}
	else if( const SgEnumDeclaration *edec = isSgEnumDeclaration( node ) ) {
		id = edec->get_name().getString();
		if( !(result = isNameReservedForImplementation( id )) ) {
			const SgInitializedNamePtrList &enumerators = edec->get_enumerators();
			for( SgInitializedNamePtrList::const_iterator i = enumerators.begin(); i != enumerators.end(); ++i ) {
				id = (*i)->get_name().getString();
				if( result = isNameReservedForImplementation( id ) )
					break;
			}
		}
	}
	else if( const SgTemplateDeclaration *tdec = isSgTemplateDeclaration( node ) ) {
		id = tdec->get_name().getString();
		result = isNameReservedForImplementation( id );
	}
	else if( const SgTypedefDeclaration *tddec = isSgTypedefDeclaration( node ) ) {
		id = tddec->get_name().getString();
		result = isNameReservedForImplementation( id );
	}
	else if( const SgClassDeclaration *cdec = isSgClassDeclaration( node ) ) {
		id = cdec->get_name().getString();
		result = isNameReservedForImplementation( id );
	}

	if( result )
		diagnostic( "DCL30-C", node, std::string("the identifier") + id + " is reserved for the implementation" );

	return result;
}

bool DCL32_C( const SgNode *node ) { // Avoid runtime static initialization of objects with external linkage
	if( const SgVariableDefinition *vdef = isSgVariableDefinition( node ) ) {
		//XXX get defining declaration?
		const SgDeclarationModifier &dm = vdef->get_declarationModifier();
		const SgStorageModifier &sm = const_cast<SgDeclarationModifier &>(dm).get_storageModifier();
		if( sm.isExtern() || sm.isUnknown() || sm.isUnspecified() ) { //XXX not so sure this is right...
			//XXX should also permit externs in anonymous namespaces
			const SgInitializedName *in = vdef->get_vardefn();
			const SgInitializer *init = in->get_initializer();
			bool runtimeInit = isSgConstructorInitializer( init );
			if( !runtimeInit ) {
				if( const SgAssignInitializer *ai = isSgAssignInitializer( init ) ) {
					const SgExpression *e = ai->get_operand_i();
					runtimeInit = !isSgValueExp( e );
				}
			}
			if( !runtimeInit ) {
				if( const SgAggregateInitializer *ai = isSgAggregateInitializer( init ) ) {
					//XXX need to check aggregates
				}
			}

			if( runtimeInit ) {
				diagnostic( "DCL32-C", node, "runtime static initialization of object with external linkage." );
				return true;
			}
			else {
				// To avoid a plethora of messages, check for runtime static destruction
				// only if there was no flagging of initialization.
				// See if the type of the object being defined has a non-trivial dtor.
				const SgType *t = in->get_type();
				t = t->stripType( SgType::STRIP_TYPEDEF_TYPE );
				if( const SgClassType * ct = isSgClassType( t ) ) {
					const SgDeclarationStatement *decl = ct->get_declaration();
					const SgClassDeclaration *cdecl = isSgClassDeclaration( decl );
					if( const SgClassDefinition *cdef = cdecl->get_definition() ) {
						const SgDeclarationStatementPtrList &members = cdef->get_members();
						for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) {
							if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration( *i ) ) {
								if( fdec->get_specialFunctionModifier().isDestructor() ) {
									diagnostic( "DCL32-C", node, "runtime static initialization of object with external linkage." );
									return true;
								}
							}
						}
					}	
				}
			}
		}
	}
	return false;
}
/*XXXXXXXXXXXXXXXXXXXXXX OLD
bool DCL32_C( const SgNode *node ) { // Avoid runtime static initialization of objects with external linkage
	//if( const SgVariableDeclaration *vdecl = isSgVariableDeclaration( node ) ) {
		//if( const SgVariableDefinition *vdef = const_cast<SgVariableDeclaration *>(vdecl)->get_definition() ) {
		if( const SgVariableDefinition *vdef = isSgVariableDefinition( node ) ) {

			//XXX get defining declaration?
			const SgDeclarationModifier &dm = vdecl->get_declarationModifier();
			const SgStorageModifier &sm = const_cast<SgDeclarationModifier &>(dm).get_storageModifier();
			if( sm.isExtern() || sm.isUnknown() || sm.isUnspecified() ) { //XXX not so sure this is right...
				//XXX should also permit externs in anonymous namespaces
				const SgInitializedName *in = vdef->get_vardefn();
				const SgInitializer *init = in->get_initializer();
				bool runtimeInit = isSgConstructorInitializer( init );
				if( !runtimeInit ) {
					if( const SgAssignInitializer *ai = isSgAssignInitializer( init ) ) {
						const SgExpression *e = ai->get_operand_i();
						runtimeInit = !isSgValueExp( e );
					}
				}
				if( !runtimeInit ) {
					if( const SgAggregateInitializer *ai = isSgAggregateInitializer( init ) ) {
						//XXX need to check aggregates
					}
				}

				if( runtimeInit ) {
					std::cout << "DCL32-C." << LOC(node)
						<< "runtime static initialization of object with external linkage." << std::endl;
					return true;
				}
				else {
					// To avoid a plethora of messages, check for runtime static destruction
					// only if there was no flagging of initialization.
					// See if the type of the object being defined has a non-trivial dtor.
					const SgType *t = in->get_type();
					t = t->stripType( SgType::STRIP_TYPEDEF_TYPE );
					if( const SgClassType * ct = isSgClassType( t ) ) {
						const SgDeclarationStatement *decl = ct->get_declaration();
						const SgClassDeclaration *cdecl = isSgClassDeclaration( decl );
						if( const SgClassDefinition *cdef = cdecl->get_definition() ) {
							const SgDeclarationStatementPtrList &members = cdef->get_members();
							for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) {
								if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration( *i ) ) {
									if( fdec->get_specialFunctionModifier().isDestructor() ) {
										std::cout << "DCL32-C." << LOC(node)
											<< "runtime static destruction of object with external linkage." << std::endl;
										return true;
									}
								}
							}
						}	
					}
				}
			}
		}
	//}
	return false;
}
xxxxxxxxxxxxxxxxxx*/

bool DCL( SgProject *project ) {
	bool violation = false;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( project, V_SgNode );
	for( Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		violation |= DCL04_A( *i );
		violation |= DCL30_C( *i ); //XXX false positives; hard to configure
		violation |= DCL32_C( *i );
	}
	return violation;
}

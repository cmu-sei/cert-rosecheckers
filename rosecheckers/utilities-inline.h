/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.// 
// DM21-0505</legal>// 
 */

/**
 * \file utilities-inline.h
 */

#ifndef UTILITIES_INLINE_H
#define UTILITIES_INLINE_H

#define findParentOfType(node, t) \
	is ## t ( findParentNodeOfType(node, V_ ##t ))

/**
 * Iterate over all initializers "i" in a list of variables
 * nodes should be something like SgVariableDeclaration->get_variables()
 */
#define FOREACH_INITNAME(nodes,i) \
	for(SgInitializedNamePtrList::const_iterator (i) = (nodes).begin(); (i) < (nodes).end(); (i)++)

/** Iterate over all nodes of type type below node, name the collection nodes
 * and name the iteroator i
 *
 * \param[in] node Root node of the tree
 * \param[out] nodes What to call the collection
 * \param[out] i What to call the iterator
 * \param[in] type Only iterate over nodes of this type
 */
#define FOREACH_SUBNODE(node, nodes, i, type) \
	assert(node);\
	Rose_STL_Container<SgNode *> (nodes) = NodeQuery::querySubTree( const_cast<SgNode*>((const SgNode *) (node)), (type) ); \
	Rose_STL_Container<SgNode *>::iterator (i) = (nodes).begin(); \
	for (; (i) != (nodes).end(); ++(i) )

/**
 * Remove all modifiers such as const or volatile, but leave the typedefs
 */
inline const SgType *stripModifiers(const SgType *type) {
	assert(type);
	return type->stripType(SgType::STRIP_MODIFIER_TYPE);
}

/**
 * Remove all typedefs, but leave the modifiers
 */
inline const SgType *stripTypedefs(const SgType *type) {
	assert(type);
	return type->stripType(SgType::STRIP_TYPEDEF_TYPE);
}

/**
 * Checks to see if the type has the const modifier
 */
inline bool isConstType(const SgType *t) {
	assert(t);
	const SgModifierType *mt = isSgModifierType(stripTypedefs(t));
	return mt && mt->get_typeModifier().get_constVolatileModifier().isConst();
}
/**
 * Checks to see if the type has the volatile modifier
 */
inline bool isVolatileType(const SgType *t) {
	assert(t);
	const SgModifierType *mt = isSgModifierType(stripTypedefs(t));
	return mt && mt->get_typeModifier().get_constVolatileModifier().isVolatile();
}

/**
 * Returns a variable's declaration, given a reference to that var
 */
inline const SgInitializedName* getRefDecl(const SgVarRefExp* ref) {
	assert(ref);
	return ref->get_symbol()->get_declaration();
}

/**
 * Checks to see if the variable was declared static
 */
inline bool isStaticVar(const SgInitializedName *var) {
	assert(var);
	return const_cast<SgInitializedName*>(var)->get_declaration()->get_declarationModifier().get_storageModifier().isStatic();
}

/**
 * Checks to see if the variable was declared extern
 */
inline bool isExternVar(const SgInitializedName * var) {
	return const_cast<SgInitializedName*>(var)->get_declaration()->get_declarationModifier().get_storageModifier().isExtern();
}

/**
 * Checks the scope of the variable to see if it is global or not
 */
inline bool isGlobalVar(const SgInitializedName *var) {
	assert(var);
	return isSgGlobal(var->get_scope());
}

/**
 * Until ROSE implements these types natively, we will have to beat the
 * names out of the types with unparseToString()
 */
inline bool isTypeWcharT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "wchar_t";
}

inline bool isTypeTimeT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "time_t";
}
inline bool isTypeSizeT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "size_t";
}

inline bool isTypeRSizeT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "rsize_t";
}

inline bool isTypeUintptrT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "uintptr_t";
}

inline bool isTypeVoid(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "void";
}

inline bool isTypeVoidStar(const SgType *t) {
	assert(t);
	const SgType *d = const_cast<SgType *>(t)->dereference();
	return (t !=d ) && isTypeVoid(d);
}

inline bool isTypeFile(const SgType *t) {
	assert(t);
	std::string str = stripModifiers(t)->unparseToString();
	return str == "FILE" || str == "struct _IO_FILE";
}

inline bool isCompilerGeneratedNode(const SgNode *node) {
	assert(node);
	return node->get_file_info()->isCompilerGenerated();
}

#endif

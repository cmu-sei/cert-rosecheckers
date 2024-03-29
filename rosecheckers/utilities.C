/*
// <legal>
// CERT Rosecheckers
// Copyright 2022 Carnegie Mellon University.
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.
// CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.
// DM21-0505
// </legal>
 */

/**
 * \file utilities.C
 */

#include <list>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include "rose.h"
#include "utilities.h"

/**
 * Spin up the AST until we find a parent of the given type, if not, return
 * NULL
 *
 *
 */
const SgNode *findParentNodeOfType(const SgNode *node, VariantT t) {
	const SgNode *parent = (node)->get_parent();
	for (; parent; parent = parent->get_parent() ) {
		if( parent->variantT() == t)
			break;
	}
	return parent;
}

const SgExpression *removeImplicitPromotions( const SgExpression *e ) {
	if( const SgCastExp *cast = isSgCastExp( e ) ) {
		if( isCompilerGeneratedNode( cast ) ) {
		// implicit promotions seem to be implemented as casts
			e = cast->get_operand();
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

/**
 * \return value indicates whether there was a default case.
 */
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
		else if(isSgDefaultOptionStmt( *i ) ) {
			sawDefault = true;
		}
	}
	return sawDefault;
}

bool isCallOfFunctionNamed( const SgFunctionRefExp *f, const std::string &name ) {
	assert(f);
	return isSgFunctionCallExp(f->get_parent())
		&& (f->get_symbol()->get_name().getString() == name);
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

/**
 * \param[in] node Node at which to genererate the error
 * \param[in] rule Abbreviation of the violated rule (ie. ARR01-C)
 * \param[in] desc Short description of the error
 * \param[in] warning If true than generate a warning, else an error
 */
void print_error(const SgNode* node, const char* rule, const char* desc, bool warning) {
  const Sg_File_Info* fi = node->get_file_info();
  std::string filename = fi->get_filename();
  size_t found = filename.rfind("/");
  if( found != std::string::npos)
    filename.erase( 0, found+1);
  /// don't use << ':' << fi->get_col(), not flymake-compliant
  std::cerr << filename << ':' << fi->get_line() << ':' << fi-> get_col()
	    << ": " << (warning ? "warning" : "error")
	    << ": " << rule << ": " << desc << std::endl;
}

/**
 * Returns True if node is inside an expression that tests its value to see if
 * it is NULL
 *
 * \bug This doesn't actually check for NULL, just the existence of a
 * comparison
 *
 * \todo merge this with valueVerified
 */
bool isTestForNullOp(const SgNode* node) {
  if (node == NULL) return false;
  const SgNode* parent = node->get_parent();
  assert(parent != NULL);
  while (isSgNotOp( parent) || isSgExprStatement( parent)) parent = parent->get_parent(); // !expr
  if (isSgEqualityOp( parent)) return true; // expr == ...
  else if (isSgNotEqualOp( parent)) return true; // ptr != ...
  else if (isSgIfStmt( parent)) return true; // if (ptr) ...
  else if (isSgOrOp( parent)) return true; // if (ptr) ...
  else if (isSgAndOp( parent)) return true; // if (ptr) ...
  else if (isSgCastExp( parent)) {
    const SgCastExp* cast = isSgCastExp( parent);
    assert(cast != NULL);
    if (isSgTypeBool( cast->get_type())) return true;  // if (ptr) ...
  }

  const SgAssignOp* assignment = isSgAssignOp( parent);
  return (assignment != NULL) ? isTestForNullOp( assignment) : false;
}

/**
 * Returns reference to ith argument of function call. Dives through
 * typecasts. Returns NULL if no such parm
 */
const SgExpression* getFnArg(const SgFunctionCallExp* fnCall, unsigned int i) {
	assert(fnCall);
	const SgExpressionPtrList exprs = fnCall->get_args()->get_expressions();
	assert(i < exprs.size());
	const SgExpression *fnArg = exprs[i];
	assert(fnArg);
	const SgCastExp* castArg = isSgCastExp(fnArg);
	return castArg ? castArg->get_operand() : fnArg;
}

/**
 * Returns reference to ith argument of function reference. Dives through
 * typecasts. Returns NULL if no such parm
 */
const SgExpression* getFnArg(const SgFunctionRefExp* node, unsigned int i) {
	assert(node);
	const SgFunctionCallExp *fnCall = isSgFunctionCallExp(node->get_parent());
	assert(fnCall);
	return getFnArg(fnCall, i);
}

/**
 * Fills list with all nodes of type \c SgVarRefExp in enclosing function
 */
const Rose_STL_Container<SgNode*> getNodesInFn( const SgNode* node) {
	const SgFunctionDefinition* block = findParentOfType(node, SgFunctionDefinition);

	if(block == NULL)
	  return *(new Rose_STL_Container<SgNode*>());

	assert( block != NULL);
	return NodeQuery::querySubTree( const_cast< SgFunctionDefinition*>( block), V_SgVarRefExp);
}

/**
 * Returns iterator of next node that refers to same variable as ref.
 *
 * \return nodes.end() if unsuccessful
 */
Rose_STL_Container<SgNode *>::const_iterator nextVarRef(const Rose_STL_Container<SgNode *>& nodes, Rose_STL_Container<SgNode *>::const_iterator i, const SgInitializedName* var) {
  for (++i; i != nodes.end(); ++i ) {
    const SgVarRefExp* ref = isSgVarRefExp(*i);
    assert( ref != NULL);
    const SgInitializedName* ref_var = getRefDecl( ref);
    if (ref_var == var) return i;
  }
  return nodes.end();
}

/**
 * Returns true if function( ref) appears in code somewhere after ref
 */
bool isVarUsedByFunction(const char* function, const SgVarRefExp* ref) {
  if (ref == NULL) return false;
  const SgInitializedName* var = getRefDecl( ref);
  assert(var != NULL);

  // Find reference in containing function's node list
  Rose_STL_Container<SgNode *> nodes = getNodesInFn( ref);
  Rose_STL_Container<SgNode *>::const_iterator i;
  for (i = nodes.begin(); i != nodes.end(); ++i ) if (*i == ref) break;
  assert(i != nodes.end());

  // Now to future variable references, find one with bad usage
  for (i = nextVarRef( nodes, i, var); i != nodes.end();
       i = nextVarRef( nodes, i, var)) {
    const SgFunctionCallExp* fn = findParentOfType(*i, SgFunctionCallExp);
    if (fn == NULL) continue;
    const SgFunctionRefExp* fn_ref = isSgFunctionRefExp( fn->get_function());
    assert( fn_ref != NULL);

    if (isCallOfFunctionNamed( fn_ref, function))
      return true;
  }

  return false;
}

/**
 * Searches inside of the parent block, then returns the statement that's
 * delta lines before or after node (usually -1 or +1 for previous and next
 * statement)
 *
 * \todo Use next visitor to make this better
 *
 * \param[in] node Find the BasicBlock above this node
 * \param[in] delta Number of lines to search before or after node
 * \return NULL on failure
 */
const SgStatement * findInBlockByOffset(const SgNode *node, int delta) {
	// first, find the parent block
	const SgNode *parent = node;
	const SgNode *block = node->get_parent();
	if (!block) return NULL;

	while(!isSgBasicBlock(block)) {
		parent = block;
		block = parent->get_parent();
		if (!block) return NULL;
	}

	// second, find the expression offset by delta from node
	const SgStatementPtrList &nodes = isSgBasicBlock(block)->get_statements();
	Rose_STL_Container<SgStatement *>::const_iterator i = find(nodes.begin(), nodes.end(), parent);
	if (i == nodes.end())
		return NULL;
	while (delta > 0) {
		delta--;
		if ((++i) == nodes.end())
			return NULL;
	}
	while (delta < 0) {
		if ((i--) == nodes.begin()) {
			if (delta == -1) {
				parent = block->get_parent();
				if (!parent) return NULL;
				if (isSgForStatement(parent)
				||  isSgIfStmt(parent))
					return isSgStatement(parent);
			}
			return NULL;
		}
		delta++;
	}

	assert(*i);
	return *i;
}

/**
 * Strips casts, preferering to take the originalExpressionTree branch when
 * possible
 */
const SgExpression* removeCasts(const SgExpression * expr) {
	const SgCastExp * cast;
	while ((cast = isSgCastExp(expr)) != NULL) {
	  if ((expr = cast->get_originalExpressionTree()) != NULL)
			continue;
		else
			expr = cast->get_operand();
		assert(expr);
	}
	return expr;
}

/**
 * Takes a function reference and tries to find the variable that the return
 * value is stored into
 *
 * \param[in] fnRef Function reference to search
 * \param[out] varRef_p If this is an assignment not just an initializer, we
 * store the varref here
 */
const SgInitializedName *getVarAssignedTo(const SgFunctionRefExp *fnRef, const SgVarRefExp **varRef_p) {

	assert(fnRef);

	const SgInitializedName* var = NULL;
	// The node in the function where the variable first gets referred to
	const SgVarRefExp* ref = NULL;

	const SgAssignOp* assignment = findParentOfType(fnRef, SgAssignOp);
	if (assignment != NULL) {
		ref = isSgVarRefExp( assignment->get_lhs_operand());
		if (ref == NULL)
			return NULL;
		/**
		 * \todo LHS is more complex than variable, it might be array ref or
		 * struct member or pointer; we're only handling variables (for
		 * now)
		 */
		var = getRefDecl( ref);
	} else {
		const SgAssignInitializer* ass_init = findParentOfType(fnRef, SgAssignInitializer);
		if (ass_init == NULL)
			return NULL; // malloc ptr not assigned.

		var = isSgInitializedName( ass_init->get_parent());
	}

	if (ref && varRef_p) *varRef_p = ref;

	assert(var);
	return var;
}

/**
 * Returns size argument to malloc, calloc, or realloc, if node is appropriate
 * function call, otherwise returns NULL
 *
 * \note As written, these tests catch template declarations only if
 * instantiated.
 */
const SgExpression* getAllocFunctionExpr(const SgFunctionRefExp *node) {
	if (!node) return NULL;
	return isCallOfFunctionNamed(node, "malloc") ? getFnArg(node, 0)
		: isCallOfFunctionNamed(node, "calloc") ? getFnArg(node, 1)
		: isCallOfFunctionNamed(node, "realloc") ? getFnArg(node, 1)
		: NULL;
}

enum VAL_TYPE {BOOL, CHAR, COMPLEX, DOUBLE, ENUM, FLOAT, INT, LDOUBLE, LONG, LLONG, SHORT, STRING, UCHAR, UINT, ULONG, ULLONG, USHORT, WCHAR, UNKNOWN };

const VAL_TYPE getValType(const SgValueExp* node) {
	if (isSgBoolValExp(node)) {
		return BOOL;
	} else if (isSgCharVal(node)) {
		return CHAR;
	} else if (isSgComplexVal(node)) {
		return COMPLEX;
	} else if (isSgDoubleVal(node)) {
		return DOUBLE;
	} else if (isSgEnumVal(node)) {
		return ENUM;
	} else if (isSgFloatVal(node)) {
		return FLOAT;
	} else if (isSgIntVal(node)) {
		return INT;
	} else if (isSgLongDoubleVal(node)) {
		return LDOUBLE;
	} else if (isSgLongIntVal(node)) {
		return LONG;
	} else if (isSgLongLongIntVal(node)) {
		return LLONG;
	} else if (isSgShortVal(node)) {
		return SHORT;
	} else if (isSgStringVal(node)) {
		return STRING;
	} else if (isSgUnsignedCharVal(node)) {
		return UCHAR;
	} else if (isSgUnsignedIntVal(node)) {
		return UINT;
	} else if (isSgUnsignedLongLongIntVal(node)) {
		return ULLONG;
	} else if (isSgUnsignedLongVal(node)) {
		return ULONG;
	} else if (isSgUnsignedShortVal(node)) {
		return USHORT;
	} else if (isSgWcharVal(node)) {
		return WCHAR;
	} else {
		return UNKNOWN;
	}
}

/**
 * Evaluates an SgValueExp using the originalExpressionTree
 *
 * \note We only support a limited amount of operations and types
 */
SgValueExp* computeValueTree(SgValueExp* node) {
	if (!node)
		return NULL;

	const SgExpression* tree = isSgValueExp(node)->get_originalExpressionTree();
	if (!tree)
		return node;

	enum VAL_OP {ADD, ANDASSIGN, AND, ARROW, ARROWSTAR, ASSIGN, BITAND, BITOR, BITXOR, COMMA, CONCATENATION, DIVASSIGN, DIV, DOT, DOTSTAR, EQ, EXPONENTIATION, GEQ, GT, INTDIV, IORASSIGN, LEQ, LSHIFTASSIGN, LSHIFT, MINUSASSIGN, MODASSIGN, MOD, MULTASSIGN, MULT, NEQ, OR, PLUSASSIGN, PNTRARRREF, RSHIFTASSIGN, RSHIFT, SCOPE, SUBTRACT, XORASSIGN, BITCOMPLEMENT, CAST, MINUSMINUS, MINUS, NOT, PLUSPLUS, POINTERDEREF, THROW, UNKNOWN} val_op = UNKNOWN;
	SgValueExp *lhs = NULL;
	SgValueExp *rhs = NULL;

	if (isSgBinaryOp(tree)) {
		const SgBinaryOp * op = isSgBinaryOp(tree);
		lhs = computeValueTree(isSgValueExp(op->get_lhs_operand()));
		rhs = computeValueTree(isSgValueExp(op->get_rhs_operand()));
		if (!lhs || !rhs)
			return NULL;
		if (isSgSubtractOp(op))
			val_op = SUBTRACT;
		else if (isSgPlusAssignOp(op))
			val_op = PLUSASSIGN;
		else if (isSgMinusAssignOp(op))
			val_op = MINUSASSIGN;
		else if (isSgAddOp(op))
			val_op = ADD;
		else if (isSgMultiplyOp(op))
			val_op = MULT;
		else if (isSgMultAssignOp(op))
			val_op = MULTASSIGN;
		else
			return NULL;
	} else if (isSgUnaryOp(tree)) {
		const SgUnaryOp * op = isSgUnaryOp(tree);
		lhs = computeValueTree(isSgValueExp(op->get_operand()));
		if (!lhs)
			return NULL;
		if (isSgMinusOp(op))
			val_op = MINUS;
		else
			return NULL;
	} else {
		return NULL;
	}

	switch(getValType(node)) {
	case INT: {
		SgIntVal* val = isSgIntVal(node);
		SgIntVal* lhsT = isSgIntVal(lhs);
		SgIntVal* rhsT = isSgIntVal(rhs);

		assert(val);
		assert(lhsT);
		if (rhs) assert(rhsT);

		switch(val_op) {
		case MULTASSIGN:
		case MULT:
			val->set_value(lhsT->get_value() * rhsT->get_value());
			break;
		case PLUSASSIGN:
		case ADD:
			val->set_value(lhsT->get_value() + rhsT->get_value());
			break;
		case MINUSASSIGN:
		case SUBTRACT:
			val->set_value(lhsT->get_value() - rhsT->get_value());
			break;
		case MINUS:
			val->set_value(0 - lhsT->get_value());
			break;
		default:
			return NULL;
		}
		return val;
	}
	default:
		return NULL;
	}
}

/**
 * If node is a function reference to scanf, or any of its derivitaves,
 * returns the argument number of the format string (eg 0 for scanf, 1 for fscanf, etc)
 * Otherwise, returns -1
 */
int getScanfFormatString(const SgFunctionRefExp *node) {
	if (!node) return -1;
	return isCallOfFunctionNamed(node, "scanf")
		|| isCallOfFunctionNamed(node, "vscanf") ? 0
		: isCallOfFunctionNamed(node, "fscanf")
		|| isCallOfFunctionNamed(node, "sscanf")
		|| isCallOfFunctionNamed(node, "vfscanf")
		|| isCallOfFunctionNamed(node, "vsscanf") ? 1
		: -1;
}

/**
 * If node is a function reference to printf, or any of its derivitaves,
 * returns the argument number of the format string (eg 0 for printf, 1 for fprintf, etc)
 * Otherwise, returns -1
 */
int getPrintfFormatString(const SgFunctionRefExp *node) {
	if (!node) return -1;
	return isCallOfFunctionNamed(node, "printf")
		|| isCallOfFunctionNamed(node, "vprintf") ? 0
		: isCallOfFunctionNamed(node, "fprintf")
		|| isCallOfFunctionNamed(node, "sprintf")
		|| isCallOfFunctionNamed(node, "vfprintf")
		|| isCallOfFunctionNamed(node, "vsprintf")
		|| isCallOfFunctionNamed(node, "syslog") ? 1
		: isCallOfFunctionNamed(node, "snprintf")
		|| isCallOfFunctionNamed(node, "vsnprintf") ? 2
		: -1;
}

/**
 * Returns node if node is a reference to memcpy(), memmove(), or memset(). NULL otherwise
 */
const SgFunctionRefExp* isMemoryBlockFunction(const SgNode* node) {
  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  if (fnRef == NULL) return NULL;
  if (isCallOfFunctionNamed(fnRef, "memcpy") ||
      isCallOfFunctionNamed(fnRef, "memmove") ||
      isCallOfFunctionNamed(fnRef, "memset"))
    return fnRef;
  return NULL;
}

/**
 * Checks to see if the variable is being written to by some kind of
 * assignment or ++/-- operator
 *
 * \todo Consider merging this with isAssignToVar
 */
bool varWrittenTo(const SgNode* var) {
	assert(var);
	const SgNode* parent = var;
	const SgNode* child = NULL;

	while (1) {
		assert(parent);
		child = parent;
		parent = child->get_parent();
		if (!parent)
			return false;

		if (isSgCastExp(parent)
		|| isSgAddressOfOp(parent)) {
			continue;
		} else if (isAnyAssignOp(parent)) {
			if (isSgBinaryOp(parent)->get_lhs_operand() == child)
				return true;
		} else if(isSgMinusMinusOp(parent)
		|| isSgPlusPlusOp(parent)) {
			return true;
		}
		return false;
	}
}


/**
 * Checks to see if node is an assignment with var as the lhs and not in
 * the rhs
 *
 * \todo node should be limited to SgExpression
 */
bool isAssignToVar( const SgNode *node, const SgInitializedName *var) {
	const SgBinaryOp *assignOp = isAnyAssignOp(node);
	if (!assignOp)
		return false;

	// Ensure that we are assigning to the variable in the LHS
	const SgVarRefExp *lhsVar = isSgVarRefExp(assignOp->get_lhs_operand());
	/**
	 * \todo LHS Could be more complicated than a variable
	 */
	if (!lhsVar || (var != getRefDecl(lhsVar)))
		return false;

	// Ensure variable does not appear in RHS
	FOREACH_SUBNODE(assignOp->get_rhs_operand(), nodes, i, V_SgVarRefExp) {
		const SgVarRefExp *rhsVar = isSgVarRefExp(*i);
		assert(rhsVar);
		if (var == getRefDecl(rhsVar))
			return false;
	}
	return true;
}


/**
 * Takes a statement and sees if a variable is being compared to 0 inside
 *
 * \todo merge this with isTestForNull
 * \todo merge this with valueVerified
 */
bool isCheckForZero(const SgStatement *stat, const SgVarRefExp *varRef) {
	if (!stat)
		return false;

	const SgVarRefExp *compareVar;
	const SgExpression *lhs;
	const SgExpression *rhs;
	FOREACH_SUBNODE(stat, nodes, i, V_SgBinaryOp) {
		assert(*i);
		if(!(isSgEqualityOp(*i) || isSgNotEqualOp(*i)))
			continue;
		lhs = removeImplicitPromotions(isSgBinaryOp(*i)->get_lhs_operand());
		rhs = removeImplicitPromotions(isSgBinaryOp(*i)->get_rhs_operand());
		if ((compareVar = isSgVarRefExp(lhs))
		&&  (compareVar->get_symbol()->get_name() == varRef->get_symbol()->get_name())
		&& isZeroVal(rhs))
			return true;
		if ((compareVar = isSgVarRefExp(rhs))
		&&  (compareVar->get_symbol()->get_name() == varRef->get_symbol()->get_name())
		&& isZeroVal(lhs))
			return true;
	}

	return false;
}

/**
 * Tries to find sizeof(type)
 *
 * \note this function only works on the basic types, nothing fancy
 *
 * \return 0 on error
 */
size_t sizeOfType(const SgType *type) {
	const SgType *t = type->stripTypedefsAndModifiers();
	if (isSgTypeBool(t)) return sizeof(bool);
	else if (isSgTypeChar(t)) return sizeof(char);
	else if (isSgTypeDouble(t)) return sizeof(double);
	else if (isSgTypeLongDouble(t)) return sizeof(long double);
	else if (isSgTypeFloat(t)) return sizeof(float);
	else if (isSgTypeInt(t)) return sizeof(int);
	else if (isSgTypeLong(t)) return sizeof(long);
	else if (isSgTypeLongDouble(t)) return sizeof(long double);
	else if (isSgTypeLongLong(t)) return sizeof(long long);
	else if (isSgTypeShort(t)) return sizeof(short);
	else if (isSgTypeSignedChar(t)) return sizeof(signed char);
	else if (isSgTypeSignedInt(t)) return sizeof(signed int);
	else if (isSgTypeSignedLong(t)) return sizeof(signed long);
	else if (isSgTypeSignedShort(t)) return sizeof(signed short);
	else if (isSgTypeUnsignedChar(t)) return sizeof(unsigned char);
	else if (isSgTypeUnsignedInt(t)) return sizeof(unsigned int);
	else if (isSgTypeUnsignedLong(t)) return sizeof(unsigned long);
	else if (isSgTypeUnsignedShort(t)) return sizeof(unsigned short);
	else if (isSgTypeWchar(t)) return sizeof(wchar_t);
	else if (isSgPointerType(t)) return sizeof(void *);
	else return 0;
}


/**
 * Checks to see if there was a comparison involving the value on the previous
 * line.  If the value is an expression or not a variable, return true.
 */
bool valueVerified(const SgExpression *expr) {
	/**
	 * Allow compile time known values
	 */
	if (isSgValueExp(expr))
		return true;

	const SgVarRefExp *varRef = isSgVarRefExp(expr);
	if (!varRef)
		return true;
	const SgInitializedName *var = getRefDecl(varRef);
	assert(var);

	const SgStatement *prevSt = findInBlockByOffset(expr, -1);
	if (prevSt) {
		FOREACH_SUBNODE(prevSt, nodes, i, V_SgBinaryOp) {
			const SgBinaryOp *iOp = isSgBinaryOp(*i);
			if (!iOp)
				continue;
			if (!isAnyRelationalOp(iOp))
				continue;

			const SgVarRefExp *iVar = isSgVarRefExp(removeCasts(iOp->get_lhs_operand()));
			if (iVar && (getRefDecl(iVar) == var))
				return true;
			iVar = isSgVarRefExp(removeCasts(iOp->get_lhs_operand()));
			if (iVar && (getRefDecl(iVar) == var))
				return true;
		}
	}

	return false;
}


/**
 * Given a node in a block, returns the node at the top of the block.
 *
 * \note This looks for a lot of blocks, but maybe not all of them?
 */
const SgNode *popBlock(const SgNode *node) {
  if(node == NULL)
    return NULL;
  const SgNode *parent = (node)->get_parent();
  for (; parent; parent = parent->get_parent() ) {
    VariantT variant = parent->variantT();

    //Fall through if we catch something
    switch(variant) {
    case V_SgIfStmt:
    case V_SgBasicBlock:
    case V_SgWhileStmt:
    case V_SgDoWhileStmt:
    case V_SgSwitchStatement:
    case V_SgTryStmt:
    case V_SgCatchOptionStmt:
    case V_SgCaseOptionStmt:
    case V_SgForAllStatement:

      return parent;

    default:
      break;
    }
  }

  return parent;
}


/** Do something with this node, then visit its successors, in a bfs order */
void CFGVisitor::visit(const VirtualCFG::CFGNode& node) {
  if (visited.find(node) != visited.end()) return; // don't revisit the same nodes
  if (node.getNode() == NULL) return;
  if (!doSomething( node.getNode())) return;
  visitOthers( node);
}

/** Visit node's successors */
void CFGVisitor::visitOthers(const VirtualCFG::CFGNode& node) {
  visited.insert( node);
  Rose_STL_Container<VirtualCFG::CFGEdge> children = node.outEdges();
  Rose_STL_Container<VirtualCFG::CFGEdge>::iterator i;
  for (i = children.begin(); i != children.end(); ++i) visit( i->target());
  // TODO: currently visited nodes are not erased...have a 'reset' method
  // to let a visitor do multiple visits
}

/**
 * Class that returns list of next references to a variable
 */
class CFGNextVarRef : public CFGVisitor {
private:
  const SgInitializedName* variable_;
public:
  Rose_STL_Container<const SgVarRefExp*> result_;

  CFGNextVarRef(const SgVarRefExp* ref)
    : variable_( getRefDecl( ref)) {}

  virtual bool doSomething(const SgNode* node);
};

bool CFGNextVarRef::doSomething(const SgNode* node) {
  const SgVarRefExp* ref = isSgVarRefExp(node);
  if (ref == NULL) return true;
  const SgInitializedName* ref_var = getRefDecl( ref);
  if (ref_var != variable_) return true;
  result_.push_back( ref);
  return false;
}

/**
 * /param ref  Reference to a variable
 * /return list of succeeding variable references
 */
Rose_STL_Container<const SgVarRefExp*> next_var_references(const SgVarRefExp* ref) {
  CFGNextVarRef visitor( ref);
  visitor.visitOthers( const_cast< SgVarRefExp*>(ref)->cfgForEnd());
  return visitor.result_;
}

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
 * \file ARR.C
 */

#include "rose.h"
#include "utilities.h"
#include "type.h"

using namespace std;

static bool arr37check(const SgNode *node, const SgExpression *rhs);

/**
 *  Do not apply the sizeof operator to an object of pointer type
 */
bool ARR001_C(const SgNode * node) {
	const SgSizeOfOp *sizeOfOp = isSgSizeOfOp(node);
	if (!sizeOfOp)
		return false;

	const SgExpression *expr = removeImplicitPromotions(
                                                      sizeOfOp->get_operand_expr());
	if (!expr)
		return false;

	/*
	 * if the expression is of a typedefined type, then get_type() returns
	 * an SgTypedefType and not the real type of the object so we need to
	 * call the get_base_type() function
	 */
	const SgType *type = expr->get_type();
	const SgTypedefType *typedeffed = isSgTypedefType(type);
	if (typedeffed)
		type = typedeffed->get_base_type();

	/*while (isSgUnaryOp(expr)){
    expr = removeImplicitPromotions(isSgUnaryOp(expr)->get_operand());
    }
    expr = removeImplicitPromotions(expr); */

	/* We have to pay some attention to array typed objects.  When an array is
     passed as a function parameter, it is passed by reference and is
     treated as a pointer.  Thus calling sizeof(var) where var is an array
     and also parameter to the function effectively violates ARR001_C.
     Here we check this using the same methodology as in ARR01_C below.  If
     this overlap is not intended (through NCCEs for ARR001_C suggest it
     is), simply remove the code block below dealing with arrT.
  */

	const SgArrayType *arrT = isSgArrayType(type);
	if (arrT) {
		/*
		 * On binary operations, get_type() returns type of lhs argument,
		 * so if this is an array, then it will decay into a pointer
		 */
		if (isSgBinaryOp(expr) && !(isSgDotExp(expr) || isSgArrowExp(expr)
                                || isSgDotStarOp(expr) || isSgArrowStarOp(expr))) {
			print_error(node, "ARR001_C",
                  "Do not apply the sizeof operator to an object of pointer",
                  true);
		}

		const SgVarRefExp *var = isSgVarRefExp(expr);
		if (!var) {
			return false;
		}
		const SgFunctionDeclaration *fnRef =
      findParentOfType(node, SgFunctionDeclaration);
		if (!fnRef) {
			return false;
		}
		const SgName & varName = var->get_symbol()->get_name();
		FOREACH_INITNAME(fnRef->get_args(), i) {
			if ((*i)->get_name() == varName) {
				print_error(
                    node,
                    "ARR001-C",
                    "Do not apply the sizeof operator to an object of pointer",
                    true);
				return true;
			}
		}
	}

	/* If expr has pointer type, then it is a violation of this rule, unless
     this node is the second operand of a binary / or * operator.  Those
     cases are exempt to allow the "sizeof(arr)/sizeof(arr[0])" and
     "num*sizeof(arr[0])" practices to be used even when arr is an array
     of pointer type.
  */
	const SgPointerType *ptrT = isSgPointerType(type);
	if (ptrT) // if true is either violation or exception
    {
      /*
       * Weed out false positives: check if pointer of type T * is argument to
       * function call whose result is being assigned to a variable of type T **.
       * For example:
       * char *ptr;
       * char **array_of_ptrs = malloc(100 * sizeof(ptr));
       */
      const SgFunctionCallExp *fn = findParentOfType(expr, SgFunctionCallExp);
      if (fn) {
        const SgPointerType *lhsType = NULL;
        const SgAssignOp *assignOp = findParentOfType(fn, SgAssignOp);
        if (assignOp) {
          lhsType = isSgPointerType(assignOp->get_type());
        } else {
          const SgInitializedName *init =
						findParentOfType(fn, SgInitializedName);
          if (init) {
            lhsType = isSgPointerType(init->get_type());
          }
        }
        if (lhsType) {
          const SgType *lhsBase = lhsType->get_base_type();
          if (isSgTypedefType(lhsBase))
            lhsBase
							= ((const SgTypedefType *) lhsBase)->get_base_type();
          const SgType *exprBase = type;
          while (isSgPointerType(lhsBase) && isSgPointerType(exprBase)) {
            lhsBase
							= ((const SgPointerType *) lhsBase)->get_base_type();
            exprBase
							= ((const SgPointerType *) exprBase)-> get_base_type();
          }
          if (lhsBase->class_name().compare(exprBase->class_name()) == 0)
            return false;
        }
        /*
         * also ignore cases when the function being called is of the form:
         * memcpy(T **, T **, n*sizeof(T *));
         * memmove(T **, T **, n*sizeof(T *));
         * memset(T **, val, n*sizeof(T *));
         * memchr(T **, val, n*sizeof(T *));
         * memcmp(T **, T **, n*sizeof(T *));
         * memset_s(T **, T **, n*sizeof(T *));
         * We will check that the function name is one of these five functions.
         *
         * We will be lazy and just check that the first argument is a T **.
         */
        FOREACH_SUBNODE(fn, nodes, i, V_SgFunctionRefExp) {
          const SgFunctionRefExp *ref = isSgFunctionRefExp(*i);
          const string name = ref->get_symbol()->get_name().getString();
          if (!((name.compare("memcpy") == 0)
                || (name.compare("memmove") == 0)
                || (name.compare("memset") == 0)
                || (name.compare("memchr") == 0)
                || (name.compare("memcmp") == 0)
                || (name.compare("memset_s") == 0)))
            {
              break;
            }
          const SgExpressionPtrList &args = fn->get_args()->get_expressions();
          const SgPointerType *arg0type = isSgPointerType(removeCasts(args[0])->get_type());
          if (arg0type) {
            const SgType *arg0base = arg0type->get_base_type();
            if (isSgTypedefType(arg0base))
              arg0base = ((const SgTypedefType *)arg0base)->get_base_type();
            const SgType *exprBase = type;
            while (isSgPointerType(arg0base) && isSgPointerType(exprBase)) {
              arg0base
                = ((const SgPointerType *) arg0base)->get_base_type();
              exprBase
                = ((const SgPointerType *) exprBase)-> get_base_type();
            }
            if (arg0base->class_name().compare(exprBase->class_name()) == 0)
              return false;
          }
          break;
        }
      }
      const SgPntrArrRefExp *ptArrE = isSgPntrArrRefExp(expr);
      if (ptArrE) // possible exception; investigate further
        {

          // try to access grandparent node of node
          // where we will find * or / in exception cases
          const SgNode *parent = node->get_parent();
          const SgNode *gParent = parent ? parent->get_parent() : NULL;
          if (gParent) {
            if (isSgMultiplyOp(gParent)) {
              return false;
            }
            const SgDivideOp *divop = isSgDivideOp(gParent);
            if (divop && (divop->get_rhs_operand() == parent)) {
              return false;
            }
          }
        }
      // no exception found => violation
      print_error(node, "ARR001_C",
                  "Do not apply the sizeof operator to an object of pointer",
                  true);
      return true;
    }
	return false;
}

/**
 * Do not apply the sizeof operator to a pointer when taking the size of an
 * array
 *
 * We find cases of sizeof(var) where var is an array and is also a function
 * parameter
 */
bool ARR01_C(const SgNode * node) {
	const SgSizeOfOp *sizeOfOp = isSgSizeOfOp(node);
	if (!sizeOfOp)
		return false;
	const SgExpression *expr = removeImplicitPromotions(
                                                      sizeOfOp->get_operand_expr());
	if (!expr)
		return false;
	if (isSgUnaryOp(expr))
		expr = isSgUnaryOp(expr)->get_operand();
	const SgVarRefExp *var = isSgVarRefExp(removeImplicitPromotions(expr));
	if (!var)
		return false;
	const SgArrayType *arrT = isSgArrayType(var->get_type());
	if (!arrT)
		return false;
	const SgFunctionDeclaration *fnRef =
    findParentOfType(node, SgFunctionDeclaration);
	if (!fnRef)
		return false;
	const SgName & varName = var->get_symbol()->get_name();
	FOREACH_INITNAME(fnRef->get_args(), i) {
		if ((*i)->get_name() == varName) {
			print_error(
                  node,
                  "ARR01-C",
                  "Do not apply the sizeof operator to a pointer when taking the size of an array",
                  true);
			return true;
		}
	}
	return false;
}

/**
 * Explicitly specify array dimensions, even if implicitly defined by an
 * initializer
 *
 * \see STR36_C
 */
bool ARR02_C(const SgNode * node) {
	const SgVariableDeclaration *varDecl = isSgVariableDeclaration(node);
	if (!varDecl)
		return false;
	FOREACH_INITNAME(varDecl->get_variables(), i) {
		assert(*i);
		const SgArrayType *varType = isSgArrayType((*i)->get_type());
		if (!varType)
			continue;

		// Ignore chars as per STR36
		if (isAnyCharType(varType->get_base_type()))
			continue;
		if (!(*i)->get_initializer())
			continue;
		if (!varType->get_index()) {
			print_error(
                  *i,
                  "ARR02-C",
                  "Explicitly specify array dimensions, even if implicitly defined by an initializer",
                  true);
			return true;
		}
	}
	return false;
}

/**
 * Guarantee that array indices are within the valid range
 *
 * \note Without tainting, the only thing we can really do is ensure that if a
 * value is already getting checked and is signed, that it's also being
 * checked against 0.
 *
 * \bug This doesn't work with if statement blocks (i.e. with {}) because
 * of the SgBasicBlock that is thrown in.
 */
bool ARR30_C(const SgNode * node) {
	if (isCompilerGeneratedNode(node))
		return false;
	const SgPntrArrRefExp *deref = isSgPntrArrRefExp(node);
	if (!deref)
		return false;
	const SgVarRefExp *varRef = isSgVarRefExp(removeCasts(
                                                        deref->get_rhs_operand()));
	if (!varRef)
		return false;
	if (varRef->get_type()->stripTypedefsAndModifiers()->isUnsignedType())
		return false;

	/**
	 * \note Technically we should throw a violation here since there's no
	 * excuse to have signed array indices, but that would cause too many
	 * false positives
	 */
	const SgInitializedName *var = getRefDecl(varRef);
	assert(var);
	const SgStatement *prevSt = findInBlockByOffset(varRef, -1);
	if (!prevSt || isSgForStatement(prevSt))
		return false;
	if (isSgIfStmt(prevSt)) {
		prevSt = isSgIfStmt(prevSt)->get_conditional();
	} else if (isSgWhileStmt(prevSt)) {
		prevSt = isSgWhileStmt(prevSt)->get_condition();
	}
	bool check = false;
	FOREACH_SUBNODE(prevSt, nodes, i, V_SgBinaryOp) {
		const SgBinaryOp *op = isAnyComparisonOp(*i);
		if (!op)
			continue;
		const SgExpression *lhs = removeCasts(op->get_lhs_operand());
		const SgExpression *rhs = removeCasts(op->get_rhs_operand());
		assert(lhs && rhs);
		const SgVarRefExp *lhsVar = isSgVarRefExp(lhs);
		const SgVarRefExp *rhsVar = isSgVarRefExp(rhs);
		if (lhsVar && (getRefDecl(lhsVar) == var)) {
			if (isZeroVal(rhs))
				return false;
			check = true;
		} else if (rhsVar && (getRefDecl(rhsVar) == var)) {
			if (isZeroVal(lhs))
				return false;
			check = true;
		}
	}
	if (check) {
		print_error(node, "ARR30-C",
                "Do not form or use out-of-bounds pointers or array subscripts");
		return true;
	}
	return false;
}


/**
 * Do not add or subtract an integer to a pointer to a non-array object
 */
bool ARR37_C(const SgNode * node) {
	const SgVarRefExp *varRef = isSgVarRefExp(node);
	if (!varRef || !isSgPointerType(varRef->get_type()))
		return false;
	const SgNode *parent = removeCasts(isSgExpression(varRef->get_parent()));
	if (!parent)
		return false;

	/**
	 *  See if we have a case of pointer arithmetic
	 */
	if (!(isSgPlusPlusOp(parent) || isSgMinusMinusOp(parent) || isSgAddOp(
                                                                        parent) || isSgSubtractOp(parent) || isSgPlusAssignOp(parent)
        || isSgMinusAssignOp(parent)))
		return false;

	/* Search forward to find the varRef */
	FOREACH_SUBNODE(varRef->get_symbol()->get_scope(), nodes, i,
                  V_SgVarRefExp) {
		const SgVarRefExp *iVar = isSgVarRefExp(*i);
		assert(iVar);
		if (iVar == varRef)
			break;
	}

	if (i != nodes.begin()) {
		/* Now walk backwards looking at the assignments to our variable */
		bool flag = true;
		for (--i; flag; --i) {
			/* We have to be  a little tricky here since we can't do >= begin */
			if (i == nodes.begin())
				flag = false;
			const SgVarRefExp *iVar = isSgVarRefExp(*i);
			assert(iVar);
			if (getRefDecl(iVar) != getRefDecl(varRef))
				continue;
			parent = removeCasts(isSgExpression(iVar->get_parent()));
			if (!parent)
				continue;
			const SgExpression *rhs = NULL;
			if (isSgAssignInitializer(parent)) {
				rhs = isSgAssignInitializer(parent)->get_operand();
			} else if (isSgAssignOp(parent)) {
				rhs = isSgBinaryOp(parent)->get_rhs_operand();
			} else {

				/* Hopefully this is just a reference to var */
				continue;
			}
			return arr37check(node, rhs);
		}
	}
	/* now check if there was an assignment in the declaration */
	const SgInitializedName *decl = getRefDecl(varRef);
	const SgExpression *init = decl->get_initializer();
	if (init) {
		const SgAssignInitializer *assignInitOp = isSgAssignInitializer(init);
		if (assignInitOp) {
			const SgExpression *rhs = assignInitOp->get_operand();
			return arr37check(node, rhs);
		}
	}
	return false;
}

static bool arr37check(const SgNode *node, const SgExpression *rhs) {
	assert(rhs);
	rhs = removeImplicitPromotions(rhs);
	if (isSgAddressOfOp(rhs)) {
		FOREACH_SUBNODE(rhs, rhs_nodes, j, V_SgExpression) {
			if (isSgPntrArrRefExp(*j) || isSgArrayType(
                                                 isSgExpression((*j))->get_type()))
				return false;
		}
		print_error(node, "ARR37-C",
                "Do not add or subtract an integer to a pointer to a non-array object");
		return true;
	}
	return false;
}

bool ARR_C(const SgNode * node) {
	bool violation = false;

	violation |= ARR01_C(node);
	violation |= ARR02_C(node);
	violation |= ARR30_C(node);
	violation |= ARR37_C(node);
	violation |= ARR001_C(node);
	return violation;
}

/// C++ checkers
bool ARR_CPP(const SgNode * node) {
	bool violation = false;
	violation |= ARR_C(node);
	return violation;
}

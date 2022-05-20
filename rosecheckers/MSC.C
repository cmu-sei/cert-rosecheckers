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
 * \file MSC.C
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"
#include "utilities_cpp.h"

/**
 * Strive for logical completeness
 */
bool MSC01_C(const SgNode *node) {
	const SgSwitchStatement *swch = isSgSwitchStatement(node);
	if (!swch) {
          return false;
        }
        if (switchHasDefault(swch)) {
          return false;
        }
	print_error(node, "MSC01-C", "Strive for logical completeness", true);
	return true;
}

/**
 * Avoid errors of addition
 *
 * \note Because of a problem with the expansion of isnan/isless/etc, this
 * rule is disabled, ROSE catches most of this on it's own, so this should not
 * be a problem
 */
bool MSC03_C(const SgNode *node) {
	/* This rule has been deprecated in favor of the following rules
	 * EXP15-C. Do not place a semicolon on the same line as an if, for, or while statement
   * MSC12-C. Detect and remove code that has no effect
	 *
	 * Currently, only MSC12-C is actually enabled by rosecheckers.
	 */
	return false;
}

/**
 * Do not manipulate time_t typed values directly
 */
bool MSC05_C(const SgNode *node) {
	if (isAnyBinArithOp(node)
      ||isAnyBinArithAssignOp(node)
      ||isAnyBinBitOp(node)
      ||isAnyBinBitAssignOp(node)
      ||isAnyRelationalOp(node)) {
		const SgBinaryOp *binOp = isSgBinaryOp(node);
		assert(binOp);
		const SgType *lhsT = binOp->get_lhs_operand()->get_type();
		const SgType *rhsT = binOp->get_rhs_operand()->get_type();
		if (!(isTypeTimeT(lhsT) || isTypeTimeT(rhsT)))
			return false;
	} else if (isSgBitComplementOp(node)
             ||isSgMinusMinusOp(node)
             ||isSgNotOp(node)
             ||isSgPlusPlusOp(node)
             ||isSgUnaryAddOp(node)
             ||isSgMinusOp(node)) {
		const SgUnaryOp *op = isSgUnaryOp(node);
		assert(op);
		const SgType *opT = op->get_operand()->get_type();
		if (!isTypeTimeT(opT))
			return false;
	} else {
		return false;
	}

	print_error(node, "MSC05-C", "Do not manipulate time_t typed values directly", true);
	return true;
}

/**
 * Detect and remove code that has no effect
 */
bool MSC12_C(const SgNode *node) {
	const SgExprStatement *exprStmt = isSgExprStatement(node);
	if (exprStmt) {
		/**
		 * Check for statements with no effect
		 */
		const SgBasicBlock *parent = isSgBasicBlock(exprStmt->get_parent());
		if (!parent)
			return false;

		/**
		 * Ignore the last statement in a block because it could be an implicit
		 * return value, this is GNU extension
		 */
		if ((exprStmt == isSgExprStatement(parent->get_statements().back()))
        &&  (!isSgFunctionDefinition(parent->get_parent())))
			return false;

		const SgExpression *expr = exprStmt->get_expression();
		assert(expr);

		if (isCompilerGeneratedNode(expr))
			return false;

		if (isSgFunctionCallExp(expr)
        || isSgAssignOp(expr)
        || isSgConditionalExp(expr)
        || isAnyAssignOp(expr)
        || isSgPointerDerefExp(expr)
        || isSgPlusPlusOp(expr)
        || isSgMinusMinusOp(expr)
        || isSgDeleteExp(expr))
			return false;
	} else {
		return false;
	}

	print_error(node, "MSC12-C", "Detect and remove code that has no effect", true);
	return true;
}

/**
 * Detect and remove unused values
 *
 * \bug Disabled until a better algorithm can be found
 */
bool MSC13_C(const SgNode *node) {
	const SgInitializedName *var = isSgInitializedName(node);
	if (!var)
		return false;
	bool unused = false;
	bool violation = false;
	if (var->get_initializer())
		unused = true;
	const SgNode * prev = var;
	FOREACH_SUBNODE(var->get_scope(), nodes, i, V_SgVarRefExp) {
		const SgVarRefExp *iVar = isSgVarRefExp(*i);
		if (!iVar || (getRefDecl(iVar) != var))
			continue;
		if (varWrittenTo(iVar) && (!findParentOfType(iVar, SgPointerDerefExp))) {
			if (unused) {
				print_error(prev, "MSC13-C", "Detect and remove unused values", true);
				violation = true;
			} else {
				unused = true;
				prev = iVar;
			}
		} else {
			unused = false;
		}
	}
	return violation;
}

/**
 * Do not use rand()
 */
bool MSC30_C(const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "rand")))
		return false;
	print_error(node, "MSC30-C", "Do not use the rand() function for generating pseudorandom numbers");
	return true;
}

bool MSC_C(const SgNode *node) {
  bool violation = false;
  violation |= MSC01_C(node);
  violation |= MSC05_C(node);
  violation |= MSC12_C(node);
  //  violation |= MSC13_C(node);
  violation |= MSC30_C(node);
  return violation;
}


/// C++ checkers

bool MSC_CPP(const SgNode *node) {
  bool violation = false;
  violation |= MSC_C(node);
  return violation;
}

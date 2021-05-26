/*
 * <legal>
 * CERT Rosecheckers
 * Copyright 2007-2021 Carnegie Mellon University.
 *
 * NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 * INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 * UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 * IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 * FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 * OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT
 * MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 * TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 *
 * Released under a BSD (SEI)-style license, please see license.txt or
 * contact permission@sei.cmu.edu for full terms.
 *
 * [DISTRIBUTION STATEMENT A] This material has been approved for public
 * release and unlimited distribution.  Please see Copyright notice for
 * non-US Government use and distribution.
 *
 * CERT® is registered in the U.S. Patent and Trademark Office by
 * Carnegie Mellon University.
 *
 * This Software includes and/or makes use of the following Third-Party
 * Software subject to its own license:
 *
 * 1. rose
 *    (https://github.com/rose-compiler/rose/blob/weekly/LicenseInformation/ROSE_BSD_License.txt)
 *    Copyright 2005 Lawrence Livermore National Laboratory.
 *
 * DM21-0505
 * </legal>
 */

/**
 * \file FLP.C
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"

/**
 * Consider avoiding floating point numbers when precise computation is needed
 */
bool FLP02_C(const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if (!op)
		return false;
	if (!(isSgEqualityOp(op)
		||isSgNotEqualOp(op)))
		return false;

	const SgExpression *lhs = op->get_lhs_operand();
	assert(lhs);
	const SgExpression *rhs = op->get_rhs_operand();
	assert(rhs);

	if (lhs->get_type()->isFloatType()
	  ||rhs->get_type()->isFloatType()) {
		if (isZeroVal(removeCasts(lhs))
		||  isZeroVal(removeCasts(rhs)))
			return false;

		print_error(node, "FLP02-C", "Consider avoiding floating-point numbers when precise computation is needed", true);
		return true;
	}
	return false;
}

/**
 * Detect and handle floating-point errors
 *
 * \bug ROSE can't handle the FENV_ACCESS pragma :(so there's no way to a
 * have a compliant solution here
 */
bool FLP03_C(const SgNode *node ) {
	if (isCompilerGeneratedNode(node))
		return false;

	/**
	 * We are looking for floating point mult, divide (with non const/value on
	 * rhs), and floating point casts from double->float
	 */
	if (isSgBinaryOp(node)) {
		const SgBinaryOp *op = isSgBinaryOp(node);
		assert(op);
		const SgType *lhsT = removeCasts(op->get_lhs_operand())->get_type();
		const SgType *rhsT = removeCasts(op->get_rhs_operand())->get_type();
		if (!(lhsT->isFloatType() || rhsT->isFloatType()))
			return false;
		bool lhsKnown = isSgValueExp(removeCasts(op->get_lhs_operand())) || isConstType(lhsT);
		bool rhsKnown = isSgValueExp(removeCasts(op->get_rhs_operand())) || isConstType(rhsT);
		if (isSgDivideOp(op)) {
			if (rhsKnown)
				return false;

			/** Also check for a divde-by-zero check on the previous line */
			const SgStatement *prevSt = findInBlockByOffset(node, -1);
			const SgVarRefExp *varRef = isSgVarRefExp(removeCasts(op->get_rhs_operand()));
			if (prevSt && varRef) {
				FOREACH_SUBNODE(prevSt, nodes, i, V_SgStatement) {
					if (isCheckForZero(isSgStatement(*i), varRef))
					return false;
				}
			}
		} else if (isSgMultiplyOp(op)) {
			if (lhsKnown && rhsKnown)
				return false;
		} else {
			return false;
		}
	} else if (isSgCastExp(node)) {
		const SgCastExp *cast = isSgCastExp(node);
		assert(cast);
		const SgType *lhsT = cast->get_type();
		const SgType *rhsT = cast->get_operand()->get_type();
		/**
		 * Bail if we're not dealing with floating point, or if the types are
		 * compatible
		 */
		if (!lhsT->isFloatType() || !rhsT->isFloatType())
			return false;
		if (!((isSgTypeDouble(lhsT) && isSgTypeFloat(rhsT))
			||(isSgTypeLongDouble(rhsT) && !isSgTypeLongDouble(lhsT))))
			return false;
	} else {
		return false;
	}

	const SgStatement *prevSt = findInBlockByOffset(node, -1);
	bool no_feclearexcept = true;
	if (prevSt) {
		FOREACH_SUBNODE(prevSt, nodes, i, V_SgFunctionRefExp) {
			if (isCallOfFunctionNamed(isSgFunctionRefExp(*i), "feclearexcept"))
				no_feclearexcept = false;
		}
	}

	const SgStatement *nextSt = findInBlockByOffset(node,  1);
	bool no_fetestexcept = true;
	if (nextSt) {
		FOREACH_SUBNODE(nextSt, nodes, i, V_SgFunctionRefExp) {
			if (isCallOfFunctionNamed(isSgFunctionRefExp(*i), "fetestexcept"))
				no_fetestexcept = false;
		}
	}

	if (no_feclearexcept || no_fetestexcept) {
		print_error(node, "FLP03-C", "Detect and handle floating point errors", true);
		return true;
	}

	return false;
}

/**
 * Do not use floating point variables as loop counters
 */
bool FLP30_C( const SgNode *node ) {
	const SgForStatement *forSt = isSgForStatement(node);
	if (!forSt)
		return false;

	/* Check test */
	FOREACH_SUBNODE(forSt->get_test(), nodes, i, V_SgVarRefExp) {
		if (isSgVarRefExp(*i)->get_type()->isFloatType()) {
			print_error(*i, "FLP30-C", "Do not use floating point variables as loop counters");
			return true;
		}
	}

	/* Check increment */
	FOREACH_SUBNODE(forSt->get_increment(), nodes2, i2, V_SgVarRefExp) {
		if (isSgVarRefExp(*i2)->get_type()->isFloatType()) {
			print_error(*i2, "FLP30-C", "Do not use floating point variables as loop counters");
			return true;
		}
	}

	return false;
}

/**
 * Do not call functions expecting real values with complex values
 */
bool FLP31_C( const SgNode *node ) {
	bool violation = false;;
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;
	if (!isSgFunctionCallExp(fnRef->get_parent()))
		return false;
	std::string str = fnRef->get_symbol()->get_name().getString();
	/* one arg */
	if ((str == "cbrt")
	||  (str == "ceil")
	||  (str == "erf")
	||  (str == "erfc")
	||  (str == "exp2")
	||  (str == "expm1")
	||  (str == "floor")
	||  (str == "frexp")
	||  (str == "ilogb")
	||  (str == "ldexp")
	||  (str == "lgamma")
	||  (str == "llrint")
	||  (str == "llround")
	||  (str == "log10")
	||  (str == "log1p")
	||  (str == "log2")
	||  (str == "logb")
	||  (str == "lrint")
	||  (str == "lround")
	||  (str == "nearbyint")
	||  (str == "rint")
	||  (str == "round")
	||  (str == "tgamma")
	||  (str == "trunc")) {
		violation = isSgTypeComplex(getFnArg(isSgFunctionRefExp(node), 0)->get_type());
	} else
	/* two args */
	if ((str == "atan2")
	||  (str == "copysign")
	||  (str == "fdim")
	||  (str == "fmax")
	||  (str == "fmin")
	||  (str == "fmod")
	||  (str == "hypot")
	||  (str == "nextafter")
	||  (str == "nexttoward")
	||  (str == "remainder")
	||  (str == "remquo")
	||  (str == "scalbn")
	||  (str == "scalbln")) {
	} else
	/* three args */
	if ((str == "fma")) {
	}
	if (violation) {
		print_error(node, "FLP31-C", "Do not call functions expecting real values with complex values");
		return true;
	}
	return false;
}

/**
 * Convert integers to floating point for floating point operations
 *
 * \see INT07_C
 */
bool FLP33_C( const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	const SgInitializedName *var = isSgInitializedName(node);
	const SgType *lhsT;
	const SgType *rhsT;

	if (binOp) {
		/**
		 * \todo Does this rule even make sense for binary ops?
		 */
		/**
		 * This should allow macros like isnan
		 */
		if (isCompilerGeneratedNode(binOp))
			return false;

		if (!binOp->get_type()->isFloatType())
			return false;
		if (isSgPntrArrRefExp(binOp))
			return false;

		lhsT = binOp->get_lhs_operand()->get_type()->stripTypedefsAndModifiers();
		/**
		 * Let's leave those casts in place to allow for macros like isnan
		 */
		rhsT = binOp->get_rhs_operand()->get_type()->stripTypedefsAndModifiers();
//		rhsT = removeImplicitPromotions(binOp->get_rhs_operand())->get_type();
		assert(lhsT);
		assert(rhsT);
		if (lhsT->isFloatType() || rhsT->isFloatType()) {
			return false;
		}
	} else if (var) {
		lhsT = var->get_type()->stripTypedefsAndModifiers();
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if (!init)
			return false;
		rhsT = removeImplicitPromotions(init->get_operand())->get_type()->stripTypedefsAndModifiers();

		assert(lhsT);
		assert(rhsT);
		if (!(lhsT->isFloatType() && rhsT->isIntegerType()))
			return false;
	} else
		return false;


	print_error(node, "FLP33-C", "Convert integers to floating point for floating point operations");
			return true;
		}

/**
 * Ensure that floating point conversions are within range of the new type
 */
bool FLP34_C(const SgNode *node ) {
	/**
	 * \bug We really shouldn't have this cop-out here, but a lot of macros
	 * (ie. isnan) will trigger this rule otherwise
	 */
	if (isCompilerGeneratedNode(node))
		return false;

	const SgType *lhsType = NULL;
	const SgExpression *rhs = NULL;
	if (const SgBinaryOp *op = isSgBinaryOp(node)) {
		if (!isAnyAssignOp(op))
			return false;
		lhsType = op->get_lhs_operand()->get_type()->stripTypedefsAndModifiers();
		rhs = op->get_rhs_operand();
	} else if (const SgCastExp *cast = isSgCastExp(node)) {
		lhsType = cast->get_type()->stripTypedefsAndModifiers();
		rhs = cast->get_operand();
	} else {
		return false;
	}
	assert(lhsType && rhs);
	const SgType *rhsType = rhs->get_type()->stripTypedefsAndModifiers();
	assert(rhsType);
	if (!rhsType->isFloatType())
		return false;
	if (lhsType->isFloatType() && (sizeOfType(lhsType) >= sizeOfType(rhsType)))
		return false;

	if (valueVerified(removeCasts(rhs)))
		return false;

	print_error(node, "FLP34-C", "Ensure that floating point conversions are within range of the new type");
	return true;
}

bool FLP_C(const SgNode *node) {
  bool violation = false;
  violation |= FLP02_C(node);
  violation |= FLP03_C(node);
  violation |= FLP30_C(node);
  violation |= FLP31_C(node);
  violation |= FLP33_C(node);
  violation |= FLP34_C(node);
  return violation;
}


/// C++ checkers

bool FLP_CPP(const SgNode *node) {
  bool violation = false;
  violation |= FLP_C(node);
  return violation;
}

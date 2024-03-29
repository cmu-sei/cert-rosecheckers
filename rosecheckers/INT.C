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
 * \file INT.C
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"
#include <boost/regex.hpp>

/**
 * Use rsize_t or size_t for all integer values representing the size of an
 * object
 */
bool INT01_C(const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if (!op)
		return false;

	/* Ignore commas and dots */
	if (isSgCommaOpExp(op) || isSgDotExp(op))
		return false;

	/* Ignore array derefences */
	if (isSgPntrArrRefExp(op))
		return false;

	const SgExpression *lhs = op->get_lhs_operand();
	const SgExpression *rhs = op->get_rhs_operand();
	const SgType *lhsType = stripModifiers(lhs->get_type());
	const SgType *rhsType = stripModifiers(rhs->get_type());

	/**
	 * We should allow pointer arithmetic, and a special exception for
	 * uintptr_t because it in a typedef to unsigned int and is not actually a
	 * pointer type
	 */
	if (isAnyPointerType(lhsType)
      || isAnyPointerType(rhsType)) {
		return false;
	}

	/**
	 * scalars are ok
	 */
	if (isSgValueExp(op->get_lhs_operand())
      || isSgValueExp(op->get_rhs_operand()))
		return false;

	bool lhsIsSize = isTypeSizeT(lhsType);
	bool lhsIsRSize = isTypeRSizeT(lhsType);
	bool rhsIsSize = isTypeSizeT(rhsType);
	bool rhsIsRSize = isTypeRSizeT(rhsType);

	// Treat SizeOfOp as having type size_t even though compiler will assign
	// some kind of unsigned integer type to it
	if (isSgSizeOfOp(lhs))
		lhsIsSize = true;
	if (isSgSizeOfOp(rhs))
		rhsIsSize = true;

	// Common idiom is to do something like (sizeof(foo) - 1)
	const SgBinaryOp *lhsOp = isSgBinaryOp(lhs);
	if (lhsOp) {
		if ((isSgSizeOfOp(lhsOp->get_lhs_operand())
         && isSgValueExp(lhsOp->get_rhs_operand()))
        || ((isSgSizeOfOp(lhsOp->get_rhs_operand())
             && isSgValueExp(lhsOp->get_lhs_operand()))))
			lhsIsSize = true;
	}
	const SgBinaryOp *rhsOp = isSgBinaryOp(rhs);
	if (rhsOp) {
		if ((isSgSizeOfOp(rhsOp->get_rhs_operand())
         && isSgValueExp(rhsOp->get_lhs_operand()))
        || ((isSgSizeOfOp(rhsOp->get_lhs_operand())
             && isSgValueExp(rhsOp->get_rhs_operand()))))
			rhsIsSize = true;
	}

	if ((lhsIsSize ^ rhsIsSize) || (lhsIsRSize ^ rhsIsRSize)) {
		print_error(node,"INT01-C", "Use rsize_t or size_t for all integer values representing the size of an object", true);
		return true;
	}

	return false;
}

/**
 * Do not use input functions to convert character data if they cannot handle
 * all possible inputs
 */
bool INT05_C(const SgNode *node ) {
	const SgFunctionRefExp *ref = isSgFunctionRefExp(node);
	if (ref == NULL) return false;
	int numArg = getScanfFormatString(ref);
	if (numArg == -1) return false;

	const SgStringVal *format = isSgStringVal(removeCasts(getFnArg(ref,numArg)));
	if (!format)
		return false;

	const std::string formatStr = format->get_value();

	boost::regex r("\%.?[fDdiouxXegE]");
	if (regex_search(formatStr,r)) {
		print_error(node, "INT05-C", "Do not use input functions to convert character data if they cannot handle all possible inputs", true);
		return true;
	}

	return false;
}

/**
 * Use strtol() or a related function to convert a string token to an integer
 *
 * \note sscanf conversions are caught by INT05_C
 */
bool INT06_C(const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;
	if (isCallOfFunctionNamed(fnRef,"atoi")
			|| isCallOfFunctionNamed(fnRef,"atol")
			|| isCallOfFunctionNamed(fnRef,"atoll")
			|| isCallOfFunctionNamed(fnRef,"atoq")) {
		print_error(node,"INT05-C", "Use strtol() or a related function to convert a string token to an integer", true);
		return true;
	}

	return false;
}

/**
 * Use only explicitly signed or unsigned char type for numeric values
 *
 * \bug (char c = 'a') will trigger a false positive, work around by
 * using (char c; c = 'a')
 *
 * The AST trees for char c = 'a' and char c = '3' are the same...
 * Don't know how to solve this issue currently.
 */
bool INT07_C(const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	const SgInitializedName *var = isSgInitializedName(node);
	const SgType *lhsT;
	const SgType *rhsT;

	if (binOp) {
		lhsT = binOp->get_lhs_operand()->get_type();
		rhsT = removeImplicitPromotions(binOp->get_rhs_operand())->get_type();
	} else if (var) {
		lhsT = var->get_type();
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if (!init)
			return false;
		rhsT = removeCasts(init->get_operand())->get_type();
	} else return false;

	lhsT = lhsT->stripTypedefsAndModifiers();
	rhsT = rhsT->stripTypedefsAndModifiers();
  //	const Type &lhsType = Type(lhsT).stripTypedefsAndModifiers();
  //	const Type &rhsType = Type(rhsT).stripTypedefsAndModifiers();

	if (isSgTypeChar(lhsT) && !isSgTypeChar(rhsT)) {
    //	if (lhsType.isPlainChar() && (rhsType.isSigned()||rhsType.isUnsigned())) {
		/**
		 * Most of our false positives will come from getchar et al
		 * However, there is no good way to find those cases... there's just
		 * too many ways a getchar can get called
		 */
    /*		const SgNode * parent = findParentOfType(node, SgBasicBlock)->get_parent();
          const SgStatement *stat =
          isSgIfStmt(parent)? isSgIfStmt(parent)->get_conditional():
          isSgWhileStmt(parent)? isSgWhileStmt(parent)->get_condition():
          isSgDoWhileStmt(parent)? isSgDoWhileStmt(parent)->get_condition():
          isSgForStatement(parent)? isSgForStatement(parent)->get_test():
          NULL;

          if (stat) {
          FOREACH_SUBNODE(stat, nodes, i, V_SgAssignOp) {
          const SgAssignOp *assign = isSgAssignOp(*i);
          const SgFunctionRefExp *fn = isSgFunctionRefExp(assign->get_rhs_operand());
          if (isCallOfFunctionNamed(fn, "fgetc")
          || isCallOfFunctionNamed(fn, "gettc")
          || isCallOfFunctionNamed(fn, "getchar")) {
					return false;
          }
          }
          }*/

		print_error(node, "INT07-C", "Use only explicitly signed or unsigned char type for numeric values", true);
		return true;
	}
	return false;
}

/**
 * Ensure enumeration constants map to unique values
 * \todo implement INT09-EX1 when ROSE supports it
 */
bool INT09_C(const SgNode *node ) {
	const SgEnumDeclaration* enumDec = isSgEnumDeclaration(node);
	if (!enumDec)
		return false;

	bool violation = false;
	bool implicit = false;
	unsigned long long base = 0;
	std::map<unsigned long long, const SgInitializedName*> m;
	FOREACH_INITNAME(enumDec->get_enumerators(), i) {
		const SgInitializedName *var = isSgInitializedName(*i);
		assert(var);
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if (init) {
			/** Explicit numbering */
			const SgExpression *val = init->get_operand();
			assert(val);
			if (isSgIntVal(val)) {
				base = ((const SgIntVal *)val)->get_value();
			}
			else if (isSgLongIntVal(val)) {
				base = ((const SgLongIntVal *)val)->get_value();
			}
			else if (isSgLongLongIntVal(val)) {
				base = ((const SgLongLongIntVal *)val)->get_value();
			}
			else if (isSgUnsignedIntVal(val)) {
				base = ((const SgUnsignedIntVal *)val)->get_value();
			}
			else if (isSgUnsignedLongLongIntVal(val)) {
				base = ((const SgUnsignedLongLongIntVal *)val)->get_value();
			}
			else if (isSgUnsignedLongVal(val)) {
				base = ((const SgUnsignedLongVal *)val)->get_value();
			}
			else if (isSgEnumVal(val)) {
				base = ((const SgEnumVal *)val)->get_value();
			}
			else {
                          std::cerr << "What is this? " << val->unparseToString() << std::endl;
				assert(false); //should have been one of the above types. it's possible i didn't cover all cases.
			}
		}
		else {
		  implicit = true;
		}

		if (m[base]) {
			/** Collision */
			violation = true;
		} else {
			/** First time we've seen this implicit value */
			m[base] = var;
		}

		base++;
	}

	/* We allow double values if all initializations are explicit */
	if (implicit && violation)
	  print_error(node, "INT09-C", "Ensure enumeration constants map to unique values", true);
	return violation;
}


/**
 * Do not make assumptions about the type of a plain int bit-field when used
 * in an expression
 */
bool INT12_C(const SgNode *node ) {
	const SgInitializedName *varName = isSgInitializedName(node);
	if (!varName)
		return false;

	if (!findParentOfType(varName, SgClassDefinition))
		return false;

	const SgVariableDefinition *varDef = isSgVariableDefinition(varName->get_definition());
	/**
	 * Disabling assertion due to C++ code
	 */
	if (!varDef)
		return false;
  //	assert(varDef);

	if (!varDef->get_bitfield())
		return false;

	const SgType *varType = varName->get_type();
	if (isSgTypeChar(varType)
      ||  isSgTypeShort(varType)
      ||  isSgTypeInt(varType)
      ||  isSgTypeLong(varType)
      ||  isSgTypeLongLong(varType)) {
		print_error(node, "INT12-C", "Do not make assumptions about the type of a plain int bit-field when used in an expression", true);
		return true;
	}

	return false;
}

/**
 * Use bitwise operators only on unsigned operands
 */
bool INT13_C(const SgNode *node ) {
	bool violation = false;
	if (isSgBitComplementOp(node)) {
		const SgBitComplementOp *bitOp = isSgBitComplementOp(node);
		/** Allow compile time constants to be bit negated */
		if (isSgValueExp(bitOp->get_operand())) {
			return false;
		}
		if (!stripModifiers(bitOp->get_operand()->get_type())->isUnsignedType()) {
			violation = true;
		}
	} else if (isSgBinaryOp(node)) {
		const SgBinaryOp *binOp = isSgBinaryOp(node);
		/**
		 * Allow &/| on compile time constants
		 */
		if (isSgBitAndOp(binOp)
        || isSgBitOrOp(binOp)) {
			if (isSgValueExp(binOp->get_lhs_operand())
          || isSgValueExp(binOp->get_rhs_operand())) {
				return false;
			}
		}
		if (isSgLshiftOp(binOp)
        || isSgLshiftAssignOp(binOp)
        || isSgRshiftOp(binOp)
        || isSgRshiftAssignOp(binOp)) {
			/** Allow compile time constants in a shift */
			if (isSgValueExp(binOp->get_lhs_operand())
          || isSgValueExp(binOp->get_rhs_operand())) {
				return false;
			}
			if (!stripModifiers(binOp->get_lhs_operand()->get_type())->isUnsignedType()) {
				violation = true;
			}
		} else if (isAnyBinBitOp(binOp)
               || isAnyBinBitAssignOp(binOp)) {
		  if ((!stripModifiers(binOp->get_lhs_operand()->get_type())->isUnsignedType())) {
		    if (isSgValueExp(binOp->get_rhs_operand()))
		      return false;
		    else
		      if (!stripModifiers(binOp->get_rhs_operand()->get_type())->isUnsignedType()) {
            violation = true;
		      }
		  }
		}
	}

	if (violation) {
		print_error(node, "INT13-C", "Use bitwise operators only on unsigned operands", true);
		return true;
	}

	return false;
}

/**
 * Avoid performing bitwise and arithmetic operations on the same data
 */
bool INT14_C(const SgNode *node ) {
	const SgBinaryOp *assign = isSgBinaryOp(node);
	if (!assign)
		return false;

	bool arith = false;
	bool bitwise = false;

	if (isAnyBinArithAssignOp(assign)) {
		arith = true;
	} else if (isAnyBinBitAssignOp(assign)) {
		bitwise = true;
	} else if (!isSgAssignOp(assign)) {
		return false;
	}

	FOREACH_SUBNODE(assign, nodes, i, V_SgExpression) {
		if (isAnyBinArithOp(*i)
        ||  isSgMinusMinusOp(*i)
        ||  isSgPlusPlusOp(*i)
        ||  isSgMinusOp(*i)
        ||  isSgUnaryAddOp(*i)) {
			arith = true;
		} else if (isAnyBinBitOp(*i)
               ||  isSgBitComplementOp(*i)) {
			bitwise = true;
		}
	}

	if (bitwise && arith) {
		print_error(node, "INT14-C", "Avoid performing bitwise and arithmetic operations on the same data", true);
		return true;
	}

	return false;
}


/**
 * Ensure that operations on signed integers do not result in overflow
 *
 * \note We only check the Unary negation case here
 * \todo this rule needs to be completely rewritten
 *
 * \see INT33_C
 */
/* note: compiler gets mad if we name this function
 * INT32_C because of this line in stdint.h:
 * # define INT32_C(c)	c
 */
bool INT32C(const SgNode *node ) {
	const SgMinusOp *negOp = isSgMinusOp(node);
	if (!negOp)
		return false;

	const SgVarRefExp * var= isSgVarRefExp(negOp->get_operand());
	if (!var)
		return false;
	const SgInitializedName *varName = getRefDecl(var);
	assert(varName);
	const SgStatement *statement = findInBlockByOffset(node, -1);
	if (!statement) {
		print_error(node,"INT32-C", "Ensure that operations on signed integers do not result in overflow");
		return true;
	}
	FOREACH_SUBNODE(statement, nodes, i, V_SgBinaryOp) {
		const SgBinaryOp *binOp = isSgBinaryOp(*i);
		assert(binOp);

		const SgExpression *lhs = binOp->get_lhs_operand();
		SgValueExp *rhs = isSgValueExp(binOp->get_rhs_operand());
		const SgVarRefExp *iVar = isSgVarRefExp(lhs);
		if (iVar && (getRefDecl(iVar) == varName)) {
			if (isMinVal(computeValueTree(rhs)))
				return false;
		}
		iVar = isSgVarRefExp(rhs);
		if (iVar && (getRefDecl(iVar) == varName)) {
			if (isMinVal(lhs))
				return false;
		}
	}

	print_error(node,"INT32-C", "Ensure that operations on signed integers do not result in overflow");
	return true;
}

/**
 * Ensure that division and modulo operations do not result in divide-by-zero
 * errors
 *
 * \note INT07 sets off false positives, buts that's OK
 */
bool INT33_C(const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	if (!(isSgModOp(binOp) || isSgModAssignOp(binOp) || isSgDivideOp(binOp) || isSgDivAssignOp(binOp))) {
		return false;
	}

	const SgVarRefExp *varRef = isSgVarRefExp(removeCasts(binOp->get_rhs_operand()));
	if (!varRef)
		return false;

	/**
	 * If the variable is constant, it's likely that the programmer already
	 * knows the value and a check is not necessary
	 */
	if (isConstType(varRef->get_type()))
		return false;

	/**
	 * See if we checked the var against zero in the previous line or
	 * maybe we're in an if statement or something that has the check in the
	 * conditional
	 */
	const SgNode * parent = findParentOfType(node, SgBasicBlock)->get_parent();
	const SgStatement *stat =
		isSgIfStmt(parent)? isSgIfStmt(parent)->get_conditional():
		isSgWhileStmt(parent)? isSgWhileStmt(parent)->get_condition():
		isSgDoWhileStmt(parent)? isSgDoWhileStmt(parent)->get_condition():
		isSgForStatement(parent)? isSgForStatement(parent)->get_test():
		findInBlockByOffset(node, -1);
	if (stat && isCheckForZero(stat, varRef))
		return false;

	print_error(node,"INT33-C", "Ensure that division and remainder operations do not result in divide-by-zero errors");
	return true;
}

/**
 * Do not shift a negative number of bits or more bits than exist in the
 * operand
 */
bool INT34_C(const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if (!op)
		return false;
	if (!(isSgLshiftOp(op)
        ||isSgLshiftAssignOp(op)
        ||isSgRshiftOp(op)
        ||isSgRshiftAssignOp(op)))
		return false;

	const SgExpression *rhs = removeCasts(op->get_rhs_operand());
	assert(rhs);

	if (valueVerified(rhs))
		return false;

	print_error(node, "INT34-C", "Do not shift a negative number of bits or more bits than exist in the operand");
	return true;
}

/**
 * helper function for INT36, this should eventually be merged into
 * categories.C
 */
bool INT36_isPointer(const SgType *type) {
	const SgType *base = type->findBaseType();
	return (isAnyPointerType(type)
          ||  isSgTypeString(type)
          ||  isSgFunctionType(type)
          ||  isAnyPointerType(base)
          ||  isSgTypeString(base)
          ||  isSgFunctionType(base));
}

/**
 * Take care when converting from pointer to integer or integer to pointer
 */
bool INT36_C(const SgNode *node ) {
	if (isCompilerGeneratedNode(node))
		return false;

	const SgCastExp *cast = isSgCastExp(node);
	if (!cast)
		return false;

	/**
	 * Allow pointer arith, also allow pointer subtraction
	 */
	if (isSgAddOp(cast->get_parent())
      ||  isSgSubtractOp(cast->get_parent())
      ||  isSgSubtractOp(cast->get_operand()))
		return false;

	/**
 	 * Allow casting constant expressions such as NULL to pointers
 	 */
	if (isSgValueExp(cast->get_operand())
      ||  isSgValueExp(cast->get_parent()))
		return false;

	/**
	 * casts are dumb... *sigh*
	 */
	if (isSgUnaryOp(cast->get_operand())
      &&  isSgValueExp(isSgUnaryOp(cast->get_operand())->get_operand()))
		return false;

	const SgType * outerType = cast->get_type();
	assert(outerType);

	const SgType * innerType = cast->get_operand()->get_type();
	assert(innerType);

	if (INT36_isPointer(outerType) && ! INT36_isPointer(innerType)) {
		print_error(node, "INT36-C", "Take care when converting from pointer to integer or integer to pointer", true);
		return true;
	}

	return false;
}

bool INT_C(const SgNode *node) {
  bool violation = false;
  violation |= INT01_C(node);
  violation |= INT05_C(node);
  violation |= INT06_C(node);
  violation |= INT07_C(node);
  violation |= INT09_C(node);
  violation |= INT12_C(node);
  violation |= INT13_C(node);
  violation |= INT14_C(node);
  violation |= INT32C(node);
  violation |= INT33_C(node);
  violation |= INT34_C(node);
  violation |= INT36_C(node);
  return violation;
}


/// C++ checkers

bool INT_CPP(const SgNode *node) {
  bool violation = false;
  violation |= INT_C(node);
  return violation;
}

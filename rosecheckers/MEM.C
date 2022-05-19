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

/**
 * \file MEM.C
 */

#include "rose.h"
#include "utilities.h"
#include "utilities_cpp.h"

/**
 * Store a new value in pointers immediately after free()
 */
bool MEM01_C(const SgNode *node) {
  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  if (!(fnRef && isCallOfFunctionNamed(fnRef, "free")))
    return false;

  // Figure out which variable is being freed
  const SgExpression *argExp = getFnArg(fnRef, 0);
  assert(argExp);
  const SgVarRefExp *argVar = isSgVarRefExp(argExp);
  if (!argVar)
    return false;
  const SgInitializedName *argVarName = getRefDecl(argVar);
  assert(argVarName);
  bool longlifetime = isGlobalVar(argVarName) || isStaticVar(argVarName);
  /* Block where the variable is defined */
  const SgBasicBlock* defBlock = findParentOfType(argVar->get_symbol()->get_declaration(),SgBasicBlock);

  // Pop up to the BasicBlock so that we can find the next line of code
  const SgStatement* nextStat = findInBlockByOffset(node,1);
  // block in which the free statement is enclosed
  const SgBasicBlock* block = findParentOfType(node,SgBasicBlock);
  assert(block);

  while (nextStat == NULL) {
    // If we're in a for-loop and imediately assign in the increment,
    // that is OK
    const SgForStatement *forLoop = isSgForStatement(block->get_parent());
    if (forLoop && isAssignToVar(forLoop->get_increment(), argVarName))
      return false;
    // If this block is the one in which the variable is defined, that is
    // OK
    if ((block == defBlock) && !longlifetime)
      return false;
    // Pop up to the next block
    nextStat = findInBlockByOffset(block,1);
    if (nextStat == NULL) break;
    block = findParentOfType(block, SgBasicBlock);
    assert(block);
  }

  // Return Statements are also OK, but only for local vars
  if (isSgReturnStmt(nextStat) && (!longlifetime)) {
    return false;
  } else {
    // Assignments to the pointer are OK
    const SgExprStatement *nextExpr = isSgExprStatement(nextStat);
    if (nextExpr && isAssignToVar(nextExpr->get_expression(), argVarName))
      return false;
  }

  print_error(node, "MEM01-C", "Store a new value in pointers immediately after free()", true);

  return true;
}

/**
 * pointer to the allocated type
 *
 * \see EXP36-C which catches this
 */
bool MEM02_C(const SgNode *node) {
  return false;
}

/**
 * Do not perform zero length allocations
 */
bool MEM04_C(const SgNode *node) {
  const SgExpression *allocArg = removeImplicitPromotions(getAllocFunctionExpr(isSgFunctionRefExp(node)));
  if (!allocArg)
    return false;

  const SgVarRefExp *varRef = isSgVarRefExp(allocArg);
  if (!varRef)
    return false;
  const SgInitializedName *var = getRefDecl(varRef);
  const SgValueExp *val = isSgValueExp(allocArg);
  if (var) {
    const SgFunctionDefinition *fn = findParentOfType(node,SgFunctionDefinition);
    const SgVarRefExp *ref = NULL;
    /**
     * First try, just look for a check against NULL
     */
    FOREACH_SUBNODE(fn, nodes1, i, V_SgVarRefExp) {
      ref = isSgVarRefExp(*i);
      assert(ref);
      if ((var == getRefDecl(ref)) && isTestForNullOp(ref)) {
        return false;
      }
    }
    /**
     * If there isn't one, maybe the progammer has assigned this var to
     * something sane and doesn't need to check
     */
    FOREACH_SUBNODE(fn, nodes2, i2, V_SgAssignOp) {
      const SgAssignOp *op = isSgAssignOp(*i2);
      assert(op);
      ref = isSgVarRefExp(op->get_lhs_operand());
      if (!ref || (getRefDecl(ref) != var))
        continue;
      if (!isZeroVal(removeCasts(op->get_rhs_operand())))
        return false;
    }
    const SgAssignInitializer *init = isSgAssignInitializer(var->get_initptr());
    if (init && !isZeroVal(removeCasts(init->get_operand()))) {
      return false;
    }

  } else if (val && !isZeroVal(val)) {
    /** compile time constant that is 0 */
    return false;
  } else {
    /** sizeof or something else we can't handle */
    return false;
  }

  print_error(node,"MEM04-C", "Do not perform zero length allocations", true);
  return true;
}

/**
 * Ensure that the arguments to calloc() when multiplied can be represented as
 * a size_t
 */
bool MEM07_C(const SgNode *node) {
  const SgFunctionRefExp* fnRef = isSgFunctionRefExp(node);
  if (!(fnRef && isCallOfFunctionNamed(fnRef, "calloc")))
    return false;

  const SgExpression* nmembExp = getFnArg(fnRef, 0);
  const SgExpression* sizeExp = getFnArg(fnRef, 1);
  assert(nmembExp && sizeExp);

  const SgVarRefExp* nmembRef = isSgVarRefExp(nmembExp);
  size_t nmembVal;
  if (!nmembRef && !getSizetVal(nmembExp, &nmembVal))
    return false;
  const SgVarRefExp* sizeRef = isSgVarRefExp(sizeExp);
  size_t sizeVal;
  if (!sizeRef && !getSizetVal(sizeExp, &sizeVal))
    return false;

  if (nmembRef || sizeRef) {
    /* Search for the previous line, see if it is a check for overflow */
    /**
     * \todo We need to find a more rigorous way to find checks for
     * overflow
     */
    const SgStatement *prevStat = findInBlockByOffset(node, -1);
    if (prevStat) {
      FOREACH_SUBNODE(prevStat, nodes, i, V_SgBinaryOp) {
        const SgBinaryOp *binOp = isSgBinaryOp(*i);
        assert(binOp);
        if (!isAnyComparisonOp(binOp))
          continue;
        const SgVarRefExp *lhs = isSgVarRefExp(binOp->get_lhs_operand());
        const SgVarRefExp *rhs = isSgVarRefExp(binOp->get_rhs_operand());
        if ((lhs && nmembRef && (getRefDecl(lhs)==getRefDecl(nmembRef)))
           || (lhs && sizeRef  && (getRefDecl(lhs)==getRefDecl(sizeRef)))
           || (rhs && nmembRef && (getRefDecl(rhs)==getRefDecl(nmembRef)))
           || (rhs && sizeRef  && (getRefDecl(rhs)==getRefDecl(sizeRef))))
          return false;
      }
    }
  } else {
    if (nmembVal <= (std::numeric_limits<size_t>::max() / sizeVal))
      return false;
  }

  print_error(node, "MEM07-C", "Ensure that the arguments to calloc() when multiplied can be represented as a size_t", true);
  return true;
}

/**
 * Use realloc() only to resize dynamically allocated arrays
 */
bool MEM08_C(const SgNode *node) {
  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  if (!(fnRef && isCallOfFunctionNamed(fnRef, "realloc")))
    return false;

  const SgExpression *arg = removeImplicitPromotions(getFnArg(fnRef, 0));
  assert(arg);

  const SgCastExp* cast = isSgCastExp(node->get_parent()->get_parent());
  if (!cast)
    return false;

  if (cast->get_type() != arg->get_type()) {
    print_error(node, "MEM08-C", "Use realloc() only to resize dynamically allocated arrays", true);
    return true;
  }

  return false;
}


/**
 * Ensure that freed pointers are not reused
 *
 * \bug Need to check for conditional return statements
 *
 * \bug Throws errors in loops as well.
 */
bool MEM30_C(const SgNode *node) {
  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  if (!(fnRef && isCallOfFunctionNamed(fnRef, "free")))
    return false;

  // Get variable as first arg
  const SgVarRefExp* ref = isSgVarRefExp(getFnArg(isSgFunctionRefExp(node), 0));
  if (ref == NULL) return false;
  const SgInitializedName* var = getRefDecl(ref);
  assert(var != NULL);

  Rose_STL_Container<const SgVarRefExp*> references = next_var_references(ref);
  Rose_STL_Container<const SgVarRefExp*>::iterator i;
  for (i = references.begin(); i != references.end(); ++i) {
    if (isTestForNullOp(*i)) continue;
    if (isAssignToVar((*i)->get_parent(), var)) continue; // return false? ~DS 2010-03-15
    print_error(node, "MEM30-C", "Do not access freed memory");
    return true;
  }

  return false;
}

/**
 * Free dynamically allocated memory exactly once
 *
 * If the variable is passed by reference to a function which allocates memory this could throw a false positive.
 * If the variable is freed multiple times by a single loop this could throw a false negative.
 */
bool MEM31_C(const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "free")))
		return false;

	const SgVarRefExp* ref = isSgVarRefExp(
                                         getFnArg(isSgFunctionRefExp(node), 0));

	if (ref == NULL)
		return false;
	const SgInitializedName* ref_var = getRefDecl(ref);

	bool before = true;

	const SgNode *parent = findParentOfType(node, SgFunctionDefinition);
	if (parent == NULL)
		return false;

	FOREACH_SUBNODE(parent, nodes, i, V_SgNode) {
		if (before) {
			if (*i == node)
				before = false;
			continue;
		}

		/**
		 * Checking for return statements lowers the false positive rate by
		 * allowing conditional frees followed by returns
		 */
		if (isSgReturnStmt(*i))
			return false;

		const SgFunctionRefExp *iFn = isSgFunctionRefExp(*i);
		if (iFn && isCallOfFunctionNamed(iFn, "free")) {
			const SgVarRefExp* ref2 = isSgVarRefExp(removeCasts(
                                                          getFnArg(iFn, 0)));
			if (!ref2)
				continue;
			const SgInitializedName* ref2_var = getRefDecl(ref2);

			if (ref_var == ref2_var) {
				print_error(node, "MEM31-C", "Free dynamically allocated memory when no longer needed");
				return true;
			}
		} else if (isSgAssignOp(*i)) {
			const SgVarRefExp *ref2 = isSgVarRefExp(
                                              isSgAssignOp(*i)->get_lhs_operand());
			if (ref2 != NULL) {
				const SgInitializedName* ref2_var = ref2 ? getRefDecl(ref2)
          : NULL;

				if (ref_var == ref2_var)
					return false;
			}
		}
	}

  return false;
}

/**
 * Only free memory allocated dynamically
 *
 * \bug Doesn't correctly ignore members inside struct fn arguments
 */
bool MEM34_C(const SgNode *node) {
  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  if (!fnRef)
    return false;
  if (!(isCallOfFunctionNamed(fnRef, "free")
        ||isCallOfFunctionNamed(fnRef, "realloc")))
    return false;

  const SgVarRefExp *varRef = isSgVarRefExp(removeImplicitPromotions(getFnArg(fnRef,0)));
  if (!varRef)
    return false;
  const SgInitializedName *var = getRefDecl(varRef);
  assert(var);
  /**
   * It's much too hard to analyze these kinds of variables
   */
  if (isGlobalVar(var) || isStaticVar(var))
    return false;

  /**
   * Ignore arguments to a function
   */
  const SgFunctionDefinition *parent = findParentOfType(node, SgFunctionDefinition);
  FOREACH_INITNAME(parent->get_declaration()->get_args(), p) {
    if (var == *p)
      return false;
  }

  assert(parent);
  Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree(const_cast<SgFunctionDefinition*>(parent), V_SgNode);
  Rose_STL_Container<SgNode *>::iterator i = nodes.begin();
  while (fnRef != isSgFunctionRefExp(*i)) {
    assert(i != nodes.end());
    i++;
  }

  const SgNode *block = NULL;
  do {
    if (block) {
      if (block == *i) {
        block = NULL;
      }
      continue;
    }
    else if (isSgReturnStmt(*i)) {
      block = popBlock(*i);
    }

    const SgFunctionRefExp *iFn = isSgFunctionRefExp(*i);
    const SgVarRefExp *iVar = isSgVarRefExp(*i);

    if (iFn) {
      if (!(isCallOfFunctionNamed(iFn, "malloc")
            ||isCallOfFunctionNamed(iFn, "calloc")
            ||isCallOfFunctionNamed(iFn, "realpath")
            ||isCallOfFunctionNamed(iFn, "strdup")
            ||isCallOfFunctionNamed(iFn, "realloc")
            ||isCallOfFunctionNamed(iFn, "getline")))
        continue;

      if (isCallOfFunctionNamed(iFn, "getline")) {
        const SgExpression *argExp = getFnArg(fnRef, 0);
        const SgVarRefExp *varRefExp = isSgVarRefExp(argExp);
        if (!varRefExp)
          continue;

        if (var == getRefDecl(varRefExp))
          return false;
      }

      if (var == getVarAssignedTo(iFn, NULL))
        return false;
    } else if (iVar && (getRefDecl(iVar) == var)) {
      if (varWrittenTo(iVar)) {
        break;
      }
    }
  } while ((i--) != nodes.begin());

  print_error(node, "MEM34-C", "Only free memory allocated dynamically");
  return true;
}

/**
 * Guarantee that copies are made into storage of sufficient size
 *
 * We make sure that the length argument to memcpy is at most the size
 * of memcpy's first argument (destination). This rule fires if:
 * * the destination is a fixed-length array
 * * the last argument is N * sizeof(arraytype)
 * * N is known at compile time
 * * N > destination array index
 */
bool MEM35_C(const SgNode * node) {
	const SgFunctionRefExp *fnRef = isMemoryBlockFunction(node);
	if (!fnRef)
		return false;
	const SgExpression *dstExp = removeImplicitPromotions(getFnArg(fnRef, 0));
	const SgExpression *lenExp = getFnArg(fnRef, 2);
	assert(dstExp && lenExp);
	if (isSgAddressOfOp(dstExp))
		dstExp = isSgAddressOfOp(dstExp)->get_operand();
	const SgArrayType *arrT = isSgArrayType(dstExp->get_type());
	if (!arrT)
		return false;
	size_t len;
	if (!getSizetVal(lenExp, &len))
		return false;
	const SgValueExp *dstVal = isSgValueExp(arrT->get_index());
	if (!dstVal) // VLA or some such...
		return false;
	size_t dst_size;
	if (!getSizetVal(dstVal, &dst_size))
		return false;
	dst_size *= sizeOfType(arrT->findBaseType());
	if (dst_size == 0)
		return false;
	if (dst_size > len) {
		print_error(node, "MEM35-C", "Allocate sufficient memory for an object");
		return true;
	}
	return false;
}

/* MEM41-CPP. Declare a copy constructor, a copy assignment operator, and a destructor in a class that manages resources */
bool MEM41_CPP(const SgNode *node) {
  bool ret = false;
  const SgClassDefinition *cdef;
  if ((cdef = isSgClassDefinition(node)) != NULL) {
    // Skip the check if this is a POD (like a C struct).
    if (!isPODClass(cdef)) {
      // First, see which of these three functions the class has.
      size_t count = hasExplicitCopyCtor(cdef) + hasExplicitCopyAssignment(cdef) + hasExplicitDtor(cdef);
      if (count > 0 && count < 3) {
        print_error(node, "MEM41-CPP", "If any of copy constructor, copy assignment, and destructor are declared, all three should be.", true);
        ret = true;
      }
      //XXX more...how do we know if a class manages resources?  Punt and just check for a pointer member?
      if (hasPointerMember(cdef) && count < 3) { //XXX should omit this check for unions
        print_error(node, "MEM41-CPP", "A class with a pointer data member should probably define a copy constructor, copy assignment, and destructor.", true);
        ret = true;
      }
    }
  }
  return ret;
}

bool MEM_C(const SgNode *node) {
  bool violation = false;
  violation |= MEM01_C(node);
  violation |= MEM02_C(node);
  violation |= MEM04_C(node);
  violation |= MEM07_C(node);
  violation |= MEM08_C(node);
  violation |= MEM30_C(node);
  violation |= MEM31_C(node);
  violation |= MEM34_C(node);
  violation |= MEM35_C(node);
  return violation;
}


/// C++ checkers

bool MEM_CPP(const SgNode *node) {
  bool violation = false;
  violation |= MEM41_CPP(node);
  violation |= MEM_C(node);
  return violation;
}

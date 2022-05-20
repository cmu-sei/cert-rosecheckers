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
 * \file categories.C
 */

#include "rose.h"
#include "categories.h"

const SgBinaryOp *isAnyBinArithOp(const SgNode *node) {
	if (isSgAddOp(node)
	  ||isSgDivideOp(node)
	  ||isSgModOp(node)
	  ||isSgSubtractOp(node)
	  ||isSgMultiplyOp(node)
	  ||isSgExponentiationOp(node)
	  ||isSgIntegerDivideOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinArithAssignOp(const SgNode *node) {
	if (isSgPlusAssignOp(node)
	  ||isSgDivAssignOp(node)
	  ||isSgModAssignOp(node)
	  ||isSgMinusAssignOp(node)
	  ||isSgMultAssignOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinBitOp(const SgNode *node) {
	if (isSgBitAndOp(node)
	  ||isSgBitOrOp(node)
	  ||isSgBitXorOp(node)
	  ||isSgLshiftOp(node)
	  ||isSgRshiftOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinBitAssignOp(const SgNode *node) {
	if (isSgAndAssignOp(node)
	  ||isSgIorAssignOp(node)
	  ||isSgXorAssignOp(node)
	  ||isSgLshiftAssignOp(node)
	  ||isSgRshiftAssignOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinLogicOp(const SgNode *node) {
	if (isSgAndOp(node)
	  ||isSgOrOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyAssignOp(const SgNode *node) {
	if (isSgAssignOp(node)
	  ||isAnyBinArithAssignOp(node)
	  ||isAnyBinBitAssignOp(node)
	  ||isAnyBinLogicOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyEqualityOp(const SgNode *node) {
	if (isSgEqualityOp(node)
	  ||isSgNotEqualOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyRelationalOp(const SgNode *node) {
	if (isSgGreaterOrEqualOp(node)
	  ||isSgGreaterThanOp(node)
	  ||isSgLessOrEqualOp(node)
	  ||isSgLessThanOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyComparisonOp(const SgNode *node) {
	if (isAnyEqualityOp(node)
	  ||isAnyRelationalOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

/** NOT ACCOUNTED FOR
	  ||isSgConcatenationOp(node)
	  ||isSgScopeOp(node)
	  ||isSgArrowExp(node)
	  ||isSgArrowStarOp(node)
	  ||isSgCommaOpOp(node)
	  ||isSgDotExp(node)
	  ||isSgDotStarOp(node)
	  ||isSgPntrArrRefExp(node)
*/

/**
 * Checks to see if the type is some kind of char or wchar_t
 *
 * \note we return false on [un]signed chars since they are numbers not
 * characters
 */
bool isAnyCharType(const SgType *type) {
	const SgType *t = type->stripTypedefsAndModifiers();
	return isSgTypeChar(t)
		|| isSgTypeWchar(t)
		|| isTypeWcharT(type);
}

bool isAnyPointerType(const SgType *type) {
	const SgType *t = type->stripTypedefsAndModifiers();
	return isSgPointerType(t)
		|| isSgArrayType(t)
		|| isTypeUintptrT(type);
}

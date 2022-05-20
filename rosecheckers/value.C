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
 * \file value.C
 */

#include "rose.h"
#include "value.h"

/**
 * Takes a Value node and tries to extract the numeric value from it.  The
 * need for this exists because size_t are expressed as both UnsignedIntVal's
 * and UnsignedLongVal's.
 */
bool getSizetVal(const SgExpression *node, size_t *value) {
	if(!node)
		return false;
	if (isSgUnsignedIntVal(node)) {
		*value = isSgUnsignedIntVal(node)->get_value();
	} else if (isSgUnsignedLongVal(node)) {
		*value = isSgUnsignedLongVal(node)->get_value();
	} else {
		return false;
	}
	return true;
}

bool getIntegerVal(const SgExpression *node, intmax_t *n) {
	if(!node)
		return false;
	if (isSgUnsignedIntVal(node)) {
		*n = isSgUnsignedIntVal(node)->get_value();
	} else if (isSgIntVal(node)) {
		*n = isSgIntVal(node)->get_value();
	} else if (isSgUnsignedLongVal(node)) {
		*n = (intmax_t) (isSgUnsignedLongVal(node)->get_value());
	} else if (isSgLongIntVal(node)) {
		*n = isSgLongIntVal(node)->get_value();
	} else if (isSgUnsignedLongLongIntVal(node)) {
		*n = (intmax_t) isSgUnsignedLongLongIntVal(node)->get_value();
	} else if (isSgLongLongIntVal(node)) {
		*n = isSgLongLongIntVal(node)->get_value();
	} else if (isSgUnsignedShortVal(node)) {
		*n = isSgUnsignedShortVal(node)->get_value();
	} else if (isSgShortVal(node)) {
		*n = isSgShortVal(node)->get_value();
	} else {
		return false;
	}
	return true;
}

bool getFloatingVal(const SgExpression *node, long double *n) {
	if(!node)
		return false;
	if (isSgFloatVal(node)) {
		*n = isSgFloatVal(node)->get_value();
	} else if (isSgDoubleVal(node)) {
		*n = isSgDoubleVal(node)->get_value();
	} else if (isSgLongDoubleVal(node)) {
		*n = isSgLongDoubleVal(node)->get_value();
	} else {
		return false;
	}
	return true;
}

bool isVal(const SgExpression *node, const intmax_t n) {
	if (!node)
		return false;
	intmax_t x;
	if (!getIntegerVal(node, &x))
		return false;
	return x == n;
}

/**
 * Takes a Value node and tries to make sure it is 0
 */
bool isZeroVal(const SgExpression *node) {
	if (!node)
		return false;
	if (node->get_type()->isIntegerType()) {
		return isVal(node,0);
	} else if (node->get_type()->isFloatType()) {
		long double x;
		if (!getFloatingVal(node, &x))
			return false;
		return x == 0.0l;
	} else {
		return false;
	}
}

/**
 * Takes a Value node and tries to make sure it is the minimum
 */
bool isMinVal(const SgExpression *node) {
	if(!node)
		return false;
	if (isSgUnsignedIntVal(node)) {
		return 0 == isSgUnsignedIntVal(node)->get_value();
	} else if (isSgIntVal(node)) {
		return INT_MIN == isSgIntVal(node)->get_value();
	} else if (isSgUnsignedLongVal(node)) {
		return 0 == isSgUnsignedLongVal(node)->get_value();
	} else if (isSgLongIntVal(node)) {
		return LONG_MIN == isSgLongIntVal(node)->get_value();
	} else if (isSgUnsignedLongLongIntVal(node)) {
		return 0 == isSgUnsignedLongLongIntVal(node)->get_value();
	} else if (isSgLongLongIntVal(node)) {
		return std::numeric_limits<long long>::min() == isSgLongLongIntVal(node)->get_value();
	} else if (isSgUnsignedShortVal(node)) {
		return 0 == isSgUnsignedShortVal(node)->get_value();
	} else if (isSgShortVal(node)) {
		return SHRT_MIN == isSgShortVal(node)->get_value();
	} else if (isSgUnsignedCharVal(node)) {
		return 0 == isSgUnsignedCharVal(node)->get_value();
	} else if (isSgCharVal(node)) {
		return CHAR_MIN == isSgCharVal(node)->get_value();
	} else {
		return false;
	}
}

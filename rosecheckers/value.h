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
 * \file value.h
 */

#ifndef VALUE_H
#define VALUE_H

#include "rose.h"

bool getSizetVal(const SgExpression *node, size_t *value);
bool getIntegerVal(const SgExpression *node, intmax_t *n);
bool getFloatingVal(const SgExpression *node, long double *n);
bool isVal(const SgExpression *node, const intmax_t n);
bool isZeroVal(const SgExpression *node);
bool isMinVal(const SgExpression *node);

#endif

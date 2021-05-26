/*
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <list>
#include <algorithm>
#include <iostream>
#include "rose.h"
#include "utilities.h"

extern bool EXP( SgProject *project );
extern bool OBJ( SgProject *project );
extern bool DCL( SgProject *project );
extern bool ERR( SgProject *project );
extern bool FLP( SgProject *project );
extern bool RES( SgProject *project );

int main( int argc, char * argv[] ) {
	SgProject* project = frontend( argc, argv );
	ROSE_ASSERT( project );
	AstTests::runAllTests( project );

	EXP( project );
	OBJ( project );
	DCL( project );
	ERR( project );
	FLP( project );
	RES( project );
}


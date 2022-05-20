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

#include <stdlib.h>

// This routine is strictly for testing the visit_next function; it should
// visit nodes in the order of i = 1, 2, 3 ...

void foo() {
	char* x = NULL;
	int i = 0;
	while (i == -1) {
		i = -2;
	}

	for (i = -3; i == -4; i++) {
		i = -5;
	}

	for (i = -3; i == 5; i++) {
		i = 6;
		do {
			i = 3;
			free(x);
			i = 1;
		} while (i == 2);
		i = 4;
	}

	while (i == 7) {
		i = 8;
		if (i == 9) {
			i = 10;
			return;
		}
		i == 11;
	}


	for (i = 12; i == 13; i++) {
		i = 14;
	}
}

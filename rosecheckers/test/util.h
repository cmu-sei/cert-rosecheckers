#ifndef UTIL_H
#define UTIL_H

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
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
//#include <complex.h> <-- not working with G++, not needed for C
#include <time.h>
#include <float.h>
#include <unistd.h>
#include <time.h>
#include <wchar.h>
#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <fenv.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>

extern char **environ;

/* This is for the case where we just want to run it through a static
 * analysis tool and don't care about runtime errors. This is not
 * specified as a #define to avoid being ignored by the tools.
*/
#define RUNTIME_IGNORE 1

#endif

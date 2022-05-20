#ifndef UTIL_CPP_H
#define UTIL_CPP_H

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

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <deque>
#include <algorithm>
#include <set>
#include <functional>
#include <utility>
#include <map>
#include <cassert>
#include <sys/resource.h>
#include <cstdarg>
#include <errno.h>
#include <signal.h>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csetjmp>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>


typedef unsigned int rsize_t;
typedef int errno_t;

extern char **environ;

using namespace std;

/* This is for the case where we just want to run it through a static
 * analysis tool and don't care about runtime errors. This is not
 * specified as a #define to avoid being ignored by the tools.
 */

#define RUNTIME_IGNORE 1

#endif

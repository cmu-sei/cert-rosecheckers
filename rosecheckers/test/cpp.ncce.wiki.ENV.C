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

#include "util.h"
#include "cpp.util.h"
#include <pwd.h>

void ENV00();
void ENV01();
void ENV02();
void ENV03();
void ENV04();
void ENV30();
void ENV31(char const *envp[]);
void ENV32();

void ENV(char const *envp[]) {
  ENV00();
  ENV01();
  ENV02();
  ENV03();
  ENV04();
  ENV30();
  ENV31(envp);
  ENV32();
}

void ENV00(void) {
  char *tmpvar;
  char *tempvar;

  tmpvar = getenv("TMP");
  if (!tmpvar) return;
  tempvar = getenv("TEMP");
  if (!tempvar) return;
}

void ENV01(void) {
  char copy[16];
  const char *temp = getenv("TEST_ENV");
  if (temp != NULL) {
    strcpy(copy, temp);
  }
}

void ENV02(void) {
  if (putenv("TEST_ENV=foo") != 0) {
    return;
  }
  if (putenv("Test_ENV=bar") != 0) {
    return;
  }

  const char *temp = getenv("TEST_ENV");
}

void ENV03(void) {
  if (system("/bin/ls dir.`date +%Y%m%d`") == -1) {
    return;
  }
}

void ENV04(void) {
  char *input = NULL;
  /* input gets initialized by user */

  char cmdbuf[512];
  int len_wanted = snprintf(
			    cmdbuf, sizeof(cmdbuf), "any_cmd '%s'", input
			    );
  if (len_wanted >= sizeof(cmdbuf)) {
    perror("Input too long");
  }
  else if (len_wanted < 0) {
    perror("Encoding error");
  }
  else if (system(cmdbuf) == -1) {
    perror("Error executing input");
  }
}

void ENV30(void) {
  char *env = getenv("TEST_ENV");
  if (env == NULL) {
    return;
  }
  env[0] = 'a';
}

void ENV31(char const *envp[]) {
  size_t i;
  if (setenv("MY_NEW_VAR", "new_value", 1) != 0) {
    return;
  }
  if (envp != NULL) {
    for (i = 0; envp[i] != NULL; i++) {
      if (puts(envp[i]) == EOF) {
	return;
      }
    }
  }
}

void ENV32_exit1() {
  exit(0);
}

void ENV32(void) {
  if (atexit(ENV32_exit1) != 0) {
    return;
  }
}

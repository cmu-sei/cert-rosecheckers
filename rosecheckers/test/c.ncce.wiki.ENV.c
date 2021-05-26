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

void ENV00();
void ENV01();
void ENV02();
void ENV03();
void ENV04();
void ENV30();
void ENV31(char const *envp[]);
void ENV32();
void ENV33();

void ENV(char const *envp[]) {
  ENV00();
  ENV01();
  ENV02();
  ENV03();
  ENV04();
  ENV30();
  ENV31(envp);
  ENV32();
  ENV33();
}


/* ENV00 v.36 */

void ENV00() {
  const char *tmpvar;
  const char *tempvar;

  tmpvar = getenv("TMP");
  if (!tmpvar) return;
  tempvar = getenv("TEMP");
  if (!tempvar) return;

  if (strcmp(tmpvar, tempvar) == 0) {
    if (puts("TMP and TEMP are the same.\n") == EOF) {}
  }
  else {
    if (puts("TMP and TEMP are NOT the same.\n") == EOF) {}
  }
}


/* ENV01 v.29 */

void ENV01() {
  char copy[16];
  const char *temp = getenv("TEST_ENV");
  if (temp != NULL) {
    strcpy(copy, temp);
  }
}


/* ENV02_A v.43 */

void ENV02() {
  if (putenv("TEST_ENV=foo") != 0) {
    /* Handle Error */
  }
  if (putenv("Test_ENV=bar") != 0) {
    /* Handle Error */
  }

  const char *temp = getenv("TEST_ENV");

  if (temp == NULL) {
    /* Handle Error */
  }
}


/* ENV03_A v.62 */

void ENV03() {
  if (system("/bin/ls dir.`date +%Y%m%d`") != 0) {}
}


/* ENV04_A v.65 */

void ENV04() {
  char *input = NULL;

  /* input gets initialized by user */

  char cmdbuf[512];
  const int len_wanted = snprintf(
			    cmdbuf,
			    sizeof(cmdbuf),
			    "any_cmd '%s'",
			    input
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


/* ENV30_C v.41 */

void ENV30_trstr(char *str, char orig, char rep) {
  while (*str != '\0') {
    if (*str == orig) {
      *str = rep;
    }
    str++;
  }
}

void ENV30() {
	/* ... */

	char *env = getenv("TEST_ENV");
	if (env == NULL) {
	  /* Handle error */
	}

	ENV30_trstr(env,'"', '_');


	/* ... */
}

/* ENV31_C v.43 */

void ENV31(char const *envp[]) {
   size_t i;
   if (setenv("MY_NEW_VAR", "new_value", 1) == -1) {}
   if (envp != NULL) {
      for (i = 0; envp[i] != NULL; i++) {
         if (puts(envp[i]) == EOF) {}
      }
   }
}


/* ENV32_C v.35 */

void ENV32_exit1(void) {
  /* ...cleanup code... */
  return;
}

void ENV32_exit2(void) {
  if (1) {
    /* ...more cleanup code... */
    exit(0);
  }
  return;
}

void ENV32() {
  if (atexit(ENV32_exit1) != 0) {
    /* Handle Error */
  }
  if (atexit(ENV32_exit2) != 0) {
    /* Handle Error */
  }
  /* ...program code... */
  exit(0);
}


/* ENV33_C v.25 */

jmp_buf ENV33_env;
int ENV33_val;

void ENV33_exit1(void) {
  /* ... */
  longjmp(ENV33_env, 1);
}

void ENV33(void) {
  if (atexit(ENV33_exit1) != 0) {
    /* handle error */
  }
  /* ... */
  if (setjmp(ENV33_env) == 0) {
    exit(0);
  }
  else {
    return;
  }
}

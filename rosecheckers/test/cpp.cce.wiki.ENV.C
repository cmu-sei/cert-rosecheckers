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

#include "util.h"
#include "cpp.util.h"
#include <pwd.h>

void ENV00();
void ENV01();
void ENV02();
void ENV03();
void ENV04();
void ENV30();
void ENV31();
void ENV32();

void ENV() {
  ENV00();
  ENV01();
  ENV02();
  ENV03();
  ENV04();
  ENV30();
  ENV31();
  ENV32();
}

void ENV00(void) {
  char *tempvar;
  char *temp = getenv("TEMP");
  if (temp != NULL) {
    tempvar = (char *)malloc(strlen(temp)+1);
    if (tempvar != NULL) {
      strcpy(tempvar, temp);
    }
    else {
      /* Handle error */
      return;
    }
  }
  free(tempvar);
  tempvar = NULL;
}

void ENV01(void) {
  char *copy = NULL;
  const char *temp = getenv("TEST_ENV");
  if (temp != NULL) {
    copy = (char *)malloc(strlen(temp) + 1);
    if (copy != NULL) {
      strcpy(copy, temp);
    }
    else {
      /* Handle error condition */
    }
  }
}

void ENV02(void) {
  if (putenv("TEST_ENV=foo") != 0) {
    /* Handle error */
  }
  if (putenv("OTHER_ENV=bar") != 0) {
    /* Handle error */
  }

  const char *temp = getenv("TEST_ENV");

  if (temp == NULL) {
    /* Handle error */
  }
}

void ENV03(void) {
  if (clearenv() != 0) {
    /* Handle error */
    return;
  }
  if (system("ls dir.`date +%Y%m%d`") == -1) {
    /* Handle error */
  }
}

void ENV04(void) {
  const char *file_format = "%s/foo";
  size_t len;
  char *file;
  struct passwd *pwd;

  /* Get /etc/passwd entry for current user */
  pwd = getpwuid(getuid());
  if (pwd == NULL) {
    /* Handle error */
    return;
  }

  /* build full path name home dir from pw entry */

  len = strlen(pwd->pw_dir) + strlen(file_format);
  file = (char *)malloc(len+1);
  snprintf(file, len, file_format, pwd->pw_dir);
  if (unlink(file) != 0) {
    /* Handle error in unlink */
  }

  free(file);
  file = NULL;
}

void ENV30(void) {
  const char *env;
  char *copy_of_env;

  env = getenv("TEST_ENV");
  if (env == NULL) {
    /* Handle error */
  }

  copy_of_env = new char[strlen(env) + 1];
  strcpy(copy_of_env, env);
}

void ENV31(void) {
  size_t i;
  if (setenv("MY_NEW_VAR", "new_value", 1) != 0) {
    /* Handle error */
    return;
  }
  if (environ != NULL) {
    for (i = 0; environ[i] != NULL; i++) {
      if (puts(environ[i]) == EOF) {
	return;
      }

    }
  }
}

void ENV32_exit1(void) {
  return;
}

void ENV32(void) {
  if (atexit(ENV32_exit1) != 0) {
    return;
    }
}

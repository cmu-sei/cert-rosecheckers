/*
// <legal>
// CERT Rosecheckers
// Copyright 2022 Carnegie Mellon University.
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.
// CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.
// DM21-0505
// </legal>
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
  char *tmpvar;
  char *tempvar;

  const char *temp = getenv("TMP");
  if (temp != NULL) {
    tmpvar = (char *)malloc(strlen(temp)+1);
    if (tmpvar != NULL) {
      strcpy(tmpvar, temp);
    }
    else {
      /* handle error condition */
    }
  }
  else {
    return;
  }

  temp = getenv("TEMP");
  if (temp != NULL) {
    tempvar = (char *)malloc(strlen(temp)+1);
    if (tempvar != NULL) {
      strcpy(tempvar, temp);
    }
    else {
      free(tmpvar);
      tmpvar = NULL;
      /* handle error condition */
      return;
    }
  }
  else {
    free(tmpvar);
    tmpvar = NULL;
    return;
  }

  if (strcmp(tmpvar, tempvar) == 0) {
    if (puts("TMP and TEMP are the same.\n") == EOF) {
      /* Handle Error */
    }
  }
  else {
    if (puts("TMP and TEMP are NOT the same.\n") == EOF) {
      /* Handle Error */
    }
  }
  free(tmpvar);
  tmpvar = NULL;
  free(tempvar);
  tempvar = NULL;
}


/* ENV01 v.29 */

void ENV01() {
  char *copy = NULL;
  const char *temp = getenv("TEST_ENV");
  if (temp != NULL) {
    copy = (char *)malloc(strlen(temp) + 1);
    if (copy != NULL) {
      strcpy(copy, temp);
    }
    else {
      /* handle error condition */
    }
  }
  free(copy);
}


/* ENV02_A v.43 */

void ENV02() {
  if (putenv("TEST_ENV=foo") != 0) {
    /* Handle Error */
  }
  if (putenv("OTHER_ENV=bar") != 0) {
    /* Handle Error */
  }

  const char *temp = getenv("TEST_ENV");

  if (temp == NULL) {
    /* Handle Error */
  }
}


/* ENV03_A v.62 */

void ENV03() {
  char *pathbuf;
  size_t n;

  if (clearenv() != 0) {
    /* Handle Error */
  }

	n = confstr(_CS_PATH, NULL, 0);
	if (n == 0) {
		/* Handle Error */
	}
	if ((pathbuf = (char *)malloc(n)) == NULL) {
		/* Handle Error */
	}
	if (confstr(_CS_PATH, pathbuf, n) == 0) {
		/* Handle Error */
	}

	if (setenv("PATH", pathbuf, 1) == -1) {
		/* Handle Error */
	}

  if (setenv("IFS", " \t\n", 1) == -1) {
    /* Handle Error */
  }

  if (system("/bin/ls dir.`date +%Y%m%d`") == -1) {
    /* Handle Error */
  }

  free(pathbuf);
}


/* ENV04_A v.65 */

void ENV04() {
  char *input = NULL;

  /* input gets initialized by user */

  pid_t pid;
  int status;
  char *const args[3] = {"any_exe", input, NULL};
  char **env = NULL;

  /* Sanitize args here... */
  pid = fork();
  if (pid == -1) {
    perror("fork error");
  }
  else if (pid != 0) {
    if (waitpid(pid, &status, 0) == -1) {
      perror("Error waiting for child process");
    }
    else if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
      /* report unexpected child status */
    }
  } else {

    /* Initialize env as a sanitized copy of environ here... */

    if (execve("/usr/bin/any_exe", args, env) == -1) {
      perror("Error executing any_exe");
      _exit(127);
    }
  }
}


/* ENV30_C v.41 */

void ENV30() {
  char const *env;
  char *copy_of_env;

  if ((env = getenv("TEST_ENV")) != NULL) {
    copy_of_env = (char *)malloc(strlen(env) + 1);

    if (copy_of_env != NULL) {
      strcpy(copy_of_env, env);
    }
    else {
      /* Error handling */
    }

    copy_of_env[0] = 'a';

    free(copy_of_env);
    copy_of_env = NULL;
  }
}


/* ENV31_C v.43 */

void ENV31(char const *envp[]) {
	size_t i;
	if (!setenv("MY_NEW_VAR", "new_value", 1)) {
		/* Handle Error */
	}
	if (environ != NULL) {
		for (i = 0; environ[i] != NULL; i++) {
			if (puts(environ[i]) == EOF) {
				/* Handle Error */
			}
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
  }
  return;
}

void ENV32(void) {
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

void ENV33_exit1(void) {
  /* ... */
  return;
}

void ENV33(void) {
  if (atexit(ENV33_exit1) != 0) {
    /* handle error */
  }
  /* ... */
  exit(0);
}

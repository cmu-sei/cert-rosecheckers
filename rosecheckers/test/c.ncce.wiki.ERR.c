/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.// 
// DM21-0505</legal>// 
 */

#include "util.h"

void ERR01();
void ERR04();
void ERR06();
void ERR30();
void ERR31();
void ERR32();


void ERR() {
  ERR01();
  ERR04();
  ERR06();
  ERR30();
  ERR31();
  ERR32();
}


/* ERR01_A v.27 */

void ERR01() {
  errno = 0;
  printf("This\n");
  printf("is\n");
  printf("a\n");
  printf("test.\n");
  if (errno != 0) {
    fprintf(stderr, "printf failed: %s\n", strerror(errno));
  }
}


/* ERR04_A v.40 */

void ERR04() {
  char const *filename = "hello.txt";
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    /* handle error */
  }
  fprintf(f, "Hello, World\n");

  abort(); /* oops! data might not get written! */
}


/* ERR06_A v.14 */

void ERR06_cleanup(void) {
  /* delete temporary files, restore consistent state, etc. */
}

void ERR06() {
  if (atexit(ERR06_cleanup) != 0) {
    /* Handle Error */
  }

  assert(1);
}


/* ERR30_C v.38 */

void ERR30() {
  unsigned long number;
  char *string = NULL;
  char *endptr;

  /* ... */

  number = strtoul(string, &endptr, 0);
  if (endptr == string || (number == ULONG_MAX
			   && errno == ERANGE))
    {
      /* handle the error */
    }
  else {
    /* computation succeeded */
  }
}


/* ERR31_C v.22 */

extern int errno;

void ERR31() {}


/* ERR32_C v.29 */

typedef void (*ERR32_pfv)(int);

void ERR32_handler(int signum) {
  const ERR32_pfv old_handler = signal(signum, SIG_DFL);
  if (old_handler == SIG_ERR) {
    perror("SIGINT handler"); /* undefined behavior */
    /* handle error condition */
  }
}

void ERR32() {
  const ERR32_pfv old_handler = signal(SIGINT, ERR32_handler);
  if (old_handler == SIG_ERR) {
    perror("SIGINT handler");
    /* handle error condition */
  }

  /* main code loop */

  return;
}

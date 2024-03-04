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


int POS34_1(char const *var) {
  static char *oldenv;
  char const *env_format = "TEST=%s";
  size_t const len = strlen(var) + strlen(env_format);
  char *environment = (char *) calloc( len, 256);
  if (environment == NULL) {
    return -1;
  }
  const int rc = snprintf(environment, len, env_format, var);
  if (rc < 0 || (size_t)rc >= len) {
    /* Handle Error */
  }
  if (putenv(environment) != 0) {
    free(environment);
    return -1;
  }
  if (oldenv != NULL)
    free(oldenv); // avoid memory leak
  oldenv = environment;
  return 0;
}


char goodenv[1024];

int POS34_2(const char *var) {
  char *environment;
  environment = goodenv;

  if (snprintf(environment, sizeof(environment),"TEST=%s", var) < 0) {
    /* Handle error */
  }

  return putenv(environment);
}


char env0[1024];
int POS34_3(const char *var) {

  if (snprintf(env0, sizeof(env0),"TEST=%s", var) < 0) {
    /* Handle error */
  }

  return putenv(env0);
}

const char *spongeBob = "bob the sponge, here again!!!";
int POS34_4(){
  return putenv((char*) spongeBob); // const cast necessary for putenv
}


int POS34_5(const char *var) {
  static char environment[1024];

  if (snprintf(environment, sizeof(environment),"TEST=%s", var) < 0) {
    /* Handle error */
  }

  return putenv(environment);
}

int POS34_6(){
  char *env_1 = (char*)(malloc(10));
  if (env_1 ==  NULL) {/* handle error */}
  char *env_2 = env_1;
  return putenv(env_2);
}

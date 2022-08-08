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

void DCL00();
void DCL01();
void DCL03();
void DCL04();
void DCL05();
void DCL06();
void DCL08();
void DCL10();
void DCL11();
void DCL13();
void DCL30();
void DCL31();
void DCL32();
void DCL33();

void DCL() {
  DCL00();
  DCL01();
  DCL03();
  DCL04();
  DCL06();
  DCL08();
  DCL10();
  DCL11();
  DCL13();
  DCL30();
  DCL31();
  DCL32();
  DCL33();
}


/* DCL00_A v.69 */

void DCL00() {
  float const pi = 3.14159f;
  const float degrees = 180.0;
  float radians;
  /* ... */
  radians = degrees * pi / 180;
  printf("DCL00 %f %f\n", degrees, radians);
}


/* DCL01_A v.51 */

char DCL01_system_msg[100];

void DCL01_report_error(char const *error_msg) {
  char DCL01_default_msg[80];

  /* Assume error_msg isn't too long */
  strncpy(DCL01_system_msg, error_msg, sizeof(DCL01_system_msg));
  printf("DCL01 %p %p\n", DCL01_system_msg, DCL01_default_msg);
  return;
}

void DCL01(void) {
  char error_msg[] = "Error!";
  DCL01_report_error(error_msg);
}


/* DCL03_A v.35 */

#define DCL03_JOIN(x, y) DCL03_JOIN_AGAIN(x, y)
#define DCL03_JOIN_AGAIN(x, y) x ## y

#define DCL03_static_assert(e) \
  typedef char DCL03_JOIN(assertion_failed_at_line_, __LINE__)\
[(e) ? 1 : -1]

struct DCL03_timer {
  uint8_t MODE;
  uint32_t DATA;
  uint32_t COUNT;
};

void DCL03() {
  DCL03_static_assert(offsetof(struct DCL03_timer, DATA) == 4);
}


/* DCL04_A v.37 */
void DCL04() {
  char *src = 0;
  char *c = 0;
  printf("DCL04 %p %p\n", src, c);
}

/* DCL05_C */
typedef void (*DCL05_SighandlerType)(int signum);
extern DCL05_SighandlerType DCL05_signal(
  int signum,
  DCL05_SighandlerType handler
);

/* DCL06_A v.88 */

void DCL06() {
  enum { ADULT_AGE = 18 };

  int age;

  age = random() % 100;

  if(age > ADULT_AGE) {
    /* something */
  }
  else {
    /* something */
  }
}


/* DCL08_A v.20 */

void DCL08() {
  enum { ADULT_AGE=18 };
  enum { ALCOHOL_AGE=21 };
}


/* DCL10_A v.43 */

void DCL10() {
  char const *error_msg = "Resource not available to user.";

  printf("Error: %s", error_msg);
}


/* DCL11_A v.49 */

void DCL11() {
  const long long a = 1;
  char const msg[] = "Default message";

  printf("%lld %s", a, msg);
}


/* DCL13_A v.22 */

void DCL13_foo(int const * x) {
  if (x != NULL) {
    printf("Value is %d\n", *x);
  }
  /* ... */
}

void DCL13() {
  int *x = NULL;
  DCL13_foo(x);
}


/* DCL30_A v.63 */

void DCL30() {
  char const str[] = "Everything OK";
  char const *p = str;
  printf("DCL30 %s\n" , p);
}


/* DCL31_C v.20 */

void DCL31() {
  const unsigned int foo = random();
  printf("DCL31 %d\n" , foo);
}


/* DCL32_C v.47 */

void DCL32() {
  extern int *a_global_symbol_definition_lookup_table;
  extern int *b_global_symbol_definition_lookup_table;
  printf("DCL32 %p %p\n", a_global_symbol_definition_lookup_table, b_global_symbol_definition_lookup_table);
}

/* DCL33_C v.42 */

void DCL33() {
  char str[]="test string";
  char *ptr1=str;
  char *ptr2;

  ptr2 = ptr1 + 3;
  memmove(ptr2, ptr1, 6);
}

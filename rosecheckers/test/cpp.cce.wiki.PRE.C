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
#include "cpp.util.h"

void PRE00(void);
void PRE01(void);
void PRE02(void);
void PRE03(void);
void PRE04(void);
void PRE05(void);
void PRE06(void);
void PRE07(void);
void PRE08(void);
void PRE09(void);
void PRE10(void);
void PRE11(void);
void PRE12(void);
void PRE31(void);
void PRE32(void);


void PRE(void) {
  PRE00();
  PRE01();
  PRE02();
  PRE03();
  PRE04();
  PRE05();
  PRE06();
  PRE07();
  PRE08();
  PRE09();
  PRE10();
  PRE11();
  PRE12();
  PRE31();
  PRE32();
}

void PRE00_A();
void PRE00_B();
void PRE00_C();

void PRE00(void) {
  PRE00_A();
  PRE00_B();
  PRE00_C();
}

inline int PRE00_A_A(int i) {
  return i * i * i;
}

void PRE00_A() {
  int i = 2;
  const int a = 81 / PRE00_A_A(++i);
}

size_t PRE00_B_count = 0;

void PRE00_B_B(void) {
  printf("Called PRE00_B_B, PRE00_B_count = %zu.\n", PRE00_B_count);
}

typedef void (*exec_func)(void);
inline void PRE00_B_exec_bump(exec_func f) {
  f();
  ++PRE00_B_count;
}

void PRE00_B_A(void) {
  PRE00_B_count = 0;
  while (PRE00_B_count++ < 10) {
    PRE00_B_exec_bump(PRE00_B_B);
  }
}

void PRE00_B() {
  PRE00_B_A();
}

unsigned int PRE00_C_operations;
unsigned int calls_to_PRE00_C_A;
unsigned int calls_to_PRE00_C_B;

inline int PRE00_C_A(int x) {
   ++PRE00_C_operations;
   ++calls_to_PRE00_C_A;
   return 2*x;
}

inline int PRE00_C_B(int x) {
   ++PRE00_C_operations;
   ++calls_to_PRE00_C_B;
   return x + 1;
}

void PRE00_C() {
  const unsigned int x = 3;
  const unsigned int y = PRE00_C_A(x) + PRE00_C_B(x);
}


/* PRE01_A v.66 */

#define PRE01_CUBE(I) ( (I) * (I) * (I) )

void PRE01() {
  const int a = 81 / PRE01_CUBE(2 + 1);
  printf("PRE01 %d\n", a);
}

#define PRE02_CUBE(X) ((X) * (X) * (X))

void PRE02_A() {
  const int i = 3;
  const int a = 81 / PRE02_CUBE(i);
  printf("PRE02 %d\n", a);
}

enum { PRE02_EOF = -1 };

void PRE02_B() {
  if (getchar() != PRE02_EOF) {
   /* ... */
  }
}

void PRE02() {
  PRE02_A();
  PRE02_B();
}

typedef char * PRE03_cstring;

void PRE03() {
  PRE03_cstring s1, s2;
  s1 = s2 = NULL;
  printf("PRE03 %p %p\n", s1, s2);
}


#include "my_stdio_cpp.h"

void PRE04() {

}

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

void PRE05() {}

#include "PRE06_CS_CPP.h"

void PRE06() {}

void PRE07() {
  size_t i;
  /* assignment of i */
  i = random();
  if (i > 9000) {
    if (puts("Over 9000!?""?!") == EOF) {
      /* Handle Error */
    }
  }
}

#include "lib1_cpp.h"
#include "lib2_cpp.h"

void PRE08() {}

#ifndef __USE_ISOC99
#include "09_cpp.h"
#endif

void PRE09() {}


#define PRE10_SWAP(x, y) \
  do { \
    tmp = x;					\
    x = y;					\
    y = tmp; } \
  while(0)

void PRE10() {
  int x, y, z, tmp;
  z = random() % 2;
  if (z == 0)
    PRE10_SWAP(x, y);
}

#define FOR_LOOP(n)  for(i=0; i<(n); i++)

void PRE11() {
  int i;
  FOR_LOOP(3)
    {
      puts("Inside for loop\n");
    }
}

long MASK = -1L;

void PRE12() {
  long x = -1L;
  long positive_x = (x ^ MASK) + 1;

  printf("%d", positive_x);
}

#define PRE31_ABS(x) (((x) < 0) ? -(x) : (x))

void PRE31() {
  int n=0, m=0;
  ++n;
  m = PRE31_ABS(++n);
  printf("PRE31 %d\n", m);
}

void PRE32() {
  char *dest=NULL;
  char *src=NULL;

#ifdef PLATFORM1
  memcpy(dest, src, 12);
#else
  memcpy(dest, src, 24);
#endif
}

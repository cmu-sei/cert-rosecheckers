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

#define SIZE_MAX 256

void INT00();
void INT01();
void INT02();
void INT04();
void INT05();
void INT06();
void INT07();
void INT08();
void INT09();
void INT10();
void INT11();
void INT12();
void INT13();
void INT14();
void INT30();
void INT31();
void INT32();
void INT33();
void INT34();
void INT35();

void INT() {
  INT00();
  INT01();
  INT02();
  INT04();
  INT05();
  INT06();
  INT07();
  INT08();
  INT09();
  INT10();
  INT11();
  INT12();
  INT13();
  INT14();
  INT30();
  INT31();
  INT32();
  INT33();
  INT34();
  INT35();
}

void INT00() {
  unsigned int a=0;
  unsigned int b=0;
  unsigned long c;
  /* Initialize a and b */
  c = static_cast<unsigned long>(a) * b; /* not guaranteed to fit */

  printf("%d", c);
}

void *INT01_alloc(unsigned int blocksize) {
  return malloc(blocksize);
}

void INT01() {
  free(INT01_alloc(4));
}

void INT02() {
  int si = -1;
  unsigned ui = 1;
  cout << (si < ui) << endl;
}

void INT04_create_table(size_t length) {
  char **table;

  if (sizeof(char *) > SIZE_MAX/length) {
    /* handle overflow */
  }

  const size_t table_length = length * sizeof(char *);
  table = (char **)malloc(table_length);

  if (table == NULL) {
    /* Handle error condition */
  }
  else
    free(table);
  /* ... */
}

void INT04() {
  INT04_create_table(4);
}

void INT05() {
  long sl;
  if(scanf("%ld", &sl) != 1) {}
}

void INT06() {
  int si=0;
  si = atoi("4");

  printf("%d", si);
}


void INT07() {
  char c = 4;
  int i = 1000;
  cout << "i/c = " << i/c << endl;
}

void INT08() {
  int i = 32766 + 1;
  i++;
}

void INT09() {
  enum { red=4, orange, yellow, green, blue, indigo=6, violet};
}

int INT10_insert(int INT10_index, int *list, int size, int value) {
  if (size != 0) {
    INT10_index = (INT10_index + 1) % size;
    list[INT10_index] = value;
    return INT10_index;
  }
  else {
    return -1;
  }
}

void INT10() {
  int list[10];

  if(INT10_insert(0, list, -1, 4) != 0) {}
}


void INT11() {
  char *ptr = NULL;
  const unsigned int flag = 0;

  unsigned int number = (unsigned int)ptr;
  number = (number & 0x7fffff) | (flag << 23);
  ptr = (char *)number;
}

struct {
  int a: 8;
} INT12_bits = {225};

void INT12() {
  cout << "bits.a = " << INT12_bits.a << endl;
}

void INT13() {
  int rc = 0;
  const int stringify = 0x80000000;
  char buf[sizeof("256")];
  rc = snprintf(buf, sizeof(buf), "%u", stringify >> 24);
  if (rc == -1 || rc >= sizeof(buf)) {
    /* handle error */
  }
}

void INT14() {
  unsigned int x = 50;
  x += (x << 2) + 1;
}

void INT30() {
  const unsigned int ui1 = 0, ui2 = 0;
  unsigned int sum;

  sum = ui1 + ui2;

  printf("%d", sum);
}

void INT31() {
  unsigned long int ul = ULONG_MAX;
  signed char sc;
  sc = static_cast<signed char>(ul);

  printf("%c", sc);
}

void INT32() {
  const int si1 = 0, si2 = 0;
  const int sum = si1 + si2;

  signed int result;

  result = -sum;

  printf("%d", result);
}

void INT33() {
  const signed long sl1 = 0;
  signed long sl2;
  sl2 = random();

  const signed long result = sl1/sl2;

  printf("%d", result);
}

void INT34() {
  const int si1 = 0, si2 = 0;

  const int sresult = si1 << si2;

  printf("%d", sresult);
}


enum { INT35_BLOCK_HEADER_SIZE = 16 };
unsigned long long INT35_max = UINT_MAX;

void *INT35_AllocateBlock(size_t length) {
  struct memBlock *mBlock;

  if (length + INT35_BLOCK_HEADER_SIZE > INT35_max) return NULL;
  mBlock
    = (struct memBlock *)malloc(length + INT35_BLOCK_HEADER_SIZE);
  if (!mBlock) return NULL;

  /* fill in block header and return data portion */

  return mBlock;
}


void INT35() {
  free(INT35_AllocateBlock(10));
}

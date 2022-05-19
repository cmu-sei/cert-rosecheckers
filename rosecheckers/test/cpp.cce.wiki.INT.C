/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Third-Party Software subject to its own license.// 
// DM21-0505</legal>// 
 */

#include "util.h"
#include "cpp.util.h"

#define RSIZE_MAX 256
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

/*
void INT00() {
#if UINT_MAX > (UINTMAX_MAX/UINT_MAX)
 #error "No safe type is available."
#endif

  const unsigned int a = 2, b = 3;
  uintmax_t c;

  c = (uintmax_t)a * b;
  printf("INT00 %ju\n", c);
}
*/

void *INT01_alloc(rsize_t blocksize) {
  if (blocksize == 0 || blocksize > RSIZE_MAX) {
    /* Handle error */
  }
  return malloc(blocksize);
}

void INT01() {
  (void)INT01_alloc(4);
}


void INT02() {
  int si = -1;
  unsigned ui = 1;
  cout << (si < static_cast<int>(ui)) << endl;
}

enum { INT04_MAX_TABLE_LENGTH = 256 };

int INT04_create_table(size_t length) {
  size_t table_length;
  char **table;

  if (length == 0 || length > INT04_MAX_TABLE_LENGTH) {
    /* Handle invalid length */
  }

  /*
   * The wrap check has been omitted based on the assumption
   * that MAX_TABLE_LENGTH * sizeof(char *) cannot exceed
   * SIZE_MAX. If this assumption is not valid, a check must
   * be added.
   */
  assert(length <= SIZE_MAX/sizeof(char *));

  table_length = length * sizeof(char *);

  table = (char **)malloc(table_length);
  if (table == NULL) {
    /* Handle error condition */
  }
  else
    free(table);

  /* ... */
  return 0;
}

void INT04() {
	if (INT04_create_table(5) != 0) {
		/* Handle Error */
	}
}

void INT05() {
  char buff [25];
  char *end_ptr;
  long sl;
  int si=0;

  cin.width(24);
  cin >> buff;

  errno = 0;

  sl = strtol(buff, &end_ptr, 0);

  if (ERANGE == errno) {
    cout << "number out of range" << endl;
  }
  else if (sl > INT_MAX) {
    cout << sl << " too large!" << endl;
  }
  else if (sl < INT_MIN) {
    cout << sl << " too small!" << endl;
  }
  else if (end_ptr == buff) {
    cout << "not valid numeric input" << endl;
  }
  else if ('\0' != *end_ptr) {
    cout << "extra characters on input line" << endl;
  }
  else {
    si = static_cast<int>(sl);
  }

  printf("%d", si);
}

void INT06() {
  char buff [25];
  char *end_ptr;
  long sl;
  int si=0;

  cin.width(24);
  cin >> buff;

  errno = 0;

  sl = strtol(buff, &end_ptr, 0);

  if (ERANGE == errno) {
    cout << "number out of range" << endl;
  }
  else if (sl > INT_MAX) {
    cout << sl << " too large!" << endl;
  }
  else if (sl < INT_MIN) {
    cout << sl << " too small!" << endl;
  }
  else if (end_ptr == buff) {
    cout << "not valid numeric input" << endl;
  }
  else if ('\0' != *end_ptr) {
    cout << "extra characters on input line" << endl;
  }
  else {
    si = static_cast<int>(sl);
  }

  printf("%d", si);
}

void INT07() {
  unsigned char c = 200;
  int i = 1000;
  cout << "i/c = " << i/c << endl;
}

void INT08() {
  long i = 32766 + 1;
  i++;
}

void INT09() {
  enum { red=4, orange, yellow, green, blue, indigo, violet};
}

int INT10_insert(size_t INT10_index, int *list, size_t size, int value) {
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

  if (INT10_insert(0, list, SIZE_MAX, 4) != 0) {
    /* Handle Error */
  }
}

struct INT11_ptrflag {
  char *pointer;
  unsigned int flag :9;
} ptrflag;

void INT11() {
  char *ptr = NULL;
  const unsigned int flag = 0;

  ptrflag.pointer = ptr;
  ptrflag.flag = flag;
}

struct {
  unsigned int a: 8;
} INT12_bits = {225};

void INT12() {
  cout << "bits.a = " << INT12_bits.a << endl;
}

void INT13() {
  int rc = 0;
  const unsigned int stringify = 0x80000000;
  char buf[sizeof("256")];
  rc = snprintf(buf, sizeof(buf), "%u", stringify >> 24);
  if (rc == -1 || rc >= sizeof(buf)) {
    /* handle error */
  }
}

void INT14() {
  unsigned int x = 50;
  x = 5*x+1;
}

void INT30() {
  unsigned int ui1, ui2, sum=0;

 ui1 = 0;
 ui2 = 0;

  if(UINT_MAX - ui1 < ui2)
    return;
  sum = ui1 + ui2;

  printf("%d", sum);
}

void INT31() {
  const unsigned long int ul = ULONG_MAX;
  signed char sc=0;
  if (ul <= SCHAR_MAX) {
    sc = (signed char)ul;  /* use cast to eliminate warning */
  }
  else {
    /* handle error condition */
  }

  printf("%d", sc);
}

void INT32() {
  signed int si1=0, si2=0, sum;

  /* Initialize si1 and si2 */

  if (((si1>0) && (si2>0) && (si1 > (INT_MAX-si2)))
      || ((si1<0) && (si2<0) && (si1 < (INT_MIN-si2)))) {
    /* handle error condition */
  }
  else {
    sum = si1 + si2;
  }

  printf("%d", sum);
}

void INT33() {
  signed long sl1, sl2, result;
  sl1 = sl2 = 0;

  if ( (sl2 == 0) || ( (sl1 == LONG_MIN) && (sl2 == -1) ) ) {
    /* handle error condition */
  }
  result = sl1 / sl2;

  printf("%d", result);
}

void INT34() {
  unsigned int ui1=0;
  unsigned int ui2=0;
  unsigned int uresult;

  unsigned int mod1=0; /* modulo behavior is allowed by INT36-EX1 */
  unsigned int mod2=0; /* modulo behavior is allowed by INT36-EX1 */

  /* Initialize ui1, ui2, mod1, and mod2 */

  if ( (ui2 >= sizeof(unsigned int)*CHAR_BIT) || (ui1 > (UINT_MAX  >> ui2)) ) {
    /* handle error condition */
  } else {
    uresult = ui1 << ui2;
  }

  if (mod2 >= sizeof(unsigned int)*CHAR_BIT) {
    /* handle error condition */
  } else {
    /* modulo behavior is allowed by exception */
    uresult = mod1 << mod2;
  }

  printf("%d", uresult);
}

enum { INT35_BLOCK_HEADER_SIZE = 16 };
unsigned long long INT35_max = UINT_MAX;

void *INT35_AllocateBlock(size_t length) {
  struct memBlock *mBlock;

  if ((unsigned long long)length + INT35_BLOCK_HEADER_SIZE > INT35_max)
    return NULL;
  mBlock
    = (struct memBlock *)malloc(length + INT35_BLOCK_HEADER_SIZE);
  if (!mBlock) return NULL;

  /* fill in block header and return data portion */

  return mBlock;
}

void INT35() {
  free(INT35_AllocateBlock(10));
}

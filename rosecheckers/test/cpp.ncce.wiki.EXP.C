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

void EXP00();
void EXP01();
void EXP02();
void EXP03();
void EXP04();
void EXP05();
void EXP06();
void EXP07();
void EXP09();
void EXP10();
void EXP11();
void EXP12();
void EXP15();
void EXP17();
void EXP30();
void EXP31();
void EXP33();
void EXP34();
void EXP35();

void EXP() {
  EXP00();
  EXP01();
  EXP02();
  EXP03();
  EXP04();
  EXP05();
  EXP06();
  EXP07();
  EXP09();
  EXP10();
  EXP11();
  EXP12();
  EXP15();
  EXP17();
  EXP30();
  EXP31();
  EXP33();
  EXP34();
  EXP35();
}

void EXP00() {
  const int x = 0;

  printf("EXP00 %d\n" ,x & 1 == 0);
}

enum { SIZE_MAX = 256 };

double *EXP01_allocate_array(size_t num_elems) {
  double *d_array;

  if (num_elems > SIZE_MAX/sizeof(d_array)) {
    /* handle error condition */
  }
  d_array = (double *)malloc(sizeof(d_array) * num_elems);
  if (d_array == NULL) {
    /* handle error condition */
  }
  return d_array;
}

void EXP01() {
  free(EXP01_allocate_array(3));
}

void EXP02() {
  char *p = NULL;

  const unsigned int BUF_SIZE = 12;

  if ( p || (p = (char *)malloc(BUF_SIZE)) ) {
    /* do stuff with p */
  }
  else {
    /* handle error */
    return;
  }
}


struct EXP03_buffer {
  size_t size;
  char bufferC[50];
};

void EXP03_func(const struct EXP03_buffer *buf) {

  /* Assumes sizeof( struct buffer) =
   * sizeof( size_t) + 50 * sizeof( char) = 54 */
  struct EXP03_buffer *buf_cpy = (struct EXP03_buffer *)malloc(54);

  if (buf_cpy == NULL) {
    /* Handle malloc() error */
  }

  /*
   * With padding, sizeof(struct buffer) may be greater than
   * 54, causing some data to be written outside the bounds
   * of the memory allocated.
   */
  memcpy(buf_cpy, buf, sizeof(struct EXP03_buffer));

  /* ... */

  free(buf_cpy);
}

void EXP03() {
  EXP03_func(NULL);
}

class my_buf {
public:
  // ...
private:
  char buff_type;
  size_t size;
  char buffer[50];
};

unsigned int EXP04_buf_compare(
			 const my_buf *s1,
			 const my_buf *s2)
{
  if (!memcmp(s1, s2, sizeof(my_buf))) {
    return 1;
  }
  return 0;
}

void EXP04() {
  if (EXP04_buf_compare(NULL, NULL) == 0) {}
}

void EXP05() {
  int dividend=1, divisor=2;
  // ...
  double result = ((double)dividend)/divisor;
  cout << result << endl;
}

void EXP06() {
  int a = 14;
  const int b = sizeof(a++);
  printf("EXP06 %d %d\n", a, b);
}

void EXP07() {
  const unsigned int nbytes = 4;
  const unsigned int nblocks = 1 + ((nbytes - 1) >> 9); /* BUFSIZ = 512 = 2^9 */
  printf("EXP07 %d\n", nblocks);
}

void EXP09() {
  size_t i;
  int **matrix = (int **)calloc(100, 4);
  if (matrix == NULL) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    matrix[i] = (int *)calloc(i, 4);
    if (matrix[i] == NULL) {
      /* handle error */
    }
  }
}

int EXP10_g;

int EXP10_f(int i) {
  EXP10_g = i;
  return i;
}

void EXP10() {
  const int x = EXP10_f(1) + EXP10_f(2);
  cout << x << endl;
}


void EXP11() {
  float f = 0.0;
  int i = 0;
  float *fp;
  int *ip;

  assert(sizeof(int) == sizeof(float));
  ip = (int*) &f;
  fp = (float*) &i;
  printf("int is %d, float is %f\n", i, f);
  (*ip)++;
  (*fp)++;

  printf("int is %d, float is %f\n", i, f);
}

void EXP12() {
  cout << "foo";
}

void EXP15() {
  int a = 2;
  int b = 3;
  cout <<"Originally, a = "<<a <<" and b = "<< b<< endl;
  if (a==b); {
         a = a + 1;
         b = b + 1;
         cout<<"Incrementing a and b since they're equal"<<endl;
  }
  cout<< "Now, a = "<< a <<" b = "<< b << endl;
}

void EXP17() {
  int a = 2;
  int b = 2;
  int c = 2;

  if (a < b < c) {}

  if (a == b == c) {}
}

void EXP30() {
  int i = 0;
  i = ++i + i;
}

void EXP31() {
  unsigned int EXP31_index = 0;
  assert(EXP31_index++ > 0);
}


void EXP33_set_flag(int number, int *sign_flag) {
  if (sign_flag == NULL) {
    return;
  }
  if (number > 0) {
    *sign_flag = 1;
  }
  else if (number < 0) {
    *sign_flag = -1;
  }
}

void EXP33() {
  int sign;
  int number = 2;

  EXP33_set_flag(number, &sign);

  number = sign + 1;
}

void EXP34() {
  char *input_str = (char*) malloc(3);
  const size_t size = strlen(input_str)+1;
  char *str = (char*) malloc(size);
  memcpy(str, input_str, size);
  /* ... */
  free(str);
  str = NULL;
}


void EXP35() {
  int const vals[] = {3, 4, 5};
  memset((int*) vals, 0, sizeof(vals));
}

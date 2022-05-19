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
void EXP05();
void EXP06();
void EXP07();
void EXP09();
void EXP10();
void EXP11();
void EXP12();
void EXP15();
void EXP17();
void EXP31();
void EXP32();
void EXP33();
void EXP34();
void EXP35();

void EXP() {
  EXP00();
  EXP01();
  EXP02();
  EXP05();
  EXP06();
  EXP07();
  EXP09();
  EXP10();
  EXP11();
  EXP12();
  EXP15();
  EXP17();
  EXP31();
  EXP32();
  EXP33();
  EXP34();
  EXP35();
}

void EXP00() {
  const int x = random();

  if ((x & 1) == 0) {
    printf("EXP00");
  }
}

enum { SIZE_MAX = 256 };

double *EXP01_allocate_array(size_t num_elems) {
  double *d_array;

  if (num_elems > SIZE_MAX/sizeof(*d_array)) {
    /* handle error condition */
  }
  d_array = (double *)malloc(sizeof(*d_array) * num_elems);
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

  if (p == NULL) p = (char *)malloc(BUF_SIZE);
  if (p == NULL) {
    /* handle error */
    return;
  }

  /* do stuff with p */

  free(p);
}

enum {EXP03_buffer_size = 50};

struct EXP03_buffer {
  size_t size;
  char bufferC[EXP03_buffer_size];
};

void EXP03_func(const struct EXP03_buffer *buf) {

  struct EXP03_buffer *buf_cpy =
    (struct EXP03_buffer *)malloc(sizeof(struct EXP03_buffer));

  if (buf_cpy == NULL) {
    /* Handle malloc() error */
  }

  /* ... */

  memcpy(buf_cpy, buf, sizeof(struct EXP03_buffer));

  /* ... */

  free(buf_cpy);
}

class my_buf {
public:
  /* ... */
  bool buf_compare(const my_buf *that);
private:
  char buff_type;
  unsigned int size;
  char buffer[50];
};


bool my_buf::buf_compare(const my_buf *that)
{
  if (this->buff_type != that->buff_type) return 0;
  if (this->size != that->size) return 0;
  if (strcmp(this->buffer, that->buffer) != 0) return 0;
  return 1;
}

void EXP05() {
  int dividend=1, divisor=2;
  double result = static_cast<double>(dividend)/divisor;
  cout << result << endl;
}


void EXP06() {
  int a = 14;
  const int b = sizeof(a);
  a++;
  printf("EXP06 %d\n", b);
}

void EXP07() {
  const unsigned int nbytes = 4;
  const unsigned int nblocks = 1 + (nbytes - 1) / BUFSIZ;
  printf("EXP07 %d\n", nblocks);
}

void EXP09() {
  size_t i;
  int **matrix = (int **)calloc(100, sizeof(*matrix));
  if (matrix == NULL) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    matrix[i] = (int *)calloc(i, sizeof(**matrix));
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
  int x = EXP10_f(1);
  x += EXP10_f(2);
}

void EXP11() {
  float f = 0.0;
  int i = 0;
  float *fp;
  int *ip;

  ip = &i;
  fp = &f;
  printf("int is %d, float is %f\n", i, f);
  (*ip)++;
  (*fp)++;

  printf("int is %d, float is %f\n", i, f);
}

void EXP12() {
  if (!(cout << "foo")) {
    /* Handle Error */
  }
}

void EXP15() {
  int a = 2;
  int b = 3;
  cout <<"Originally, a = "<<a <<" and b = "<< b<< endl;
  if (a==b) {
         a = a + 1;
         b = b + 1;
         cout<<"Incrementing a and b since they're equal"<<endl;
  }
  cout<< "Now, a = "<< a <<" b = "<< b << endl;
}

void EXP17() {
  int a=2,b=3,c=4;

  if ( a < b && b < c ) {}

  if ( a == b && a == c ){}
}

void EXP30() {
  int i = 0;
  i = i + 2;
}

void EXP31() {
  unsigned int EXP31_index = 0;
  assert(EXP31_index > 0);
  EXP31_index++;
}


void EXP32() {
  static volatile int **ipp;
  static volatile int *ip;
  static volatile int i = 0;

  cout << "i = " << i << "." << endl;

  ipp = &ip;
  *ipp = &i;
  if (*ip != 0) {
    /* ... */
  }
}

void EXP33_set_flag(int number, int *sign_flag) {
  if (sign_flag == NULL) {
    return;
  }
  if (number >= 0) {
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
  char input_str[] = "foo";
  const size_t size = strlen(input_str)+1;
  char *str = (char*) malloc(size);
  if(str == NULL) {
    /* Handle allocation error */
  }

  memcpy(str, input_str, size);
  /* ... */
  free(str);
  str = NULL;
}

void EXP35() {
  int vals[] = {3, 4, 5};
  memset(vals, 0, sizeof(vals));
}

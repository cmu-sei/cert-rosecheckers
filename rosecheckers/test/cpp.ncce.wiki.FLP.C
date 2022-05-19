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

void FLP02();
void FLP03();
void FLP04();
void FLP05();
void FLP30();
void FLP32();
void FLP33();
void FLP34();
void FLP35();

void FLP() {
  FLP02();
  FLP03();
  FLP04();
  FLP05();
  FLP30();
  FLP32();
  FLP33();
  FLP34();
  FLP35();
}

float FLP02_mean(float array[], int size) {
  float total = 0.0;
  int i;
  for (i = 0; i < size; i++) {
    total += array[i];
    printf("array[%d] = %f and total is %f\n", i, array[i], total);
  }
  if (size != 0) {
    return total / size;
  } else {
    return 0.0;
  }
}

enum {array_size = 10};
float array_value = 10.1;

void FLP02(void) {
  float array[array_size];
  float avg;
  int i;
  for (i = 0; i < array_size; i++) {
    array[i] = array_value;
  }

  avg = FLP02_mean( array, array_size);
  printf("mean is %f\n", avg);
  if (avg == array[0]) {
    printf("array[0] is the mean\n");
  } else {
    printf("array[0] is not the mean\n");
  }
}

void FLP03(void) {
  double a = 1e-40, b, c = 0.1;
  float x = 0, y;
  y = a;
  b = y / x;
  c = sin(30) * a;

  printf("%f", b);
  printf("%f", c);
}

enum { MAX_VALUE = 256 };

void FLP04(void) {
  float currentBalance=0.0; /* User's cash balance */

  float val;

  scanf("%f", &val);

  if(val >= MAX_VALUE - currentBalance) {
    /* Handle range error */
  }

  currentBalance += val;
}

void FLP05(void) {
  float x = 1/3.0;
  printf("Original      : %e\n", x);
  x = x * 7e-45;
  printf("Denormalized? : %e\n", x);
  x = x / 7e-45;
  printf("Restored      : %e\n", x);
}

void FLP30(void) {
  float x;
  for (x = 0.1f; x <= 1.0f; x += 0.1f) {
    /* ... */
  }

  for (x = 100000001.0f; x <= 100000010.0f; x += 1.0f) {
    /* ... */
  }
}

void FLP32(void) {
  const double x = 0;
  const double y = 0;
  double result;

  result = acos(x);
  result = atan2(y, x);
  result = pow(x, y);
  if (((x == 0.f) && islessequal(y, 0)) ||
      (isless(x, 0))) {
    /* handle domain and range errors */
  }
  result = pow(x, y);
  result = sqrt(x);

  printf("%f", result);
}

void FLP33(void) {
  const short a = 533;
  const int b = 6789;
  const long c = 466;

  const float d = a / 7;
  const double e = b / 30;
  const double f = c * 9;

  printf("%f", d);
  printf("%f", e);
  printf("%f", f);
}

void FLP34(void) {
  float f1 = (float)INT_MAX * 2.0;
  int i1;

  /* initializations */

  i1 = (int)f1; /* Undefined if the integral part of f1 > INT_MAX */

  printf("%d", i1);

  const long double ld = 0.0;
  const double d1 = 0.0;
  double d2 = 0.0;
  f1 = 0.0f;
  float f2 = 0.0f;

  /* initializations */

  f1 = (float)d1;
  f2 = (float)ld;
  d2 = (double)ld;

  printf("%f", f1);
  printf("%f", f2);
  printf("%f", d2);
}

void FLP35(void) {
  float a = 3.0;
  float b = 7.0;
  float c = a / b;

  if (c == a / b) {
    puts("Comparison succeeds");
  } else {
    puts("Unexpected result");
  }
}

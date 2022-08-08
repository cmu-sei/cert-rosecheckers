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

void FLP02();
void FLP03();
void FLP30();
//void FLP31();
void FLP32();
void FLP33();
void FLP34();

void FLP() {
	FLP02();
	FLP03();
	FLP30();
//	FLP31();
	FLP32();
	FLP33();
	FLP34();
}


/* FLP02_A v.42 */

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

/**
 * Due to a bug in rose we can't use the pragma in the CCE
 */
//
//void FLP03(void) {
//  /* ... */
//  double a = 1e-40, b, c = 0.1;
//  float x = 0, y;
//  /* inexact and underflows */
//  y = a;
//  /* divide by zero operation */
//  b = y / x;
//  /* inexact (loss of precision) */
//  c = sin(30) * a;
//  /* ... */
//  printf("FLP03 %f %f\n", b, c);
//}
//
void FLP30(void) {
	float x;
	for (x = 0.1f; x <= 1.0f; x += 0.1f) {
	  /* ... */
	}

	for (x = 100000001.0f; x <= 100000010.0f; x += 1.0f) {
	  /* ... */
	}
}

/* FLP31_A v.15 */

//void FLP31(void) {
//	double complex c = 2.0 + 4.0 * I;
//	/* ... */
//	double complex result = log2(c);
//	printf("%f + %fi\n", creal(result), cimag(result));
//}


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
	printf("FLP30 %f\n", result);
}

void FLP33(void) {
	const short a = 533;
	const int b = 6789;
	const long c = 466438237;

	const float d = a / 7; /* d is 76.0 */
	const double e = b / 30; /* e is 226.0 */
	const double f = c * 789; /*  f may be negative due to overflow */
}

void FLP34(void) {
	float f1 = (float)INT_MAX * 2.0;
	int i1;

	/* initializations */

	i1 = (int)f1; /* Undefined if the integral part of f1 > INT_MAX */

	/* End {code} */

	/* Begin {code} */

	const long double ld = 0.0;
	const double d1 = 0.0;
	double d2 = 0.0;
	f1 = 0.0f;
	float f2 = 0.0f;

	/* initializations */

	f1 = (float)d1;
	f2 = (float)ld;
	d2 = (double)ld;

	/* End {code} */
	printf("FLP34 %f %f %i\n", d2, f2, i1);
}

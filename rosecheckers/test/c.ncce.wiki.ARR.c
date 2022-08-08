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

#include <limits.h>
#include "util.h"

void ARR01();
void ARR02();
void ARR30();
//void ARR31();
void ARR32();
void ARR33();
void ARR34();
//void ARR35();
void ARR36();
void ARR37();
void ARR38();

void ARR() {
	ARR01();
	ARR02();
	ARR30();
	//	ARR31();
	ARR32();
	ARR33();
	ARR34();
//	ARR35();
	ARR36();
	ARR37();
	ARR38();
}

/*ARR01_A v.46 */

void clear2(int array[]) {
  size_t i;
  for (i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
     array[i] = 0;
   }
}

size_t ncce6() {
	char *arr2[12];
	const size_t x = sizeof(arr2[0]); /* error */
	return x;
}

size_t foo8() {
	int *ptr;
	const size_t realloc_size1 = sizeof ptr;
	const size_t realloc_size2 = sizeof(ptr) / 10;
	const size_t realloc_size3 = sizeof(ptr - 1);
	const size_t realloc_size4 = 10 * sizeof(ptr);
	const size_t realloc_size5 = sizeof(ptr) * 10;
	return (realloc_size1 + realloc_size2 + realloc_size3 + realloc_size4 + realloc_size5);
}

size_t foo9() {
	int *array[5];
	const size_t realloc_size1 = sizeof(array[0]) / 10;
	const size_t realloc_size2 = sizeof(array[0] - 1);
	return (realloc_size1 + realloc_size2);
}

/* ARR001 */

void foo1() {
  const char *s = "Hello, World!"; /* pointer to char */
  char *t = (char *)malloc(sizeof(s)); /* error */
  strcpy(t, s); /* buffer overflow */
}

int foo2() {
	extern int *c;
	int k;
	k = sizeof(c);   /* pointer to int */ /* error */
	return k;
}

void foo3() {
	const size_t nelems = 1;
	char **ptr_array = calloc(nelems, sizeof(ptr_array)); /* pointer to pointer to char */
}

void f(char array[]) {
  char *p;
  memcpy(&p, &array, sizeof(array)); /* error ARR01-C */
}

void clear(int a[12]) {
  memset(a, 0, sizeof(a)); /* error ARR01-C */
}

void ARR01(void) {
	int dis[12];
	size_t return_value = 0;
	char arr[10];

	clear2(dis);
	clear(dis);
	f(arr);
	foo1();
	return_value = return_value + foo2();
	foo3();
	return_value = return_value + foo9();
	return_value = return_value + foo8();
	return_value = return_value + ncce6();
}

/*ARR02_A v.08 */

void ARR02(void) {
/* gcc won't allow this
  int ARR02_a[3] = {1, 2, 3, 4};
*/
}

/*ARR30_C v.47 */

int *table2 = NULL;

int ARR30_NCCE_insert_in_table(int pos, int value){
enum { TABLESIZE = 100 };
  if (!table2) {
    table2 = (int *)malloc(sizeof(int) * TABLESIZE);
  }
  if (pos >= TABLESIZE) {
    return -1;
  }
  table2[pos] = value;
  return 0;
}

void ARR30() {
	if (ARR30_NCCE_insert_in_table(0, 1) == -1) {}
}


/*ARR31_C v.53 */

/* not implemented */


///*ARR32_C v.71 */
//void func(size_t s) {
//  int vla[s];
//  /* ... */
//  vla[s-1] = 0;
//}
//
//void ARR32(void) {
//func(50);
//}

/*ARR33_C v.50 */

void ARR33_C(int const src[], size_t len) {
enum { WORKSPACE_SIZE = 256 };
  int dest[WORKSPACE_SIZE];
  memcpy(dest, src, len * sizeof(int));
  /* ... */
}

void ARR33(void) {
	int lol[32];
	ARR33_C(lol, 15);
}

/*ARR34_C v.30 */

void ARR34(void) {
	enum { a = 10, b = 15, c = 20 };

	int arr1[c][b];
	int (*arr2)[a] = NULL;
	/* GCC won't let this by, (the whole purpose of the rule
	arr2 = arr1;
	*/
	printf("ARR34 %p %p\n", arr1, arr2);
}

/*ARR35_C v.21 */

/* No linux solution */

/*ARR36_C v.17 */

void ARR36(void) {
	const int nums[32];
	char *strings[32];
	const int *next_num_ptr = nums;
	int free_bytes = 0;

	free_bytes = strings - (char **)next_num_ptr;
	printf("ARR36 %d\n", free_bytes);
}

/*ARR37_C v.22 */
struct numbers {
  int num1;
  int num2;
  /* ... */
  int num64;
 };

int sum_numbers2(const struct numbers *numb){
  int total = 0;
  int const *numb_ptr;

  for (numb_ptr = &numb->num1;
       numb_ptr <= &numb->num64;
       numb_ptr++)
  {
    total += *(numb_ptr);
  }

  return total;
}

void ARR37(void) {
  struct numbers my_numbers = { 1, 2, /* ... */ 64 };
  if (sum_numbers2(&my_numbers) < 100) {}
}

/*ARR38_C v.28 */
void ARR38(void) {
	int ar[20];
	int *ip;

/* ROSE catches this */
	for (ip = &ar[0]; ip < &ar[21]; ip++) {
	  *ip = 0;
	}

	char *buf = NULL;
	size_t len = 1 << 30;

	/* Check for overflow */
	if (buf + len < buf) {
	  len = -(uintptr_t)buf-1;
	}
}

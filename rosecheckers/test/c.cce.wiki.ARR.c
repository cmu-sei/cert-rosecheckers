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

void ARR01();
void ARR02();
void ARR30();
//void ARR31();
//void ARR32();
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
	//	ARR32();
	ARR33();
	ARR34();
	//	ARR35();
	ARR36();
	ARR37();
	ARR38();
}

/*ARR01_A v.46 */

void clear(int array[], size_t size) {
	size_t i;
	for (i = 0; i < size; i++) {
		array[i] = 0;
	}
}

size_t foo1() {
	size_t *arr[12];
	const size_t num_elems = sizeof(arr) / sizeof(arr[0]);
	return num_elems;
}

size_t foo2() {
	size_t **arr;
	const size_t raw_size = 10;
	const size_t num_elems = raw_size / sizeof(arr[0]);
	return num_elems;
}

size_t foo3() {
	size_t *arr[12];
	const size_t realloc_size = 24 * sizeof(arr[0]);
	return realloc_size;
}

size_t array_exceptions() {
	int *array[5];
	const int x = 5;
	const size_t realloc_size3 = x / sizeof(array[0]);
	const size_t realloc_size1 = x * sizeof(array[0]);
	const size_t realloc_size2 = sizeof(array[0]) * x;
	return (realloc_size3 + realloc_size1 + realloc_size2);
}

size_t cce1() {
	const char * arr[12];
	size_t num_elems;
	num_elems = sizeof(arr);//This might be supposed to fail..?

	return num_elems;
}

size_t cce4() {
	const int *arr[12];
	const int arr2[12];
	const int x = 5;
	const size_t realloc_size1 = 24 * sizeof(arr[0]);
	const size_t realloc_size2 = sizeof(arr[0]) * 24;
	const size_t realloc_size3 = sizeof x + x;
	const size_t realloc_size4 = sizeof(arr2[0]);

	return realloc_size1 + realloc_size2 + realloc_size3 + realloc_size4;
}

void cce5(){
	int *ptr = (int *)malloc(10 * sizeof(*ptr));
}

void ARR01(void) {
	int dis[12];
	size_t return_value = 0;

	clear(dis, sizeof(dis) / sizeof(dis[0]));

	return_value = return_value + array_exceptions();
	return_value = return_value + cce1();
	return_value = return_value + cce4();
	cce5();
	return_value = return_value + foo1();
	return_value = return_value + foo2();
	return_value = return_value + foo3();
}

/*ARR02_A v.08 */

void ARR02(void) {
	const int ARR02_a[4] = { 1, 2, 3, 4 };
	printf("ARR02: %d\n", ARR02_a);
}

/*ARR30_C v.47 */

int *table = NULL;

int ARR30_CS_insert_in_table(size_t pos, int value) {
	enum {
		TABLESIZE = 100
	};
	if (!table) {
		table = (int *) malloc(sizeof(int) * TABLESIZE);
	}
	if (pos >= TABLESIZE) {
		return -1;
	}
	table[pos] = value;
	return 0;
}

void ARR30(void) {
	if (ARR30_CS_insert_in_table(0, 1) != 0) {
		/* Handle Error */
	}
}

/*ARR31_C v.53 */

/* Not implemented. */

/*ARR32_C v.71 */

/* VLAs are NOT Supported */

//enum { MAX_ARRAY = 1024 };
//
//void func(size_t s) {
//  if (s < MAX_ARRAY && s != 0) {
//    int vla[s];
//    /* ... */
//	vla[s-1] = 0;
//  } else {
//    /* Handle Error */
//  }
//}
//
//void ARR32(void) {
//  func(50);
//}

/*ARR33_C v.50 */

void ARR33_A(int const src[], size_t len) {
	enum {
		WORKSPACE_SIZE = 256
	};
	int dest[WORKSPACE_SIZE];
	if (len > WORKSPACE_SIZE) {
		/* Handle Error */
	}
	memcpy(dest, src, sizeof(int) * len);
	/* ... */
}

void ARR33_B(int const src[], size_t len) {
	int *dest;
	if (len > SIZE_MAX / sizeof(int)) {
		/* handle overflow */
	}
	dest = (int *) malloc(sizeof(int) * len);
	if (dest == NULL) {
		/* Couldn't get the memory - recover */
	}
	memcpy(dest, src, sizeof(int) * len);
	/* ... */
	free(dest);
}

void ARR33(void) {
	int lol[32];
	ARR33_A(lol, 15);
	ARR33_B(lol, 15);
}

/*ARR34_C v.30 */

void ARR34(void) {
	enum {
		ARR34_a = 10, ARR34_b = 10, ARR34_c = 20
	};
	int arr1[ARR34_c][ARR34_b];
	int (*arr2)[ARR34_a];
	arr2 = arr1; /* OK, because a == b */
	printf("ARR34 %d\n", arr2[0]);
}

/*ARR35_C v.21 */

/* No linux solution */

/*ARR36_C v.17 */

void ARR36(void) {
	const int nums[16];
	char *strings[16];
	const int *next_num_ptr = nums;
	int free_bytes;

	free_bytes = (next_num_ptr - nums) * sizeof(int);
	printf("ARR36 %d %p\n", free_bytes, strings);
}

/*ARR37_C v.22 */

int sum_numbers(int const *numb, size_t dim) {
	int total = 0;
	int const *numb_ptr;

	for (numb_ptr = numb; numb_ptr < numb + dim; numb_ptr++) {
		total += *(numb_ptr);
	}

	return total;
}

void ARR37(void) {
	int my_numbers[3] = { 1, 2, 3 };
	const int sum = sum_numbers(my_numbers, sizeof(my_numbers)
			/ sizeof(my_numbers[0]));
	printf("%d\n", sum);
}

/*ARR38_C v.28 */

void ARR38(void) {
	int ar[20];
	int *ip;

	for (ip = &ar[0]; ip < &ar[sizeof(ar) / sizeof(ar[0])]; ip++) {
		*ip = 0;
	}

	char buf[] = "foo";
	size_t len = 1u << 30u;

	/* Check for overflow */
	if ((uintptr_t) buf + len < (uintptr_t) buf) {
		len = -(uintptr_t) buf - 1;
	}

	/* Check for overflow */
	if (UINTPTR_MAX - len < (uintptr_t) buf) {
		len = -(uintptr_t) buf - 1;
	}
}

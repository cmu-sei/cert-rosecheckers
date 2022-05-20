/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.// 
// DM21-0505</legal>// 
 */

#include "util.h"

void EXP09_1() {
  const char* random1 = (char *)malloc(10);
  const unsigned char* random2 = (unsigned char*)malloc(20);
  const signed char* random3 = (signed char*)malloc(5);
  int **matrix = (int**) malloc( 10 * sizeof( int*));
  matrix[0] = (int*)realloc( matrix[0], sizeof(int)*10);
  const char* random4 = (char *)malloc(sizeof(char));
}


void* Malloc(size_t amt) {
    void* ptr;

    if(amt == 0)
        return NULL;

    ptr = (void*) malloc(amt);
    if (ptr == NULL) {
        printf("Memory error\n");
        exit(1);
    }
    return ptr;
}


void EXP09_2() {
  const char* foo = (char*) Malloc(5);

  const size_t ptr_size = sizeof(char *);
  size_t i;

  if (ptr_size > SIZE_MAX/100) return;
  char **triarray = (char**) calloc(100, ptr_size);
  if (!triarray) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    triarray[i] = (char*) calloc(20, 0 * 1 + 1 * 1 + 1 * 0);
    if (!triarray[i]) {
      /* handle error */
    }
  }
}

void EXP05(const char *str, size_t slen) {
  const char *p = (const char *) str;

  printf("%p", p);
}

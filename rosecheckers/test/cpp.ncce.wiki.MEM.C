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

void MEM00(void);
void MEM01(void);
void MEM03(void);
void MEM04(void);
void MEM05(void);
void MEM07(void);
void MEM09(void);
void MEM10(void);
void MEM12(void);
void MEM30(void);
void MEM31(void);
void MEM32(void);
void MEM34(void);
void MEM35(void);
void MEM36(void);
void MEM39(void);
void MEM43(void);
void MEM44(void);

void MEM() {
  MEM00();
  MEM01();
  MEM03();
  MEM04();
  MEM05();
  MEM07();
  MEM09();
  MEM10();
  MEM12();
  MEM30();
  MEM31();
  MEM32();
  MEM34();
  MEM35();
  MEM36();
  MEM39();
  MEM43();
  MEM44();
}

void MEM00(void) {
  int num = 5;
  auto_ptr<double> tmp(new double[num]);
}

void MEM01(void) {
  char *message = NULL;
  int message_type=2;

  /* initialize message and message_type */

  if (message_type == 2) {
    /* Process message type 1 */
    free(message);
  }
  /* ...*/
  if (message_type == 3) {
    /* Process message type 2 */
    free(message);
  }
}

struct MEM02_gadget {
  int i;
  double d;
};

struct MEM02_widget {
  char c[10];
  int i;
  double d;
};

/* GCC catches this
void MEM02(void) {
  MEM02_widget *p;

  p = malloc(sizeof(MEM02_gadget));
  if (p != NULL) {
    p->i = 0;
    p->d = 0.0;
  }
}
*/

void MEM03(void) {
  char *secret = NULL;

  /* initialize secret */

  char *new_secret;
  size_t size = strlen(secret);
  if (size == 32) {
    /* Handle error */
  }

  new_secret = (char *)malloc(size+1);
  if (!new_secret) {
    /* Handle error */
  }
  strcpy(new_secret, secret);

  /* Process new_secret... */

  free(new_secret);
  new_secret = NULL;
}

void MEM04(void) {
  size_t size = 8;

  /* initialize size, possibly by user-controlled input */

  int *list = (int *)malloc(size);
  if (list == NULL) {
    /* Handle allocation error */
  }
  else {
    /* Continue processing list */
  }
}

unsigned long MEM05_fib1(unsigned int n) {
  if (n == 0) {
    return 0;
  }
  else if (n == 1 || n == 2) {
    return 1;
  }
  else {
    return MEM05_fib1(n-1) + MEM05_fib1(n-2);
  }
}

void MEM05(void) {
  MEM05_fib1(4);
}

/* MEM06 deals with data security but is ok code. */

void MEM07(void) {
  size_t num_elements = 8;

  long *buffer = (long *)calloc(num_elements, sizeof(long));
  if (buffer == NULL) {
    /* Handle error condition */
  }
  /*...*/
  free(buffer);
  buffer = NULL;
}

struct MEM08_SomeClass {
  int i;
  double d;
  char *p;
};

void MEM08() {
  int num = 5;
  if (num * sizeof(MEM08_SomeClass) > 256) {
    /* handle error */
  }
  MEM08_SomeClass *sc = static_cast<MEM08_SomeClass*>(malloc(sizeof(MEM08_SomeClass)*num));
  if (sc == NULL) {
    /* handle error */
  }
  // initialize sc array
  // ...
  // destroy sc
  free(sc);
}

void MEM09() {
  char *str = NULL;
  /* User supplied data */

  size_t len = strlen(str);
  if (len >= 256 - 1) {
    /* Handle string too long error */
  }
  char *buf = (char *)malloc(256);
  if (buf == NULL) {
    /* Handle allocation error */
  }
  strncpy(buf, str, len);

  /* Process buf */

  free(buf);
  buf = NULL;
}

void MEM10_incr(int *intptr) {
  if (intptr == NULL) {
    /* Handle error */
  }
  *intptr++;
}

void MEM10() {
  int *a = NULL;
  MEM10_incr(a);
}

/* MEM11 is not helpful at this time. */

void MEM12() {
  vector<string> names;
  string new_name;

  do {
    // Adding unknown number of records to a list
    // The user can enter as much data as he wants and exhaust the heap
    cout << "To quit, enter \"quit\"" << endl;
    cout << "Enter record:" << endl;
    getline( cin, new_name);
    if (new_name != "quit")
      // Names continue to be added without bothering about the size on the heap
      names.push_back( new_name);
    cout << new_name << endl;
  } while (new_name != "quit");
}

typedef struct MEM30_node {
  void *data;
  struct MEM30_node *next;
} MEM30_node;

void MEM30() {
  MEM30_node *p;
  MEM30_node *head=NULL;

  for (p = head; p != NULL; p = p->next)
    free(p);
}

void MEM31() {
  size_t num_elem = 7;
  int error_condition = 0;

  int *x = new int[num_elem];
  /* ... */
  if (error_condition == 1) {
    /* handle error condition*/
    delete[] x;
    x = NULL;
  }
  /* ... */
  delete[] x;
  x = NULL;
}

void MEM32() {
  char *input_string=NULL;
  /* initialize from untrusted data */;

  size_t size = strlen(input_string) + 1;
  char *str = (char *)malloc(size);
  strcpy(str, input_string);
  /* ... */
  free(str);
  str = NULL;
}

enum { MEM34_MAX_ALLOCATION = 1000 };

void MEM34() {
  int argc=2;
  const char *argv[20];
  argv[0]=NULL;
  char *str = NULL;
  size_t len;

  if (argc == 2) {
    len = strlen(argv[1])+1;
    if (len > MEM34_MAX_ALLOCATION) {
      return;
    }
    str = new char[len];
    if (str == NULL) {
      return;
    }
    strcpy(str, argv[1]);
  }
  else {
    str = "usage: $>a.exe [string]";
    printf("%s\n", str);
  }

  delete[] str;
}


void MEM35() {
  int len=3;
  char *str=NULL;
  char buf[256];

  /* ... */
  if (len < 256){
    memcpy(buf, str, len);
  }
  /* ... */
}

void MEM36() {
  new int[3], new int[4];
}

/* MEM37 not useful at this time */

void MEM39() {
  int num = 5;
  MEM08_SomeClass *sc = new MEM08_SomeClass[num];
  // ...
  delete sc;
}

/* MEM40 not useful at this time */

class MEM41_Widget {
  // ...
};
class MEM41_Thingy {
public:
  // ...
private:
  MEM41_Widget *w;
};

class MEM42_Widget {
  // ...
};
class MEM42_Thingy {
public:
  // ...
  MEM42_Thingy& operator=(const MEM42_Thingy& rhs);
  // ...
private:
  MEM42_Widget *w;
};
// ...
MEM42_Thingy& MEM42_Thingy::operator=(const MEM42_Thingy& rhs) {
  delete w;                // delete the current MEM42_Widget
  w = new MEM42_Widget(*rhs.w);  // create a copy of rhs's MEM42_Widget
  return *this;
}

/* Caller should free return value */
MEM08_SomeClass* nextInstance() {
  MEM08_SomeClass *result = NULL;
  /* ... */
  return result;
}

void MEM43() {
  MEM08_SomeClass *pst = nextInstance();

  delete pst;
}

void MEM44() {
  while (1) {
    MEM08_SomeClass *pst = new MEM08_SomeClass();
    try {

    }
    catch (...) {
      // deal with exception
      throw;
    }
    delete pst;
  }
}

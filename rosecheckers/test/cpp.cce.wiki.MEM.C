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
#include "cpp.util.h"

void MEM00(void);
void MEM01(void);
void MEM02(void);
void MEM03(void);
void MEM04(void);
void MEM06(void);
void MEM07(void);
void MEM08(void);
void MEM09(void);
void MEM10(void);
void MEM30(void);
void MEM31(void);
void MEM32(void);
void MEM34(void);
void MEM35(void);

void MEM() {
  MEM00();
  MEM01();
  MEM02();
  MEM03();
  MEM04();
  MEM06();
  MEM07();
  MEM08();
  MEM09();
  MEM10();
  MEM30();
  MEM31();
  MEM32();
  MEM34();
  MEM35();
}

void MEM00(void) {
  int num = 5;
  vector<double> tmp(num);
}

void MEM01(void) {
  char *message=NULL;
  int message_type=2;

  /* initialize message and message_type */

  if (message_type == 2) {
    /* Process message type 1 */
    free(message);
    message = NULL;
  }
  /* ...*/
  if (message_type == 3) {
    /* Process message type 2 */
    free(message);
    message = NULL;
  }
}


struct MEM02_date {
  unsigned char dd;
  unsigned yy;
};

void MEM02(void) {
  struct MEM02_date *d = (struct MEM02_date *)malloc(sizeof(struct MEM02_date));
}

void MEM03(void) {
  char *secret=NULL;

  /* initialize secret */

  char *new_secret=NULL;
  size_t size = strlen(secret);
  if (size == 32) {
    /* Handle error */
  }

  /* use calloc() to zero-out allocated space */
  new_secret = (char *)calloc(size+1, sizeof(char));
  if (!new_secret) {
    /* Handle error */
  }
  strcpy(new_secret, secret);

  /* Process new_secret... */

  /* sanitize memory  */
  memset(new_secret, '\0', size);
  free(new_secret);
  new_secret = NULL;
}

void MEM04(void) {
  size_t size=0;

  /* initialize size, possibly by user-controlled input */

  if (size == 0) {
    return;
  }
  int *list = (int *)malloc(size);
  if (list == NULL) {
    /* Handle allocation error */
  }
  /* Continue processing list */
}

/* CCE for MEM05 has no value */


void MEM06(void) {
  struct rlimit limit;
  char *secret=NULL;

  limit.rlim_cur = 0;
  limit.rlim_max = 0;
  if (setrlimit(RLIMIT_CORE, &limit) != 0) {
    /* Handle error */
  }

  /* Create or otherwise obtain some sensitive data */
  if (fgets(secret, sizeof(secret), stdin) == NULL) {
    /* Handle error */
  }
}

void MEM07(void) {
  long *buffer;
  size_t num_elements=0;

  /* initialize num_elements to the number of elements needed */

  if (num_elements > 32/sizeof(long)) {
    /* Handle error condition */
  }
  buffer = (long *)calloc(num_elements, sizeof(long));
  if (buffer == NULL) {
    /* Handle error condition */
  }
}

class MEM08_class {

};

/* Also the same as MEM37 */
void MEM08(void) {
  int num = 5;
  if (num * sizeof(MEM08_class) > 32) {
    /* handle error */
  }
  MEM08_class *sc = new MEM08_class[num];
  // ...
  delete [] sc;
}

void MEM09(void) {
  enum { MAX_BUF_SIZE = 256 };

  char *str=NULL;

  /* initialize string to be copied */

  size_t len = strlen(str);
  if (len >= MAX_BUF_SIZE - 1) {
    /* Handle string too long error */
  }
  char *buf = new char[MAX_BUF_SIZE];
  strncpy(buf, str, len);

  /* Null-terminate string */
  buf[len] = '\0';

  /* process buf */

  free(buf);
 buf = NULL;
}

int MEM10_valid(void *ptr) {
  return (ptr != NULL);
}

void MEM10_incr(int *intptr) {
  if (!MEM10_valid(intptr)) {
    /* Handle error */
  }
  *intptr++;
}

void MEM10(void) {
  int a = 5;
  int *i = &a;
  MEM10_incr(i);
}

/* MEM11 isn't useful at this time */

typedef struct MEM30_node {
  void *data;
  struct MEM30_node *next;
} MEM30_node;


void MEM30(void) {
  MEM30_node *p;
  MEM30_node *q;
  MEM30_node *head = NULL;

  for (p = head; p != NULL; p = q) {
    q = p->next;
    free(p);
  }
  head = NULL;


  char *buff;
  char *source=NULL;

  buff = new char[BUFSIZ];
  /* ... */
  strncpy(buff, source, BUFSIZ-1);
  /* ... */
  delete[] buff;
  buff = NULL;
}

void MEM31(void) {
  size_t num_elem = 4;
  int error_condition = 0;

  if (num_elem > 32/sizeof(int)) {
    /* Handle overflow */
  }
  int *x = new int[num_elem];
  /* ... */
  if (error_condition == 1) {
    /* Handle error condition*/
  }
  /* ... */
  delete[] x;
  x = NULL;
}

void MEM32(void) {
  char *input_string=NULL;

  /* initialize input_string */

  size_t size = strlen(input_string) + 1;
  char *str = new(std::nothrow) char[size];
  if (str == NULL) {
    /* Handle allocation error */
  }
  else {
    strcpy(str, input_string);
    /* ... */
    delete[] str;
    str = NULL;
  }
}

void MEM34(void) {
  char *str = NULL;
  size_t len;

  unsigned int argc = 2;

  if (argc == 2) {
    len = strlen("kitty")+1;
    if (len > 32) {
      /* Handle error */
    }
    str = new char[len];
    strcpy(str, "kitty");
  }
  else {
    printf("%s\n", "usage: $>a.exe [string]");
    return;
  }

  delete[] str;
}

void MEM35(void) {
  size_t len=2;
  char *str=NULL;
  char buf[32];

  /* ... */
  if (len < 32){
    memcpy(buf, str, len);
  }
}

/* MEM36 is not helpful at this time */

/* MEM37 is not helpful at this time */

/* MEM40  is not helpful at this time */

/* MEM41 */
class MEM41_Widget {
};
class MEM41_Thingy {
public:
  ~MEM41_Thingy();

private:
  MEM41_Widget *w;
  MEM41_Thingy (const MEM41_Thingy&);            // declared private, but not defined, to prevent copying
  MEM41_Thingy& operator=(const MEM41_Thingy&);  // declared private, but not defined, to prevent copying
};


/* MEM42 */
MEM41_Thingy& MEM41_Thingy::operator=(const MEM41_Thingy& rhs) {
  if (this != &rhs) {
    delete w;
    w = new MEM41_Widget(*rhs.w);
  }
  return *this;
}

/* MEM43 is not helpful at this time */

/* MEM44 is not helpful at this time */

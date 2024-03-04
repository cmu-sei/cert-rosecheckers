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

#include "cpp.util.h"

void DCL00();
//void DCL02();
void DCL03();
void DCL04();
void DCL05();
void DCL06();
void DCL07();
void DCL08();
void DCL09();
//void DCL10();
//void DCL11();
void DCL12();
void DCL13();
//void DCL14();
void DCL15();
void DCL16();
void DCL17();
void DCL30();
void DCL31();
void DCL32();
//void DCL33();
void DCL34();
//void DCL35();
void DCL36();
//void DCL37();

void DCL() {
  DCL00();
  //DCL02();
  DCL03();
  DCL04();
  DCL05();
  DCL06();
  DCL07();
  DCL08();
  DCL09();
  //DCL10();
  //DCL11();
  DCL12();
  DCL13();
  //DCL14();
  DCL15();
  DCL16();
  DCL17();
  DCL30();
  DCL31();
  DCL32();
  //DCL33();
  DCL34();
  //DCL35();
  DCL36();
  //DCL37();
}

/* DCL_00_CPP */

void DCL00_0() {

  float pi = 3.14159f;
  float degrees=0;
  float radians;
  /* ... */
  radians = degrees * pi / 180;
  printf("%f", radians);
}

#define DCL00_1_MAX (1<<16)
void DCL00_1_f(int a) { return; }
void DCL00_1_f(long a) { return; }

void DCL00_1() {
  DCL00_1_f(DCL00_1_MAX);
}

void DCL00() {
  DCL00_0();
  DCL00_1();
}


/* DCL_01_CPP */

char DCL01_msg[100];

void DCL01_report_error(const char *error_msg) {
  char msg[80];
  /* ... */
  strncpy(msg, error_msg, sizeof(msg));
  return;
}

void DCL01() {
  char error_msg[80];
  /* ... */
  DCL01_report_error(error_msg);
  /* ... */
}


/* DCL_02_CPP */
//blank

/* DCL_03_CPP */
struct DCL_03_timer {
  uint8_t MODE;
  uint32_t DATA;
  uint32_t COUNT;
};


void DCL03() {
  assert(offsetof(struct DCL_03_timer, DATA) == 4);
}


/* DCL_04_CPP */
void DCL04() {
  char *src = 0, c = 0;
  int i, j = 1;
  printf("%s", src);
  printf("%s", c);
  i=4;
  printf("%i", i);
  printf("%d", j);
}


/* DCL_05_CPP */

void (*DCL05_signal(int, void (*)(int)))(int);

//no DCL05 function
void DCL05() { }


/* DCL_06_CPP */

void DCL06_0() {
  int age = 4;
  /* ... */
  if (age >= 18) {
    /* Take action */
  }
  else {
    /* Take a different action */
  }
  /* ... */
}

void DCL06_1() {
  char buffer[256];
  /* ... */
  fgets(buffer, 256, stdin);
}

/*XXX how to get this to run? */
//void DCL06_2() {
//  LDAP *ld = ldap_init("localhost", 1234);
//  if (ld == NULL) {
//    perror("ldap_init");
//    return(1);
//  }
//}

void DCL06() {
  DCL06_0();
  DCL06_1();
  //DCL06_2();
}


/* DCL_07_CPP */

void DCL07() {
  //
  // other code
  //
  size_t i = 0;
  for (i = 0; i < 10; i++) {
    // perform operations
  }
}


/* DCL_08_CPP */

void DCL08() {
  enum { IN_STR_LEN=18, OUT_STR_LEN=20 };
  enum { ADULT_AGE=18 };
  enum { ALCOHOL_AGE=ADULT_AGE+3 };
}


/* DCL_09_CPP */

/* Begin {code} */

enum { NO_FILE_POS_VALUES = 3 };


int DCL09_opener(
    FILE *file,
    int *width,
    int *height,
    int *data_offset
    ) {
  int file_w;
  int file_h;
  int file_o;
  fpos_t offset;

  if (file == NULL) { return EINVAL; }
  errno = 0;
  if (fgetpos(file, &offset) != 0) { return errno; }
  if (fscanf(file, "%i %i %i", &file_w, &file_h, &file_o)
      != NO_FILE_POS_VALUES) {
    return EIO;
  }

  errno = 0;
  if (fsetpos(file, &offset) != 0) { return errno; }

  if (width != NULL) { *width = file_w; }
  if (height != NULL) { *height = file_h; }
  if (data_offset != NULL) { *data_offset = file_o; }

  return 0;
}

//nothing to do
void DCL09() { }


/* DCL_10_CPP */
//blank


/* DCL_11_CPP */
//blank


/* DCL_12_CPP */
template <typename T>
class DCL12_A{
  public:
    void f1() { /* ... */ }
    void f2() {
      T t;
      t.x = 50;
    }
};

void DCL12() {
  DCL12_A<int32_t> a;
  a.f1();
}

/* DCL_13_CPP */
void DCL13_0(int * x) {
  if (x != NULL) {
    printf("Value is %d\n", *x);
  }
  /* ... */
}

char *DCL13_strcat_nc(char *s1, char *s2) {return NULL;}
void DCL13_1() {
  char *str1 = "str1";
  const char *str2 = "str2";
  char str3[9] = "str3";
  const char str4[9] = "str4";
  printf("%s", str2);
  printf("%s", str4);

  //XXX Compiler gives errors on these two, so we have to comment them out
  //DCL13_strcat_nc(str3, str2);	/* Compiler warns that str2 is const */
  //DCL13_strcat_nc(str4, str3);  /* Compiler warns that str4 is const */
  DCL13_strcat_nc(str1, str3);  /* Attempts to overwrite string literal! */
}

void DCL13() {
  int x = 4;
  DCL13_0(&x);
  DCL13_1();
}


/* DCL_14_CPP */
//XXX works across multiple files, so we can't test for it with rose


/* DCL_15_CPP */
enum { DCL15_MAX = 100 };

int DCL15_helper(int i) {
  /* perform some computation based on i */
  return 0;
}

void DCL15() {
  size_t i;
  int out[DCL15_MAX];

  for (i = 0; i < DCL15_MAX; i++) {
    out[i] = DCL15_helper(i);
  }
  printf("%s", out);
}


/* DCL_16_CPP */
void DCL16() {
  std::cout << "Sum is " << 1111 + 111l << std::endl;
}


/* DCL_17_CPP */

typedef int Contact;

void DCL17_0_print_vector(vector<Contact> v){ /* Pass parameters by value decrease performance */
  for(int i=0; i<v.size(); i++){
    cout<<v[i]<<endl;
  }
}

void DCL17_0() {
  /* ... */
  vector<Contact> vec;
  /* Initialize the vector vec with instantiations of Contact class */
  /* ... */
  DCL17_0_print_vector(vec); /* Slow */
}

void DCL17_1_print_vector(vector<Contact>& v){ /* Pass parameters by non-const reference violates DCL13-CPP */
  for(int i=0; i<v.size(); i++){
    cout<<v[i]<<endl;
  }
}

void DCL17_1() {
  /* ... */
  vector<Contact> vec;
  /* Initialize the vector vec with instantiations of Contact class */
  /* ... */
  DCL17_1_print_vector(vec); /* Fast */
}

void DCL17_2_print_vector(vector<Contact>& v){
  sort(v.begin(), v.end()); /* Visible outside function */
  for(int i=0; i<v.size(); i++){
    cout<<v[i]<<endl;
  }
}

void DCL17_2() {
  /* ... */
  vector<Contact> vec;
  /* Initialize the vector vec with instantiations of Contact class */
  /* ... */
  DCL17_2_print_vector(vec);
}

//XXX has (predicted) compiler errors
//void DCL17_3_print_vector(const vector<Contact>& v){
//  sort(v.begin(), v.end()); /* Compiler error in attempt to alter const parameter v */
//  for(int i=0; i<v.size(); i++){
//    cout<<v[i]<<endl;
//  }
//}
//
//void DCL17_3() {
//  /* ... */
//  vector<Contact> vec;
//  /* Initialize the vector vec with instantiations of Contact class */
//  /* ... */
//  DCL17_3_print_vector(vec);
//  /* End {code} */
//}

void DCL17() {
  DCL17_0();
  DCL17_1();
  DCL17_2();
  //DCL17_3();
}


/* DCL_30_CPP */

/* Begin {code} */

const char *DCL30_0_p;
void DCL30_0_dont_do_this(void) {
  const char str[] = "This will change";
  DCL30_0_p = str; /* dangerous */
  /* ... */
}

void DCL30_0_innocuous(void) {
  const char str[] = "Surprise, surprise";
  printf("%s", str);
}
/* ... */
void DCL30_0() {
  DCL30_0_dont_do_this();
  DCL30_0_innocuous();
} /* p might be pointing to "Surprise, surprise" */


char *DCL30_1_init_array(void) {
  char array[10];
  /* Initialize array */
  //XXX EXPECT WARNING XXX
  return array;
}

void DCL30() {
  DCL30_0();
  //just leave DCL30_1 in the code
  //DCL30_1();
}


/* DCL_31_CPP */

char *DCL31_concatenate(const char *s, ...) {
  // code to actually concatenate the strings
  char *a=NULL;
  return a;
}

void DCL31() {
  char *separator = " ";
  char *t = DCL31_concatenate("hello", separator, "world", NULL);
  char *u = DCL31_concatenate("hello", separator, "world"); // undefined behavior
  char *v = DCL31_concatenate("hello", ' ', "world", NULL); // undefined behavior
}


/* DCL_32_CPP */

void DCL32() {
  int _Page_Count=0;
  printf("%d", _Page_Count);
}


/* DCL_33_CPP */

/*XXX gcc catches the error with this...*/
//void DCL33() {
//  char c = 'c';
//  char &const p = c;
//  p = 'p';
//  cout << c << endl;
//}


/* DCL_34_CPP */

sig_atomic_t DCL34_0_i;

void DCL34_0_handler(int signum) {
  DCL34_0_i = 0;
}

void DCL34_0() {
  DCL34_0_i = 1;
  signal(SIGINT, DCL34_0_handler);
  while (DCL34_0_i) {
    /* do something */
  }
}

sig_atomic_t DCL34_1_i;

void DCL34_1_handler(int signum) {
  DCL34_1_i = 0;
}

void DCL34_1() {
  DCL34_1_i = 1;
  signal(SIGINT, DCL34_1_handler);
  while (*(volatile sig_atomic_t *)&DCL34_1_i) {
    /* do something */
  }
}

void DCL34() {
  DCL34_0();
  DCL34_1();
}


/* DCL_35_CPP */
// no code


/* DCL_36_CPP */

int DCL36_i1() {return 10;}  /* definition, external linkage */
static int DCL36_i2() {return 20;}  /* definition, internal linkage */
extern int DCL36_i3() {return 30;}  /* definition, external linkage */

int DCL36_i1();  /* valid declaration */
int DCL36_i2();  /* not valid, linkage disagreement with previous */
int DCL36_i3();  /* valid declaration */

void DCL36() {
  (void)DCL36_i1;
  (void)DCL36_i2;
  (void)DCL36_i3;
}


/* DCL_37_CPP */
// no code

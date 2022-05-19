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

#include "cpp.util.h"
#include "cpp.lib.FIO.h"


void FIO00();
void FIO01();
void FIO02();
void FIO03();
void FIO04();
void FIO05();
void FIO06();
void FIO07();
void FIO08();
void FIO09();
void FIO10();
//void FIO11();
void FIO12();
void FIO13();
//void FIO14();
void FIO15();
void FIO16();
void FIO17();
void FIO18();
void FIO30();
void FIO31();
void FIO32();
void FIO33();
void FIO34();
void FIO35();
void FIO36();
void FIO37();
void FIO38();
void FIO39();
void FIO40();
void FIO41();
void FIO42();
void FIO43();
void FIO44();
void FIO45();

void FIO() {
  FIO00();
  FIO01();
  FIO02();
  FIO03();
  FIO04();
  FIO05();
  FIO06();
  FIO07();
  FIO08();
  FIO09();
  FIO10();
  //FIO11();
  FIO12();
  FIO13();
  //FIO14();
  FIO15();
  FIO16();
  FIO17();
  FIO18();
  FIO30();
  FIO31();
  FIO32();
  FIO33();
  FIO34();
  FIO35();
  FIO36();
  FIO37();
  FIO38();
  FIO39();
  FIO40();
  FIO41();
  FIO42();
  FIO43();
  FIO44();
  FIO45();
}



/* FIO00_cpp */
void FIO00() {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  /* ... */
  printf("Error (type %s): %d\n", error_type, error_msg);
}


/* FIO01_cpp */
void FIO01_0() {
  char file_name[] = "foo";
  FILE *f_ptr;

  /* initialize file_name */

  f_ptr = fopen(file_name, "w");
  if (f_ptr == NULL) {
    /* Handle error */
  }

  /*... Process file ...*/

  if (fclose(f_ptr) != 0) {
    /* Handle error */
  }

  if (remove(file_name) != 0) {
    /* Handle error */
  }
}

void FIO01_1() {
  char file_name[] = "foo";
  FILE *f_ptr;

  /* initialize file_name */

  f_ptr = fopen(file_name, "w");
  if (f_ptr == NULL)  {
    /* Handle error */
  }

  /* ... */

  if (chmod(file_name, S_IRUSR) == -1) {
    /* Handle error */
  }
}

void FIO01() {
  FIO01_0();
  FIO01_1();
}


/* FIO02_cpp */


/* Verify argv[1] is supplied */

void FIO02_0() {
  char string[] = "string";
  if (!verify_file(string)) {
    /* Handle error */
  }

  if (fopen(string, "w") == NULL) {
    /* Handle error */
  }
  /* ... */
}


void FIO02_1() {

  char *realpath_res = NULL;
  char *canonical_filename = NULL;
  size_t path_size = 0;
  long pc_result;
  char string[] = "string";

  /* Verify string is supplied */

  errno = 0;

  /* Query for PATH_MAX */
  pc_result = pathconf(string, _PC_PATH_MAX);

  if ( (pc_result == -1) && (errno != 0) ) {
    /* Handle error */
  } else if (pc_result == -1) {
    /* Handle error */
  } else if (pc_result <= 0) {
    /* Handle error */
  }
  path_size = (size_t)pc_result;

  if (path_size > 0) {
    canonical_filename = (char *) malloc(path_size);

    if (canonical_filename == NULL) {
      /* Handle error */
    }

    realpath_res = realpath(string, canonical_filename);
  }

  if (realpath_res == NULL) {
    /* Handle error */
  }

  if (!verify_file(realpath_res)) {
    /* handle error */
  }

  if (fopen(realpath_res, "w") == NULL) {
    /* Handle error */
  }

  /* ... */

  free(canonical_filename);
  canonical_filename = NULL;

}
//XXX a windows thing
//void FIO02_2() {
//  /* ... */
//
//  enum { INITBUFSIZE = 256 };
//  //using unsigned long instead of original DOWRD
//  unsigned long ret = 0;
//  unsigned long new_ret = 0;
//  char *canonical_filename;
//  char *new_file;
//  char *file_name;
//  char string[] = "string";
//
//  /* ... */
//
//  file_name = (char *)malloc(strlen(string)+1);
//  canonical_filename = (char *)malloc(INITBUFSIZE);
//
//  if ( (file_name != NULL) && (canonical_filename != NULL) ) {
//    strcpy(file_name, string);
//    strcpy(canonical_filename, "");
//  } else {
//    /* Handle error */
//  }
//
//    ret = GetFullPathName(
//        file_name,
//        INITBUFSIZE,
//        canonical_filename,
//        NULL
//        );
//
//    if (ret == 0) {
//      /* Handle error */
//    }
//    else if (ret > INITBUFSIZE) {
//      new_file = (char *)realloc(canonical_filename, ret);
//      if (new_file == NULL) {
//        /* Handle error */
//      }
//
//      canonical_filename = new_file;
//
//      new_ret = GetFullPathName(
//          file_name,
//          ret,
//          canonical_filename,
//          NULL
//          );
//      if (new_ret > ret) {
//        /*
//         * The length of the path changed between calls
//         * to GetFullPathName(), handle error
//         */
//      }
//      else if (new_ret == 0) {
//        /* Handle error */
//      }
//    }
//
//  if (!verify_file(canonical_filename)) {
//    /* Handle error */
//  }
//  /* Verify file name before using */
//}
void FIO02() {
  FIO02_0();
  FIO02_1();
  //FIO02_2();
}


/* FIO03_cpp */

void FIO03_0() {
  char file_name[] = "foo";
  FILE *fp;

  /* initialize file_name */

  fp = fopen(file_name, "w");
  if (!fp) {
    /* Handle error */
  }
}

//XXX can't use this since gcc doesn't support
//XXX fopen_s yet.
//void FIO03_1() {
//  char *file_name;
//  FILE *fp;
//
//  /* initialize file_name */
//  errno_t res = fopen_s(&fp, file_name, "w");
//  if (res != 0) {
//    /* Handle error */
//  }
//}
//
void FIO03_2() {
  ifstream fi("foo.txt");// attempt to open as input file
  if (!fi) {
    // file doesn't exist; so it's safe to create it and write to it
    ofstream fo("foo.txt");
    // can now write to fo, e.g., fo << "Some output" << endl;
    // /* ... */
  }
  else {  // file exists; close and handle error
    fi.close();
    // handle error
  }

}

void FIO03() {
  FIO03_0();
  //FIO03_1();
  FIO03_2();
}


/* FIO04_cpp */

void FIO04() {
  FILE *file = fopen("foo", "w");
  long offset = 0;

  /* initialize file and offset */

  fseek(file, offset, SEEK_SET);
  /* process file */
}


/* FIO05_cpp */

void FIO05_0() {
  char file_name[] = "foo";

  /* initialize file_name */

  FILE *fd = fopen(file_name, "w");
  if (fd == NULL) {
    /* Handle error */
  }

  /*... write to file ...*/

  fclose(fd);
  fd = NULL;

  /*
   * A race condition here allows for an attacker to switch
   * out the file for another
   */

  /* ... */

  fd = fopen(file_name, "r");
  if (fd == NULL) {
    /* Handle error */
  }

  /*... read from file ...*/

  fclose(fd);
  fd = NULL;
}

void FIO05_1() {

  char file_name[] = "foo";
  FILE *fd;

  /* initialize file_name */

  fd = fopen(file_name, "w+");
  if (fd == NULL) {
    /* Handle error */
  }

  /* read user's file */

  fclose(fd);
  fd = NULL;
}

void FIO05() {
  FIO05_0();
  FIO05_1();
}


/* FIO06_cpp */

void FIO06_0() {
  char file_name[] = "foo";
  FILE *fp;

  /* initialize file_name */

  fp = fopen(file_name, "w");
  if (!fp){
    /* Handle error */
  }
}

void FIO06_1() {
  char file_name[] = "foo";
  int fd;

  /* initialize file_name */

  fd = open(file_name, O_CREAT | O_WRONLY);
  /* access permissions were missing */

  if (fd == -1){
    /* Handle error */
  }
}

void FIO06() {
  FIO06_0();
  FIO06_1();
}


/* FIO07_cpp */

void FIO07() {

  char file_name[] = "foo";
  FILE *fp;

  /* initialize file_name */

  fp = fopen(file_name, "r");
  if (fp == NULL) {
    /* Handle open error */
  }

  /* read data */

  rewind(fp);

  /* continue */
}


/* FIO08_cpp */

void FIO08() {
  char file_name[] = "foo";
  FILE *file;

  file = fopen(file_name, "w+");
  if (file == NULL) {
    /* Handle error condition */
  }

  /* ... */

  if (remove(file_name) != 0) {
    /* Handle error condition */
  }

  /* continue performing I/O operations on file */

  fclose(file);
}

/* FIO09_cpp */

struct FIO09_myData {
  char c;
  long l;
};

/* ... */

void FIO09() {
  FILE *file = fopen("foo", "rb");
  struct FIO09_myData data;

  /* initialize file */

  if (fread(&data, sizeof(struct FIO09_myData), 1, file) < sizeof(struct FIO09_myData)) {
    /* Handle error */
  }
}


/* FIO10_cpp */

void FIO10_0() {
  const char *src_file = "foo";
  const char *dest_file = "bar";
  if (rename(src_file, dest_file) != 0) {
    /* Handle Error */
  }
}

void FIO10_1() {
  const char *src_file = "foo";
  const char *dest_file = "bar";
  if (rename(src_file, dest_file) != 0) {
    /* Handle error */
  }
}

void FIO10() {
  FIO10_0();
  FIO10_1();
}


/* FIO11_cpp */
//XXX no tester code


/* FIO12_cpp */
void FIO12() {

  FILE *file = fopen("foo", "w");
  /* Setup file */
  setbuf(file, NULL);
  /* ... */

}


/* FIO13_cpp */

void FIO13() {
  FILE *fp;
  char *file_name = "foo";

  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    /* Handle error */
  }

  /* read data */

  if (ungetc('\n', fp) == EOF) {
    /* Handle error */
  }
  if (ungetc('\r', fp) == EOF) {
    /* Handle error */
  }
  /* continue on */
}


/* FIO14_cpp */
//XXX no tester code


/* FIO15_cpp */

void FIO15() {
  char file_name[] = "foo";
  FILE *fp;

  /* initialize file_name */

  fp = fopen(file_name, "w");
  if (fp == NULL) {
    /* Handle error */
  }

  /*... Process file ...*/

  if (fclose(fp) != 0) {
    /* Handle error */
  }

  if (remove(file_name) != 0) {
    /* Handle error */
  }

}


/* FIO16_cpp */

void FIO16() {
  enum { array_max = 100 };

  char str1[] = "foo";
  char str2[] = "bar";

  /*
   * Program running with elevated privileges where argv[1]
   * and argv[2] are supplied by the user
   */

  char x[array_max];
  FILE *fp = fopen(str1, "w");

  strncpy(x, str2, array_max);
  x[array_max - 1] = '\0';

  /*
   * Write operation to an unintended file like /etc/passwd
   * gets executed
   */
  if (fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp) <
      sizeof(x)/sizeof(x[0])) {
    /* Handle Error */
  }

}


/* FIO17_cpp */

void FIO17() {
  char filename[256];
  FILE *f;
  char format[256];

  fscanf(stdin, "%s", filename);
  f = fopen(filename, "r");  // read only

  if (f == 0) {
    sprintf(format, "Error opening file %s\n", filename);
    fprintf(stderr, format);
    return;
  }
  fclose(f);
}

/* FIO18_cpp */
void FIO18() {
  char * buffer;
  long size1 = 10, size2 = 10;
  ofstream outfile ("new.txt", ofstream::binary);
  buffer = new char [size1];

  // write to outfile
  outfile.write (buffer, size2);

  delete[] buffer;
  outfile.close();
}


/* FIO30_cpp */


void FIO30_0_incorrect_password(const char *user) {
  /* user names are restricted to 256 characters or less */
  static const char *msg_format
    = "%s cannot be authenticated.\n";
  size_t len = strlen(user) + strlen(msg_format) + 1;
  char *msg = new char[len];
  int ret = sprintf(msg, msg_format, user);
  if (ret < 0 || ((size_t) ret) >= len) {
    /* Handle error */
  }
  fprintf(stderr, msg);
  delete[] msg;
  msg = NULL;
}



void FIO30_1_incorrect_password(const char *user) {
  /* user names are restricted to 256 characters or less */
  static const char *msg_format
    = "%s cannot be authenticated.\n";
  size_t len = strlen(user) + strlen(msg_format) + 1;
  char *msg = new char[len];
  int ret = sprintf(msg, msg_format, user);
  if (ret < 0 || ((size_t) ret) >= len) {
    /* Handle error */
  }
  syslog(LOG_INFO, msg);
  delete[] msg;
  msg = NULL;
}

void FIO30() {
  //nothing to run
}


/* FIO31_cpp */

void FIO31_do_stuff(void) {
  ofstream logfile("log", ios::app);

  /* Write logs pertaining to do_stuff() */
  logfile << "do_stuff" << endl;

  /* ... */
}

void FIO31() {
  ofstream logfile("log", ios::app);

  /* Write logs pertaining to main() */
  logfile << "main" << endl;

  FIO31_do_stuff();

  /* ... */
}

/* FIO32_cpp */

void FIO32() {
  char file_name[] = "foo";
  FILE *file;

  /* initialize file_name */

  if (!fgets(file_name, sizeof(file_name), stdin)) {
    /* Handle error */
  }

  if ((file = fopen(file_name, "wb")) == NULL) {
    /* Handle error */
  }

  /* operate on file */

  fclose(file);
}


/* FIO33_cpp */

void FIO33_0() {
  char buf[BUFSIZ];

  fgets(buf, sizeof(buf), stdin);
  buf[strlen(buf) - 1] = '\0'; /* Overwrite newline */
}

void FIO33_1() {
  char file_name[] = "str";
  FILE * fptr = fopen(file_name, "w");
  /* process file */
}

void FIO33_2() {
  char buffer[BUFFERSIZE];
  char s[] = "computer";
  char c = 'l';
  int i = 35;
  int j = 0;
  float fp = 1.7320534f;

  /* Format and print various data: */
  j = snprintf(
      buffer,
      sizeof(buffer),
      " String:    %s\n",
      s
      );
  j += snprintf(
      buffer + j,
      sizeof(buffer) - j,
      " Character: %c\n",
      c
      );
  j += snprintf(
      buffer + j,
      sizeof(buffer) - j,
      " Integer:   %d\n",
      i
      );
  j += snprintf(
      buffer + j,
      sizeof(buffer) - j,
      " Real:      %f\n",
      fp
      );
}

void FIO33() {
  FIO33_0();
  FIO33_1();
  FIO33_2();
}


/* FIO34_cpp */
void FIO34() {
  char buf[BUFSIZ];
  char c;
  int i = 0;

  while ( (c = cin.get()) != '\n' && c != EOF ) {
    if (i < BUFSIZ-1) {
      buf[i++] = c;
    }
  }
  buf[i] = '\0'; /* terminate NTBS */
  printf("%s\n", buf);
}


/* FIO35_cpp */
void FIO35() {
  int c;

  do {
    /* ... */
    c = getchar();
    /* ... */
  } while (c != EOF);
}


/* FIO36_cpp */
void FIO36() {
  char buf[BUFSIZ + 1];

  if (fgets(buf, sizeof(buf), stdin)) {
    if (*buf) { /* see FIO37-CPP */
      buf[strlen(buf) - 1] = '\0';
    }
  }
  else {
    /* Handle error condition */
  }
}


/* FIO37_cpp */
void FIO37() {
  char buf[BUFSIZ + 1];

  if (fgets(buf, sizeof(buf), stdin) == NULL) {
    /* Handle error */
  }
  buf[strlen(buf) - 1] = '\0';
}


/* FIO38_cpp */


void FIO38() {
  FILE my_stdout = *(stdout);
  if (fputs("Hello, World!\n", &my_stdout) == EOF) {
    /* Handle error */
  }
}


/* FIO39_cpp */
void FIO39() {
  char data[BUFFERSIZE];
  char append_data[BUFFERSIZE];
  char file_name[] = "foo";
  FILE *file;

  /* Initialize file_name */

  file = fopen(file_name, "a+");
  if (file == NULL) {
    /* Handle error */
  }

  /* initialize append_data */

  if (fwrite(append_data, BUFFERSIZE, 1, file) != BUFFERSIZE) {
    /* Handle error */
  }
  if (fread(data, BUFFERSIZE, 1, file) != 0) {
    /* Handle there not being data */
  }

  fclose(file);

}


/* FIO40_cpp */

void FIO40() {
  char buf[BUFSIZ];
  FILE *file = fopen("foo", "r");
  /* Initialize file */

  if (fgets(buf, sizeof(buf), file) == NULL) {
    /* set error flag and continue */
  }

}


/* FIO41_cpp */

void FIO41_0() {
  char file_name[] = "foo";
  FILE *fptr;

  /* Initialize file_name */

  int c = getc(fptr = fopen(file_name, "r"));
  if (c == EOF) {
    /* Handle error */
  }
  fclose(fptr);
}

void FIO41_1() {
  char file_name[] = "foo";
  FILE *fptr = NULL;

  /* Initialize file_name */

  int c = 'a';
  while (c <= 'z') {
    if (putc(c++, fptr ? fptr :
          (fptr = fopen(file_name, "w"))) == EOF) {
      /* Handle error */
    }
  }
}
void FIO41() {
  FIO41_0();
  FIO41_1();
}


/* FIO42_cpp */
void FIO42() {

  FILE* f;
  const char *editor;
  char file_name[] = "foo";

  /* Initialize file_name */

  f = fopen(file_name, "r");
  if (f == NULL) {
    /* Handle fopen() error */
  }
  /* ... */
  editor = getenv("EDITOR");
  if (editor == NULL) {
    /* Handle getenv() error */
  }
  if (system(editor) == -1) {
    /* Handle error */
  }
}


/* FIO43_cpp */

void FIO43_0() {
  char file_name[] = "str";

  FILE *fp = fopen(file_name, "wb+");
  if (fp == NULL) {
    /* Handle error */
  }
}


//XXX except warnings about use of tmpnam
void FIO43_1() {

  char file_name[L_tmpnam];
  FILE* fp;

  if (!tmpnam(file_name)) {
    /* Handle error */
  }

  /* A TOCTOU race condition exists here */

  fp = fopen(file_name, "wb+");
  if (fp == NULL) {
    /* Handle error */
  }
}


void FIO43_2() {
  char file_name[L_tmpnam];
  int fd;

  if (!(tmpnam(file_name))) {
    /* Handle error */
  }

  /* A TOCTOU race condition exists here */

  fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
  if (fd < 0) {
    /* Handle error */
  }
}

//XXX not yet in GCC
//void FIO43_3() {
//  char file_name[L_tmpnam_s];
//  int fd;
//
//  if (tmpnam_s(file_name, L_tmpnam_s) != 0) {
//    /* Handle error */
//  }
//
//  /* A TOCTOU race condition exists here */
//  fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
//  if (fd < 0) {
//    /* Handle error */
//  }
//}

void FIO43_4() {
  char file_name[] = "tmp-XXXXXX";
  int fd;

  if (!mktemp(file_name)) {
    /* Handle error */
  }

  /* A TOCTOU race condition exists here */

  fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
  if (fd < 0) {
    /* Handle error */
  }
}

void FIO43_5() {
  FILE* fp = tmpfile();
  if (fp == NULL) {
    /* Handle error */
  }
}

//XXX not yet in GCC
//void FIO43_6() {
//  if (tmpfile_s(&fp)) {
//    /* Handle error */
//  }
//}

void FIO43() {
  void FIO43_0();
  void FIO43_1();
  void FIO43_2();
  //void FIO43_3();
  void FIO43_4();
  void FIO43_5();
  //void FIO43_6();
}


/* FIO44_cpp */

enum { NO_FILE_POS_VALUES = 3 };

int opener(FILE* file/*,  ... */ ) {
  int rc;
  fpos_t offset;

  /* ... */

  memset(&offset, 0, sizeof(offset));

  if (file == NULL) { return EINVAL; }

  /* Read in data from file */

  rc = fsetpos(file, &offset);
  if (rc != 0 ) { return rc; }

  /* ... */

  return 0;
}

void FIO44() {
  //nothing to run
}


/* FIO45_cpp */

void FIO45() {
  char * buffer;
  size_t size = BUFFERSIZE;

  ifstream infile ("test.txt", ifstream::binary);

  // allocate memory for file content
  buffer = new char [size];

  // read content of infile
  infile.read (buffer, size);

  infile.close();
}

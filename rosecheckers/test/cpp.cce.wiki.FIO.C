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

void FIO00_0() {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  /* ... */
  printf("Error (type %d): %s\n", error_type, error_msg);
}

void FIO00_1() {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  /* ... */
  cout << "Error (type " << error_type << ": " << error_msg << endl;
}

void FIO00() {
  FIO00_0();
  FIO00_1();
}


/* FIO01_cpp */

void FIO01() {
  char *file_name=NULL;
  int fd;

  /* initialize file_name */

  fd = open(
      file_name,
      O_WRONLY | O_CREAT | O_EXCL,
      S_IRWXU
      );

  if (fd == -1) {
    /* Handle error */
  }

  /* ... */

  if (fchmod(fd, S_IRUSR) == -1) {
    /* Handle error */
  }
}



/* FIO02_cpp */

char FIO02_argv[] = "string";

void FIO02_0() {
  char *realpath_res = NULL;

  /* Verify FIO02_argv is supplied */

  realpath_res = realpath(FIO02_argv, NULL);
  if (realpath_res == NULL) {
    /* Handle error */
  }

  if (!verify_file(realpath_res)) {
    /* Handle error */
  }

  if (fopen(realpath_res, "w") == NULL) {
    /* Handle error */
  }

  /* ... */

  free(realpath_res);
  realpath_res = NULL;
}

void FIO02_1() {
  char *realpath_res = NULL;
  char *canonical_filename = NULL;
  size_t path_size = 0;

  /* Verify FIO02_argv is supplied */

  path_size = (size_t)PATH_MAX;

  if (path_size > 0) {
    canonical_filename = (char *) malloc(path_size);

    if (canonical_filename == NULL) {
      /* Handle error */
    }

    realpath_res = realpath(FIO02_argv, canonical_filename);
  }

  if (realpath_res == NULL) {
    /* Handle error */
  }

  if (!verify_file(realpath_res)) {
    /* Handle error */
  }
  if (fopen(realpath_res, "w") == NULL ) {
    /* Handle error */
  }

  /* ... */

  free(canonical_filename);
  canonical_filename = NULL;
}

void FIO02_2() {
  /* Verify FIO02_argv is supplied */

  char *canonical_filename = canonicalize_file_name(FIO02_argv);
  if (canonical_filename == NULL) {
    /* Handle error */
  }

  /* Verify file name */

  if (fopen(canonical_filename, "w") == NULL) {
    /* Handle error */
  }

  /* ... */

  free(canonical_filename);
  canonical_filename = NULL;
}

void FIO02() {
  FIO02_0();
  FIO02_1();
  FIO02_2();
}


/* FIO03_cpp */

void FIO03_0() {
  char *file_name=NULL;
  int new_file_mode=0;

  /* initialize file_name and new_file_mode */

  int fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, new_file_mode);
  if (fd == -1) {
    /* Handle error */
  }
}

void FIO03_1() {
  char *file_name=NULL;

  /* initialize file_name */

  FILE *fp = fopen(file_name, "wx");
  if (!fp) {
    /* Handle error */
  }
}

void FIO03_2() {
  char *file_name=NULL;
  int new_file_mode=0;
  FILE *fp;
  int fd;

  /* initialize file_name and new_file_mode */

  fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, new_file_mode);
  if (fd == -1) {
    /* Handle error */
  }

  fp = fdopen(fd, "w");
  if (fp == NULL) {
    /* Handle error */
  }
}

void FIO03() {
  FIO03_0();
  FIO03_1();
  FIO03_2();
}


/* FIO04_cpp */
void FIO04() {
  FILE *file=NULL;
  long offset=0;

  /* initialize file and offset */

  if (fseek(file, offset, SEEK_SET) != 0) {
    /* Handle error */
  }
  /* process file */
}


/* FIO05_cpp */

void FIO05_0() {
  struct stat orig_st;
  struct stat new_st;
  char *file_name=NULL;

  /* initialize file_name */

  int fd = open(file_name, O_WRONLY);
  if (fd == -1) {
    /* Handle error */
  }

  /*... write to file ...*/

  if (fstat(fd, &orig_st) == -1) {
    /* Handle error */
  }
  close(fd);
  fd = -1;

  /* ... */

  fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    /* Handle error */
  }

  if (fstat(fd, &new_st) == -1) {
    /* Handle error */
  }

  if ((orig_st.st_dev != new_st.st_dev) ||
      (orig_st.st_ino != new_st.st_ino)) {
    /* file was tampered with! */
  }

  /*... read from file ...*/

  close(fd);
  fd = -1;
}

void FIO05_1() {
  char *file_name=NULL;
  FILE *fd;

  /* initialize file_name */

  fd = fopen(file_name, "w+");
  if (fd == NULL) {
    /* Handle error */
  }

  /*... write to file ...*/

  /* go to beginning of file */
  fseek(fd, 0, SEEK_SET);

  /*... read from file ...*/

  fclose(fd);
  fd = NULL;
}

void FIO05_2() {
  struct stat st;
  char *file_name=NULL;

  /* initialize file_name */

  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    /* Handle error */
  }

  if ((fstat(fd, &st) == -1) ||
      (st.st_uid != getuid()) ||
      (st.st_gid != getgid())) {
    /* file does not belong to user */
  }

  /*... read from file ...*/

  close(fd);
  fd = -1;
}

void FIO05() {
  FIO05_0();
  FIO05_1();
  FIO05_2();
}


/* FIO06_cpp */

void FIO06_0() {

  //  char *file_name=NULL;
  //  FILE *fp;

  /* initialize file_name */

  //XXX we don't have fopen_s
  //errno_t res = fopen_s(&fp, file_name, "w");
  //if (res != 0) {
  //  /* Handle error */
  //}
}

void FIO06_1() {
  char *file_name=NULL;
  int file_access_permissions=0;

  /* initialize file_name and file_access_permissions */

  int fd = open(
      file_name,
      O_CREAT | O_WRONLY,
      file_access_permissions
      );
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
  char *file_name = "foo";
  FILE *fp;

  fp = fopen(file_name, "r");
  if (fp == NULL) {
    /* Handle open error */
  }

  /* read data */

  if (fseek(fp, 0L, SEEK_SET) != 0) {
    /* Handle repositioning error */
  }

  /* continue */
}


/* FIO08_cpp */
void FIO08() {
  FILE *file;
  char *file_name = "foo";

  file = fopen(file_name, "w+");
  if (file == NULL) {
    /* Handle error condition */
  }

  if (unlink(file_name) != 0) {
    /* Handle error condition */
  }

  /*... continue performing I/O operations on file ...*/

  fclose(file);
}


/* FIO09_cpp */

struct FIO09_myData {
  char c;
  long l;
};

void FIO09() {
  /* ... */

  FILE *file=NULL;
  struct FIO09_myData data;
  char buf[25];
  char *end_ptr;

  /* initialize file */

  if (fgets(buf, 1, file) == NULL) {
    /* Handle error */
  }

  data.c = buf[0];

  if (fgets(buf, sizeof(buf), file) == NULL) {
    /* Handle Error */
  }

  data.l = strtol(buf, &end_ptr, 10);

  if ((ERANGE == errno)
      || (end_ptr == buf)
      || ('\n' != *end_ptr && '\0' != *end_ptr)) {
    /* Handle Error */
  }
  printf("%d", data.c);
}


/* FIO10_cpp */
void FIO10_0() {
  const char *src_file = "foo";
  const char *dest_file = "bar";

  if (access(dest_file, F_OK) != 0) {
    if (rename(src_file, dest_file) != 0) {
      /* Handle error condition */
    }
  }
  else {
    /* Handle file-exists condition */
  }
}

void FIO10_1() {
  const char *src_file = "foo";
  const char *dest_file = "bar";
  if (rename(src_file, dest_file) != 0) {
    /* Handle Error */
  }
}

void FIO10_2() {
  const char *src_file = "foo";
  const char *dest_file = "bar";

  //XXX microsoft stuff
  //  if (_access_s(dest_file, 0) == 0) {
  //    if (remove(dest_file) != 0) {
  //      /* Handle error condition */
  //    }
  //  }

  if (rename(src_file, dest_file) != 0) {
    /* Handle error condition */
  }
}

void FIO10_3() {
  const char *src_file = "foo";
  const char *dest_file = "bar";
  if (rename(src_file, dest_file) != 0) {
    /* Handle error condition */
  }
}

void FIO10_4() {
  const char *src_file = "foo";
  const char *dest_file = "bar";

  (void)remove(dest_file);

  if (rename(src_file, dest_file) != 0) {
    /* Handle error condition */
  }
}

void FIO10_5() {
  const char *src_file = "foo";
  const char *dest_file = "bar";

  //XXX there is no file exists function
  if (!access(dest_file, F_OK)) {
    if (rename(src_file, dest_file) != 0) {
      /* Handle error condition */
    }
  }
  else {
    /* Handle file-exists condition */
  }
}

void FIO10() {
  FIO10_0();
  FIO10_1();
  FIO10_2();
  FIO10_3();
  FIO10_4();
  FIO10_5();
}


/* FIO11_cpp */
//XXX no tester code


/* FIO12_cpp */

void FIO12() {
  FILE *file=NULL;
  char *buf = NULL;
  /* Setup file */
  if (setvbuf(file, buf, buf ? _IOFBF : _IONBF, BUFSIZ) != 0) {
    /* Handle error */
  }
  /* ... */
}


/* FIO13_cpp */

void FIO13() {
  FILE *fp=NULL;
  fpos_t pos;
  fgetpos(fp, &pos);
  char *file_name=NULL;

  /* initialize file_name */

  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    /* Handle error */
  }

  /* read data */

  if (fgetpos(fp, &pos)) {
    /* Handle error */
  }

  /* read the data that will be "pushed back" */

  if (fsetpos(fp, &pos)) {
    /* Handle error */
  }

  /* Continue on */
}


/* FIO14_cpp */
//XXX no tester code

/* FIO15_cpp */

/* Returns nonzero if directory is secure, zero otherwise */
int secure_dir(const char *fullpath) {
  char *path_copy = NULL;
  char *dirname_res = NULL;
  char ** dirs = NULL;
  int num_of_dirs = 0;
  int secure = 1;
  int i;
  struct stat buf;
  uid_t my_uid = geteuid();

  if (!(path_copy = strdup(fullpath))) {
    /* Handle error */
  }

  dirname_res = path_copy;
  /* Figure out how far it is to the root */
  while (1) {
    dirname_res = dirname(dirname_res);

    num_of_dirs++;

    if ((strcmp(dirname_res, "/") == 0) ||
        (strcmp(dirname_res, "//") == 0)) {
      break;
    }
  }
  free(path_copy);
  path_copy = NULL;

  /* Now allocate and fill the dirs array */
  if (!(dirs = (char **)malloc(num_of_dirs*sizeof(*dirs)))) {
    /* Handle error */
  }
  if (!(dirs[num_of_dirs - 1] = strdup(fullpath))) {
    /* Handle error */
  }

  if (!(path_copy = strdup(fullpath))) {
    /* Handle error */
  }

  dirname_res = path_copy;
  for (i = 1; i < num_of_dirs; i++) {
    dirname_res = dirname(dirname_res);

    dirs[num_of_dirs - i - 1] = strdup(dirname_res);

  }
  free(path_copy);
  path_copy = NULL;

  /* Traverse from the root to the leaf, checking
   * permissions along the way */
  for (i = 0; i < num_of_dirs; i++) {
    if (stat(dirs[i], &buf) != 0) {
      /* Handle error */
    }
    if ((buf.st_uid != my_uid) && (buf.st_uid != 0)) {
      /* Directory is owned by someone besides user or root */
      secure = 0;
    } else if ((buf.st_mode & (S_IWGRP | S_IWOTH))
        && ((i == num_of_dirs - 1) || !(buf.st_mode & S_ISVTX))) {
      /* Others have permissions to the leaf directory
       * or are able to delete or rename files along the way */
      secure = 0;
    }

    free(dirs[i]);
    dirs[i] = NULL;
  }

  free(dirs);
  dirs = NULL;

  return secure;
}

void FIO15() {
  char *dir_name=NULL;
  char *canonical_dir_name;
  const char *file_name = "passwd"; /* file name within the secure directory */
  FILE *fp;

  /* initialize dir_name */

  canonical_dir_name = realpath(dir_name, NULL);
  if (canonical_dir_name == NULL) {
    /* Handle error */
  }

  if (!secure_dir(canonical_dir_name)) {
    /* Handle error */
  }

  if (chdir(canonical_dir_name) == -1) {
    /* Handle error */
  }

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
  /*
   * Make sure that the chroot/jail directory exists within
   * the current working directory. Also assign appropriate
   * permissions to the directory to restrict access. Close
   * all file system descriptors to outside resources lest
   * they escape the jail.
   */
  char str1[] = "foo";
  char str2[] = "bar";
  const size_t array_max = 10;

  if (setuid(0) == -1) {
    /* Handle error */
  }

  if (chroot("chroot/jail") == -1) {
    /* Handle error */
  }

  if (chdir("/") == -1) {
    /* Handle error */
  }

  /* Drop privileges permanently */
  if (setgid(getgid()) == -1) {
    /* Handle error */
  }

  if (setuid(getuid()) == -1) {
    /* Handle error */
  }

  /* Perform unprivileged operations */
  enum {array_mex = 100};

  FILE *fp = fopen(str1, "w");
  char x[array_max];
  strncpy(x, str2, array_max);
  x[array_max - 1] = '\0';

  /* Write operation safe is safe within jail */
  if (fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp) <
      sizeof(x)/sizeof(x[0])) {
    /* Handle error */
  }

}


/* FIO17_cpp */

void FIO17() {
  string filename;
  ifstream ifs;

  cin >> filename;
  ifs.open(filename.c_str());

  if (ifs.fail()) {
    cerr << "Error opening file" << filename << endl;
    return;
  }
  ifs.close();
}


/* FIO18_cpp */

void FIO18() {
  char * buffer=NULL;
  size_t size, length=0;

  ofstream outfile ("new.txt", ofstream::binary);
  // suppose buffer is populated with a c-string
  size = sizeof(buffer);
  outfile.write (buffer, length); // suppose length is already initialized.&#xA0;
  if (size != length) {// File is not properly null-terminated. Handle this
    delete[] buffer;
    outfile.close();
  }
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
  if (fputs(msg, stderr) == EOF) {
    /* Handle error */
  }
  delete[] msg;
  msg = NULL;
}

void FIO30_1_incorrect_password(const char *user) {
  fprintf(stderr, "%s cannot be authenticated.\n", user);
}

void FIO30_2_incorrect_password(const char *user) {
  cerr << user << " cannot be authenticated." << endl;
}

void FIO30_3_incorrect_password(const char *user) {
  syslog(LOG_INFO, "%s cannot be authenticated.", user);
}

void FIO30() {
  //nothing to run
}


/* FIO31_cpp */


void FIO31_do_stuff(ostream& logfile) {
  /* Write logs pertaining to do_stuff() */
  logfile << "do_stuff" << endl;

  /* ... */
}

void FIO31() {
  ofstream logfile("log", ios::app);

  /* Write logs pertaining to main() */
  logfile << "main" << endl;

  FIO31_do_stuff(logfile);

  /* ... */
}

/* FIO32_cpp */

#ifdef O_NOFOLLOW
#define OPEN_FLAGS O_NOFOLLOW | O_NONBLOCK
#else
#define OPEN_FLAGS O_NONBLOCK
#endif

/* ... */

void FIO32_0() {
  struct stat orig_st;
  struct stat open_st;
  int fd;
  int flags;
  char *file_name=NULL;

  /* initialize file_name */

  if (!fgets(file_name, sizeof(file_name), stdin)) {
    /* Handle error */
  }

  if ((lstat(file_name, &orig_st) != 0)
      || (!S_ISREG(orig_st.st_mode)))
  {
    /* Handle error */
  }

  /* A TOCTOU race condition exists here, see below */

  fd = open(file_name, OPEN_FLAGS | O_WRONLY);
  if (fd == -1) {
    /* Handle error */
  }

  if (fstat(fd, &open_st) != 0) {
    /* Handle error */
  }

  if ((orig_st.st_mode != open_st.st_mode) ||
      (orig_st.st_ino  != open_st.st_ino) ||
      (orig_st.st_dev  != open_st.st_dev)) {
    /* file was tampered with */
  }

  /* Optional: drop the O_NONBLOCK now that we are sure
   * this is a good file */
  if ((flags = fcntl(fd, F_GETFL)) == -1) {
    /* Handle error */
  }

  if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) != 0) {
    /* Handle error */
  }

  /* Operate on file */

  close(fd);
}

//XXX this is for windows
//void FIO32_1() {
//  HANDLE hFile = CreateFile(
//      pFullPathName, 0, 0, NULL, OPEN_EXISTING, 0, NULL
//      );
//  if (hFile == INVALID_HANDLE_VALUE) {
//    /* Handle error */
//  }
//  else {
//    if (GetFileType(hFile) != FILE_TYPE_DISK) {
//      /* Handle error */
//    }
//    /* operate on file */
//  }
//}
//
void FIO32() {
  FIO32_0();
  //FIO32_1();
}


/* FIO33_cpp */

void FIO33_0() {
  char buf[BUFSIZ];
  char *p;

  if (fgets(buf, sizeof(buf), stdin)) {
    /* fgets succeeds, scan for newline character */
    p = strchr(buf, '\n');
    if (p) {
      *p = '\0';
    }
    else {
      /* newline not found, flush stdin to end of line */
      while ((getchar() != '\n')
          && !feof(stdin)
          && !ferror(stdin)
          );
    }
  }
  else {
    /* fgets failed, handle error */
  }
}

void FIO33_1() {
  char file_name[] = "foo";
  FILE * fptr = fopen(file_name, "w");
  if (fptr == NULL) {
    /* Handle error */
  }
  else {
    /* process file */



    char buffer[BUFFERSIZE];
    char s[] = "computer";
    char c = 'l';
    int i = 35;
    int j = 0;
    int rc = 0;
    float fp = 1.7320534f;

    /* Format and print various data: */
    rc = snprintf(
        buffer,
        sizeof(buffer),
        " String: %s\n",
        s
        );
    if (rc == -1 || rc >= sizeof(buffer)) /* Handle error */ ;
    else j += rc;

    rc = snprintf(
        buffer + j,
        sizeof(buffer) - j,
        " Character: %c\n",
        c
        );
    if (rc == -1 || rc >= sizeof(buffer) - j) /* Handle error */ ;
    else j += rc;

    rc = snprintf(
        buffer + j,
        sizeof(buffer) - j,
        " Integer: %d\n",
        i
        );
    if (rc == -1 || rc >= sizeof(buffer) - j) /* Handle error */ ;
    else j += rc;

    rc = snprintf(
        buffer + j,
        sizeof(buffer) - j,
        " Real: %f\n",
        fp
        );
    if (rc == -1 || rc >= sizeof(buffer) - j) /* Handle error */ ;
  }
}

void FIO33() {
  FIO33_0();
  FIO33_1();
}


/* FIO34_cpp */
void FIO34() {
  char buf[BUFSIZ];
  buf[0]='a';
  int c;
  int i = 0;

  while (((c = cin.get()) != '\n')
      && !cin.eof()
      && !cin)
  {
    if (i < BUFSIZ-1) {
      buf[i++] = c;
    }
  }
  printf("%d", c);
  buf[i] = '\0'; /* terminate NTBS */
  printf("%s", buf);
}


/* FIO35_cpp */
void FIO35_0() {
  int c;

  do {
    /* ... */
    c = getchar();
    /* ... */
  } while (!feof(stdin) && !ferror(stdin));
  printf("%d", c);
}


//XXX static assert??
//void FIO35_1() {
//  int c;
//
//  static_assert(sizeof(char) < sizeof(int));
//  do {
//    /* ... */
//    c = getchar();
//    /* ... */
//  } while (c != EOF);
//}

void FIO35() {
  FIO35_0();
  //FIO35_1();
}

/* FIO36_cpp */

void FIO36() {
  char buf[BUFSIZ + 1];
  char *p;

  if (fgets(buf, sizeof(buf), stdin)) {
    p = strchr(buf, '\n');
    if (p) {
      *p = '\0';
    }
  }
  else {
    /* Handle error condition */
  }
}

/* FIO37_cpp */

void FIO37() {
  char buf[BUFSIZ + 1];
  char *p;

  if (fgets(buf, sizeof(buf), stdin)) {
    p = strchr(buf, '\n');
    if (p) {
      *p = '\0';
    }
  }
  else {
    /* Handle error condition */
  }
}


/* FIO38_cpp */

void FIO38() {
  FILE *my_stdout = stdout;
  if (fputs("Hello, World!\n", my_stdout) == EOF) {
    /* Handle error */
  }
}


/* FIO39_cpp */
void FIO39() {
  char data[BUFFERSIZE];
  char append_data[BUFFERSIZE];
  char *file_name=NULL;
  FILE *file;

  /* initialize file_name */

  file = fopen(file_name, "a+");
  if (file == NULL) {
    /* Handle error */
  }

  /* Initialize append_data */

  if (fwrite(append_data, BUFFERSIZE, 1, file) != BUFFERSIZE) {
    /* Handle error */
  }

  if (fseek(file, 0L, SEEK_SET) != 0) {
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
  FILE *file=NULL;
  /* Initialize file */

  if (fgets(buf, sizeof(buf), file) == NULL) {
    /* set error flag and continue */
    *buf = '\0';
  }
}


/* FIO41_cpp */

void FIO41_0() {
  int c;
  char *file_name=NULL;
  FILE *fptr;

  /* Initialize file_name */

  fptr = fopen(file_name, "r");
  if (fptr == NULL) {
    /* Handle error */
  }

  c = getc(fptr);
  if (c == EOF) {
    /* Handle error */
  }
  printf("%d", c);
}

void FIO41_1() {
  char *file_name=NULL;

  /* Initialize file_name */

  FILE *fptr = fopen(file_name, "w");
  if (fptr == NULL) {
    /* Handle error */
  }

  int c = 'a';

  while (c <= 'z') {
    if (putc(c++, fptr) == EOF) {
      /* Handle error */
    }
  }
}

void FIO41() {
  FIO41_0();
  FIO41_1();
}


/* FIO42_cpp */

void FIO42_0() {
  FILE* f;
  const char *editor;
  char *file_name=NULL;

  /* Initialize file_name */

  f = fopen(file_name, "r");
  if (f == NULL) {
    /* Handle fopen() error */
  }
  /* ... */
  fclose(f);
  f = NULL;
  editor = getenv("EDITOR");
  if (editor == NULL) {
    /* Handle getenv() error */
  }
  /* Sanitize environment before calling system()! */
  if (system(editor) == -1) {
    /* Handle Error */
  }
}

void FIO42_1() {
  int flags;
  char *editor;
  char *file_name=NULL;

  /* Initialize file_name */

  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    /* Handle error */
  }

  flags = fcntl(fd, F_GETFD);
  if (flags == -1) {
    /* Handle error */
  }

  if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1) {
    /* Handle error */
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

//XXX Andrew computers have linux kernel version
//XXX 2.6.16, but 2.6.23 is required for this
//void FIO41_2() {
//  char *editor;
//  char *file_name;
//
//  /* Initialize file_name */
//
//  int fd = open(file_name, O_RDONLY | O_CLOEXEC);
//  if (fd == -1) {
//    /* Handle error */
//  }
//
//  /* ... */
//
//  editor = getenv("EDITOR");
//  if (editor == NULL) {
//    /* Handle getenv() error */
//  }
//  if (system(editor) == -1) {
//    /* Handle error */
//  }
//}

void FIO42() {
  FIO42_0();
  FIO42_1();
  //FIO42_2();
}


/* FIO43_cpp */

void FIO43() {
  const char *sdn = "/home/usr1/";
  char sfn[] = "/home/usr1/temp-XXXXXX";
  FILE *sfp;

  if (!secure_dir(sdn)) {
    /* Handle error */
  }

  int fd = mkstemp(sfn);
  if (fd == -1) {
    /* Handle error */
  }

  /*
   * Unlink immediately to hide the file name.
   * The race condition here is inconsequential if the file
   * is created with exclusive permissions (glibc >= 2.0.7)
   */

  if (unlink(sfn) == -1) {
    /* Handle error */
  }

  sfp = fdopen(fd, "w+");
  if (sfp == NULL) {
    close(fd);
    /* Handle error */
  }

  /* use temporary file */

  fclose(sfp); /* also closes fd */
}


/* FIO44_cpp */


enum { NO_FILE_POS_VALUES = 3 };

int opener(FILE* file /*, ... */) {
  int rc;
  fpos_t offset;

  /* ... */

  if (file == NULL) { return EINVAL; }

  rc = fgetpos(file, &offset);
  if (rc != 0 ) { return rc; }

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
  size_t size=0;
  streampos length;

  ifstream infile ("test.txt", ifstream::binary);

  // get length of the file
  infile.seekg(0, ifstream::end);
  length = infile.tellg();
  infile.seekg(0);

  // allocate memory for the content of the file
  buffer = new char [size];
  infile.read (buffer, size);
  if (size != length) {
    // buffer is not properly null-terminated. Handle this case
  }
  infile.close();
}

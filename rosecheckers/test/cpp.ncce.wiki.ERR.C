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

#include "cpp.util.h"
#include "cpp.lib.ERR.h"

//void ERR00();
void ERR01();
void ERR02();
void ERR03();
void ERR04();
void ERR05();
void ERR06();
void ERR07();
void ERR08();
void ERR09();
void ERR10();
void ERR30();
void ERR31();
void ERR32();
void ERR33();
void ERR34();
void ERR35();
void ERR36();
void ERR37();
void ERR() {
  //ERR00();
  ERR01();
  ERR02();
  ERR03();
  ERR04();
  ERR05();
  ERR06();
  ERR07();
  ERR08();
  ERR09();
  ERR10();
  ERR30();
  ERR31();
  ERR32();
  ERR33();
  ERR34();
  ERR35();
  ERR36();
  ERR37();
}



/* ERR00_cpp */
void ERR00() { }


/* ERR01_cpp */
void ERR01() {
  errno = 0;
  printf("This\n");
  printf("is\n");
  printf("a\n");
  printf("test.\n");
  if (errno != 0) {
    fprintf(stderr, "printf failed: %s\n", strerror(errno));
  }
}


/* ERR02_cpp */

//XXX context specific
//int i;
//ssize_t count = 0;
//
//for (i = 0; i < 9; ++i) {
//  count += sprintf(
//      buf + count, "%02x ", ((u8 *)&slreg_num)[i]
//      );
//}
//count += sprintf(buf + count, "\n");
//


constraint_handler_t handle_errors() {
  constraint_handler_t data = NULL;
  /* define what to do when error occurs */
  return data;
}


//XXX can't use this part, because it is implementation
//specific, but its not the important part
//set_constraint_handler(handle_errors);
//
///* Returns zero on success */
//errno_t function(char *dst1){
//  char src1[100] = "hello";
//
//  strcpy_s(dst1, sizeof(dst1), src1);
//  /* At this point strcpy_s may have yielded an
//     error and handle_errors() might have returned */
//
//  /* ... */
//  return 0;
//}
//

//nothing to run
void ERR02() { }


/* ERR03_cpp */

//again, we dont have strcpy_s
//errno_t function(char *dst1, size_t size){
//  char src1[100] = "hello";
//
//  if (strcpy_s(dst1, size, src1) != 0) {
//    return -1;
//  }
//  /* ... */
//  return 0;
//}
//
//nothing to run
void ERR03() { }


/* ERR04_cpp */

void ERR04_0() {
  //XXX changed from priting "Hello, World!"
  //to hello.txt to just printing to stdout
  printf("Hello, World\n!");
  abort(); /* oops! data might not get written! */
  /* ... */
}

void ERR04() { }

void ERR05_g(void) {
  /* ... */
  int a = 0;
  int b = 1;
  if (/*something_really_bad_happens*/ a == b) {
    fprintf(stderr, "Something really bad happened!\n");
    abort();
  }
  /* ... */
}

void ERR05() {
  ERR05_g();
  /* ... do the rest of f ... */
}


/* ERR06_cpp */

void ERR06_cleanup() {
  /* delete temporary files, restore consistent state, etc. */
}

void ERR06() {
  int a = 1;
  if (atexit(ERR06_cleanup) != 0) {
    /* Handle error */
  }


  assert(a == a);

  /* ... */
}


/* ERR07_cpp */

/* Begin {code} */

void ERR07() {
  int result = 0;
  if (result == -1) {
    /* deal with error */
  }
  /* normal processing from here */
  // /* ... */
}

/* ERR08_cpp */

void ERR08() {
  try {
    printf("Hello, World!\n");
  }
  catch( const char *msg ) {
    std::string m( msg );
    if ( m == "stack underflow" ) { }
    else throw;
  }
}


/* ERR09_cpp */

//XXX dont run!!
void ERR09_0() {
  try {
    printf("hello world\n");

  static runtime_error r2("runtime");
  extern std::runtime_error *lastRuntimeError;
  lastRuntimeError = &r2;
  throw r2;

  }
  catch( StackUnderflow su ) {
    su.modify(); // modify argument
    throw; // modifications lost
  }
  catch( std::runtime_error re ) {
    // /* ... */
    throw re; // original type of exception lost
  }
}

void ERR09() { }


/* ERR10_cpp */

void ERR10() {
  char input_string[] = "input";
  char *str = (char *)malloc(strlen(input_string)+1);
  strcpy(str, input_string); /* What if malloc() fails? */
}


/* ERR30_cpp */

void ERR30_0() {
  string object; // might not get destroyed if exception thrown
  /*do useful work*/
  return;
}

class ERR30_exception1 : public exception {};
class ERR30_exception2 : public exception {};

void f(void) throw( ERR30_exception1) {
  // /* ... */
  throw (ERR30_exception2());
}

void ERR30_1() {
  try {
    f();
  }
  catch(ERR30_exception1 &e) {
    cerr << "F called" << endl;
  }
}

void ERR30() { }


/* ERR31_cpp */

void ERR31() {
  //compile time error
  //  extern int errno;
}



/* ERR32_cpp */


typedef void (*pfv)(int);

void handler(int signum) {
  pfv old_handler = signal(signum, SIG_DFL);
  if (old_handler == SIG_ERR) {
    perror("SIGINT handler"); /* undefined behavior */
    /* handle error condition */
  }
}

void ERR32_0() {
  pfv old_handler = signal(SIGINT, handler);
  if (old_handler == SIG_ERR) {
    perror("SIGINT handler");
    /* handle error condition */
  }

  /* main code loop */
}

void reaper(int signum) {
  errno = 0;
  for (;;) {
    int rc = waitpid(-1, NULL, WNOHANG);
    if ( (0 == rc) || (-1 == rc && EINTR != errno) )
      break;
  }
  if (ECHILD != errno) {
    /* handle error */
  }
}

void ERR32_1() {
  struct sigaction act;
  act.sa_handler = reaper;
  act.sa_flags = 0;
  if (sigemptyset(&act.sa_mask) != 0) {
    /* handle error */
  }
  if (sigaction(SIGCHLD, &act, NULL) != 0) {
    /* handle error */
  }

  /* ... */
}

void ERR32() {
  ERR32_0();
  ERR32_1();
}


/* ERR33_cpp */

class ERR33_X {
  public:
    ERR33_X() { }
    ~ERR33_X() {
      bool someCondition = false;
      if (someCondition) {
        throw exception();
      }
    }
};

void ERR33() {
  ERR33_X a = ERR33_X();
}

/* ERR34_cpp */

static jmp_buf ERR34_env;

class ERR34_Counter {
  public:
    static int Instances;

    ERR34_Counter() {Instances++;}
    ~ERR34_Counter() {Instances--;}
  private:
    ERR34_Counter(const ERR34_Counter& that);
    ERR34_Counter& operator=(const ERR34_Counter& that);
};

int ERR34_Counter::Instances = 0;
class ERR34_Error {};

void ERR34_func() {
  ERR34_Counter c;
  cout << "ERR34_func(): Instances: " << ERR34_Counter::Instances << endl;
  longjmp( ERR34_env, 1);
}

void ERR34() {

  cout << "Before setjmp(): Instances: " << ERR34_Counter::Instances << endl;
  if (setjmp(ERR34_env) == 0) {
    ERR34_func();
  } else {
    cout << "From longjmp(): Instances: " << ERR34_Counter::Instances << endl;
  }

  cout << "After longjmp(): Instances: " << ERR34_Counter::Instances << endl;
}



/* ERR35_cpp */
class ERR35_C {
  private:
    int x;
  public:
    ERR35_C() : x(0) {
      try {
        // /* ... */
      } catch (...) {
        if (0 == x) {
          // /* ... */
        }
      }
    }
};

void ERR35() {
  ERR35_C a = ERR35_C();
}


/* ERR36_cpp */

// /* ... */ Using the classes from above
// compiler successfully warns about this, but it should be kept
void ERR36() {
  try {
    // /* ... */
  } catch (ERR36_B &b) {
    // /* ... */
  } catch (ERR36_D &d) {
    // /* ... */
  }
}

/* ERR37_cpp */
void ERR37_foo() {
  throw ERR37_exception2(); // ok...since ERR37_foo() promises nothing wrt exceptions
}

void ERR37_bar() throw (ERR37_exception1) {
  ERR37_foo();    // bad, since ERR37_foo() can throw ERR37_exception2
}

//don't need to run anything, leave blank
void ERR37() { }

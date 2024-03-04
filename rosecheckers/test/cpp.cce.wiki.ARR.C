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

void ARR00();
void ARR01();
void ARR02();
//void ARR03();
void ARR04();
void ARR30();
//void ARR31();
void ARR32();
void ARR33();
void ARR34();
void ARR35();
void ARR36();
void ARR37();
void ARR38();
void ARR39();
void ARR40();
//void ARR41();
void ARR42();
void ARR43();
void ARR44();

void ARR() {
  ARR00();
  ARR01();
  ARR02();
  //ARR03();
  ARR04();
  ARR30();
  //ARR31();
  ARR32();
  ARR33();
  ARR34();
  ARR35();
  ARR36();
  ARR37();
  ARR38();
  ARR39();
  ARR40();
  //ARR41();
  ARR42();
  ARR43();
  ARR44();
}

/*use for all such standard array lengths*/
enum {ARR_LEN = 100};

/* ARR_00_CPP */
enum {ARR00_ARRAY_SIZE = 100};
void ARR00() {
  std::vector<int> array(ARR_LEN);
}

/* ARR_01_CPP */
void ARR01_0_clear(int array[], size_t len) {
  for (size_t i = 0; i < len; i++) {
    array[i] = 0;
  }
}
void ARR01_0() {
  int dis[12];
  ARR01_0_clear(dis, sizeof(dis) / sizeof(dis[0]));
  /* ... */
}


void ARR01_1_clear(int a[], size_t len) {
  memset(a, 0, len * sizeof(int));
}

void ARR01_1() {
  int b[ARR_LEN];
  ARR01_1_clear(b, ARR_LEN);
  assert(b[ARR_LEN / 2]==0); /* cannot fail */
}


template <typename T, int N>
inline int  ARR01_arraySize(T (&)[N])   { return N; }

void ARR01_2() {
  char src[] = "hello, world";
  char *dest = new char[ARR01_arraySize(src)];
  strcpy(dest, src);
}

void ARR01() {
  ARR01_0();
  ARR01_1();
  ARR01_2();
}

/* ARR_02_CPP */
void ARR02() {
  int a[4] = {1, 2, 3, 4};
  //to satisfy the warnings
  a[0] = a[1];
}

/* ARR_03_CPP */
/*no example compliance code from wiki*/

class Widget {/**/};
const int WIDGET_SIZE = 10;

/* ARR_04 CPP */
void ARR04_0() {
  vector<Widget*> container;
  for (size_t i = 0; i < WIDGET_SIZE; i++) {
    container.push_back(new Widget(/* ... */));
  }
  /* Work with container */
  /* Free container's widgets*/
  for (vector<Widget*>::iterator it = container.begin(); it != container.end(); ++it) {
    delete *it;
  }
  /* frees container */
}

/*XXX CANT GET UNIQUE PTR TO WORK:*/
//#include <utility>
//using std::unique_ptr;
//void ARR04_1() {
//  vector< unique_ptr<Widget> > container;
//  for (size_t i = 0; i < WIDGET_SIZE; i++) {
//    container.push_back(new Widget(/* ... */));
//  }
//  /* Work with container */
//  /* frees container, which frees widgets */
//}
//
//void ARR04_2() {
//  Widget[WIDGET_SIZE] widgets;
//  {
//    for (size_t i = 0; i < WIDGET_SIZE; i++) {
//      widgets[i] = Widget(/* ... */);
//    }
//
//    vector< unique_ptr<Widget> > container;
//    for (size_t i = 0; i < WIDGET_SIZE; i++) {
//      container.push_back( widgets[i]);
//    }
//    /* Work with container */
//    /* frees container, but not widgets */
//  }
//  /* ... */
//} /* widgets freed now */

void ARR04() {
  ARR04_0();
}

/*RULES*/

/* ARR_30_CPP */
enum { TABLESIZE = 100 };

int *ARR30_0_table = NULL;

int ARR30_0_insert_in_table(size_t pos, int value){
  if (!ARR30_0_table) {
    ARR30_0_table = new int[TABLESIZE];
  }
  if (pos >= TABLESIZE) {
    return -1;
  }
  ARR30_0_table[pos] = value;
  return 0;
}

template <typename T>
void ARR30_1_clear(T array[], size_t n) {
  for (size_t i = 0; i < n; ++i) {
    array[i] = 0;
  }
}
template <typename T, size_t n>
inline void ARR30_1_clear(T (&array)[n]) {
  ARR30_1_clear(array, n);
}
void ARR30_1() {
  int int_array[12];
  ARR30_1_clear(int_array); // deduce T is int, and that n is 12
}

vector<int> ARR30_2_table;

int ARR30_2_insert_in_table(size_t pos, int value){
  if (pos >= ARR30_2_table.size()) {
    return -1;
  }
  ARR30_2_table[pos] = value;
  return 0;
}

vector<int> ARR30_3_table;

int ARR30_3_insert_in_table(size_t pos, int value){
  ARR30_3_table.at(pos) = value;
  return 0;
}

void ARR30() {
  ARR30_0_insert_in_table(0, 0);
  ARR30_1();
  ARR30_2_insert_in_table(0, 0);
  ARR30_3_insert_in_table(0, 0);
}

/* ARR_31_CPP */
/*XXX How would we detect something across
 * multiple files?*/
/* insert_a.hpp include file */
//enum { ARRAYSIZE = 100 };
//
//extern char *a;
//void insert_a(void);
///* end include file */
//
//char *a;
//void insert_a(void) {
//   a[0] = 'a';
//}
//
//void ARR31(void) {
//  a = new char [ARRAYSIZE];
//  insert_a();
//}


/* ARR_32_CPP */

void ARR32_0() {
  double data[5] = { 2.3, 3.7, 1.4, 0.8, 9.6 };

  deque<double> d;
  deque<double>::iterator pos = d.begin();

  for (size_t i = 0; i < 5; ++i) {
    pos = d.insert(pos, data[i] + 41);
    ++pos;
  }
}

/*XXX Not sure how to get this example to work*/
//void ARR32_1() {
//  double data[5] = { 2.3, 3.7, 1.4, 0.8, 9.6 };
//  deque<double> d;
//
//  transform(data, data+5,
//      inserter(d, d.begin()),
//      bind2nd(plus<int>(), 41));
//}

void ARR32() {
  ARR32_0();
  //ARR32_1();
}

/* ARR_33_CPP */

enum { WORKSPACE_SIZE = 256 };

void ARR33_0(const int src[], size_t len) {
  int dest[WORKSPACE_SIZE];
  if (len > WORKSPACE_SIZE) {
    /* Handle Error */
  }
  memcpy(dest, src, sizeof(int)*len);
  /* ... */
}



void ARR33_1(const vector<int> src) {
  vector<int> dest;
  dest.resize( src.size());
  copy( src.begin(), src.end(), dest.begin());
  /* ... */
}

void ARR33_2(const vector<int> src) {
  vector<int> dest;
  copy( src.begin(), src.end(), back_inserter( dest));
  /* ... */
}

void ARR33() {
  //XXX consider making a basic run for ARR33_{0,1,2}
  //to rule out our errors
}

/* ARR_34_CPP */

enum {ARR34_VECTOR_SIZE = 10};
void ARR34_helper(int a) { return; }
void ARR34() {
  vector<int> c (ARR34_VECTOR_SIZE);
  for_each( c.begin(), c.end(), ARR34_helper );
}

/* ARR_35_CPP */

/* XXX will this work out of context? */
//HRESULT GetMachineName(
//  wchar_t *pwszPath,
//  wchar_t wszMachineName[MAX_COMPUTERNAME_LENGTH_FQDN+1])
//{
//  wchar_t *pwszServerName = wszMachineName;
//  wchar_t *pwszTemp = pwszPath + 2;
//  wchar_t *end_addr
//    = pwszServerName + MAX_COMPUTERNAME_LENGTH_FQDN;
//  while ( (*pwszTemp != L'\\')
//     &&  ((*pwszTemp != L'\0'))
//     && (pwszServerName < end_addr) )
//  {
//    *pwszServerName++ = *pwszTemp++;
//  }
//
//  /* ... */
//}

void ARR35() {
  vector<int> ar( 20, 0);
  for (vector<int>::iterator ip = ar.begin(); ip < ar.end(); ip++) {
    *ip++ = 1;
  }
}

/* ARR_36_CPP */

enum {ARR36_ARRAY_SIZE = 10 };

void ARR36_0() {
  int nums[ARR36_ARRAY_SIZE];
  char *strings[ARR36_ARRAY_SIZE];
  int *next_num_ptr = nums;
  int free_bytes;

  /* increment next_num_ptr as array fills */

  free_bytes = (nums + ARR36_ARRAY_SIZE - next_num_ptr) * sizeof(int);

  printf("%p", strings);
  printf("%d", free_bytes);
}

void ARR36_1() {
  vector<int> nums1(10, 0);
  vector<int> nums2(10, 0);
  vector<int>::iterator i1 = nums1.begin();
  vector<int>::iterator i2 = nums1.end();

  int distance = i2 - i1;
}

void ARR36() {
  ARR36_0();
  ARR36_1();
}

/* ARR_37_CPP */

struct ARR37_numbers {
  int num1;
  int num2;
  int num3;
  int num4;
};

void ARR37_0() {
  struct ARR37_numbers numb = { 1, 2, 3, 4, };
  int total = numb.num1 + numb.num2 + numb.num3 + numb.num4;

  printf("%d", total);
}

int ARR37_sum_numbers(const int *numb, size_t dim) {
  int total = 0;
  const int *numb_ptr;

  for (numb_ptr = numb; numb_ptr < numb + dim; numb_ptr++) {
    total += *(numb_ptr);
  }

  return total;
}

void ARR37_1(void) {
  int my_numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  ARR37_sum_numbers(
      my_numbers,
      sizeof(my_numbers)/sizeof(my_numbers[0])
      );
}

void ARR37() {
  ARR37_0();
  ARR37_1();
}

/* ARR_38_CPP */
void ARR38_0() {
  int ar[20];

  for (int *ip = &ar[0]; ip < &ar[sizeof(ar)/sizeof(ar[0])]; ip++) {
    *ip = 0;
  }
}

void ARR38_1() {
  vector<int> ar( 20, 0);
  for (vector<int>::iterator ip = ar.begin(); ip < ar.end(); ip++) {
    *ip++ = 1;
  }
}

void ARR38_2() {
  char *buf = NULL;
  size_t len = 1 << 30;

  /* Check for overflow */
  if ((size_t)buf+len < (size_t)buf) {
    len = -(size_t)buf-1;
  }
}

int ARR38_3_process_array(char *buf, size_t n) {
  return n < 100;
}

void ARR38() {
  ARR38_0();
  ARR38_1();
  ARR38_2();
}

/* ARR_39_CPP */
/* Begin {code} */
class ARR39_Base {
  public:
    virtual void func(void) {
      cout << "Base" << endl;
    }
};

class ARR39_Derived : public ARR39_Base {
  public:
    int i;
    ARR39_Derived() { i = 0; }

    void func(void) {
      cout << "Derived " << ++i << endl;
    }
};

void ARR39_0_walk(class ARR39_Base *bar [], int count) {
  for (int i = 0; i < count; i++) {
    (bar[i])->func();
  }
}

void ARR39_0() {
  ARR39_Base* dis[3] = {new ARR39_Base, new ARR39_Base, new ARR39_Base};
  ARR39_Base* dat[3] = {new ARR39_Derived, new ARR39_Derived, new ARR39_Derived};

  ARR39_0_walk(dis, 3);
  ARR39_0_walk(dat, 3);
}

void ARR39_1_walk(vector<ARR39_Base*>bar) {
  for_each (bar.begin(), bar.end(), std::mem_fun(&ARR39_Base::func));
}

int ARR39_1() {
  vector<ARR39_Base*> dis(3);
  for (int i=0; i<3; i++) dis[i] = new ARR39_Base;

  vector<ARR39_Base*> dat(3);
  for (int i=0; i<3; i++) dat[i] = new ARR39_Derived;

  ARR39_1_walk(dis);
  ARR39_1_walk(dat);

  return 1;
}

void ARR39() {
  ARR39_0();
  ARR39_1();
}

/* ARR_40_CPP */

typedef set<int, less<int> > IntSetLess;
void ARR40_0() {

  IntSetLess::const_iterator islIter;
  IntSetLess isl;

  isl.insert(5);
  isl.insert(10);
  isl.insert(20);

  pair<IntSetLess::const_iterator, IntSetLess::const_iterator> pisl;

  pisl = isl.equal_range(10);

  for (islIter = pisl.first; islIter != pisl.second; ++islIter) {
    cout << "Set contains: " << *islIter << endl;
  }
}

struct DereferenceLess {
  template <typename PtrType>
    bool operator()(PtrType pl1, PtrType pl2) const {
      return *pl1 < *pl2;
    }
};

typedef set<int*, DereferenceLess> IntPtrSet;

void ARR40() {
  ARR40_0();
}


/* ARR_41_CPP */
//XXX no test code on Wiki
//void ARR41() { }


/* ARR_42_CPP */

void ARR42_0() {
  const char *chars="123";
  set<char> container( chars, chars+strlen(chars));

  cout << "chars: ";
  for (set<char>::iterator i = container.begin(); i != container.end(); ++i)
    cout << *i;
  cout << endl;

  for (char c = '1'; c <= '3'; c++) {
    set<char>::iterator x = container.find(c);
    if (x == container.end()) {
      cout << c << " not found" << endl;
      abort();
    }

    container.erase(x);
    container.insert( 'a' + c - '0' - 1); // changes set contents safely

    cout << "chars: ";
    for (set<char>::iterator i = container.begin(); i != container.end(); ++i)
      cout << *i;
    cout << endl;
  }
}

void ARR42_1() {
  const char *chars="123";
  map<char, char> container;
  for (const char *ptr = chars; *ptr != '\0'; ++ptr)
    container[*ptr] = *ptr;

  cout << "chars: ";
  for (map<char, char>::iterator i = container.begin(); i != container.end(); ++i)
    cout << i->second;
  cout << endl;

  for (char c = '1'; c <= '3'; c++) {
    map<char, char>::iterator x = container.find(c);
    if (x == container.end()) {
      cout << c << " not found" << endl;
      abort();
    }

    x->second = 'a' + c - '0' - 1; // directly changes map value

    cout << "chars: ";
    for (map<char, char>::iterator i = container.begin(); i != container.end(); ++i)
      cout << i->second;
    cout << endl;
  }
}

void ARR42() {
  ARR42_0();
  ARR42_1();
}


/* ARR_43_CPP */

void ARR43() {
  vector<int> container;
  /* fill container with integers */
  int value = 42;
  container.erase( remove( container.begin(), container.end(), value), container.end());
  for (vector<int>::iterator i = container.begin(); i != container.end(); ++i) {
    cout << "Container element: " << *i << endl;
  }
}


/* ARR_44_CPP */

void ARR44() {
  vector<int> v;
  for (int i = 0; i < 10; i++) v.push_back(i);

  cout << "Vector contains:";
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
    cout << " " << *it;
  cout << endl;

  v.erase( v.begin() + 3);

  cout << "Vector contains:";
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
    cout << " " << *it;
  cout << endl;
}

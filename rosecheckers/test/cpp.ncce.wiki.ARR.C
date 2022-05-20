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

enum {ARR_LEN = 100};

/* ARR00_CPP */
void ARR00() {
  int *array = new int[ARR_LEN];
  /* ... */
  delete[] array;
}


/* ARR01_CPP */
void ARR01_0_clear(int array[]) {
  for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
    array[i] = 0;
  }
}
void ARR01_0() {
  int dis[12];
  ARR01_0_clear(dis);
  /* ... */
}

void ARR01_1_clear(int a[ARR_LEN]) {
  memset(a, 0, sizeof(a)); /* error */
}

void ARR01_1() {
  int b[ARR_LEN];
  ARR01_1_clear(b);
  assert(b[ARR_LEN / 2]==0); /* may fail */
}

void ARR01_2() {
  const char *src = "hello, world";
  char *dest = new char [sizeof(src)];
  strcpy(dest, src);
}


void ARR01() {
  ARR01_0();
  ARR01_1();
  ARR01_2();
}

void ARR02() {
  int a[] = {1, 2, 3, 4};
  a[0] = a[1];
}

class Widget {/* ... */};
const int WIDGET_SIZE = 10;
void ARR04() {
  vector<Widget*> container;
  for (size_t i = 0; i < WIDGET_SIZE; i++) {
    container.push_back(new Widget(/* ... */));
  }
  /* Work with container */
} /* frees container */


/* ARR_30_CPP */
enum { TABLESIZE = 100 };

int *ARR30_0_table = NULL;

int ARR30_0_insert_in_table(int pos, int value){
  if (!ARR30_0_table) {
    ARR30_0_table = new int[TABLESIZE];
  }
  if (pos >= TABLESIZE) {
    return -1;
  }
  ARR30_0_table[pos] = value;
  return 0;
}

vector<int> ARR30_1_table;

int ARR30_1_insert_in_table(int pos, int value){
  if (pos >= ARR30_1_table.size()) {
    return -1;
  }
  ARR30_1_table[pos] = value;
  return 0;
}

void ARR30() {
  ARR30_0_insert_in_table(0, 0);
  ARR30_1_insert_in_table(0, 0);
}

/* ARR_31_CPP */

//#include "cpp.ncce.wiki.ARR30_helper.h"
//enum { ARRAYSIZE = 100 };
//
//extern char a[];
//
//void insert_a(void) {
//  a[0] = 'a';
//}
//
//void ARR31(void) {
//  a = new char [ARRAYSIZE];
//  insert_a();
//}

/* ARR_32_CPP */
void ARR32() {
  double data[5] = { 2.3, 3.7, 1.4, 0.8, 9.6 };

  deque<double> d;
  deque<double>::iterator pos = d.begin();

  for (size_t i = 0; i < 5; ++i) {
    d.insert(pos++, data[i] + 41);
  }
}

/* ARR_33_CPP */

enum { WORKSPACE_SIZE = 256 };

void ARR33_0(const int src[], size_t len) {
  int dest[WORKSPACE_SIZE];
  memcpy(dest, src, len * sizeof(int));
  /* ... */
}

void ARR33_1(const vector<int> src) {
  vector<int> dest;
  copy( src.begin(), src.end(), dest.begin());
  /* ... */
}

void ARR33() {
  //XXX consider making a basic run for ARR33_{0,1}
}

/* ARR_34_CPP */

void ARR34_helper(int a) { return; }

enum {ARR34_VECTOR_SIZE = 10};
void ARR34_0() {
  vector<int> c (ARR34_VECTOR_SIZE);
  for_each( c.end(), c.begin(), ARR34_helper);
}

void ARR34_1() {
  vector<int> c (ARR34_VECTOR_SIZE);
  vector<int> d (ARR34_VECTOR_SIZE);
  for_each( c.begin(), d.end(), ARR34_helper);
}

void ARR34() {
  ARR34_0();
  ARR34_1();
}

/* ARR_35_CPP */

/*XXX Windows Example-- This probably won't compile
 * out of context like this*/
//error_status_t _RemoteActivation(
//      /* ... */, WCHAR *pwszObjectName, ... ) {
//   *phr = GetServerPath(
//              pwszObjectName, &pwszObjectName);
//    /* ... */
//}
//
//HRESULT GetServerPath(
//  WCHAR *pwszPath, WCHAR **pwszServerPath ){
//  WCHAR *pwszFinalPath = pwszPath;
//  WCHAR wszMachineName[MAX_COMPUTERNAME_LENGTH_FQDN+1];
//  hr = GetMachineName(pwszPath, wszMachineName);
//  *pwszServerPath = pwszFinalPath;
//}
//
//HRESULT GetMachineName(
//  WCHAR *pwszPath,
//  WCHAR wszMachineName[MAX_COMPUTERNAME_LENGTH_FQDN+1])
//{
//  pwszServerName = wszMachineName;
//  LPWSTR pwszTemp = pwszPath + 2;
//  while ( *pwszTemp != L'\\' )
//    *pwszServerName++ = *pwszTemp++;
//  /* ... */
//}

void ARR35() {
  vector<int> ar( 20, 0);
  vector<int>::iterator ip = ar.begin();
  for (int i = 1; i <= 22; i++) {
    *ip++ = 1;
  }
}

/* ARR_36_CPP */

enum {ARR36_ARRAY_SIZE = 10};

void ARR36_0() {
  int nums[ARR36_ARRAY_SIZE];
  char *strings[ARR36_ARRAY_SIZE];
  int *next_num_ptr = nums;
  int free_bytes;

  /* increment next_num_ptr as array fills */
  free_bytes = strings - (char **)next_num_ptr;

  printf("%d", free_bytes);
}


void ARR36_1() {
  vector<int> nums1(10, 0);
  vector<int> nums2(10, 0);
  vector<int>::iterator i1 = nums1.begin();
  vector<int>::iterator i2 = nums2.begin();

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

int ARR37_sum_numbers(const struct ARR37_numbers *numb){
  int total = 0;
  const int *numb_ptr;

  for (numb_ptr = &numb->num1;
      numb_ptr <= &numb->num3;
      numb_ptr++)
  {
    total += *(numb_ptr);
  }

  return total;
}

void ARR37() {
  struct ARR37_numbers my_numbers = { 1, 2, 3, 4 };
  ARR37_sum_numbers(&my_numbers);
}

/* ARR_38_CPP */
void ARR38_0() {
  int ar[20];

  for (int *ip = &ar[0]; ip < &ar[21]; ip++) {
    *ip = 0;
  }
}

void ARR38_1() {
  vector<int> ar( 20, 0);
  vector<int>::iterator ip = ar.begin();
  for (int i = 1; i <= 22; i++) {
    *ip++ = 1;
  }
}

void ARR38_2() {
  char *buf = NULL;
  size_t len = 1 << 30;

  /* Check for overflow */
  if (buf + len < buf) {
    len = -(size_t)buf-1;
  }
}

int ARR38_3_process_array(char *buf, size_t n) {
  return buf + n < buf + 100;
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

void ARR39_walk(class ARR39_Base *bar, int count) {
  for (int i = 0; i < count; i++) {
    bar[i].func();
  }
}

void ARR39() {
  ARR39_Base dis[3];
  ARR39_Derived dat[3];

  ARR39_walk(dis, 3);
  ARR39_walk(dat, 3); // crashes
}

/* ARR_40_CPP */

typedef set< int, less_equal<int> > IntSetLE;

void ARR40_0() {
  IntSetLE::const_iterator sleIter;
  IntSetLE sle;

  sle.insert(5);
  sle.insert(10);
  sle.insert(20);

  pair<IntSetLE::const_iterator, IntSetLE::const_iterator> psle;

  psle = sle.equal_range(10);

  for (sleIter = psle.first; sleIter != psle.second; ++sleIter){
    cout << "Set contains: " << *sleIter << endl;
  }
}

void ARR40_1() {
  typedef set<int*> IntPtrSet;
  IntPtrSet::const_iterator sIter;
  IntPtrSet s;

  int i[3] = {10, 20, 5};
  s.insert(&i[2]);
  s.insert(&i[1]);
  s.insert(&i[0]);

  cout << "Set contains ";
  for (sIter = s.begin(); sIter != s.end(); ++sIter) cout << **sIter << ' ';
  cout << endl;
}

void ARR40() {
  ARR40_0();
  ARR40_1();
}


/* ARR_41_CPP */
//XXX no test code on Wiki
//void ARR41() { }


/* ARR_42_CPP */

void ARR42() {
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
    //XXX doesn't compile on linux-- how should we test it then?
    //*x = 'a' + c - '0' - 1; // directly changes set contents!

    cout << "chars: ";
    for (set<char>::iterator i = container.begin(); i != container.end(); ++i)
      cout << *i;
    cout << endl;
  }
}


/* ARR_43_CPP */

vector<int> container;
/* fill container with integers */
void ARR43() {
  int value = 42;
  vector<int>::iterator end = remove( container.begin(), container.end(), value);
  for (vector<int>::iterator i = container.begin(); i != container.end(); ++i) {
    cout << "Container element: " << *i << endl;
  }
}


/* ARR_44_CPP */

class BadPredicate: public unary_function< int, bool> {
public:
  BadPredicate() : timesCalled(0) {}

  bool operator()(const int&) {
    return ++timesCalled == 3;
  }
private:
  size_t timesCalled;
};

void ARR44() {
  vector<int> v;
  for (int i = 0; i < 10; i++) v.push_back(i);

  cout << "Vector contains:";
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
    cout << " " << *it;
  cout << endl;

  v.erase( remove_if ( v.begin(), v.end(), BadPredicate()), v.end());

  cout << "Vector contains:";
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
    cout << " " << *it;
  cout << endl;
}

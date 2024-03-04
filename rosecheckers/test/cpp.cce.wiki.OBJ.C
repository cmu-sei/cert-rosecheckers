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
#include "cpp.util.h"

void OBJ05(void);
void OBJ31(void);
void OBJ34(void);

void OBJ() {
  OBJ05();
  OBJ31();
  OBJ34();
}


/* OBJ00 */
/* OBJ35 */
class OBJ00_Widget {
public:
  OBJ00_Widget (): total(0) {}
  void add () {
    total++;
  }
  void remove () {
    total--;
  }
  int getTotal () {return total;}

private:
  int total;
};


/* OBJ01 */
/*XXX Does not compile
class Widget {
public:
  int to_int() const;
  // ...
}
// ...
Widget w;
// ...
cout << w << endl;  // will not compile
*/

/* OBJ03 */
class OBJ03_Thing {
public:
  virtual ~OBJ03_Thing();
  virtual void updateV1( int );
  virtual void updateV2( double );
  // ...
};
class OBJ03_MyThing : public OBJ03_Thing {
public:
  void updateV1( int ); // overrides and hides only updateV1
  // ...
};

class OBJ03_1_Thing {
public:
  virtual ~OBJ03_1_Thing();
  void update( int i ) { doUpdateForInt( i ); }
  void update( double d ) { doUpdateForDouble( d ); }
  // ...
protected:
  virtual void doUpdateForInt( int );
  virtual void doUpdateForDouble( double );
};

class OBJ03_1_MyThing : public OBJ03_1_Thing {
public:
  // ...
protected:
  void doUpdateForInt( int );
};

/* OBJ04 */
class OBJ04_Thing {
public:
  virtual ~OBJ04_Thing();
  void doItNTimes( int numTimes = 10 ) {doItNTimesImpl( numTimes ); }
  void doItThisHigh( double howHigh = 1.0 ) { doItThisHighImpl( howHigh ); }
protected:
  virtual void doItNTimesImpl( int numTimes );
  virtual void doItThisHighImpl( double howHigh );
  // ...
};
class OBJ04_MyThing : public OBJ04_Thing {
protected:
  void doItNTimesImpl( int numTimes );
  void doItThisHighImpl( double howHigh );
  // ...
};

/* OBJ05 */
class OBJ05_SomeClass {
public:
  void doSomething();
  void destroy();
};

void OBJ05_SomeClass::destroy() {
  delete this;  // Dangerous!!
}

void OBJ05(void) {
  OBJ05_SomeClass *sc = new OBJ05_SomeClass();
  sc->destroy();
  sc=NULL;
}

/* OBJ06 */
class OBJ06_thing {
private:
  OBJ06_thing& operator = (const OBJ06_thing& other) { return *(new OBJ06_thing());}
  OBJ06_thing (const OBJ06_thing & other) {/* do nothing */}

public:
  OBJ06_thing();
  int *ptr;
  int a;
};

/* OBJ07 */
class OBJ07_Base1 {
public:
  const char* f(int i) {return "Base1::f()";}
};

class OBJ07_Base2 {
public:
  const char* f() {return "Base2::f()";}
};

class OBJ07_Derived : public OBJ07_Base1 {
public:
  const char* base2_f() {return b2.f();}
  const char* g() {return "Derived::g()";}
private:
  OBJ07_Base2 b2;
};

/* OBJ30 */
class OBJ30_B {
public:
  OBJ30_B() {
    // seize base resources...
  }
  virtual ~OBJ30_B() {
    // release base resources...
  }
};

class OBJ30_D : public OBJ30_B {
public:
  OBJ30_D() {
    // seize derived resources...
  }
  ~OBJ30_D() {
    // release derived resources...
  }
};

/* OBJ31 */
class OBJ31_Widget {
public:
  explicit OBJ31_Widget() {cerr << "constructed" << endl;}
};

void OBJ31(void) {
  OBJ31_Widget w;
}

/* OBJ32 */
class OBJ32_Widget {
public:
  explicit OBJ32_Widget(int size);
};

/* OBJ33 */
class OBJ33_Employee {
public:
  OBJ33_Employee(string theName) : name(theName) {};
  virtual ~OBJ33_Employee();
  string getName() const {return name;}
  virtual void print() const = 0;
private:
  string name;
};

/* OBJ34 */
class OBJ34_Base {
public:
  OBJ34_Base() {
    // Build OBJ34_Base object
  }
  virtual ~OBJ34_Base() {
    // Destroy OBJ34_Base object
  }
};

class OBJ34_Derived : public OBJ34_Base {
public:
  OBJ34_Derived() {
    // Build OBJ34_Derived object
  }
  ~OBJ34_Derived() {
    // Destroy OBJ34_Derived object
  }
};

void OBJ34(void) {
  OBJ34_Base *b = new OBJ34_Derived();
  // ...
  delete b;
}

/* OBJ36 */
class OBJ36_A {
public:
  OBJ36_A() : value(0) {
    ++invocations;
  }
  OBJ36_A(const OBJ36_A& rhs) : value(rhs.value) {}
private:
  int value;
  static int invocations;
};

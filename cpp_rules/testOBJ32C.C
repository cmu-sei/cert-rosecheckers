/*
OBJ32-C. Ensure that single-argument constructors are marked "explicit"

A single-argument constructor may be used as an implicit conversion function, unless it is marked "explicit" (see [ISO/IEC 14882-2003] Section 12.3.1, "Conversion by constructor"). Such implicit conversions may be unexpected, and lead to undesirable behavior.
*/

class Widget1 {
public:
  Widget1(int size); // diagnostic
  //...
};

class Widget2 {
public:
  explicit Widget2(int size); // no diagnostic
  //...
};

class Widget3 {
public:
  Widget3(int size, int size2 = 10 ); // diagnostic
  //...
};

class Widget4 {
public:
  explicit Widget4(int size, int size2 = 10 ); // no diagnostic
  //...
};

class Widget5 {
public:
  Widget5(int size = 0, int size2 = 10 ); // diagnostic
  //...
};

class Widget6 {
public:
  explicit Widget6(int size = 0, int size2 = 10 ); // no diagnostic
  //...
};

class Widget7 {
public:
  Widget7();
  Widget7( const Widget7 & ); // no diagnostic
  //...
};

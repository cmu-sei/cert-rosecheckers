/*
OBJ01-A. Be careful with the definition of conversion operators

A user-defined type conversion operator can be used to implicitly convert an object to another type. This may lead to unexpected behavior and undesirable results. If user-defined conversions are required, they should be defined as functions, rather than operators, so that they have to be called explicitly.
*/

class Widget {
public:
  operator int() const;
  operator short() const;
  // ...
};

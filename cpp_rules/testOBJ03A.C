/*
OBJ03-A. Prefer not to overload virtual functions

Overloading of virtual functions tends to defeat polymorphism and introduce unnecessary complexity into a class hierarchy.
*/

class Thing {
  public:
    virtual ~Thing();
    virtual void update( int );
    virtual void update( double );
    // ...
};
class MyThing : public Thing {
  public:
    void update( int ); // overrides only update(int), hides both!
    // ...
};

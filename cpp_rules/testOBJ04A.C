/*
OBJ04-A. Prefer not to give virtual functions default argument initializers

Giving virtual functions default argument initializers tends to defeat polymorphism and introduce unnecessary complexity into a class hierarchy.
*/

class Thing {
  public:
    virtual ~Thing();
    virtual void doItNTimes( int numTimes = 10 ); // diagnostic
    virtual void doItThisHigh( double howHigh = 1.0 ); // diagnostic
    // ...
};
class MyThing : public Thing {
  public:
    void doItNTimes( int numTimes );
    void doItThisHigh( double howHigh = 2.2 ); // diagnostic
    // ...
};

static double const minimumHeight = 1.0;
// ...
namespace XYZ {
class Thing {
  public:
    virtual ~Thing();
    virtual void doItThisHigh( double howHigh = minimumHeight ); // diagnostic
    // ...
};
}
// ...
namespace XYZ {
static double const minimumHeight = 2.2;
class MyThing : public Thing {
  public:
    void doItThisHigh( double howHigh = minimumHeight ); // diagnostic
    // ...
};
}

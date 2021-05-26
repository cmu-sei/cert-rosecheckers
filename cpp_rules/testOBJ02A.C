/*
OBJ02-A. Do not hide inherited non-virtual member functions

Conventionally, a non-virtual member function specifies an invariant over the hierarchy. It cannot be overridden in derived classes, but it can be hidden by a derived class member (data or function) with the same identifier. The effect of this hiding is to defeat polymorphism by causing an object to behave differently depending on which interface is used to manipulate it, resulting in unnecessary complexity and error.
*/

struct Point {
    int x_;
    int y_;
};

class Target {
  public:
    Target();
    virtual ~Target();
    Point extent() const;
    Point position() const
        { return center_; }
    // ...
    Target( const Target & );
    Target &operator =( const Target & );
    void operator =( int );
  private:
    Point center_;
};
class SquareTarget : public Target {
  public:
    SquareTarget();
    Point position() const // diagnostic:  improper hiding of non-virtual
        { return upperLeft_; }
    // ...
    SquareTarget( const SquareTarget & );
    SquareTarget &operator =( const SquareTarget & ); // no diagnostic: copy operator = not inherited
    void operator =( int ); // diagnostic:  not a copy assign
  private:
    Point upperLeft_;
};

void f() {
	SquareTarget target;
}

/*
RES35-C. Declare a copy constructor, a copy assignment operator, and a destructor in a class that manages resources

A copy constructor, a copy assignment operator, and a destructor are created automatically by the compiler for a class in which they are not provided by the coder. However, in a class that manages resources, the defaults provided by the compiler will usually not be sufficient to ensure that the resources are properly managed (see RES34-C. Encapsulate resources that require paired acquire and release in objects).

For example, suppose an object contains a string that is implemented as a pointer allocated using new. If a destructor is provided that deletes the pointer, but no copy constructor is provided, it is likely that some pointers will be deleted multiple times. This is because the default copy constructor does memberwise initialization. Hence, the pointer would be copied, not the array of characters. Therefore, we could end up with several copies of the same pointer in different objects, and deleting those objects would result in an attempt to delete the same pointer several times.
*/

class Widget {
};

class Thingy1 { // diagnostic
    public:
	Thingy1();
    private:
        Widget *w;
};

class Thingy2 { // no diagnostic
    public:
        Thingy2();
        Thingy2(const Thingy2 &value);
        Thingy2 &operator=(const Thingy2 &rhs);
        ~Thingy2();
        // ...
    private:
        Widget *w;
};

Thingy2::Thingy2(const Thingy2 &value) : w( new Widget(*value.w) )
	{}

Thingy2 &Thingy2::operator=(const Thingy2 &rhs) {
    Widget *temp = new Widget(*rhs.w);
    delete w;
    w = temp;
    return *this;
}

Thingy2::~Thingy2() {
    delete w;
}

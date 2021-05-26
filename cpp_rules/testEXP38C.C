/*
EXP38-C. Avoid calling your own virtual functions in constructors and destructors.
*/

class B {
  public:
    B() { seize(); } // diagnostic
    virtual ~B() { release(); } // diagnostic
  protected:
    virtual void seize() {}
    virtual void release() {}
  private:
    B( const B & );
    B &operator =( const B & );
};

class D : public B {
  public:
    D() {}
    ~D() {}
  protected:
    void seize() {
        B::seize();
        // get derived resources...
    }
    void release() {
        // release derived resources...
        B::release();
    }
  private:
    D( const D & );
    D &operator =( const D & );
};

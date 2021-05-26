typedef void (*constraint_handler_t) (
    const char * __restrict__ msg,
    void * __restrict__ ptr,
    errno_t error);

typedef void (*abort_handler_s) (
    const char * __restrict__ msg,
    void * __restrict__ ptr,
    errno_t error);

class StackUnderflow : public std::exception {
  /* ... */
  public:
    StackUnderflow() {};
    void modify() {};
};

// classes used for exception handling
class ERR36_B {};
class ERR36_D: public ERR36_B {};

class ERR37_exception1 : public exception {};
class ERR37_exception2 : public exception {};

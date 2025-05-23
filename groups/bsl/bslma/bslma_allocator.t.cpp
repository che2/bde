// bslma_allocator.t.cpp                                              -*-C++-*-

#include <bslma_allocator.h>
#include <bslma_allocator.fwd.h>

#include <bsls_alignmentutil.h>
#include <bsls_bsltestutil.h>
#include <bsls_compilerfeatures.h>
#include <bsls_keyword.h>
#include <bsls_platform.h>
#include <bsls_protocoltest.h>

#include <stdio.h>      // `printf`
#include <stdlib.h>     // `atoi`
#include <string.h>

#include <new>

using namespace BloombergLP;

#ifdef BSLS_PLATFORM_HAS_PRAGMA_GCC_DIAGNOSTIC
#ifdef BSLS_PLATFORM_CMP_CLANG
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
#endif

//=============================================================================
//                              TEST PLAN
//-----------------------------------------------------------------------------
//                              Overview
//                              --------
// We are testing a pure protocol class as well as a set of overloaded
// operators.  We need to verify that (1) a concrete derived class compiles
// and links, (2) that the each of the two concrete template functions,
// `deleteObject` and `deleteObjectRaw` destroys the argument object and calls
// the deallocate method of the supplied allocator, and (3) that the overloaded
// `new` and `delete` operators respectively forward the call to the
// `allocate` and `deallocate` method of the supplied allocator.
//-----------------------------------------------------------------------------
// [ 1] virtual ~Allocator();
// [ 1] virtual void *allocate(size_type) = 0;
// [ 1] virtual void deallocate(void *) = 0;
// [ 1] virtual void* do_allocate(std::size_t, std::size_t);
// [ 1] virtual void do_deallocate(void *p, std::size_t, std::size_t);
// [ 1] virtual bool do_is_equal(const bsl::memory_resource&) const;
// [ 2] template<class TYPE> void deleteObject(const TYPE *);
// [ 3] template<class TYPE> void deleteObjectRaw(const TYPE *);
// [ 2] void deleteObject(bsl::nulptr_t);
// [ 3] void deleteObjectRaw(bsl::nulptr_t);
// [ 4] void *operator new(int size, bslma::Allocator& basicAllocator);
// [ 5] void operator delete(void *address, bslma::Allocator& bA);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
// [  ] static throwBadAlloc();
#endif
//-----------------------------------------------------------------------------
// [ 1] PROTOCOL TEST - Make sure derived class compiles and links.
// [ 4] OPERATOR TEST - Make sure overloaded operators call correct functions.
// [ 5] EXCEPTION SAFETY - Ensure operator delete is invoked on an exception.
// [ 6] USAGE EXAMPLE - Make sure usage examples compiles and works properly.
//=============================================================================

// ============================================================================
//                     STANDARD BSL ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", line, message);

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BSL TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLS_BSLTESTUTIL_ASSERT
#define ASSERTV      BSLS_BSLTESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLS_BSLTESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLS_BSLTESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLS_BSLTESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLS_BSLTESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLS_BSLTESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLS_BSLTESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLS_BSLTESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLS_BSLTESTUTIL_LOOP6_ASSERT

#define Q            BSLS_BSLTESTUTIL_Q   // Quote identifier literally.
#define P            BSLS_BSLTESTUTIL_P   // Print identifier and value.
#define P_           BSLS_BSLTESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLS_BSLTESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLS_BSLTESTUTIL_L_  // current Line number

// ============================================================================
//                  NEGATIVE-TEST MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT_SAFE_PASS(EXPR) BSLS_ASSERTTEST_ASSERT_SAFE_PASS(EXPR)
#define ASSERT_SAFE_FAIL(EXPR) BSLS_ASSERTTEST_ASSERT_SAFE_FAIL(EXPR)
#define ASSERT_PASS(EXPR)      BSLS_ASSERTTEST_ASSERT_PASS(EXPR)
#define ASSERT_FAIL(EXPR)      BSLS_ASSERTTEST_ASSERT_FAIL(EXPR)
#define ASSERT_OPT_PASS(EXPR)  BSLS_ASSERTTEST_ASSERT_OPT_PASS(EXPR)
#define ASSERT_OPT_FAIL(EXPR)  BSLS_ASSERTTEST_ASSERT_OPT_FAIL(EXPR)

//=============================================================================
//                      CONCRETE DERIVED TYPES
//-----------------------------------------------------------------------------

/// This class is used with `bsls::ProtocolTest` to test the
/// `bslma::Allocator` protocol.
class AllocatorProtocolTest : public bsls::ProtocolTestImp<bslma::Allocator> {

  protected:
    void* do_allocate(std::size_t, std::size_t) BSLS_KEYWORD_OVERRIDE;
    void do_deallocate(void *, std::size_t, std::size_t) BSLS_KEYWORD_OVERRIDE;
    bool do_is_equal(const bsl::memory_resource&) const
                                   BSLS_KEYWORD_NOEXCEPT BSLS_KEYWORD_OVERRIDE;

  public:
    void *allocate(size_type) BSLS_KEYWORD_OVERRIDE;
    void deallocate(void *) BSLS_KEYWORD_OVERRIDE;
};

void *AllocatorProtocolTest::do_allocate(std::size_t, std::size_t)
{
    static char buf[1];
    return markDoneVal(static_cast<void*>(buf));  // return non-null pointer
}

void AllocatorProtocolTest::do_deallocate(void *, std::size_t, std::size_t)
{
    markDone();
}

bool AllocatorProtocolTest::do_is_equal(const bsl::memory_resource&) const
                                                          BSLS_KEYWORD_NOEXCEPT
{
    return markDone();
}

void *AllocatorProtocolTest::allocate(size_type)
{
    static char buf[1];
    return markDoneVal(static_cast<void*>(buf));  // return non-null pointer
}

void AllocatorProtocolTest::deallocate(void *)
{
    markDone();
}

/// This class is used with `bsls::ProtocolTest` to test the
/// `bslma::Allocator` protocol.  Unlike the `AllocatorProtocolTest`, the
/// non-pure `do_allocate`, `do_deallocate`, and `do_is_equal` virtual
/// functions are not overriden; their default implementations are used
/// instead.
class IndirectProtocolTest : public bsls::ProtocolTestImp<bslma::Allocator> {

  public:
    void *allocate(size_type) BSLS_KEYWORD_OVERRIDE;
    void deallocate(void *) BSLS_KEYWORD_OVERRIDE;
};

void *IndirectProtocolTest::allocate(size_type)
{
    static char buf[1];
    return markDoneVal(static_cast<void*>(buf));  // return non-null pointer
}

void IndirectProtocolTest::deallocate(void *)
{
    markDone();
}

/// Test class used to verify protocol.
class my_Allocator : public bslma::Allocator {

    int d_fun;  // holds code describing function:
                //   + 1 allocate
                //   + 2 deallocate

    size_type d_arg;  // holds argument from alloc

    char d_storage[32];  // space to allow for "large" allocations

    int d_allocateCount;    // number of times allocate called
    int d_deallocateCount;  // number of times deallocate called

    bsls::AlignmentUtil::MaxAlignedType d_align; // no use but to align this
                                                 // struct

  public:
    my_Allocator() : d_allocateCount(0), d_deallocateCount(0) { }
    ~my_Allocator() BSLS_KEYWORD_OVERRIDE { }

    void *allocate(size_type s) BSLS_KEYWORD_OVERRIDE {
        d_fun = 1;
        d_arg = s;
        ++d_allocateCount;
        return s ? this : 0;
    }

    // MANIPULATORS
    void deallocate(void *) BSLS_KEYWORD_OVERRIDE
    {
        d_fun = 2;
        ++d_deallocateCount;
    }

    // ACCESSORS

    /// Return number of times allocate called.
    int allocateCount() const { return d_allocateCount; }

    /// Return last argument value for allocate.
    size_type arg() const { return d_arg; }

    /// Return number of times deallocate called.
    int deallocateCount() const { return d_deallocateCount; }

    /// Return descriptive code for the function called.
    int fun() const { return d_fun; }
};

/// Test class used to verify examples.
class my_NewDeleteAllocator : public bslma::Allocator {

    int d_count;

    enum { MAGIC   = 0xDEADBEEF,
           DELETED = 0xBADF000D };

  public:
    my_NewDeleteAllocator(): d_count(0) { }
    ~my_NewDeleteAllocator() BSLS_KEYWORD_OVERRIDE { }

    void *allocate(size_type size) BSLS_KEYWORD_OVERRIDE {
        ++d_count;

        if (0 == size) {
            return 0;
        }

        unsigned *p = (unsigned *) operator new(
                               size + bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
        *p = MAGIC;

        return (char *) p + bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT;
    }

    void deallocate(void *address) BSLS_KEYWORD_OVERRIDE {
        ++d_count;

        if (0 == address) {
            return;
        }

        unsigned *p = (unsigned *)
                         ((bsls::AlignmentUtil::MaxAlignedType *) address - 1);
        ASSERT(MAGIC == *p);
        *p = DELETED;

        operator delete(p);
    }

    int getCount() const            { return d_count; }
};

//=============================================================================
//                   CONCRETE OBJECTS FOR TESTING `deleteObject`
//-----------------------------------------------------------------------------
static int globalObjectStatus = 0;    // global flag set by test-object d'tors
static int class3ObjectCount = 0;     // Count set by my_Class3 c'tor/d'tor

class my_Class1 {
  public:
    my_Class1() { globalObjectStatus = 1; }
    ~my_Class1() { globalObjectStatus = 0; }
};

class my_Class2 {
  public:
    my_Class2() { globalObjectStatus = 1; }
    virtual ~my_Class2() { globalObjectStatus = 0; }
};

class my_Class3Base {
  public:
    my_Class3Base() { }
    virtual ~my_Class3Base() = 0;
};

class my_Class3 : public my_Class3Base {
  public:
    my_Class3() { ++class3ObjectCount; }
    ~my_Class3() BSLS_KEYWORD_OVERRIDE;
};

my_Class3Base::~my_Class3Base() { }
my_Class3::~my_Class3() { --class3ObjectCount; }

// The "dreaded diamond".

static int virtualBaseObjectCount = 0;
static int leftBaseObjectCount    = 0;
static int rightBaseObjectCount   = 0;
static int mostDerivedObjectCount = 0;

class my_VirtualBase {
    int x;
  public:
    my_VirtualBase()          { virtualBaseObjectCount = 1; }
    virtual ~my_VirtualBase() { virtualBaseObjectCount = 0; }
};

class my_LeftBase : virtual public my_VirtualBase {
    int x;
  public:
    my_LeftBase()                        { leftBaseObjectCount = 1; }
    ~my_LeftBase() BSLS_KEYWORD_OVERRIDE { leftBaseObjectCount = 0; }
};

class my_RightBase : virtual public my_VirtualBase {
    int x;
  public:
    my_RightBase()                        { rightBaseObjectCount = 1; }
    ~my_RightBase() BSLS_KEYWORD_OVERRIDE { rightBaseObjectCount = 0; }
};

class my_MostDerived : public my_LeftBase, public my_RightBase {
    int x;
  public:
    my_MostDerived()                        { mostDerivedObjectCount = 1; }
    ~my_MostDerived() BSLS_KEYWORD_OVERRIDE { mostDerivedObjectCount = 0; }
};

//=============================================================================
//                              USAGE EXAMPLE
//-----------------------------------------------------------------------------

class my_DoubleStack {
    double           *d_stack_p;     // dynamically allocated array (d_size
                                     // elements)
    int               d_size;        // physical capacity of this stack (in
                                     // elements)
    int               d_length;      // logical index of next available stack
                                     // element
    bslma::Allocator *d_allocator_p; // holds (but doesn't own) object

    friend class my_DoubleStackIter;

  private:
    void increaseSize(); // Increase the capacity by at least one element.

  public:
    // CREATORS
    explicit my_DoubleStack(bslma::Allocator *basicAllocator = 0);
    my_DoubleStack(const my_DoubleStack&  other,
                   bslma::Allocator      *basicAllocator = 0);
    ~my_DoubleStack();

    // MANIPULATORS
    my_DoubleStack& operator=(const my_DoubleStack& rhs);
    void pop();
    void push(double value);

    // ACCESSORS
    bool isEmpty() const;
    const double& top() const;
};

enum { INITIAL_SIZE = 1, GROW_FACTOR = 2 };

// ..

my_NewDeleteAllocator myA;

/// The above initialization expression is equivalent to 'basicAllocator
/// ? basicAllocator : &bslma::NewDeleteAllocator::singleton()'.
my_DoubleStack::my_DoubleStack(bslma::Allocator *basicAllocator)
: d_size(INITIAL_SIZE)
, d_length(0)
, d_allocator_p(basicAllocator ? basicAllocator : &myA)
{
    ASSERT(d_allocator_p);
    d_stack_p = (double *) d_allocator_p->allocate(d_size * sizeof *d_stack_p);
}

my_DoubleStack::~my_DoubleStack()
{
    // CLASS INVARIANTS
    ASSERT(d_allocator_p);
    ASSERT(d_stack_p);
    ASSERT(0 <= d_length);
    ASSERT(0 <= d_size);
    ASSERT(d_length <= d_size);

    d_allocator_p->deallocate(d_stack_p);
}

/// Reallocate memory in the specified `array` to the specified `newSize`
/// using the specified `basicAllocator`.  The specified `length` number of
/// leading elements are preserved.  Since the class invariant requires that
/// the physical capacity of the container may grow but never shrink; the
/// behavior is undefined unless `length <= newSize`.
static inline
void reallocate(double           **array,
                int                newSize,
                int                length,
                bslma::Allocator  *basicAllocator)
{
    ASSERT(array);
    ASSERT(1 <= newSize);
    ASSERT(0 <= length);
    ASSERT(basicAllocator);
    ASSERT(length <= newSize);        // enforce class invariant

    double *tmp = *array;             // support exception neutrality
    *array = (double *) basicAllocator->allocate(newSize * sizeof **array);

    // COMMIT POINT

    memcpy(*array, tmp, length * sizeof **array);
    basicAllocator->deallocate(tmp);
}

void my_DoubleStack::increaseSize()
{
    int proposedNewSize = d_size * GROW_FACTOR;      // reallocate can throw
    ASSERT(proposedNewSize > d_length);

    reallocate(&d_stack_p, proposedNewSize, d_length, d_allocator_p);
    d_size = proposedNewSize;                        // we're committed
}

inline
void my_DoubleStack::push(double value)
{
    if (d_length >= d_size) {
        increaseSize();
    }
    d_stack_p[d_length++] = value;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//           Additional Functionality Need to Complete Usage Test Case

class my_DoubleStackIter {
    // PRIVATE DATA
    const double *d_stack_p;
    int           d_index;

  private:
    // NOT DEFINED
    my_DoubleStackIter(const my_DoubleStackIter&);
    my_DoubleStackIter& operator=(const my_DoubleStackIter&);

  public:
    explicit my_DoubleStackIter(const my_DoubleStack& stack)
    : d_stack_p(stack.d_stack_p), d_index(stack.d_length - 1) { }

    void operator++() { --d_index; }

    operator const void *() const { return d_index >= 0 ? this : 0; }

    const double& operator()() const { return d_stack_p[d_index]; }
};

void debugprint(const my_DoubleStack& val) {
    printf("(top) [");
    for (my_DoubleStackIter it(val); it; ++it) {
        printf(" %g", it());
    }
    printf(" ] (bottom)");
}

//-----------------------------------------------------------------------------
//                      OVERLOADED OPERATOR NEW USAGE EXAMPLE
//-----------------------------------------------------------------------------

class my_Type {
    char             *d_stuff_p;
    bslma::Allocator *d_allocator_p;

  public:
    my_Type(int size, bslma::Allocator *basicAllocator)
    : d_allocator_p(basicAllocator)
    {
        d_stuff_p = (char *) d_allocator_p->allocate(size);
    }

    ~my_Type()
    {
        d_allocator_p->deallocate(d_stuff_p);
    }
};

my_Type *newMyType(bslma::Allocator *basicAllocator) {
    return new (*basicAllocator) my_Type(5, basicAllocator);
}
void deleteMyType(bslma::Allocator *basicAllocator, my_Type *t) {
    t->~my_Type();
    basicAllocator->deallocate(t);
}

//-----------------------------------------------------------------------------
// HELPER CLASS FOR TESTING EXCEPTION SAFETY OF OVERLOADED OPERATOR NEW
//-----------------------------------------------------------------------------

class my_ClassThatMayThrowFromConstructor {
    char d_c;
  public:
    my_ClassThatMayThrowFromConstructor()
    {
#ifdef BDE_BUILD_TARGET_EXC
        throw int(13);
#endif
    }

    ~my_ClassThatMayThrowFromConstructor()
    {
    }
};

//=============================================================================
//                                 MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int                 test = argc > 1 ? atoi(argv[1]) : 0;
    bool             verbose = argc > 2;
    bool         veryVerbose = argc > 3;
    bool     veryVeryVerbose = argc > 4;
    bool veryVeryVeryVerbose = argc > 5;

    (void)veryVerbose;           // suppress unused variable warning
    (void)veryVeryVerbose;       // suppress unused variable warning
    (void)veryVeryVeryVerbose;   // suppress unused variable warning

    printf("TEST " __FILE__ " CASE %d\n", test);

    switch (test) { case 0:
      case 6: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE
        //   The usage example provided in the component header file must
        //   compile, link, and run on all platforms as shown.
        //
        // Plan:
        //   Incorporate usage example from header into driver, remove leading
        //   comment characters, and replace `assert` with `ASSERT`.
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------

        if (verbose) printf("\nUSAGE EXAMPLE"
                            "\n=============\n");

        if (verbose) printf("\nMain example usage test.\n");

        if (verbose) printf("\tCreating a stack with a specified allocatorn");
        {
            my_NewDeleteAllocator myA;
            bslma::Allocator& a = myA;
            my_DoubleStack s(&a);
            s.push(1.25);
            s.push(1.5);
            s.push(1.75);

            if (verbose) {
                T_ T_ P(s);
            }
        }

        if (verbose) printf(
                "\tCreating a stack without a specified allocator\n");
        {
            my_DoubleStack s;
            s.push(2.25);
            s.push(2.5);
            s.push(2.75);

            if (verbose) {
                T_ T_ P(s);
            }
        }

        if (verbose) printf("\nUsage test for `new` operator.\n");
        {
            my_NewDeleteAllocator myA;
            bslma::Allocator& a = myA;
            my_Type *t = newMyType(&a);
            deleteMyType(&a, t);
        }

      } break;
      case 5: {
        // --------------------------------------------------------------------
        // EXCEPTION SAFETY OF OPERATOR NEW TEST
        //   We want to make sure that when the overloaded operator new
        //   is invoked and the constructor of the new object throws an
        //   exception, the overloaded delete operator is invoked
        //   automatically to deallocate the object.
        //
        // Plan:
        //   Invoke `operator new` for a class that throws an exception from
        //   the chosen constructor.  Catch the exception and verify that
        //   deallocation was performed automatically.
        //
        // Testing:
        //   void operator delete(void *address, bslma::Allocator& bA);
        //   CONCERN: EXCEPTION SAFETY
        // --------------------------------------------------------------------

        if (verbose) printf("\nEXCEPTION SAFETY OF OPERATOR NEW TEST"
                            "\n=====================================\n");
        {
            my_Allocator myA;
            bslma::Allocator& a = myA;

            my_ClassThatMayThrowFromConstructor *p=0;
#ifdef BDE_BUILD_TARGET_EXC
            try
#endif
            {
                p = new (a) my_ClassThatMayThrowFromConstructor;
                p->~my_ClassThatMayThrowFromConstructor();
                a.deallocate(p);
                p = 0;
            }
#ifdef BDE_BUILD_TARGET_EXC
            catch(int n)
            {
                if (verbose) printf("\nCaught exception.\n");
                ASSERT(13 == n);
            }
#endif
            ASSERT(!p);
            ASSERT(2 == myA.fun());
            ASSERT(1 == myA.allocateCount());
            ASSERT(1 == myA.deallocateCount());
        }

      } break;
      case 4: {
        // --------------------------------------------------------------------
        // OPERATOR TEST
        //   We want to make sure that the correct underlying method is
        //   called based on the type of the overloaded `new` operator.
        //
        // Plan:
        //   Invoke `operator new` for types of various size.  Verify that
        //   correct parameters are passed to allocate method.
        //
        // Testing:
        //   void *operator new(int size, bslma::Allocator& basicAllocator);
        // --------------------------------------------------------------------

        if (verbose) printf("\nOPERATOR TEST"
                            "\n=============\n");
        my_Allocator myA;
        bslma::Allocator& a = myA;

        if (verbose) printf("\nTesting scalar input operators.\n");
        {
            ASSERT((char *) &myA == new(a) char);
            ASSERT(1 == myA.fun());     ASSERT(1 == myA.arg());

            ASSERT((short *) &myA == new(a) short);
            ASSERT(1 == myA.fun());     ASSERT(sizeof(short) == myA.arg());

            ASSERT((int *) &myA == new(a) int);
            ASSERT(1 == myA.fun());     ASSERT(sizeof(int) == myA.arg());

            ASSERT((double *) &myA == new(a) double);
            ASSERT(1 == myA.fun());     ASSERT(sizeof(double) == myA.arg());

            struct X15 { char d_z[15]; };       ASSERT(15 == sizeof(X15));
            ASSERT((X15 *) &myA == new(a) X15);
            ASSERT(1 == myA.fun());     ASSERT(15 == myA.arg());
        }

      } break;
      case 3: {
        // --------------------------------------------------------------------
        // MEMBER TEMPLATE METHOD `deleteObjectRaw` TEST
        //   We want to make sure that when `deleteObjRaw` is used both
        //   destructor and `deallocate` are invoked.
        //
        // Plan:
        //   Using an allocator and placement new operator construct objects of
        //   two different classes.  Invoke `deleteObjectRaw` to delete
        //   constructed objects and check that both destructor and
        //   `deallocate` have been called.  Repeat tests with a derived-class
        //   object with a virtual destructor.  Test with null pointer.
        //
        // Testing:
        //   template<class TYPE> void deleteObjectRaw(const TYPE *);
        //   void deleteObjectRaw(bsl::nulptr_t);
        // --------------------------------------------------------------------

        if (verbose) printf(
                          "\nMEMBER TEMPLATE METHOD `deleteObjectRaw` TEST"
                          "\n=============================================\n");

        if (verbose) printf("\nTesting `deleteObjectRaw`:\n");
        {
            my_NewDeleteAllocator myA;  bslma::Allocator& a = myA;

            if (verbose) printf("\twith a my_Class1 object\n");

            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);

            ASSERT(0 == myA.getCount());
            my_Class1 *pC1 = (my_Class1 *) a.allocate(sizeof(my_Class1));
            const my_Class1 *pC1CONST = pC1;
            ASSERT(1 == myA.getCount());

            new(pC1) my_Class1;
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(1 == globalObjectStatus);

            ASSERT(1 == myA.getCount());
            a.deleteObjectRaw(pC1CONST);
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);
            ASSERT(2 == myA.getCount());

            if (verbose) printf("\twith a my_Class2 object\n");

            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);

            ASSERT(2 == myA.getCount());
            my_Class2 *pC2 = (my_Class2 *) a.allocate(sizeof(my_Class2));
            const my_Class2 *pC2CONST = pC2;
            ASSERT(3 == myA.getCount());

            new(pC2) my_Class2;
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(1 == globalObjectStatus);

            ASSERT(3 == myA.getCount());
            a.deleteObjectRaw(pC2CONST);
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);
            ASSERT(4 == myA.getCount());

            if (verbose) printf("\tWith a polymorphic object\n");

            ASSERT(0 == class3ObjectCount);
            my_Class3 *pC3 = (my_Class3 *) a.allocate(sizeof(my_Class3));
            const my_Class3Base *pC3bCONST = pC3;
            (void) pC3bCONST;
            ASSERT(0 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);
            ASSERT(5 == myA.getCount());

            new(pC3) my_Class3;
            if (verbose) { T_;  T_;  P(class3ObjectCount); }
            ASSERT(1 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);

            ASSERT(5 == myA.getCount());
            a.deleteObjectRaw(pC3);
            if (verbose) { T_;  T_;  P(class3ObjectCount); }
            ASSERT(0 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);
            ASSERT(6 == myA.getCount());

            if (verbose) printf("\tWith a null my_Class3 pointer\n");

            pC3 = 0;
            a.deleteObjectRaw(pC3);
            if (verbose) { T_;  T_;  P(class3ObjectCount); }
            ASSERT(0 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);
            ASSERT(6 == myA.getCount());

            if (verbose) printf("\tWith a null pointer literal\n");

            a.deleteObjectRaw(0);
            ASSERT(6 == myA.getCount());

#if defined(BSLS_COMPILERFEATURES_SUPPORT_NULLPTR)
            a.deleteObjectRaw(nullptr);
            ASSERT(6 == myA.getCount());
#endif // BSLS_COMPILERFEATURES_SUPPORT_NULLPTR

        }
        {
            my_NewDeleteAllocator myA;  bslma::Allocator& a = myA;

            if (verbose) printf("\tdeleteObjectRaw(my_MostDerived*)\n");

            ASSERT(0 == myA.getCount());
            my_MostDerived *pMost =
                         (my_MostDerived *) a.allocate(sizeof(my_MostDerived));
            const my_MostDerived *pMostCONST = pMost;
            ASSERT(1 == myA.getCount());

            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);
            new(pMost) my_MostDerived;
            ASSERT(1 == mostDerivedObjectCount);
            ASSERT(1 == rightBaseObjectCount);
            ASSERT(1 == leftBaseObjectCount);
            ASSERT(1 == virtualBaseObjectCount);

            a.deleteObjectRaw(pMostCONST);
            ASSERT(2 == myA.getCount());
            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);
        }

      } break;

      case 2: {
        // --------------------------------------------------------------------
        // MEMBER TEMPLATE METHOD `deleteObject` TEST
        //
        // Concerns:
        // 1. `deleteObject` can be called with a null pointer, having no
        //    effect.
        // 2. `deleteObject` can be called with a null pointer literal, having
        //    no effect.
        // 3. `deleteObject`, when passed a pointer to an object allocated by
        //    a given allocator, runs the destructor for the referenced object,
        //    and then calls `deallocate` for the footprint of that object.
        // 4. `deleteObject`, when passed a pointer to a derived object (where
        //    the base type has a virtual destructor) allocated by a given
        //    allocator, runs the derived destructor for the referenced object,
        //    and then calls `deallocate` for the footprint of that whole
        //    object, even when the base class is not the left-most base of the
        //    derived type.
        //
        // Plan:
        //   Using an allocator and placement new operator construct objects of
        //   two different classes.  Invoke `deleteObject` to delete
        //   constructed objects and check that both destructor and
        //   `deallocate` have been called.  Repeat tests with a derived-class
        //   object with a virtual destructor.  Test with null pointer.
        //
        // Testing:
        //   template<class TYPE> void deleteObject(const TYPE *);
        //   void deleteObject(bsl::nulptr_t);
        // --------------------------------------------------------------------

        if (verbose) printf("\nMEMBER TEMPLATE METHOD `deleteObject` TEST"
                            "\n==========================================\n");

        if (verbose) printf("\nTesting `deleteObject`:\n");
        {
            my_NewDeleteAllocator myA;  bslma::Allocator& a = myA;

            if (verbose) printf("\twith a my_Class1 object\n");

            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);

            ASSERT(0 == myA.getCount());
            my_Class1 *pC1 = (my_Class1 *) a.allocate(sizeof(my_Class1));
            const my_Class1 *pC1CONST = pC1;
            ASSERT(1 == myA.getCount());

            new(pC1) my_Class1;
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(1 == globalObjectStatus);

            ASSERT(1 == myA.getCount());
            a.deleteObject(pC1CONST);
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);
            ASSERT(2 == myA.getCount());

            if (verbose) printf("\twith a my_Class2 object\n");

            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);

            ASSERT(2 == myA.getCount());
            my_Class2 *pC2 = (my_Class2 *) a.allocate(sizeof(my_Class2));
            const my_Class2 *pC2CONST = pC2;
            ASSERT(3 == myA.getCount());

            new(pC2) my_Class2;
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(1 == globalObjectStatus);

            ASSERT(3 == myA.getCount());
            a.deleteObject(pC2CONST);
            if (verbose) { T_;  T_;  P(globalObjectStatus); }
            ASSERT(0 == globalObjectStatus);
            ASSERT(4 == myA.getCount());

            if (verbose) printf("\tWith a my_Class3Base object\n");

            ASSERT(0 == class3ObjectCount);
            my_Class3 *pC3 = (my_Class3 *) a.allocate(sizeof(my_Class3));
            const my_Class3Base *pC3bCONST = pC3;
            ASSERT(0 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);
            ASSERT(5 == myA.getCount());

            new(pC3) my_Class3;
            if (verbose) { T_;  T_;  P(class3ObjectCount); }
            ASSERT(1 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);

            ASSERT(5 == myA.getCount());
            a.deleteObject(pC3bCONST);
            if (verbose) { T_;  T_;  P(class3ObjectCount); }
            ASSERT(0 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);
            ASSERT(6 == myA.getCount());

            if (verbose) printf("\tWith a null my_Class3 pointer\n");

            pC3 = 0;
            a.deleteObject(pC3);
            if (verbose) { T_;  T_;  P(class3ObjectCount); }
            ASSERT(0 == class3ObjectCount);
            ASSERT(0 == globalObjectStatus);
            ASSERT(6 == myA.getCount());

            if (verbose) printf("\tWith a null pointer literal\n");

            a.deleteObject(0);
            ASSERT(6 == myA.getCount());

#if defined(BSLS_COMPILERFEATURES_SUPPORT_NULLPTR)
            a.deleteObject(nullptr);
            ASSERT(6 == myA.getCount());
#endif // BSLS_COMPILERFEATURES_SUPPORT_NULLPTR
        }
        {
            my_NewDeleteAllocator myA;  bslma::Allocator& a = myA;

            if (verbose) printf("\tdeleteObject(my_MostDerived*)\n");

            ASSERT(0 == myA.getCount());
            my_MostDerived *pMost =
                         (my_MostDerived *) a.allocate(sizeof(my_MostDerived));
            const my_MostDerived *pMostCONST = pMost;
            ASSERT(1 == myA.getCount());

            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);
            new(pMost) my_MostDerived;
            ASSERT(1 == mostDerivedObjectCount);
            ASSERT(1 == rightBaseObjectCount);
            ASSERT(1 == leftBaseObjectCount);
            ASSERT(1 == virtualBaseObjectCount);

            a.deleteObject(pMostCONST);
            ASSERT(2 == myA.getCount());
            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);

            if (verbose) printf("\tdeleteObject(my_LeftBase*)\n");

            pMost = (my_MostDerived *) a.allocate(sizeof(my_MostDerived));
            ASSERT(3 == myA.getCount());

            new(pMost) my_MostDerived;
            const my_LeftBase *pLeftCONST = pMost;
            ASSERT(1 == mostDerivedObjectCount);
            ASSERT(1 == rightBaseObjectCount);
            ASSERT(1 == leftBaseObjectCount);
            ASSERT(1 == virtualBaseObjectCount);

            a.deleteObject(pLeftCONST);
            ASSERT(4 == myA.getCount());
            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);

            if (verbose) printf("\tdeleteObject(my_RightBase*)\n");

            pMost = (my_MostDerived *) a.allocate(sizeof(my_MostDerived));
            ASSERT(5 == myA.getCount());

            new(pMost) my_MostDerived;
            const my_RightBase *pRightCONST = pMost;
            ASSERT(1 == mostDerivedObjectCount);
            ASSERT(1 == rightBaseObjectCount);
            ASSERT(1 == leftBaseObjectCount);
            ASSERT(1 == virtualBaseObjectCount);

            ASSERT(static_cast<const void *>(pRightCONST) !=
                   static_cast<const void *>(pMost));

            a.deleteObject(pRightCONST);
            ASSERT(6 == myA.getCount());
            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);

            if (verbose) printf("\tdeleteObject(my_VirtualBase*)\n");

            pMost = (my_MostDerived *) a.allocate(sizeof(my_MostDerived));
            ASSERT(7 == myA.getCount());

            new(pMost) my_MostDerived;
            const my_VirtualBase *pVirtualCONST = pMost;
            ASSERT(1 == mostDerivedObjectCount);
            ASSERT(1 == rightBaseObjectCount);
            ASSERT(1 == leftBaseObjectCount);
            ASSERT(1 == virtualBaseObjectCount);

            a.deleteObject(pVirtualCONST);
            ASSERT(8 == myA.getCount());
            ASSERT(0 == mostDerivedObjectCount);
            ASSERT(0 == rightBaseObjectCount);
            ASSERT(0 == leftBaseObjectCount);
            ASSERT(0 == virtualBaseObjectCount);
        }

      } break;
      case 1: {
        // --------------------------------------------------------------------
        // PROTOCOL TEST:
        //   Ensure this class is a properly defined protocol.
        //
        // Concerns:
        // 1. The protocol is abstract: no objects of it can be created.
        // 2. The protocol has no data members.
        // 3. The protocol has a virtual destructor.
        // 4. All methods of the `bslma::Allocator` protocol are publicly
        //    accessible virtual functions.
        // 5. All virtual methods inherited from `bsl::memory_resource` are
        //    available through public pass-through functions
        // 6. The methods inherited from `bsl::memory_resource` are not pure
        //    virtual in the `bslma::Allocator` derived class. If they are not
        //    overriden, `do_allocate` and `do_deallocate` call the (overriden)
        //    `allocate` and `deallocate` methods, respectively.
        //
        // Plan:
        // 1. Define a concrete derived implementation of the protocol,
        //    `AllocatorProtocolTest`, that overrides all of the virtual
        //    methods and records when they are called.
        // 2. Create an object of the `bsls::ProtocolTest` class template
        //    parameterized by `AllocatorProtocolTest`, and use it to verify
        //    that:
        //
        //   1. The protocol is abstract. (C-1)
        //   2. The protocol has no data members. (C-2)
        //   3. The protocol has a virtual destructor. (C-3)
        //
        // 3. Use the `BSLS_PROTOCOLTEST_ASSERT` macro to verify that
        //    non-creator methods of the `bslma::Allocator` protocol are
        //    virtual and publicly available.  (C-4)
        // 4. Use the `BSLS_PROTOCOLTEST_ASSERT` macro to verify that
        //    non-creator methods inherited from `bsl::memory_resource` are
        //    virtual and available through public pass-through functions.
        //    (C-5)
        // 5. Define a concrete derivded implementation of the protocol,
        //    `IndirectProtocolTest`, that overrides the new (pure)
        //    virtual methods introduced by `bslma::Allocator` but not the (not
        //    pure) virtual methods inherited from `bsl::memory_resource`.
        //    Repeat steps 2-4 with this `IndirectProtocolTest`.
        //
        // Testing:
        //      ~Allocator();
        //      void *allocate(size_type);
        //      void deallocate(void *);
        //      void* do_allocate(std::size_t, std::size_t);
        //      void do_deallocate(void *p, std::size_t, std::size_t);
        //      bool do_is_equal(const bsl::memory_resource&) const;
        // --------------------------------------------------------------------

        if (verbose) printf("\nPROTOCOL TEST"
                            "\n=============\n");

        bsls::ProtocolTest<AllocatorProtocolTest> testObj(veryVerbose);

        ASSERT(testObj.testAbstract());
        ASSERT(testObj.testNoDataMembers());
        ASSERT(testObj.testVirtualDestructor());

        // Create a reference to `bslma::Allocator` to test protocol.
        const bslma::Allocator& other = AllocatorProtocolTest();
        void *p = 0;

        // Test `bslma::Allocator` protocol.  Note that the base-class
        // `allocate` and `deallocate` non-virtual functions are hidden.
        BSLS_PROTOCOLTEST_RV_ASSERT(testObj, allocate(2), p);
        BSLS_PROTOCOLTEST_ASSERT(testObj, deallocate(p));

        // Test `bsl::memory_resource` base-class protocol via pass-through
        // functions.  Note that the base-class `allocate` and `deallocate` are
        // hidden within `bslma::Allocator` and must be qualified in the method
        // call.
        typedef bsl::memory_resource Base;
        BSLS_PROTOCOLTEST_RV_ASSERT(testObj, Base::allocate(2, 1), p);
        BSLS_PROTOCOLTEST_ASSERT(testObj, Base::deallocate(p, 2, 1));
        BSLS_PROTOCOLTEST_ASSERT(testObj, is_equal(other));

        bsls::ProtocolTest<IndirectProtocolTest> testIndirect(veryVerbose);

        ASSERT(testIndirect.testAbstract());
        ASSERT(testIndirect.testNoDataMembers());
        ASSERT(testIndirect.testVirtualDestructor());

        // Test `bslma::Allocator` protocol.  Note that the base-class
        // `allocate` and `deallocate` non-virtual functions are hidden.
        BSLS_PROTOCOLTEST_ASSERT(testIndirect, allocate(2));
        BSLS_PROTOCOLTEST_ASSERT(testIndirect, deallocate(p));

        // Test `bsl::memory_resource` base-class protocol via pass-through
        // functions.  Note that the base-class `allocate` and `deallocate` are
        // hidden within `bslma::Allocator` and must be qualified in the method
        // call.
        typedef bsl::memory_resource Base;
        BSLS_PROTOCOLTEST_RV_ASSERT(testIndirect, Base::allocate(2, 1), p);
        BSLS_PROTOCOLTEST_ASSERT(testIndirect, Base::deallocate(p, 2, 1));
        // The default implementation of `do_is_equal` cannot be tested because
        // it does call a virtual function that we can intercept.

        // Test zero-byte allocations.
        // TBD: This is a very basic and undocumented test.  A more
        // sophisticated test will be added as part of the fix for DRQS
        // 176364960.
        my_NewDeleteAllocator myA;

        // Zero-byte allocation through `bslma::Allocator` interface.
        void *p1 = myA.allocate(0);
        ASSERT(0 == p1);
        ASSERT(1 == myA.getCount());    // Increments even for zero bytes

        myA.deallocate(p1);             // Can deallocate null pointer
        ASSERT(2 == myA.getCount());    // Increments even for zero bytes

        // Zero-byte allocation through `bsl::memory_resource` interface.
        bsl::memory_resource& myR = myA;
        void *p2 = myR.allocate(0);
        ASSERT(0 != p2);                // Non-zero return
        ASSERT(3 == myA.getCount());    // Increments even for zero bytes

        ASSERT(myR.allocate(0) == p2);  // Returns same pointer every time
        ASSERT(4 == myA.getCount());

        myR.deallocate(p2, 0);          // Can deallocate zero-size block
        ASSERT(5 == myA.getCount());    // Increments even for zero bytes

      } break;

      default: {
        fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        fprintf(stderr, "Error, non-zero test status = %d.\n", testStatus);
    }

    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------

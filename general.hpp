#ifndef general_hpp
#define general_hpp

// some primitives
struct Unspecified;
struct EmptyBase{};
struct NullType;
struct True;
struct False;

// 5 most brilliant lines of C++ code ever :), the typelist declaration
template<typename HEAD, typename TAIL>
struct List
{
  typedef HEAD Head;
  typedef TAIL Tail;
};

// for some reason, gcc does not allow PARTIAL specialization of inner classes
// but it does allow FULL specialization. VC++, doesn't support
// partial specialization at all, but it does allow for full specialization
// of inner classes. these macros are needed to make code portable
// between compilers. they allow for up to 4 layers of nested, specialized
// inner classes
//
// an example of use is below in the EfficientPass template.

#define GCC
#ifdef GCC
#define CPP_FORCE_PARTIAL_TARG  , typename EXTRA = Unspecified
#define CPP_FORCE_PARTIAL_ARG   , EXTRA
#define CPP_FORCE_PARTIAL       typename EXTRA

#define CPP_FORCE_PARTIAL_TARG1  , typename EXTRA1 = Unspecified
#define CPP_FORCE_PARTIAL_ARG1   , EXTRA1
#define CPP_FORCE_PARTIAL1       typename EXTRA1

#define CPP_FORCE_PARTIAL_TARG2  , typename EXTRA2 = Unspecified
#define CPP_FORCE_PARTIAL_ARG2   , EXTRA2
#define CPP_FORCE_PARTIAL2       typename EXTRA2

#define CPP_FORCE_PARTIAL_TARG3  , typename EXTRA3 = Unspecified
#define CPP_FORCE_PARTIAL_ARG3   , EXTRA3
#define CPP_FORCE_PARTIAL3       typename EXTRA3

#else
#define CPP_FORCE_PARTIAL_TARG
#define CPP_FORCE_PARTIAL_ARG
#define CPP_FORCE_PARTIAL

#define CPP_FORCE_PARTIAL_TARG1
#define CPP_FORCE_PARTIAL_ARG1
#define CPP_FORCE_PARTIAL1

#define CPP_FORCE_PARTIAL_TARG2
#define CPP_FORCE_PARTIAL_ARG2
#define CPP_FORCE_PARTIAL2

#define CPP_FORCE_PARTIAL_TARG3
#define CPP_FORCE_PARTIAL_ARG3
#define CPP_FORCE_PARTIAL3

#endif

// xxx: this implementation is functional, but completely wrong.
// I need to port Loki's implementation

template<class T>
class EfficientPass
{
public:

  template <bool U CPP_FORCE_PARTIAL_TARG>
  struct Spec
  {
    typedef T Result;
  };
  
  template<CPP_FORCE_PARTIAL>
  struct Spec<false CPP_FORCE_PARTIAL_ARG>
  {
    typedef T const & Result;
  };
  
  typedef Spec< (sizeof(T)>4) > Result;
};

#endif
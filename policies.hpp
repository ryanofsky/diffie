#ifndef policies_hpp
#define policies_hpp

#include "general.hpp"

// utility struct used to specify different ACTUAL and BASE parameters
// for a chainable struct. Each chainable struct, will provide
// a specialization of this template struct on their Identity attribute
template<typename IDENITY>
struct Specify
{
  template<typename PARAMS, typename ACTUAL, typename BASE>
  struct Args
  {
    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    // this next line should be
    //
    //   typedef Unspecified Result;
    //
    // but visual C++ refuses to compile templates below if this is the case
    // complaining 'base class undefined' at MagicBrains::Tail
    //
    // I fucking hope this error message is just a result of overzealous
    // semantic error checking, and that the compiler is not in fact
    // ignoring the specializations of this template and automatically
    // binding to this general instance.

    typedef EmptyBase Result;

    // Update: it is, in fact, a case of overzealous error checking
    // Hurray, and Long live microsoft!
  };
};

// some forward declarations which will appease the grumpy c++ compiler
template<typename A, typename V, typename Z> struct Magic;
template<typename> struct NoMagic;

//template<struct ACTUAL, struct HEAD> = ACTUAL, LIST::HEAD
template<typename ACTUAL, typename LIST, typename TERMINAL_BASE>
struct MagicBrains
{
private:
  typedef typename LIST::Head::Params abc123;
  typedef typename LIST::Tail ltail;

  template<typename MAGIC_BASE>
  struct Spec
  {
    typedef typename Specify<typename LIST::Head::Identity>::Args<typename LIST::Head::Params, ACTUAL, MAGIC_BASE >::Result Result;
  };

  // is this the end of the list? (no)
  template<typename TAIL CPP_FORCE_PARTIAL_TARG>
  struct Tail : public Spec< Magic<ACTUAL, ltail, TERMINAL_BASE> >::Result
  {
    typedef Magic<ACTUAL, ltail, TERMINAL_BASE> magic;
    //XXX: gcc says Spec is not a template class?
    ////typedef Spec< magic >::Result Policy;
    typedef typename magic::TheTail Next;
  };
 
  // is this the end of the list? (yes)
  template<CPP_FORCE_PARTIAL>
  struct Tail<NullType CPP_FORCE_PARTIAL_ARG> : public Spec< NoMagic<TERMINAL_BASE> >::Result
  {
    typedef NoMagic<TERMINAL_BASE> magic;
    //xxx: typedef Spec< magic >::Result Policy;
    typedef NullType Next;
  };

  typedef Tail<typename LIST::TAIL> Result;
};

template<typename ACTUAL, typename LIST, typename TERMINAL_BASE = EmptyBase>
struct Magic : public MagicBrains<ACTUAL, LIST, TERMINAL_BASE>::Result
{
  typedef True IsMagic;
  typedef TERMINAL_BASE TerminalBase;
  typedef ACTUAL Actual;
  typedef LIST List;
  typedef MagicBrains<ACTUAL, LIST, TERMINAL_BASE>::Result TheTail;
  typedef typename TheTail::Next Next;
  typedef typename TheTail::Policy Policy;
};

template<typename TERMINAL_BASE>
struct NoMagic : public TERMINAL_BASE
{
  typedef False IsMagic;
  typedef TERMINAL_BASE TerminalBase;
  typedef NullType TheTail;
  typedef NullType Next;
  typedef NullType Policy;
};

template<typename MAGIC, typename TERMINAL_BASE>
struct DoubleInheritBrains
{

private:

  // MAGIC == struct Magic
  template<typename IS_MAGIC CPP_FORCE_PARTIAL_TARG>
  struct IsMagic
  {
    // MAGIC's TerminalBase is not empty, multiple inheritance is needed
    template<typename MAGIC_BASE CPP_FORCE_PARTIAL_TARG1>
    struct MagicBase
    {
      struct Result : public Magic<typename MAGIC::Actual, typename MAGIC::List, TERMINAL_BASE>, public MAGIC_BASE
      {};
    };

    // MAGIC's TerminalBase is empty, so replace it with an actual TERMINAL_BASE
    template<CPP_FORCE_PARTIAL1>
    struct MagicBase<EmptyBase CPP_FORCE_PARTIAL_ARG1> 
    {
      struct Result : public Magic<typename MAGIC::Actual, typename MAGIC::List, TERMINAL_BASE>
      {
        
      };
    };
  };

  // MAGIC == struct NoMagic
  template<CPP_FORCE_PARTIAL>
  struct IsMagic<False CPP_FORCE_PARTIAL_ARG>
  {
    // MAGIC's TerminalBase is not empty, multiple inheritance is needed
    template<typename MAGIC_BASE CPP_FORCE_PARTIAL_TARG1>
    struct MagicBase
    {
      struct Result : public MAGIC, public TERMINAL_BASE
      {};
    };

    // MAGIC's TerminalBase is empty, so just inherit TERMINAL_BASE
    template<CPP_FORCE_PARTIAL1>
    struct MagicBase<EmptyBase CPP_FORCE_PARTIAL_ARG1>
    {
      struct Result : public TERMINAL_BASE
      {};
    };
  };

public:

  typedef IsMagic<typename MAGIC::IsMagic> Result;
};

template<typename MAGIC, typename TERMINAL_BASE>
struct DoubleInherit : DoubleInheritBrains<MAGIC, TERMINAL_BASE>::Result
{ };

#endif
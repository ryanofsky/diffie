#ifndef policies_hpp
#define policies_hpp

// some primitives
struct Unspecified;
struct EmptyBase {};
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
  typedef LIST::Head::Params abc123;
  typedef LIST::Tail ltail;
  
  template<typename TAIL>
  struct Huh
  {
    typedef typename Specify<LIST::Head::Identity>::Args<abc123 , ACTUAL, Magic<ACTUAL, ltail, TERMINAL_BASE> >::Result
    Result;

  };

  // is this the end of the list? (no)
  template<typename TAIL>
  struct Tail : public Huh<TAIL>::Result
  {
  };
 
  // is this the end of the list? (yes)
  template<>
  struct Tail<NullType> : public Specify<LIST::Head::Identity>::Args< LIST::Head::Params, ACTUAL, NoMagic<TERMINAL_BASE> >::Result
  {
  };

  typedef Tail<LIST::TAIL> Result;
};

template<typename ACTUAL, typename LIST, typename TERMINAL_BASE = EmptyBase>
struct Magic : public MagicBrains<ACTUAL, LIST, TERMINAL_BASE>::Result
{
  typedef True IsMagic;
  typedef TERMINAL_BASE TerminalBase;
  typedef ACTUAL Actual;
  typedef LIST List;
};

template<typename TERMINAL_BASE>
struct NoMagic : public TERMINAL_BASE
{
  typedef False IsMagic;
  typedef TERMINAL_BASE TerminalBase;
};

template<typename MAGIC, typename TERMINAL_BASE>
struct DoubleInheritBrains
{

private:

  // MAGIC == struct Magic
  template<typename IS_MAGIC>
  struct IsMagic
  {
    // MAGIC's TerminalBase is not empty, multiple inheritance is needed
    template<typename MAGIC_BASE>
    struct MagicBase
    {
      struct Result : public Magic<MAGIC::Actual, MAGIC::List, TERMINAL_BASE>, public MAGIC_BASE
      {};
    };

    // MAGIC's TerminalBase is empty, so replace it with an actual TERMINAL_BASE
    template<>
    struct MagicBase<EmptyBase> 
    {
      struct Result : public Magic<MAGIC::Actual, MAGIC::List, TERMINAL_BASE>
      {};
    };
  };

  // MAGIC == struct NoMagic
  template<>
  struct IsMagic<False>
  {
    // MAGIC's TerminalBase is not empty, multiple inheritance is needed
    template<typename MAGIC_BASE>
    struct MagicBase
    {
      struct Result : public MAGIC, public TERMINAL_BASE
      {};
    };

    // MAGIC's TerminalBase is empty, so just inherit TERMINAL_BASE
    template<>
    struct MagicBase<EmptyBase>
    {
      struct Result : public TERMINAL_BASE
      {};
    };
  };

public:

  typedef IsMagic<MAGIC::IsMagic> Result;
};

template<typename MAGIC, typename TERMINAL_BASE>
struct DoubleInherit : DoubleInheritBrains<MAGIC, TERMINAL_BASE>::Result
{ };

#endif
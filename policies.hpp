#ifndef policies_hpp
#define policies_hpp

class Unspecified {};
class EmptyBase {};
class Null;

template<class HEAD, class TAIL>
class List
{
  typedef HEAD head;
  typedef TAIL tail;
};


class ohno {

template<class A, class B, class C>
class Magic;

template<class ACTUAL, class HEAD>
class PartialMagic
{
  // is this the end of the list? (no)
  template<class TAIL>
  class IsNull
  {
    // is the load empty? (no)
    template<class LOAD>
    class IsEmpty
    {
      // is the terminal base empty? (no)
      template<class TERMINAL_BASE>
      class IsEmpty : public HEAD::Specify< 
      
      
      ACTUAL, 
      
      Magic<
      
      ACTUAL, TAIL, LOAD> >, public TERMINAL_BASE
      
      {};
      
      // is the terminal base empty? (yes)
      template<>
      class IsEmpty<EmptyBase> : public HEAD::Specify<ACTUAL, Magic<ACTUAL, TAIL, LOAD> >
      {};
    };     
    
    // is the load empty? (yes)
    template<>
    class IsEmpty<EmptyBase>
    {
      // is the terminal base empty? (doesn't matter)
      template<class TERMINAL_BASE>
      class IsEmpty : public HEAD::Specify< ACTUAL, Magic<ACTUAL, TAIL, TERMINAL_BASE> >
      {};
    };
  };
  
  // is this the end of the list? (yes)
  template<>
  class IsNull<Null>
  {
    // is the load empty? (no)
    template<class LOAD>
    class IsEmpty
    {
      // is the terminal base empty? (no)
      template<class TERMINAL_BASE>
      class IsEmpty : public HEAD::Specify< ACTUAL, LOAD >, public TERMINAL_BASE
      {};
      
      // is the terminal base empty? (yes)
      template<>
      class IsEmpty<EmptyBase> : public HEAD::Specify<ACTUAL, LOAD >
      {};
    };     
    
    // is the load empty? (yes)
    template<>
    class IsEmpty<EmptyBase>
    {
      // is the terminal base empty? (doesn't matter)
      template<class TERMINAL_BASE>
      class IsEmpty : public HEAD::Specify< ACTUAL, TERMINAL_BASE >
      {};
    };
  };
};

template<class ACTUAL, class PAYLOAD, class TERMINAL_BASE = EmptyBase>
class Magic //: public PartialMagic<ACTUAL, PAYLOAD::list::tail>
{
  template<class NEW_TERMINAL_BASE>
  class Inherit
  {
    typedef Magic<ACTUAL, PAYLOAD, NEW_TERMINAL_BASE> Result;
  };
};


}; // ohno

#endif
#ifndef smart_assign_hpp
#define smart_assign_hpp

#include "policies.hpp"
#include "smart_assign.hpp"

template<class ACTUAL, class BASE>
class ReluctantReferenceCountImpl : public BASE
{
  typedef typename ACTUAL::Ownership Ownership;
  enum { ACCESS = 0, TRANSFER = ~ACCESS };

  ReluctantReferenceCountImpl() : counter(ACCESS) {}

public:

  void StartCounting() const
  {
    if (counter == TRANSFER) counter = new unsigned int(1);
  };

protected:

  void Own()
  {
    if (counter == ACCESS) // if access
      counter = TRANSFER;
    else if (counter != TRANSFER) // if refcounted  
      if (*counter != 1) 
      {
        --*counter;
        counter = TRANSFER;
      }
  }

  bool Disown()
  {
    if (counter == TRANSFER) // if transfer
      counter = ACCESS;
    else if (counter != ACCESS) // if refcounted
      if (--*counter == 0)
      {
        delete counter;
        counter = ACCESS;
      }
  }

  bool Owned()
  {
    if (counter == ACCESS) // if access
      return false;
    else if (counter == TRANSFER) // if transfer
      return true;
    else // if refcounted
      return *counter == 1;
  }

  mutable int * counter;
};

class ReluctantReferenceCount
{
  typedef ReluctantReferenceCount Identity;
  typedef EmptyBase Parameterization;
};

template<>
struct Specify<ReluctantReferenceCount>
{
  template<typename PARAMS, typename ACTUAL, typename BASE>
  struct Args
  {
    typedef ReluctantReferenceCountImpl<ACTUAL, BASE> Result;
  };
};

//template<class, class> struct SmartAssign;

/*
template<>
struct SmartAssign<ReluctantReferenceCount<>, ReluctantReferenceCount<> >
{
  template<class DESTINATION, class SOURCE>
  void operator()(DESTINATION & dest, SOURCE const & src)
  {    
    dest.counter = src.counter;
    if (dest.counter == TRANSFER) // if transfer
      src.counter = ACCESS;
    else if (dest.counter != ACCESS) // if refcount
      ++*dest.counter;
  };
};
*/

#endif

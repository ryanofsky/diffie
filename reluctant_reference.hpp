#ifndef smart_assign_hpp
#define smart_assign_hpp

#include "SmartAssign.hpp"

template<>
class SmartAssign<ReluctantReferenceCount, ReluctantReferenceCount>
{
  // Is it legal to be friends with an unparamaterized class
  friend class SmartAssign;
  
  // XXX: are these even neccessary?
  typedef ReluctantReferenceCount SOURCE_OWNERSHIP;
  typedef ReluctantReferenceCount DESTINATION_OWNERSHIP;
  
  template<SOURCE_MANAGER, DESTINATION_MANAGER>
  operator()(SOURCE_OWNERSHIP<SOURCE_MANAGER> const & src, DESTINATION_OWNERSHIP<DESTINATION_MANAGER> & dest)
  {
    counter = r.counter;
    if (counter == TRANSFER) // if transfer
      r.counter = ACCESS;
    else if (counter != ACCESS) // if refcount
      ++*counter;
  }
};

template<class MANAGER>
class ReluctantReferenceCount : public MANAGER
{
  typedef ReluctantReferenceCount<MANAGER> OWNERSHIP;
  enum { ACCESS = 0; TRANSFER = ~ACCESS };

  ReluctantReferenceCount() : counter(ACCESS) {}

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

#endif

#ifndef smart_pointer_hpp
#define smart_pointer_hpp

template<typename T, class ACTUAL, class BASE>
class PointerManagerImpl : public BASE
{
  typedef typename ACTUAL::Ownership Ownership;
  
public:

  T * leak()
  {
    T * ret(p);
    Ownership * o = (Ownership *)this;
    o->Disown();
    return ret;
  }
  
protected:

  void SetNull()
  {
    t = 0;
  }

  bool IsNull()
  {
    return t == 0;
  }

  void ShallowCopy(PointerManagerImpl const & p)
  {
    t = p.t;
  }

  void DeepCopy(PointerManagerImpl const & p)
  {
    t = new T(p.t);
  }

  void Close()
  {
    delete t;
  }

  T & operator->()
  {
    return *t;
  }

  T * operator*()
  {
    return t;
  }

  T * t;
};

template<typename T>
struct PointerManager
{
  typedef PointerManager<Unspecified> Identity;
  typedef T Parameterization;
};

template<>
struct Specify< PointerManager<Unspecified> >
{
  template<typename PARAMS, typename ACTUAL, typename BASE>
  struct Args
  {
    typedef PointerManagerImpl<PARAMS, ACTUAL, BASE> Result;
  };
};

#endif
#ifndef smart_pointer_hpp
#define smart_pointer_hpp

template<typename T, class ACTUAL = Unspecified, class BASE = Unspecified>
class PointerManager : public BASE
{
  
  typedef PointerManager<Unspecified> Identity;
  typedef T Parameterization;
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

  void ShallowCopy(PointerManager const & p)
  {
    t = p.t;
  }

  void DeepCopy(PointerManager const & p)
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

template<>
struct Specify< PointerManager<Unspecified> >
{
  template<typename PARAMS, typename ACTUAL, typename BASE>
  struct Args
  {
    typedef PointerManager<PARAMS, ACTUAL, BASE> Result;
  };
};

#endif


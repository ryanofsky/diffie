#ifndef smart_pointer_hpp
#define smart_pointer_hpp

template<typename T>
class PointerManager
{
public:

  T * leak()
  {
    T * ret(p);
    OWNERSHIP * o = (OWNERSHIP *)this;
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
    t = new PointerType(p.t)
  }

  void Close()
  {
    delete T;
  }

  PointerType & operator->()
  {
    return *t;
  }

  PointerType * operator*()
  {
    return t;
  }

  T * t;
};

#endif
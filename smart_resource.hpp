#ifndef smart_resource
#define smart_resource

class EmptyBase {};

// for all 
//   BASE - type that this class should inherit from
//   ACTUAL - type that has inherited from this cilass. this pointer can be cast to ACTUAL *
//            to emulate having compile-time virtual method call
//   PARAMS - a class filled with enum's and typedefs that can be used to configure the behavior
//            of this class. the PARAM class is just a way of stuffing a large number of template
//            parameters into a single argument. 

template<class ACTUAL, class BASE = EmptyBase>
class HandleManager : public EmptyBase
{
  typedef Base<> Manager;
  typedef HandleManager<ACTUAL> MANAGER;
public:
  enum { HAS_NULLCHECKING = true };
  enum { HAS_OWNERSHIPCHECKING = true };

  //! Required by all
  void ShallowCopy(HandleManager const & HandleManager);
  void Close();
  
  //! Optional (set HAS_NULLCHECKING to true if present)
  void SetNull();
  bool IsNull();

  //! Optional (set HAS_OWNERSHIPCHECKING to false if present)
  void Disown();
  bool Owns();
  
  //! Required for COW, Duplicate
  DeepCopy();

  //! Required for intrusive count
  AddRef();
  DeleteRef();
  
  //! 
  void Swallow(OneArg);

protected:  

  void CopyForWrite()
  {
    ACTUAL & o = *((ACTUAL *)this);
    if (!this.Own())
    {
      MANAGER m;
      m.DeepCopy(o);
      o.ShallowCopy(m);
    }
  }
  
  void Dispose()
  {
    ACTUAL & o = *((ACTUAL *)this); 
    if (this.Disown())
    {
      m.Dispose();
      m.SetNull();  
    }
  }
  
  void PreOpen()
  {
    
  }

  template<class ARG_OWNERSHIP>
  void Assign(ARG_OWNERSHIP<MANAGER> const & other)
  {
    Dispose();
    ACTUAL & o = *((ACTUAL *) this);
    
    if (other.Own())
    {
      this.own();
    }
  }

  Handle handle_;
};

// optimization question
// will compilers go from:
//   inline bool f() { return false; }
//   if (f()) cout << "A"; else cout << "B";
// cout << "B";
// check it out for metrowerks and visual c++

template<class MANAGER, class POLICY>
class SmartResource : protected POLICY<MANAGER>
{
  typedef SmartResource<MANAGER, POLICY> THIS;
  typedef POLICY<MANAGER> BaseType;
  typedef SmartResource<MANAGER, POLICY> ThisType;

public:    

  SmartResource() : BaseType() {}
  ~SmartResource() { BaseType::Discard(); } 

  SmartResource(ThisType const & c) { BaseType::Assign(c); }

  BaseType & operator= () { BaseType::Assign(c); return *this; }

  // XXX: repeat 0..15
  template<A1, A2, A3>
  SmartResource(A1 a1, A2 a2, A3 a3)
  { MANAGER::Open(a1, a2, a3); }

  ~SmartResource()
  {
    if (!MANAGER::IsNull() && POLICY::DisOwn())
    {
      MANAGER::Close();
    }
  }

  template<class SMART_OTHER> 
  SmartResource (SMART_OTHER r)
  { 
    Constructor(r, int2type<type_equals<SMART_OTHER, arg1<OWNERSHIP::Release> > >);
  }

private:

  template<class T1>
  void Constructor(T1 t1, int2type<false> i) { Release(t1); }
  
  template<class T2>
  void Constructor(T2 t2, int2type<true> i) { Open(t2); }

public:

  template<class SMART_OTHER>
  THIS & operator=(SMART_OTHER r)
  {
    SmartAssign<OWNERSHIP, SMART_OTHER::OWNERSHIP> sa;
    sa(*this, r);
    MANAGER::ShallowCopy(r);
  }
  

  
  THIS copy()
  { 
    R temp;
    temp.shallowcopy(*this);
    temp.duplicate();
    return temp; 
  };
};

class HandlePolicy
{
  //! Reset ownership. Get in default state for non-nulln handle. called during open() or assignment from raw handle
  void Reset();
  
  //! take ownership
  void Own() {}
  
  //! returns true if was owned, false otherwise
  bool Disown() {}
  
  //! returns true of owned, false otherwise
  bool Owned() {}
};

#endif
#ifndef smart_resource
#define smart_resource

// reason why IsNull() and SetNull() might need to be optional:
//   for certain types of managers, IsNull() and SetNull() would
//   be expensive to implement. One example would be a pointer
//   to an object which is freely constructed and destructed
//   on a place on the stack. there is no automated way 
//   of checking to see whether a class is constructed, so the
//   only way of doing null testing is to store a boolean
//   for each instance
//   
// but certain ownership policies (ie ReluctantReference<>) don't
// need to use null checking [xxx: is this true?] so the
// expense is not neccessary

#include "general.hpp"

// for all 
//   BASE - type that this class should inherit from
//   ACTUAL - type that has inherited from this cilass. this pointer can be cast to ACTUAL *
//            to emulate having compile-time virtual method call
//   PARAMS - a class filled with enum's and typedefs that can be used to configure the behavior
//            of this class. the PARAM class is just a way of stuffing a large number of template
//            parameters into a single argument. 

/*
  
  Define the HandleManager interface

template<class ACTUAL = Unspecified, class BASE = Unspecified>
class HandleManager : public TopManager<ACTUAL, SomeBaseClass>
{
public:

  typedef HandleManager<> Identity;
  typedef Unspecified Parameterization;

  template<class NEW_ACTUAL, class NEW_BASE>
  struct Specify { typedef HandleManager<NEW_ACTUAL, NEW_BASE> result; };
  
  //! Required by all
  void ShallowCopy(HandleManager const & HandleManager);
  void Close();
  
  //! Set Handle to null value, and check for null
  void SetNull();
  bool IsNull();

  //! Set ownership to null, in most cases this is just
  // indicated by a normal null handle, but for more
  // complex resource types it is possible to store
  // the ownership value separately without any
  // additional overhead
  void SetOwnNull();
  bool IsOwnNull();
  
  //! Required for COW, Duplicate
  DeepCopy();

  //! Required for intrusive count
  AddRef();
  DeleteRef();
  
  //! Used to assign
  void Swallow(OneArg);
  
  //! Open
  void Open(...);
};

/* Define the HandlePolicy interface

template<class ACTUAL = Unspecified, class BASE = Unspecified>
class HandlePolicy : public BASE::Chain<ACTUAL, 
{
public:
  template<class NEW_ACTUAL, class NEW_BASE>
  struct Specify { typedef HandleManager<NEW_ACTUAL, NEW_BASE> result; };

  //! Reset ownership. Get in default state for non-nulln handle. called during open() or assignment from raw handle
  void Reset();
  
  //! take ownership
  void Own() {}
  
  //! returns true if was owned, false otherwise
  bool Disown() {}
  
  //! returns true of owned, false otherwise
  bool Owned() {}
};

*/

//template<class, class> class SmartResource;

/*

XXX: what is the purpose of this class?

template<class ARG_MANAGER, class ARG_POLICY>
class MagicWrapper
{
  typedef Magic
  <
    SmartResource<ARG_MANAGER, ARG_POLICY>,
    List<ARG_POLICY, List<ARG_MANAGER, NullType> >
  > Result;

  typedef Manager;
  typedef Policy;
};

*/

//template<class, class>
//struct asdf;

template<typename HEAD, typename TAIL>
struct List2
{
  typedef HEAD Head;
  typedef TAIL Tail;
};

template<class MANAGER, class OWNERSHIP>
class SmartResource : 
  public Magic
  <
    SmartResource<MANAGER, OWNERSHIP>,
    List2<OWNERSHIP, List<MANAGER, NullType> >
  >
{
  typedef SmartResource<MANAGER, OWNERSHIP> ThisType;
  
  typedef ::List2<MANAGER, NullType> a1;
  typedef ::List<OWNERSHIP, a1> a2;

  typedef Magic
  <
    SmartResource<MANAGER, OWNERSHIP>,
    a2
  > magic;
  
  typedef typename magic::Policy Ownership;

  typedef typename magic::Next a3;

  //tempxxx: for vc
  //typedef typename a3::Policy Manager;
  
public:    
 
  ~SmartResource() { Discard(); } 

  // XXX: repeat 0..15
  // also this needs to use EfficientPass
  template<typename A1, typename A2, typename A3>
  SmartResource(A1 a1, A2 a2, A3 a3)
  { Manager::Open(a1, a2, a3); }

  //XXX: need parameterized copy constructors
  //1) for other SmartResource<...>'s, this will be identifical
  //   to operator=() methods
  //2) for any other type, T, call Manager::Open(T), if it exists
  //   otherwise call Manager::Release(T)

  template<class SMART_OTHER>
  ThisType & operator=(SMART_OTHER r)
  {
    SmartAssign<OWNERSHIP, SMART_OTHER::OWNERSHIP> sa;
    sa(*this, r);
    MANAGER::ShallowCopy(r);
  }
  
  template<class ARG_MANAGER, class ARG_OWNERSHIP>
  ThisType & operator=(SmartResource<ARG_MANAGER, ARG_OWNERSHIP> const & src) 
  {
    if (this != &src)
    {
      SmartAssign<OWNERSHIP, ARG_OWNERSHIP> sa;
      sa(*this, *src);
    }
  }
  // xxx: this overload won't work for vc, only allows
  // a single declaration of operator=

  // xxx: use efficient pass
  /*
  template<class ANYTHING_EXECPT_SMART_RESOURCE>
  ThisType & operator=(typename ANYTHING_EXECPT_SMART_RESOURCE arg)
  {
    Ownership::Swallow(arg);    
  }*/
  
  ThisType copy()
  { 
    R temp;
    temp.shallowcopy(*this);
    temp.duplicate();
    return temp; 
  };
  
  void Discard()
  { 
    if (!Manager::IsNull() && Ownership::DisOwn())
    {
      Manager::Close();
      Manager::SetNull();
    }
  }
  
  void CopyForWrite()
  {
    ACTUAL & o = *((ACTUAL *)this);
    if (!this.Own())
    {
      Manager m;
      m.DeepCopy(o);
      o.ShallowCopy(m);
    }
  }
  
  void Dispose()
  {
    if (OwnerShip::Disown())
    {
      Dispose();
      SetNull();  
    }
  }
};

#endif
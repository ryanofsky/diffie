#ifndef SmartResource_hpp
#define SmartResource_hpp

#include "policy.hpp"

struct Interim;

// problems:
// what happens when there is an attempt to use
// copy on write with the Access policy?
// Ideally, it would cause a compile time error. 
// Maybe the way to do it is to not define an Own()
// method.

struct SmartResourceTop
{
  typedef ::boost::mpl::null_node Policies;
  typedef ::boost::mpl::null_node Inherits;
  
  struct BiggerThanBool { bool a; bool b; };
  
  template<class ACTUAL, class BASE, class CHAIN>
  struct Policy : WORKAROUND_InheritFrom(BASE)
  {
    // Manager class responsibilities:
    // - Needs to implement all the methods below that don't have default
    //   implementations
    // - Methods that acquire a resource need to call to call
    //   Own()
    // - If you want to allow COW, or to disable changes from a buffer,
    //   you need to do:
    //   if (!Owned()) { DeepCopy(...); Own(); }
    //   cbefore making  modifications in any method

    // By default, a call to Open() will forward to Swallow()
    // This facility is used in the 1-argument constructor.
    // which might need to call either one, depending on the
    // type of the argument. The reason why Open() and Swallow()
    // functionality need to be put into different methods is that
    // a call to SmartPointer::operator=() is allowed to Swallow()
    // but not to Open().
    template<typename T>
    void Open(T & t)
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      self.Swallow(t);  
    }

    void Close();
    
    // must cause IsNull() to be true
    void SetNull();
    
    // implies IsOwnNull()
    bool IsNull();
    void ShallowCopy(ACTUAL & other);
    void DeepCopy(ACTUAL & other);

    // Manager Optional Interface

    // Overrides of this function should return bool
    // instead of BiggerThanBool. The return value
    // doesn't matter. The whole thing is just a hack
    // to make it possible to autodetect whether 
    // a Swallow(T) function exists at compile time.
    template<typename T>
    BiggerThanBool Swallow(T);

    void SetOwnNull()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      self.SetNull();
    }

    void IsOwnNull()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.IsNull();
    }

    // Ownership Interface

    // Called during Close() calls by Manager policy to
    // prevent Close() from being called during construction.
    // Also called by the Move policy for the same purpose.
    void Disown();
    
    // called by Manager policy during Open() calls or other calls
    // causes Close() to be called during destruction
    void Own();
    
    // Called by SmartResource destructor.
    // Also called by Manager interface to facilite COW. If this returns
    // false, the resource needs to be deep copied.
    bool Owns();
    
    // used by MoveResource(). If possible, this method
    // should prevent any SmartResource pointing to the
    // current resource from automatically closing. It 
    // should the resource still needs to be deleted at
    // some point, or false otherwise.
    bool Release();
    
  };
};

template<typename MANAGER, typename OWNERSHIP>
struct SmartResource : public PolicyAssembler
  < SmartResource<MANAGER, OWNERSHIP>,
    ::boost::mpl::list3<SmartResourceTop, MANAGER, OWNERSHIP>
  >
{
  // FIX: This was not neccessary for CW8, VC6, VC7, and GCC,
  // but CW6 seems to need it. Is this a compiler bug or have
  // I done something wrong?

  typedef typename ::boost::mpl::list3<SmartResourceTop, MANAGER, OWNERSHIP> wtf;
  typedef SmartResource<MANAGER, OWNERSHIP> ok;
  typedef ::boost::mpl::null_node p3;
  typedef PolicyAssembler< ok, wtf, p3> FChain;
  typedef typename FChain::Chain TChain;
  typedef typename TChain::item Ownership;
  typedef typename TChain::next::item Manager;
  typedef typename TChain::next::next::item Top;
   
  // By default, allow default constructor. An ownership policy
  // or manager policy can override this by not providing
  // a default constructor of its own.
  SmartResource()
  {}
  
  // copy constructor
  template<typename T, typename U>
  SmartResource(SmartResource<T,U> const & t)
  {
    typedef SmartResource<MANAGER, OWNERSHIP> ThisType;
    Assign(t);
  }

  // assignment (since it so closely resembles copy constructor)
  template<typename T, typename U>
  SmartResource & operator=(SmartResource<T,U> const & t)
  {
    if (&t == this) return *this;
    Assign(t);
    return *this;
  }
  
  template<typename M>
  SmartResource(SmartResource<Interim, M> const & o)
  {
    this->Manager::ShallowCopy(o);
    o.Disown();
  }

  template<typename M>
  SmartResource & operator=(SmartResource<Interim, M> const & o)
  {
    if (&t == this) return *this;
    this->Manager::ShallowCopy(o);
    MoveResource(*static_cast<ACTUAL*>(this), o);
    return *this;
  }
  
  template<typename A1>
  SmartResource(A1 & a1)
  {
    this->Manager::Open(a1);
  }
  
  template<typename A1, typename A2>
  SmartResource(A1 & a1, A1 & a2)
  {
    this->Manager::Open(a1, a2);  
  }
  
  template<typename T>
  SmartResource & operator=(T & t)
  {
    this->Manager::Swallow(t);
  }  
  
  ~SmartResource()
  {
    if (this->Ownership::Owns())
    {
      this->Ownership::Disown();
      this->Manager::Close()
      this->Manager::SetNull();
    }
  }
};

#endif

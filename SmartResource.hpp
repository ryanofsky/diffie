#ifndef SmartResource_hpp
#define SmartResource_hpp

struct SmartResourceTop
{
  typedef ::boost::mpl::null_node Policies;
  typedef ::boost::mpl::null_node Inherits;
  
  struct BiggerThanBool { bool a; bool b; };
  
  template<class ACTUAL, class BASE, class CHAIN>
  struct Policy : WORKAROUND_InheritFrom(BASE)
  {
    // Manager Interface
    void Open(...);
    void Close();
    void SetNull();
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

    void SetNullForDisown()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      self.SetNull();
    }

    void IsNullForDisown()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.IsNull();
    }

    bool IsNullForReopen()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.IsNull();
    }

    void SetNullForReopen()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      self.SetNull();
    }

    void DeepCopyForReopen()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.IsNull();
    }

    void Reopen()
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.Open();
    }

    template<typename A1>
    void Reopen(A1 & a1)
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.Open(a1);
    }

    template<typename A1, typename A2>
    void Reopen(A1 & a1, A2 & a2)
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      return self.Open(a1, a2);
    }

    template<typename T>
    struct CanSwallow
    {
      enum { value = sizeof(((ACTUAL*)0)->Swallow(*(T*)0)) == sizeof(bool) };
    };

    // Ownership Interface
    
    void Disown();

    void Own();

    void Assign



  };
}

template<typename MANAGER, typename OWNERSHIP>
struct SmartResource : public PolicyAssembler
  < SmartResource
    boost::mpl::list3<SmartResourceTop, MANAGER, OWNERSHIP>
  >
{
  typedef Chain::item Ownership;
  typedef Chain::next::item Manager;
  typedef Chain::next::next::item Top;
  
  template<typename A1<
  SmartResource(A1 & a1)
  {

  }



};

#endif
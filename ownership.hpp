#ifndef ownership_hpp
#define ownership_hpp

template<typename, typename>
struct SmartResource;

template<typename R>
void DiscardResource(R & r)
{
  if (r.Owns())
  {
    r.Close();
    r.Disown();  
  }
};

// assumes l is currently null
template<typename LEFT, typename RIGHT>
void MoveResource(LEFT & l, RIGHT & r)
{
  l.ShallowCopy(r);
  if (!r.Release()) l.SetNull();  
};

// assumes l is currently null
template<typename LEFT, typename RIGHT>
void CopyResource(LEFT & l, RIGHT & r)
{
  l.DeepCopy(r);
  l.Own();
};

// Making an assignment through Move() and Interim
// is not generally the same the same as making a 
// direct assignment. The only case where this is true
// is when the right operand is currently owning.
// idea: there might be an automatic way to wrap
// interim around other Ownership policies so
// it will be equivalent to an assignment in all
// cases.
// todo: make a chart and find out

template<typename M, typename O>
SmartResource<M, Interim> Move(SmartResource<M, O> & t)
{
  SmartResource<M, Interim> m;
  MoveResource(m);
  return m;
};

template<typename M, typename O>
SmartResource<M, Interim> Copy(SmartResource<M, O> & t)
{
  SmartResource<M, Interim> m;
  CopyResource(m, t);
  return m;
};

struct Interim
{
  typedef ::boost::mpl::null_node Policies;
  typedef ::boost::mpl::null_node Inherits;
  
  template<typename ACTUAL, typename BASE, typename CHAIN>
  struct Policy
  {
    typedef typename ACTUAL::Manager Manager;
    
    bool Owns()
    {
      return this->Manager::IsNull();
    }
    
    void Disown()
    {
      this->Manager::SetNull();  
    }
    
    void Own()
    {
      // nothing to do here, assumes ownership by default
    }
  
    bool Release()
    {
      this->Manager::SetNull();
      return true;  
    }
    
    template<typename RIGHT>
    void Assign(RIGHT & r)
    {
      ACTUAL & self = *(ACTUAL*)this;
      DiscardResource(self);
      MoveResource(self, r)
    }
  };
};

// One goal of having SmartResource is to eliminate
// the need for reference counted resource management
// as used by boost smartptr. Still, the framework
// should be able to handle counts, so I'm making
// and testing this class to make sure the ownership
// interface is complete. 

// Counters also have very clear ownership semantics
// so they are useful to keep in mind as a model
// of ownership when implementing other parts of the
// rm interface,

// todo: make this into a reluctant counter
struct Counted
{
  typedef ::boost::mpl::null_node Policies;
  typedef ::boost::mpl::null_node Inherits;
  
  template<typename ACTUAL, typename BASE, typename CHAIN>
  struct Policy
  {
    int * counter;
    
    typedef typename ACTUAL::Manager Manager;
    
    // if true, it implies that resource is null.
    // converse is not true.
    bool Owns()
    {
      return counter && *counter == 1;
    }
    
    void Disown()
    {
      if (counter && --*counter == 0)
      {
        delete counter;
        counter = 0;
      }
    }
    
    // returns true if the resource still
    // needs to be closed, false otherwise
    bool Release()
    {
      if (Owns())
      {
        Disown();
        return true;
      }
      else
      {
        if (counter)
        {
          return true;
          counter = 0;
        }
        return false;
      }
    }
        
    void Own()
    {
      counter = new int(1);
    }

    template<typename O>
    void Assign(SmartResource<Counted, O> & o)
    {
      ACTUAL & self = *static_cast<ACTUAL*>(this);
      DiscardResource(self);
      counter = o.counter;
      ShallowCopy(o);
      if (counter) ++*counter;  
    }

    // Never takes ownership away from the
    // instance that has it unless that instance
    // is an has ownership of type Interim
    template<typename O>
    void Assign(O & o)
    {
      ACTUAL & self = *(ACTUAL*)this;
      DiscardResource(self);
      ShallowCopy(o);
    }
  };
};

struct Auto
{
  typedef ::boost::mpl::null_node Policies;
  typedef ::boost::mpl::null_node Inherits;
  
  template<typename ACTUAL, typename BASE, typename CHAIN>
  struct Policy
  {
    typedef typename ACTUAL::Manager Manager;
    
    bool Owns()
    {
      return this->Manager::IsNull();
    }
    
    void Disown()
    {
      this->Manager::SetNull();  
    }
    
    void Own()
    {
      // nothing to do, Auto always owns
    }
    
    template<typename R>
    void Assign(R & r)
    {
      DiscardResource(*(ACTUAL*)this);
      ShallowCopy(o);
      if (!IsNull()) SetNull();
    }
  };
};

struct Access
{
  typedef ::boost::mpl::null_node Policies;
  typedef ::boost::mpl::null_node Inherits;
  
  template<typename ACTUAL, typename BASE, typename CHAIN>
  struct Policy
  {
    typedef typename ACTUAL::Manager Manager;
    
    bool Owns()
    {
      return false;
    }
    
    void Disown()
    {
      // nothing to do, Access never owns
    }
        
    // intentionally not implemented. Not possible with this policy.
    void Own();
    bool Release();
  };
};

#endif
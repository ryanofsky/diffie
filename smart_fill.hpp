// here is a lot of hackery going on, and the only mention of it is in this line
// in smart_resource.hpp:
//
//   class SmartPtr : public Fill<OWNERSHIP, MANAGER>
//
// The fill class inherits from OWNERSHIP, and defines four methods
// of its own: SetNull, IsNull, Owns, Disown. If any of these methods
// are present in the ownership and manager classes, calls will
// be forwarded up to them. Otherwise Fill will emulate their
// functionality on its own.

template<class PARENT>
class EmulateDetection : public PARENT
{
protected:
  // 0 for null, 1 otherwise
  // 1 for own, 0 otherwise

  enum {NULLMASK = 1, OWNMASK = 2};
  
  int flags;
  EmulateDetection() : flags(0) {}
  
  void SetNull() { flags &= ~(int)NULLMASK; }
  bool IsNull()  { return flags & NULLMASK; }
  void Owns()    { return flags & OWNMASK;  }
  void Disown()  { flags &= ~(int)OWNMASK;  }
};

template<class PARENT, class MANAGER, class OWNER>
class Fill : typeswitch<(MANAGER::HasNull | OWNER::HasNull) & (MANAGER::HasOwn & OWNER::HasOwn), PARENT, EmulateDetection<PARENT> >
{
protected:
  typedef Fill<PARENT, MANAGER, OWNER> THIS;
  typedef EmulateDetection<PARENT> EMU;

  typedef typeswitch<MANAGER::HasNull, MANAGER, typeswitch<OWNER::HasNull, OWNER, EMU> > PROXYNULL;
  typedef typeswitch<MANAGER::HasOwn, MANAGER, typeswitch<OWNER::HasOwn, OWNER, EMU> > PROXYOWN;
    
  void SetNull() { PROXYNULL::SetNull();       }
  bool IsNull()  { return PROXYNULL::IsNull(); }
  bool Owns()    { return PROXYOWN::Owns();    }
  void Disown()  { PROXYOWN::Disown();         }
};

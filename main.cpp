#include<iostream>

#include "reluctant_reference.hpp"
#include "smart_pointer.hpp"
#include "smart_resource.hpp"

using namespace std;

class Abc
{
public:
  Abc()
  {
    cout << "Construction" << endl;
  }
  
  ~Abc()
  {
    cout << "Destruction" << endl;
  }
};

template<class MANAGER>
class DumpResource :
  public Magic
  <
    DumpResource<MANAGER>,
    List<MANAGER, NullType>
  >
{
};

void main()
{
//  DumpResource< PointerManager<Abc> > n;
  MagicBrains<DumpResource<PointerManager<Abc> >, List<PointerManager<Abc>, NullType>, EmptyBase>::Spec<NoMagic<EmptyBase> >
  a;

  //MagicBrains<DumpResource<PointerManager<Abc> >, List<PointerManager<Abc>, NullType>, EmptyBase>::Tail<NullType, Unspecified>::sd
 // b;

  
  MagicBrains<DumpResource<PointerManager<Abc> >, List<PointerManager<Abc>, NullType>, EmptyBase>::Tail<NullType, Unspecified>
  c;
  
  //SmartResource<PointerManager<Abc>, ReluctantReferenceCount > a;
  //a = new Abc();
};
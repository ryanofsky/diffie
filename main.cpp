#include<iostream>

#include "reluctant_reference.hpp"
#include "smart_pointer.hpp"
#include "smart_resource.hpp"

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

void main()
{
  //SmartResource<PointerManager<Abc>, ReluctantReference<> > a;
  //a = new Abc();
};
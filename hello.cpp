#include <iostream>
#include <typeinfo>

using namespace std;

struct P1
{
  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public BASE                  
  {                                            
  };                                           
};

struct P2
{
  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public BASE                  
  {                                            
  };                                           
};

int main()
{
  return 0;
}
#ifndef smart_assign_hpp
#define smart_assign_hpp

// external assignment functors
// pro: new conversions can be defined without modifying existing classes
// con: (maybe) is it possible to befriend an unparamaterized class?
// con: this might be way too finegrained
// pro: each specialization really needs to be commented on specifically anyway
// con: but I'm sure when I start implementing other cases, patterns will appear
//      depending on the complexity and how worthwhile it would be to generalize
//      and how much generalization would tax compilers, changes might need to
//      be made.
// I'm avoiding the need for partial template specilization, by parametrizing
// both the class and the function.
// I want to specialize by ownership's, but still paramaterize on manager's
//
// is this a general workaround for lack of partial template specialization?
//
// 
// 
template<class SOURCE_OWNERSHIP, class DESTINATION_OWNERSHIP>
class SmartAssign
{
  template<SOURCE_MANAGER, DESTINATION_MANAGER>
  operator()(SOURCE_OWNERSHIP<SOURCE_MANAGER> const & src, DESTINATION_OWNERSHIP<DESTINATION_MANAGER> & dest)
  {
    COMPILE_TIME_ASSERT(0, NO_ASSIGN_SPECIALIZATION_FOUND_FOR<SOURCE_OWNERSHIP, DESTINATION_OWNERSHIP>());
  }  
};


#endif 
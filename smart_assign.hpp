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

template<class DESTINATION_OWNERSHIP, class SOURCE_OWNERSHIP>
struct SmartAssign
{
  template<class DESTINATION, class SOURCE>
  void operator()(DESTINATION & dest, SOURCE const & src)
  {
    COMPILE_TIME_ASSERT(0, NO_ASSIGN_SPECIALIZATION_FOUND_FOR<DESTINATION, SOURCE>());
  };
};


#endif 
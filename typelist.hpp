#ifndef general_hpp
#define general_hpp

#include <boost/mpl/list.hpp>

struct EmptyType{};

typedef boost::mpl::null_node NullType;

template<typename HEAD, typename TAIL> 
struct List : public boost::mpl::list_node
  < typename BOOST_MPL_AUX_NEXT(TAIL::size)
  , HEAD
  , TAIL
  > 
{};

#endif 
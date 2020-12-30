#ifndef __NAMEOF_HPP__
#define __NAMEOF_HPP__

#include <cctype>

namespace nameof
{
namespace details
{
template<typename T>
struct nemeof_impl;
template<>
struct nemeof_impl<int>
{

};
} // namespace details
} // nameof

#endif

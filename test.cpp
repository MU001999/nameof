#include "nameof.hpp"
#include <iostream>

namespace A
{
namespace B
{
struct C {};
}
}

int main()
{
    std::cout << nameof::nameof<int> << std::endl;
    std::cout << nameof::nameof<std::int32_t> << std::endl;
    std::cout << nameof::nameof<int&> << std::endl;
    std::cout << nameof::nameof<int const &> << std::endl;
    std::cout << nameof::nameof<int (&)[12]> << std::endl;
    std::cout << nameof::nameof<A::B::C> << std::endl;

    using D = A::B::C;
    std::cout << nameof::nameof<D> << std::endl;
}

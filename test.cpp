#include "nameof.hpp"
#include <iostream>

int main()
{
    std::cout << nameof::nameof<int> << std::endl;
    std::cout << nameof::nameof<std::int32_t> << std::endl;
    std::cout << nameof::nameof<int&> << std::endl;
    std::cout << nameof::nameof<int const &> << std::endl;
    std::cout << nameof::nameof<int (&)[12]> << std::endl;
}

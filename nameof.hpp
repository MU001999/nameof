#ifndef __MU001999_NAMEOF_HPP__
#define __MU001999_NAMEOF_HPP__

#include <cctype>
#include <ostream>
#include <type_traits>

namespace nameof
{
namespace details
{
inline constexpr bool support_nameof_type =
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 7 || defined(_MSC_VER) && _MSC_VER >= 1910
true
#else
false
#endif
;

template<std::size_t N>
struct fixed_string
{
    static constexpr std::size_t size = N;
    char data[N + 1]{};

    constexpr fixed_string() {};
    constexpr fixed_string(char chr)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            data[i] = chr;
        }
    }
    template<std::size_t N2>
    constexpr fixed_string(const char(&str)[N2])
    {
        static_assert(N == N2 - 1);
        for (std::size_t i = 0; i < N; ++i)
        {
            data[i] = str[i];
        }
    }
    constexpr fixed_string(const char* begin)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            data[i] = begin[i];
        }
    }

    constexpr char operator[](std::size_t i) const
    {
        return data[i];
    }

    constexpr auto operator+(char chr) const
    {
        return *this + fixed_string<1>(chr);
    }

    template<std::size_t N2>
    constexpr auto operator+(const char(&rhs)[N2]) const
    {
        fixed_string<N + N2 - 1> result;
        for (std::size_t i = 0; i < N; ++i)
        {
            result.data[i] = data[i];
        }
        for (std::size_t i = 0; i < N2 - 1; ++i)
        {
            result.data[i + N] = rhs[i];
        }
        return result;
    }

    template<std::size_t N2>
    constexpr auto operator+(const fixed_string<N2>& rhs) const
    {
        fixed_string<N + N2> result;
        for (std::size_t i = 0; i < N; ++i)
        {
            result.data[i] = data[i];
        }
        for (std::size_t i = 0; i < N2; ++i)
        {
            result.data[i + N] = rhs[i];
        }
        return result;
    }
};
fixed_string()->fixed_string<0>;
fixed_string(char)->fixed_string<1>;
template<std::size_t N> fixed_string(const char(&str)[N])->fixed_string<N - 1>;

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(const char(&lhs)[N1], const fixed_string<N2>& rhs)
{
    fixed_string<N1 + N2 - 1> result;

    for (std::size_t i = 0; i < N1 - 1; ++i)
    {
        result.data[i] = lhs[i];
    }
    for (std::size_t i = 0; i < N2; ++i)
    {
        result.data[i + N1 - 1] = rhs[i];
    }

    return result;
}

template<auto V, std::enable_if_t<std::is_integral_v<decltype(V)>, int> = 0>
constexpr auto to_fixed_string()
{
    if constexpr (V == 0)
    {
        return fixed_string('0');
    }
    else if constexpr (V > 0)
    {
        if constexpr (V / 10 == 0)
        {
            return fixed_string((V % 10) + '0');
        }
        else
        {
            return to_fixed_string<V / 10>() + fixed_string((V % 10) + '0');
        }
    }
    else
    {
        return fixed_string('-') + to_fixed_string<-V>();
    }
}

template<std::size_t N>
inline std::ostream& operator<<(std::ostream& out, const fixed_string<N>& str)
{
    for (std::size_t i = 0; i < str.size; ++i)
    {
        out.put(str[i]);
    }
    return out;
}

template<typename T>
constexpr auto n() noexcept
{
    if constexpr (support_nameof_type)
    {
#if defined(__clang__)
        return fixed_string<sizeof(__PRETTY_FUNCTION__) - 33>(__PRETTY_FUNCTION__ + 31);
#elif defined(__GNUC__)
        return fixed_string<sizeof(__PRETTY_FUNCTION__) - 48>(__PRETTY_FUNCTION__ + 46);
#elif defined(_MSC_VER)
        return fixed_string<sizeof(__FUNCSIG__) - 49>(__FUNCSIG__ + 32);
#endif
    }
    else
    {
        return fixed_string();
    }
}

template<typename T>
struct nameof_impl
{
    static constexpr auto value = n<T>();
};

template<typename T>
struct nameof_impl<T const>
{
    static constexpr auto value = nameof_impl<T>::value + " const";
};
template<typename T>
struct nameof_impl<T volatile>
{
    static constexpr auto value = nameof_impl<T>::value + " volatile";
};

template<typename T>
struct nameof_impl<T*>
{
    static constexpr auto value = "*{" + nameof_impl<T>::value + "}";
};
template<typename T>
struct nameof_impl<T&>
{
    static constexpr auto value = "&{" + nameof_impl<T>::value + "}";
};
template<typename T, std::size_t N>
struct nameof_impl<T[N]>
{
    static constexpr auto value = nameof_impl<T>::value + '[' + to_fixed_string<N>() + ']';
};
} // namespace details

template<typename T>
inline constexpr auto nameof = details::nameof_impl<T>::value;
} // namespace nameof

#endif

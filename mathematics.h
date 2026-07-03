#pragma once
#include <complex>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <type_traits>
#include <vector>

namespace mathematics
{
const double pi = std::acos(-1.0);

template <typename T>
struct is_integer : std::false_type
{
};

template <>
struct is_integer<short> : std::true_type
{
};

template <>
struct is_integer<int> : std::true_type
{
};

template <>
struct is_integer<long> : std::true_type
{
};

template <>
struct is_integer<long long> : std::true_type
{
};

template <>
struct is_integer<unsigned short> : std::true_type
{
};

template <>
struct is_integer<unsigned int> : std::true_type
{
};

template <>
struct is_integer<unsigned long> : std::true_type
{
};

template <>
struct is_integer<unsigned long long> : std::true_type
{
};

template <typename T>
inline constexpr bool is_integer_v = is_integer<T>::value;

template <typename T>
struct is_decimal : std::false_type
{
};

template <>
struct is_decimal<float> : std::true_type
{
};

template <>
struct is_decimal<double> : std::true_type
{
};

template <>
struct is_decimal<long double> : std::true_type
{
};

template <typename T>
inline constexpr bool is_decimal_v = is_decimal<T>::value;

template <typename T>
struct is_negativable : std::false_type
{
};

template <>
struct is_negativable<short> : std::true_type
{
};

template <>
struct is_negativable<int> : std::true_type
{
};

template <>
struct is_negativable<long> : std::true_type
{
};

template <>
struct is_negativable<long long> : std::true_type
{
};

template <>
struct is_negativable<float> : std::true_type
{
};

template <>
struct is_negativable<double> : std::true_type
{
};

template <>
struct is_negativable<long double> : std::true_type
{
};

template <typename T>
inline constexpr bool is_negativable_v = is_negativable<T>::value;

template <typename T>
struct is_real : std::false_type
{
};

template <>
struct is_real<short> : std::true_type
{
};

template <>
struct is_real<int> : std::true_type
{
};

template <>
struct is_real<long> : std::true_type
{
};

template <>
struct is_real<long long> : std::true_type
{
};

template <>
struct is_real<unsigned short> : std::true_type
{
};

template <>
struct is_real<unsigned int> : std::true_type
{
};

template <>
struct is_real<unsigned long> : std::true_type
{
};

template <>
struct is_real<unsigned long long> : std::true_type
{
};

template <>
struct is_real<float> : std::true_type
{
};

template <>
struct is_real<double> : std::true_type
{
};

template <>
struct is_real<long double> : std::true_type
{
};

template <typename T>
inline constexpr bool is_real_v = is_real<T>::value;

template <typename Numeric>
inline constexpr bool abnormal(Numeric value)
{
    if constexpr (is_decimal_v<Numeric>)
    {
        return std::isnan(value) || std::isinf(value);
    }
    else
    {
        return false;
    }
}

template <typename Numeric>
inline bool abnormal(const std::complex<Numeric> &value)
{
    return abnormal(value.real()) || abnormal(value.imag());
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> absolute(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("absolute: NaN or Inf input");
            }
        }
    }
    if (value < Numeric(0))
    {
        return -value;
    }
    return value;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> exact(Numeric value, Numeric epsilon = (is_decimal_v<Numeric> ? std::numeric_limits<Numeric>::epsilon() * Numeric(10) : Numeric(0)))
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("exact: NaN or Inf input");
            }
        }
    }
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(value) < epsilon)
        {
            return Numeric(0);
        }
    }
    return value;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> exact(const std::complex<Numeric> &value, Numeric epsilon = (is_decimal_v<Numeric> ? std::numeric_limits<Numeric>::epsilon() * Numeric(10) : Numeric(0)))
{
    return std::complex<Numeric>(exact<Numeric, Safe>(value.real(), epsilon), exact<Numeric, Safe>(value.imag(), epsilon));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> negative(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("negative: NaN or Inf input");
            }
        }
    }
    return -value;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> negative(const std::complex<Numeric> &value)
{
    return std::complex<Numeric>(negative<Numeric, Safe>(value.real()), negative<Numeric, Safe>(value.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<is_integer_v<Numeric>, Numeric> modulus(Numeric value, Numeric divisor)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value) || abnormal(divisor))
            {
                throw std::domain_error("modulus: NaN or Inf input");
            }
        }
    }
    if (divisor == Numeric(0))
    {
        throw std::domain_error("modulus: division by zero");
    }
    Numeric result = value % divisor;
    if (result < Numeric(0))
    {
        result = result + absolute<Numeric, Safe>(divisor);
    }
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("modulus: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<is_decimal_v<Numeric>, Numeric> modulus(Numeric value, Numeric divisor)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value) || abnormal(divisor))
            {
                throw std::domain_error("modulus: NaN or Inf input");
            }
        }
    }
    if (divisor == Numeric(0))
    {
        throw std::domain_error("modulus: division by zero");
    }
    Numeric result = std::fmod(value, divisor);
    if (result < Numeric(0))
    {
        result = result + absolute<Numeric, Safe>(divisor);
    }
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("modulus: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> floor(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("floor: NaN or Inf input");
            }
        }
    }
    Numeric result = std::floor(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("floor: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> floor(const std::complex<Numeric> &value)
{
    return std::complex<Numeric>(floor<Numeric, Safe>(value.real()), floor<Numeric, Safe>(value.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> ceil(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("ceil: NaN or Inf input");
            }
        }
    }
    Numeric result = std::ceil(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("ceil: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> ceil(const std::complex<Numeric> &value)
{
    return std::complex<Numeric>(ceil<Numeric, Safe>(value.real()), ceil<Numeric, Safe>(value.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> round(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("round: NaN or Inf input");
            }
        }
    }
    Numeric result = std::round(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("round: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> round(const std::complex<Numeric> &value)
{
    return std::complex<Numeric>(round<Numeric, Safe>(value.real()), round<Numeric, Safe>(value.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> sign(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("sign: NaN or Inf input");
            }
        }
    }
    if (value > Numeric(0))
    {
        return Numeric(1);
    }
    if (value < Numeric(0))
    {
        return Numeric(-1);
    }
    return Numeric(0);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> squareRoot(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("squareRoot: NaN or Inf input");
            }
        }
    }
    if (value < Numeric(0))
    {
        throw std::domain_error("squareRoot: negative input");
    }
    Numeric result = std::sqrt(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("squareRoot: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> exponential(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("exponential: NaN or Inf input");
            }
        }
    }
    Numeric result = std::exp(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("exponential: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> logarithm(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("logarithm: NaN or Inf input");
            }
        }
    }
    if (value <= Numeric(0))
    {
        throw std::domain_error("logarithm: non-positive input");
    }
    Numeric result = std::log(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("logarithm: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> logarithm(Numeric base, Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(base) || abnormal(value))
            {
                throw std::domain_error("logarithm: NaN or Inf input");
            }
        }
    }
    if (base <= Numeric(0) || absolute<Numeric, Safe>(base - Numeric(1)) < (is_decimal_v<Numeric> ? std::numeric_limits<Numeric>::epsilon() * Numeric(10) : Numeric(0)))
    {
        throw std::domain_error("logarithm: base must be positive and not 1");
    }
    if (value <= Numeric(0))
    {
        throw std::domain_error("logarithm: value must be positive");
    }
    Numeric result = logarithm<Numeric, Safe>(value) / logarithm<Numeric, Safe>(base);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("logarithm: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> power(Numeric base, Numeric exponent)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(base) || abnormal(exponent))
            {
                throw std::domain_error("power: NaN or Inf input");
            }
        }
    }
    if constexpr (is_decimal_v<Numeric>)
    {
        if (base < Numeric(0) && std::abs(exponent - std::round(exponent)) > std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            throw std::domain_error("power: negative base with non-integer exponent");
        }
        if (absolute<Numeric, Safe>(base) < std::numeric_limits<Numeric>::epsilon() * Numeric(10) && exponent <= Numeric(0))
        {
            throw std::domain_error("power: zero base with non-positive exponent");
        }
    }
    else
    {
        if (base == Numeric(0) && exponent <= Numeric(0))
        {
            throw std::domain_error("power: zero base with non-positive exponent");
        }
    }
    Numeric result = std::pow(base, exponent);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("power: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> sine(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("sine: NaN or Inf input");
            }
        }
    }
    Numeric result = std::sin(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("sine: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> cosine(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("cosine: NaN or Inf input");
            }
        }
    }
    Numeric result = std::cos(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("cosine: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> tangent(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("tangent: NaN or Inf input");
            }
        }
    }
    Numeric result = std::tan(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("tangent: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> hyperbolicSine(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("hyperbolicSine: NaN or Inf input");
            }
        }
    }
    Numeric result = std::sinh(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("hyperbolicSine: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> hyperbolicCosine(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("hyperbolicCosine: NaN or Inf input");
            }
        }
    }
    Numeric result = std::cosh(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("hyperbolicCosine: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> hyperbolicTangent(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("hyperbolicTangent: NaN or Inf input");
            }
        }
    }
    Numeric result = std::tanh(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("hyperbolicTangent: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> arcsine(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("arcsine: NaN or Inf input");
            }
        }
    }
    if (value < Numeric(-1) || value > Numeric(1))
    {
        throw std::domain_error("arcsine: argument out of [-1, 1]");
    }
    Numeric result = std::asin(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("arcsine: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> arccosine(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("arccosine: NaN or Inf input");
            }
        }
    }
    if (value < Numeric(-1) || value > Numeric(1))
    {
        throw std::domain_error("arccosine: argument out of [-1, 1]");
    }
    Numeric result = std::acos(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("arccosine: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> arctangent(Numeric value)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value))
            {
                throw std::domain_error("arctangent: NaN or Inf input");
            }
        }
    }
    Numeric result = std::atan(value);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("arctangent: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> arctangent2(Numeric y, Numeric x)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(y) || abnormal(x))
            {
                throw std::domain_error("arctangent2: NaN or Inf input");
            }
        }
    }
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(y) < std::numeric_limits<Numeric>::epsilon() * Numeric(10) && absolute<Numeric, Safe>(x) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            throw std::domain_error("arctangent2: both arguments zero");
        }
    }
    else
    {
        if (y == Numeric(0) && x == Numeric(0))
        {
            throw std::domain_error("arctangent2: both arguments zero");
        }
    }
    Numeric result = std::atan2(y, x);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("arctangent2: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> add(Numeric left, Numeric right)
{
    Numeric result = left + right;
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("add: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> add(const std::complex<Numeric> &left, const std::complex<Numeric> &right)
{
    return std::complex<Numeric>(add<Numeric, Safe>(left.real(), right.real()), add<Numeric, Safe>(left.imag(), right.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> subtract(Numeric left, Numeric right)
{
    Numeric result = left - right;
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("subtract: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> subtract(const std::complex<Numeric> &left, const std::complex<Numeric> &right)
{
    return std::complex<Numeric>(subtract<Numeric, Safe>(left.real(), right.real()), subtract<Numeric, Safe>(left.imag(), right.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> multiply(Numeric left, Numeric right)
{
    Numeric result = left * right;
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("multiply: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> multiply(const std::complex<Numeric> &left, const std::complex<Numeric> &right)
{
    return std::complex<Numeric>(subtract<Numeric, Safe>(multiply<Numeric, Safe>(left.real(), right.real()), multiply<Numeric, Safe>(left.imag(), right.imag())),
                                 add<Numeric, Safe>(multiply<Numeric, Safe>(left.real(), right.imag()), multiply<Numeric, Safe>(left.imag(), right.real())));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> divide(Numeric numerator, Numeric denominator)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(numerator) || abnormal(denominator))
            {
                throw std::domain_error("divide: NaN or Inf input");
            }
        }
    }
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(denominator) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            throw std::domain_error("divide: division by zero");
        }
    }
    else
    {
        if (denominator == Numeric(0))
        {
            throw std::domain_error("divide: division by zero");
        }
    }
    Numeric result = numerator / denominator;
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("divide: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> divide(const std::complex<Numeric> &numerator, const std::complex<Numeric> &denominator)
{
    Numeric denom = add<Numeric, Safe>(multiply<Numeric, Safe>(denominator.real(), denominator.real()), multiply<Numeric, Safe>(denominator.imag(), denominator.imag()));
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(denom) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            throw std::domain_error("divide: division by zero");
        }
    }
    else
    {
        if (denom == Numeric(0))
        {
            throw std::domain_error("divide: division by zero");
        }
    }
    Numeric scale = divide<Numeric, Safe>(Numeric(1), denom);
    return std::complex<Numeric>(multiply<Numeric, Safe>(add<Numeric, Safe>(multiply<Numeric, Safe>(numerator.real(), denominator.real()), multiply<Numeric, Safe>(numerator.imag(), denominator.imag())), scale),
                                 multiply<Numeric, Safe>(subtract<Numeric, Safe>(multiply<Numeric, Safe>(numerator.imag(), denominator.real()), multiply<Numeric, Safe>(numerator.real(), denominator.imag())), scale));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> dot(Numeric left, Numeric right)
{
    return multiply<Numeric, Safe>(left, right);
}

template <typename Numeric, bool Safe = true>
inline Numeric dot(const std::complex<Numeric> &left, const std::complex<Numeric> &right)
{
    return add<Numeric, Safe>(multiply<Numeric, Safe>(left.real(), right.real()), multiply<Numeric, Safe>(left.imag(), right.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> module(const Numeric &value)
{
    return absolute<Numeric, Safe>(value);
}

template <typename Numeric, bool Safe = true>
inline Numeric module(const std::complex<Numeric> &value)
{
    Numeric maximumValue = std::max(absolute<Numeric, Safe>(value.real()), absolute<Numeric, Safe>(value.imag()));
    if constexpr (is_decimal_v<Numeric>)
    {
        if (maximumValue < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return Numeric(0);
        }
    }
    else
    {
        if (maximumValue < Numeric(1))
        {
            return Numeric(0);
        }
    }
    Numeric scale = divide<Numeric, Safe>(Numeric(1), maximumValue);
    Numeric scaledReal = multiply<Numeric, Safe>(value.real(), scale);
    Numeric scaledImag = multiply<Numeric, Safe>(value.imag(), scale);
    Numeric sumSquares = add<Numeric, Safe>(multiply<Numeric, Safe>(scaledReal, scaledReal), multiply<Numeric, Safe>(scaledImag, scaledImag));
    return multiply<Numeric, Safe>(squareRoot<Numeric, Safe>(sumSquares), maximumValue);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> squareRoot(const std::complex<Numeric> &value)
{
    Numeric magnitude = module<Numeric, Safe>(value);
    Numeric argument = arctangent2<Numeric, Safe>(value.imag(), value.real());
    Numeric sqrtMagnitude = squareRoot<Numeric, Safe>(magnitude);
    Numeric halfArgument = divide<Numeric, Safe>(argument, Numeric(2));
    return std::complex<Numeric>(multiply<Numeric, Safe>(sqrtMagnitude, cosine<Numeric, Safe>(halfArgument)), multiply<Numeric, Safe>(sqrtMagnitude, sine<Numeric, Safe>(halfArgument)));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> exponential(const std::complex<Numeric> &value)
{
    Numeric expReal = exponential<Numeric, Safe>(value.real());
    return std::complex<Numeric>(multiply<Numeric, Safe>(expReal, cosine<Numeric, Safe>(value.imag())), multiply<Numeric, Safe>(expReal, sine<Numeric, Safe>(value.imag())));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> logarithm(const std::complex<Numeric> &value)
{
    return std::complex<Numeric>(logarithm<Numeric, Safe>(module<Numeric, Safe>(value)), arctangent2<Numeric, Safe>(value.imag(), value.real()));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> logarithm(const std::complex<Numeric> &base, const std::complex<Numeric> &value)
{
    return divide<Numeric, Safe>(logarithm<Numeric, Safe>(value), logarithm<Numeric, Safe>(base));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> power(const std::complex<Numeric> &base, const Numeric &exponent)
{
    Numeric magnitude = module<Numeric, Safe>(base);
    Numeric argument = arctangent2<Numeric, Safe>(base.imag(), base.real());
    Numeric newMagnitude = power<Numeric, Safe>(magnitude, exponent);
    Numeric newArgument = multiply<Numeric, Safe>(argument, exponent);
    return std::complex<Numeric>(multiply<Numeric, Safe>(newMagnitude, cosine<Numeric, Safe>(newArgument)), multiply<Numeric, Safe>(newMagnitude, sine<Numeric, Safe>(newArgument)));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> power(const std::complex<Numeric> &base, const std::complex<Numeric> &exponent)
{
    return exponential<Numeric, Safe>(logarithm<Numeric, Safe>(base) * exponent);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> sine(const std::complex<Numeric> &value)
{
    Numeric sinReal = sine<Numeric, Safe>(value.real());
    Numeric cosReal = cosine<Numeric, Safe>(value.real());
    Numeric sinhImag = hyperbolicSine<Numeric, Safe>(value.imag());
    Numeric coshImag = hyperbolicCosine<Numeric, Safe>(value.imag());
    return std::complex<Numeric>(multiply<Numeric, Safe>(sinReal, coshImag), multiply<Numeric, Safe>(cosReal, sinhImag));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> cosine(const std::complex<Numeric> &value)
{
    Numeric cosReal = cosine<Numeric, Safe>(value.real());
    Numeric sinReal = sine<Numeric, Safe>(value.real());
    Numeric sinhImag = hyperbolicSine<Numeric, Safe>(value.imag());
    Numeric coshImag = hyperbolicCosine<Numeric, Safe>(value.imag());
    return std::complex<Numeric>(multiply<Numeric, Safe>(cosReal, coshImag), multiply<Numeric, Safe>(negative<Numeric, Safe>(sinReal), sinhImag));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> tangent(const std::complex<Numeric> &value)
{
    return divide<Numeric, Safe>(sine<Numeric, Safe>(value), cosine<Numeric, Safe>(value));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> hyperbolicSine(const std::complex<Numeric> &value)
{
    Numeric sinhReal = hyperbolicSine<Numeric, Safe>(value.real());
    Numeric coshReal = hyperbolicCosine<Numeric, Safe>(value.real());
    Numeric sinImag = sine<Numeric, Safe>(value.imag());
    Numeric cosImag = cosine<Numeric, Safe>(value.imag());
    return std::complex<Numeric>(multiply<Numeric, Safe>(sinhReal, cosImag), multiply<Numeric, Safe>(coshReal, sinImag));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> hyperbolicCosine(const std::complex<Numeric> &value)
{
    Numeric coshReal = hyperbolicCosine<Numeric, Safe>(value.real());
    Numeric sinhReal = hyperbolicSine<Numeric, Safe>(value.real());
    Numeric cosImag = cosine<Numeric, Safe>(value.imag());
    Numeric sinImag = sine<Numeric, Safe>(value.imag());
    return std::complex<Numeric>(multiply<Numeric, Safe>(coshReal, cosImag), multiply<Numeric, Safe>(sinhReal, sinImag));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> hyperbolicTangent(const std::complex<Numeric> &value)
{
    return divide<Numeric, Safe>(hyperbolicSine<Numeric, Safe>(value), hyperbolicCosine<Numeric, Safe>(value));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> arcsine(const std::complex<Numeric> &value)
{
    std::complex<Numeric> i(Numeric(0), Numeric(1));
    return negative<Numeric, Safe>(i) * logarithm<Numeric, Safe>(add<Numeric, Safe>(multiply<Numeric, Safe>(i, value), squareRoot<Numeric, Safe>(subtract<Numeric, Safe>(std::complex<Numeric>(Numeric(1), Numeric(0)), multiply<Numeric, Safe>(value, value)))));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> arccosine(const std::complex<Numeric> &value)
{
    std::complex<Numeric> i(Numeric(0), Numeric(1));
    return negative<Numeric, Safe>(i) * logarithm<Numeric, Safe>(add<Numeric, Safe>(value, multiply<Numeric, Safe>(i, squareRoot<Numeric, Safe>(subtract<Numeric, Safe>(std::complex<Numeric>(Numeric(1), Numeric(0)), multiply<Numeric, Safe>(value, value))))));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> arctangent(const std::complex<Numeric> &value)
{
    std::complex<Numeric> i(Numeric(0), Numeric(1));
    return multiply<Numeric, Safe>(multiply<Numeric, Safe>(i, Numeric(0.5)), subtract<Numeric, Safe>(logarithm<Numeric, Safe>(subtract<Numeric, Safe>(std::complex<Numeric>(Numeric(1), Numeric(0)), multiply<Numeric, Safe>(i, value))), logarithm<Numeric, Safe>(add<Numeric, Safe>(std::complex<Numeric>(Numeric(1), Numeric(0)), multiply<Numeric, Safe>(i, value)))));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> conjugate(const std::complex<Numeric> &value)
{
    return std::complex<Numeric>(value.real(), negative<Numeric, Safe>(value.imag()));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> inverse(const std::complex<Numeric> &value)
{
    Numeric denominator = add<Numeric, Safe>(multiply<Numeric, Safe>(value.real(), value.real()), multiply<Numeric, Safe>(value.imag(), value.imag()));
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(denominator) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return std::complex<Numeric>(Numeric(1), Numeric(0));
        }
    }
    else
    {
        if (denominator == Numeric(0))
        {
            return std::complex<Numeric>(Numeric(1), Numeric(0));
        }
    }
    Numeric scale = divide<Numeric, Safe>(Numeric(1), denominator);
    return std::complex<Numeric>(multiply<Numeric, Safe>(value.real(), scale), multiply<Numeric, Safe>(negative<Numeric, Safe>(value.imag()), scale));
}

template <typename Numeric, bool Safe = true>
inline Numeric distance(const std::complex<Numeric> &left, const std::complex<Numeric> &right)
{
    return module<Numeric, Safe>(subtract<Numeric, Safe>(left, right));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> distance(Numeric left, Numeric right)
{
    return absolute<Numeric, Safe>(subtract<Numeric, Safe>(left, right));
}

template <typename Numeric, bool Safe = true>
inline Numeric angle(const std::complex<Numeric> &left, const std::complex<Numeric> &right)
{
    Numeric moduleA = module<Numeric, Safe>(left);
    Numeric moduleB = module<Numeric, Safe>(right);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(moduleA) < std::numeric_limits<Numeric>::epsilon() * Numeric(10) || absolute<Numeric, Safe>(moduleB) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return Numeric(0);
        }
    }
    else
    {
        if (moduleA == Numeric(0) || moduleB == Numeric(0))
        {
            return Numeric(0);
        }
    }
    std::complex<Numeric> normalizedA = divide<Numeric, Safe>(left, moduleA);
    std::complex<Numeric> normalizedB = divide<Numeric, Safe>(right, moduleB);
    Numeric dotProduct = absolute<Numeric, Safe>(dot<Numeric, Safe>(normalizedA, normalizedB));
    if (dotProduct > Numeric(1))
    {
        dotProduct = Numeric(1);
    }
    Numeric oneMinusDotSquared = subtract<Numeric, Safe>(Numeric(1), multiply<Numeric, Safe>(dotProduct, dotProduct));
    return multiply<Numeric, Safe>(Numeric(2), arctangent2<Numeric, Safe>(squareRoot<Numeric, Safe>(oneMinusDotSquared), dotProduct));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> project(const std::complex<Numeric> &a, const std::complex<Numeric> &b)
{
    Numeric denom = dot<Numeric, Safe>(b, b);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(denom) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return std::complex<Numeric>(Numeric(0), Numeric(0));
        }
    }
    else
    {
        if (denom == Numeric(0))
        {
            return std::complex<Numeric>(Numeric(0), Numeric(0));
        }
    }
    Numeric scalar = divide<Numeric, Safe>(dot<Numeric, Safe>(a, b), denom);
    return b * scalar;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> reflect(const std::complex<Numeric> &v, const std::complex<Numeric> &normal)
{
    Numeric denom = dot<Numeric, Safe>(normal, normal);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(denom) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return v;
        }
    }
    else
    {
        if (denom == Numeric(0))
        {
            return v;
        }
    }
    Numeric factor = divide<Numeric, Safe>(Numeric(2) * dot<Numeric, Safe>(v, normal), denom);
    return v - normal * factor;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> perpendicular(const std::complex<Numeric> &v)
{
    return std::complex<Numeric>(negative<Numeric, Safe>(v.imag()), v.real());
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> refract(const std::complex<Numeric> &incident, const std::complex<Numeric> &normal, Numeric refractionRatio)
{
    Numeric denom = dot<Numeric, Safe>(normal, normal);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(denom) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return incident;
        }
    }
    else
    {
        if (denom == Numeric(0))
        {
            return incident;
        }
    }
    std::complex<Numeric> unitNormal = normal / squareRoot<Numeric, Safe>(denom);
    Numeric dotProduct = dot<Numeric, Safe>(incident, unitNormal);
    Numeric discriminant = Numeric(1) - refractionRatio * refractionRatio * (Numeric(1) - dotProduct * dotProduct);
    if (discriminant < Numeric(0))
    {
        return reflect<Numeric, Safe>(incident, normal);
    }
    return incident * refractionRatio - unitNormal * (refractionRatio * dotProduct + squareRoot<Numeric, Safe>(discriminant));
}

template <typename Numeric, bool Safe = true>
inline Numeric cross(const std::complex<Numeric> &a, const std::complex<Numeric> &b)
{
    return subtract<Numeric, Safe>(multiply<Numeric, Safe>(a.real(), b.imag()), multiply<Numeric, Safe>(a.imag(), b.real()));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> euler(Numeric angle)
{
    return std::complex<Numeric>(cosine<Numeric, Safe>(angle), sine<Numeric, Safe>(angle));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> eulerIdentity()
{
    return add<Numeric, Safe>(euler<Numeric, Safe>(Numeric(pi)), std::complex<Numeric>(Numeric(1), Numeric(0)));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> deMoivre(Numeric magnitude, Numeric angle, int n)
{
    Numeric newMagnitude = fastPower<Numeric, Safe>(magnitude, n);
    Numeric newAngle = multiply<Numeric, Safe>(angle, Numeric(n));
    return std::complex<Numeric>(multiply<Numeric, Safe>(newMagnitude, cosine<Numeric, Safe>(newAngle)), multiply<Numeric, Safe>(newMagnitude, sine<Numeric, Safe>(newAngle)));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> cis(Numeric angle)
{
    return euler<Numeric, Safe>(angle);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> root(const std::complex<Numeric> &value, int n)
{
    if (n <= 0)
    {
        throw std::domain_error("root: n must be positive");
    }
    Numeric magnitude = module<Numeric, Safe>(value);
    Numeric argument = arctangent2<Numeric, Safe>(value.imag(), value.real());
    Numeric rootMagnitude = power<Numeric, Safe>(magnitude, Numeric(1) / Numeric(n));
    Numeric rootArgument = divide<Numeric, Safe>(argument, Numeric(n));
    return std::complex<Numeric>(multiply<Numeric, Safe>(rootMagnitude, cosine<Numeric, Safe>(rootArgument)), multiply<Numeric, Safe>(rootMagnitude, sine<Numeric, Safe>(rootArgument)));
}

template <typename Numeric, bool Safe = true>
inline std::vector<std::complex<Numeric>> roots(const std::complex<Numeric> &value, int n)
{
    if (n <= 0)
    {
        throw std::domain_error("roots: n must be positive");
    }
    Numeric magnitude = module<Numeric, Safe>(value);
    Numeric argument = arctangent2<Numeric, Safe>(value.imag(), value.real());
    Numeric rootMagnitude = power<Numeric, Safe>(magnitude, Numeric(1) / Numeric(n));
    std::vector<std::complex<Numeric>> result;
    result.reserve(n);
    for (int k = 0; k < n; ++k)
    {
        Numeric rootArgument = divide<Numeric, Safe>(add<Numeric, Safe>(argument, Numeric(2) * Numeric(k) * Numeric(pi)), Numeric(n));
        result.push_back(std::complex<Numeric>(multiply<Numeric, Safe>(rootMagnitude, cosine<Numeric, Safe>(rootArgument)), multiply<Numeric, Safe>(rootMagnitude, sine<Numeric, Safe>(rootArgument))));
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> hyperbolicFormula(Numeric x)
{
    return std::complex<Numeric>(hyperbolicCosine<Numeric, Safe>(x), hyperbolicSine<Numeric, Safe>(x));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> circularToHyperbolic(const std::complex<Numeric> &z)
{
    std::complex<Numeric> i(Numeric(0), Numeric(1));
    return cosine<Numeric, Safe>(multiply<Numeric, Safe>(i, z));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> gudermannian(Numeric x)
{
    return std::complex<Numeric>(Numeric(2) * arctangent<Numeric, Safe>(exponential<Numeric, Safe>(x)) - Numeric(pi) / Numeric(2), Numeric(0));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> inverseGudermannian(Numeric x)
{
    Numeric halfX = divide<Numeric, Safe>(x, Numeric(2));
    Numeric tanHalf = tangent<Numeric, Safe>(add<Numeric, Safe>(Numeric(pi) / Numeric(4), halfX));
    return std::complex<Numeric>(logarithm<Numeric, Safe>(tanHalf), Numeric(0));
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> sinc(const std::complex<Numeric> &z)
{
    Numeric mod = module<Numeric, Safe>(z);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (mod < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return std::complex<Numeric>(Numeric(1), Numeric(0));
        }
    }
    else
    {
        if (mod == Numeric(0))
        {
            return std::complex<Numeric>(Numeric(1), Numeric(0));
        }
    }
    return divide<Numeric, Safe>(sine<Numeric, Safe>(z), z);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> sinhc(const std::complex<Numeric> &z)
{
    Numeric mod = module<Numeric, Safe>(z);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (mod < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return std::complex<Numeric>(Numeric(1), Numeric(0));
        }
    }
    else
    {
        if (mod == Numeric(0))
        {
            return std::complex<Numeric>(Numeric(1), Numeric(0));
        }
    }
    return divide<Numeric, Safe>(hyperbolicSine<Numeric, Safe>(z), z);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> halfAngleSine(Numeric x)
{
    Numeric cosX = cosine<Numeric, Safe>(x);
    return squareRoot<Numeric, Safe>(divide<Numeric, Safe>(subtract<Numeric, Safe>(Numeric(1), cosX), Numeric(2)));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> halfAngleCosine(Numeric x)
{
    Numeric cosX = cosine<Numeric, Safe>(x);
    return squareRoot<Numeric, Safe>(divide<Numeric, Safe>(add<Numeric, Safe>(Numeric(1), cosX), Numeric(2)));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> halfAngleTangent(Numeric x)
{
    Numeric sinX = sine<Numeric, Safe>(x);
    Numeric cosX = cosine<Numeric, Safe>(x);
    return divide<Numeric, Safe>(sinX, add<Numeric, Safe>(Numeric(1), cosX));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> sumToProductSine(Numeric a, Numeric b)
{
    Numeric sum = add<Numeric, Safe>(a, b);
    Numeric diff = subtract<Numeric, Safe>(a, b);
    return Numeric(2) * sine<Numeric, Safe>(divide<Numeric, Safe>(sum, Numeric(2))) * cosine<Numeric, Safe>(divide<Numeric, Safe>(diff, Numeric(2)));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> sumToProductCosine(Numeric a, Numeric b)
{
    Numeric sum = add<Numeric, Safe>(a, b);
    Numeric diff = subtract<Numeric, Safe>(a, b);
    return Numeric(2) * cosine<Numeric, Safe>(divide<Numeric, Safe>(sum, Numeric(2))) * cosine<Numeric, Safe>(divide<Numeric, Safe>(diff, Numeric(2)));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> productToSumSineCosine(Numeric a, Numeric b)
{
    Numeric sum = add<Numeric, Safe>(a, b);
    Numeric diff = subtract<Numeric, Safe>(a, b);
    return divide<Numeric, Safe>(add<Numeric, Safe>(sine<Numeric, Safe>(sum), sine<Numeric, Safe>(diff)), Numeric(2));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> productToSumCosineSine(Numeric a, Numeric b)
{
    Numeric sum = add<Numeric, Safe>(a, b);
    Numeric diff = subtract<Numeric, Safe>(a, b);
    return divide<Numeric, Safe>(subtract<Numeric, Safe>(sine<Numeric, Safe>(sum), sine<Numeric, Safe>(diff)), Numeric(2));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> productToSumCosineCosine(Numeric a, Numeric b)
{
    Numeric sum = add<Numeric, Safe>(a, b);
    Numeric diff = subtract<Numeric, Safe>(a, b);
    return divide<Numeric, Safe>(add<Numeric, Safe>(cosine<Numeric, Safe>(sum), cosine<Numeric, Safe>(diff)), Numeric(2));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> productToSumSineSine(Numeric a, Numeric b)
{
    Numeric diff = subtract<Numeric, Safe>(a, b);
    Numeric sum = add<Numeric, Safe>(a, b);
    return divide<Numeric, Safe>(subtract<Numeric, Safe>(cosine<Numeric, Safe>(diff), cosine<Numeric, Safe>(sum)), Numeric(2));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> powerReductionSineSquared(Numeric x)
{
    return divide<Numeric, Safe>(subtract<Numeric, Safe>(Numeric(1), cosine<Numeric, Safe>(Numeric(2) * x)), Numeric(2));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> powerReductionCosineSquared(Numeric x)
{
    return divide<Numeric, Safe>(add<Numeric, Safe>(Numeric(1), cosine<Numeric, Safe>(Numeric(2) * x)), Numeric(2));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> powerReductionTangentSquared(Numeric x)
{
    Numeric cos2x = cosine<Numeric, Safe>(Numeric(2) * x);
    return divide<Numeric, Safe>(subtract<Numeric, Safe>(Numeric(1), cos2x), add<Numeric, Safe>(Numeric(1), cos2x));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> tripleAngleSine(Numeric x)
{
    Numeric sinX = sine<Numeric, Safe>(x);
    return Numeric(3) * sinX - Numeric(4) * sinX * sinX * sinX;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> tripleAngleCosine(Numeric x)
{
    Numeric cosX = cosine<Numeric, Safe>(x);
    return Numeric(4) * cosX * cosX * cosX - Numeric(3) * cosX;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> lawOfCosines(Numeric a, Numeric b, Numeric angleC)
{
    Numeric a2 = a * a;
    Numeric b2 = b * b;
    Numeric twoab = Numeric(2) * a * b;
    Numeric cosC = cosine<Numeric, Safe>(angleC);
    return squareRoot<Numeric, Safe>(add<Numeric, Safe>(subtract<Numeric, Safe>(add<Numeric, Safe>(a2, b2), multiply<Numeric, Safe>(twoab, cosC)), Numeric(0)));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> lawOfSines(Numeric a, Numeric angleA, Numeric angleB)
{
    Numeric sinA = sine<Numeric, Safe>(angleA);
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(sinA) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            throw std::domain_error("lawOfSines: sin(A) is zero");
        }
    }
    else
    {
        if (sinA == Numeric(0))
        {
            throw std::domain_error("lawOfSines: sin(A) is zero");
        }
    }
    return divide<Numeric, Safe>(multiply<Numeric, Safe>(a, sine<Numeric, Safe>(angleB)), sinA);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorExp(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(1), Numeric(0));
    std::complex<Numeric> term(Numeric(1), Numeric(0));
    for (int n = 1; n <= terms; ++n)
    {
        term = term * z / Numeric(n);
        result = result + term;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorSin(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(0), Numeric(0));
    std::complex<Numeric> power = z;
    Numeric factorial = Numeric(1);
    int sign = 1;
    for (int n = 1; n <= terms; n += 2)
    {
        if (n > 1)
        {
            power = power * z * z;
            factorial = factorial * Numeric(n - 1) * Numeric(n);
        }
        result = result + (sign > 0 ? power / factorial : -power / factorial);
        sign = -sign;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorCos(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(1), Numeric(0));
    std::complex<Numeric> power(Numeric(1), Numeric(0));
    Numeric factorial = Numeric(1);
    int sign = -1;
    for (int n = 2; n <= terms; n += 2)
    {
        power = power * z * z;
        factorial = factorial * Numeric(n - 1) * Numeric(n);
        result = result + (sign > 0 ? power / factorial : -power / factorial);
        sign = -sign;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorLog(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> w = (z - std::complex<Numeric>(Numeric(1), Numeric(0))) / (z + std::complex<Numeric>(Numeric(1), Numeric(0)));
    std::complex<Numeric> result(Numeric(0), Numeric(0));
    std::complex<Numeric> power = w;
    for (int n = 1; n <= terms; n += 2)
    {
        result = result + power / Numeric(n);
        power = power * w * w;
    }
    return Numeric(2) * result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorSqrt(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(1), Numeric(0));
    std::complex<Numeric> term(Numeric(1), Numeric(0));
    std::complex<Numeric> u = z - std::complex<Numeric>(Numeric(1), Numeric(0));
    Numeric coeff = Numeric(1);
    for (int n = 1; n <= terms; ++n)
    {
        coeff = coeff * (Numeric(3) - Numeric(2) * Numeric(n)) / (Numeric(2) * Numeric(n));
        term = term * u;
        result = result + coeff * term;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorArcsin(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(0), Numeric(0));
    std::complex<Numeric> power = z;
    Numeric coeff = Numeric(1);
    for (int n = 0; n < terms; ++n)
    {
        result = result + coeff * power / Numeric(2 * n + 1);
        power = power * z * z;
        coeff = coeff * Numeric(2 * n + 1) / Numeric(2 * n + 2);
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorArctan(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(0), Numeric(0));
    std::complex<Numeric> power = z;
    int sign = 1;
    for (int n = 1; n <= terms; n += 2)
    {
        result = result + (sign > 0 ? power / Numeric(n) : -power / Numeric(n));
        power = power * z * z;
        sign = -sign;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorSinh(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(0), Numeric(0));
    std::complex<Numeric> power = z;
    Numeric factorial = Numeric(1);
    for (int n = 1; n <= terms; n += 2)
    {
        if (n > 1)
        {
            power = power * z * z;
            factorial = factorial * Numeric(n - 1) * Numeric(n);
        }
        result = result + power / factorial;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> taylorCosh(const std::complex<Numeric> &z, int terms = 20)
{
    std::complex<Numeric> result(Numeric(1), Numeric(0));
    std::complex<Numeric> power(Numeric(1), Numeric(0));
    Numeric factorial = Numeric(1);
    for (int n = 2; n <= terms; n += 2)
    {
        power = power * z * z;
        factorial = factorial * Numeric(n - 1) * Numeric(n);
        result = result + power / factorial;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> lagrangeInterpolation(const std::vector<std::complex<Numeric>> &x, const std::vector<std::complex<Numeric>> &y, const std::complex<Numeric> &xi)
{
    if (x.size() != y.size() || x.empty())
    {
        throw std::domain_error("lagrangeInterpolation: invalid input sizes");
    }
    size_t n = x.size();
    std::complex<Numeric> result(Numeric(0), Numeric(0));
    for (size_t i = 0; i < n; ++i)
    {
        std::complex<Numeric> term = y[i];
        for (size_t j = 0; j < n; ++j)
        {
            if (i != j)
            {
                term = term * (xi - x[j]) / (x[i] - x[j]);
            }
        }
        result = result + term;
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> lagrangeInterpolation(const std::vector<Numeric> &x, const std::vector<Numeric> &y, Numeric xi)
{
    std::vector<std::complex<Numeric>> cx, cy;
    cx.reserve(x.size());
    cy.reserve(y.size());
    for (size_t i = 0; i < x.size(); ++i)
    {
        cx.push_back(std::complex<Numeric>(x[i], Numeric(0)));
        cy.push_back(std::complex<Numeric>(y[i], Numeric(0)));
    }
    return lagrangeInterpolation<Numeric, Safe>(cx, cy, std::complex<Numeric>(xi, Numeric(0)));
}

template <typename Numeric, bool Safe = true>
inline std::vector<Numeric> lagrangeCoefficients(const std::vector<Numeric> &x, const std::vector<Numeric> &y)
{
    size_t n = x.size();
    std::vector<Numeric> coeffs(n, Numeric(0));
    for (size_t i = 0; i < n; ++i)
    {
        Numeric term = y[i];
        Numeric denom = Numeric(1);
        for (size_t j = 0; j < n; ++j)
        {
            if (i != j)
            {
                denom = denom * (x[i] - x[j]);
            }
        }
        coeffs[i] = term / denom;
    }
    return coeffs;
}

template <typename Numeric>
inline std::enable_if_t<is_integer_v<Numeric>, int> cycle(Numeric index, int limit)
{
    if (limit < 1)
    {
        throw std::domain_error("cycle: limit must be >= 1");
    }
    int intIndex = static_cast<int>(index);
    return (intIndex % limit + limit) % limit;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> limit(Numeric value, Numeric start, Numeric end)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(start) || abnormal(end))
            {
                return Numeric(0);
            }
            if (abnormal(value))
            {
                return (start + end) / Numeric(2);
            }
        }
    }
    if (value >= start && value <= end)
    {
        return value;
    }
    Numeric range = end - start;
    if constexpr (is_decimal_v<Numeric>)
    {
        if (absolute<Numeric, Safe>(range) < std::numeric_limits<Numeric>::epsilon() * Numeric(10))
        {
            return (start + end) / Numeric(2);
        }
    }
    else
    {
        if (range == Numeric(0))
        {
            return (start + end) / Numeric(2);
        }
    }
    Numeric middle = (start + end) / Numeric(2);
    if (value < start)
    {
        return start + (middle - start) * arctangent<Numeric, Safe>((start - value) / range) * Numeric(2) / Numeric(pi);
    }
    return end - (end - middle) * arctangent<Numeric, Safe>((value - end) / range) * Numeric(2) / Numeric(pi);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> limit(const std::complex<Numeric> &value, Numeric start, Numeric end)
{
    return std::complex<Numeric>(limit<Numeric, Safe>(value.real(), start, end), limit<Numeric, Safe>(value.imag(), start, end));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> smooth(Numeric value, Numeric threshold, Numeric steepness)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(value) || abnormal(threshold) || abnormal(steepness))
            {
                return Numeric(0);
            }
        }
    }
    Numeric argument = steepness * (value - threshold);
    Numeric tanhArgument = std::tanh(argument / Numeric(2));
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(tanhArgument))
            {
                return Numeric(0);
            }
        }
    }
    return (Numeric(1) + tanhArgument) / Numeric(2);
}

template <typename Numeric, bool Safe = true>
inline std::complex<Numeric> smooth(const std::complex<Numeric> &value, Numeric threshold, Numeric steepness)
{
    return std::complex<Numeric>(smooth<Numeric, Safe>(value.real(), threshold, steepness), smooth<Numeric, Safe>(value.imag(), threshold, steepness));
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> fastPower(Numeric base, int exponent)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(base))
            {
                throw std::domain_error("fastPower: NaN or Inf input");
            }
        }
    }
    if (exponent == 0)
    {
        return Numeric(1);
    }
    if (exponent < 0)
    {
        return divide<Numeric, Safe>(Numeric(1), fastPower<Numeric, Safe>(base, -exponent));
    }
    Numeric result = Numeric(1);
    Numeric currentBase = base;
    int exp = exponent;
    while (exp > 0)
    {
        if (exp & 1)
        {
            result = result * currentBase;
        }
        currentBase = currentBase * currentBase;
        exp >>= 1;
    }
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("fastPower: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> gaussian(Numeric x, Numeric mean, Numeric standardDeviation)
{
    if (standardDeviation <= Numeric(0))
    {
        throw std::domain_error("gaussian: standard deviation must be positive");
    }
    Numeric diff = (x - mean) / standardDeviation;
    Numeric exponent = Numeric(-0.5) * diff * diff;
    Numeric result = std::exp(exponent) / (standardDeviation * std::sqrt(Numeric(2) * Numeric(pi)));
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("gaussian: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> uniformDistribution(Numeric x, Numeric a, Numeric b)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(x) || abnormal(a) || abnormal(b))
            {
                throw std::domain_error("uniformDistribution: NaN or Inf input");
            }
        }
    }
    if (a >= b)
    {
        throw std::domain_error("uniformDistribution: a must be less than b");
    }
    if (x < a || x > b)
    {
        return Numeric(0);
    }
    return Numeric(1) / (b - a);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> exponentialDistribution(Numeric x, Numeric lambda)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(x) || abnormal(lambda))
            {
                throw std::domain_error("exponentialDistribution: NaN or Inf input");
            }
        }
    }
    if (lambda <= Numeric(0))
    {
        throw std::domain_error("exponentialDistribution: lambda must be positive");
    }
    if (x < Numeric(0))
    {
        return Numeric(0);
    }
    Numeric result = lambda * std::exp(-lambda * x);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("exponentialDistribution: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> poissonDistribution(int k, Numeric lambda)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(lambda))
            {
                throw std::domain_error("poissonDistribution: NaN or Inf input");
            }
        }
    }
    if (lambda <= Numeric(0) || k < 0)
    {
        return Numeric(0);
    }
    Numeric logResult = k * std::log(lambda) - lambda - std::lgamma(k + 1);
    Numeric result = std::exp(logResult);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("poissonDistribution: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, Numeric> binomialDistribution(int k, int n, Numeric p)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(p))
            {
                throw std::domain_error("binomialDistribution: NaN or Inf input");
            }
        }
    }
    if (k < 0 || k > n || p < Numeric(0) || p > Numeric(1))
    {
        return Numeric(0);
    }
    Numeric logResult = std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1) + k * std::log(p) + (n - k) * std::log(Numeric(1) - p);
    Numeric result = std::exp(logResult);
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(result))
            {
                throw std::overflow_error("binomialDistribution: result overflow");
            }
        }
    }
    return exact<Numeric, Safe>(result);
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<is_integer_v<Numeric>, Numeric> greatestCommonDivisor(Numeric a, Numeric b)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(a) || abnormal(b))
            {
                throw std::domain_error("greatestCommonDivisor: NaN or Inf input");
            }
        }
    }
    a = absolute<Numeric, Safe>(a);
    b = absolute<Numeric, Safe>(b);
    while (b != Numeric(0))
    {
        Numeric temp = b;
        b = modulus<Numeric, Safe>(a, b);
        a = temp;
    }
    return a;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<is_integer_v<Numeric>, Numeric> extendedGreatestCommonDivisor(Numeric a, Numeric b, Numeric &x, Numeric &y)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(a) || abnormal(b))
            {
                throw std::domain_error("extendedGreatestCommonDivisor: NaN or Inf input");
            }
        }
    }
    if (b == Numeric(0))
    {
        x = Numeric(1);
        y = Numeric(0);
        return a;
    }
    Numeric x1 = Numeric(0);
    Numeric y1 = Numeric(0);
    Numeric gcd = extendedGreatestCommonDivisor<Numeric, Safe>(b, modulus<Numeric, Safe>(a, b), x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<is_integer_v<Numeric>, Numeric> modularExponentiation(Numeric base, Numeric exponent, Numeric modulusValue)
{
    if constexpr (Safe)
    {
        if constexpr (is_decimal_v<Numeric>)
        {
            if (abnormal(base) || abnormal(exponent) || abnormal(modulusValue))
            {
                throw std::domain_error("modularExponentiation: NaN or Inf input");
            }
        }
    }
    if (modulusValue <= Numeric(0))
    {
        throw std::domain_error("modularExponentiation: modulus must be positive");
    }
    Numeric result = Numeric(1);
    base = modulus<Numeric, Safe>(base, modulusValue);
    while (exponent > Numeric(0))
    {
        if (modulus<Numeric, Safe>(exponent, Numeric(2)) == Numeric(1))
        {
            result = modulus<Numeric, Safe>(result * base, modulusValue);
        }
        base = modulus<Numeric, Safe>(base * base, modulusValue);
        exponent = exponent / Numeric(2);
    }
    return result;
}

template <typename Numeric, bool Safe = true>
inline std::enable_if_t<std::is_arithmetic_v<Numeric>, bool> isPowerOf(Numeric value, Numeric base)
{
    if (base < Numeric(2))
    {
        return false;
    }
    if (value < Numeric(1))
    {
        return false;
    }
    if (value == Numeric(1))
    {
        return true;
    }
    if (base == Numeric(2))
    {
        if constexpr (is_integer_v<Numeric>)
        {
            long long bits = static_cast<long long>(value);
            return (bits > 0) && ((bits & (bits - 1)) == 0);
        }
        else
        {
            Numeric logResult = logarithm<Numeric, Safe>(value) / logarithm<Numeric, Safe>(Numeric(2));
            return absolute<Numeric, Safe>(logResult - round<Numeric, Safe>(logResult)) <
                   (is_decimal_v<Numeric> ? std::numeric_limits<Numeric>::epsilon() * Numeric(10) : Numeric(0));
        }
    }
    if constexpr (is_integer_v<Numeric>)
    {
        long long baseBits = static_cast<long long>(base);
        if (baseBits > 0 && (baseBits & (baseBits - 1)) == 0)
        {
            long long valueBits = static_cast<long long>(value);
            if (valueBits <= 0)
            {
                return false;
            }
            int basePower = 0;
            long long temp = baseBits;
            while (temp > 1)
            {
                temp >>= 1;
                basePower++;
            }
            int valuePower = 0;
            temp = valueBits;
            while (temp > 1)
            {
                temp >>= 1;
                valuePower++;
            }
            if (basePower == 0)
            {
                return false;
            }
            return (valuePower % basePower == 0) && (valueBits & (valueBits - 1)) == 0;
        }
        Numeric maxPower = divide<Numeric, Safe>(std::numeric_limits<Numeric>::max(), base);
        Numeric power = base;
        while (power <= maxPower && power < value)
        {
            power = multiply<Numeric, Safe>(power, base);
        }
        return power == value;
    }
    else
    {
        Numeric logValue = logarithm<Numeric, Safe>(value);
        Numeric logBase = logarithm<Numeric, Safe>(base);
        Numeric logResult = divide<Numeric, Safe>(logValue, logBase);
        return absolute<Numeric, Safe>(logResult - round<Numeric, Safe>(logResult)) <
               std::numeric_limits<Numeric>::epsilon() * Numeric(10);
    }
}

template <typename Container, typename Numeric, bool Safe = true>
inline Container radix2(Container input, bool inverse = false)
{
    using ComplexType = std::complex<Numeric>;
    size_t size = input.size();
    if (size == 0)
    {
        return input;
    }
    if (!isPowerOf<Numeric, Safe>(static_cast<Numeric>(size), Numeric(2)))
    {
        throw std::domain_error("radix2: input size must be a power of 2");
    }
    size_t j = 0;
    for (size_t i = 1; i < size; i++)
    {
        size_t bit = size >> 1;
        while (j & bit)
        {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j)
        {
            std::swap(input[i], input[j]);
        }
    }
    Numeric angleSign = inverse ? negative<Numeric, Safe>(Numeric(2)) : Numeric(2);
    for (size_t length = 2; length <= size; length <<= 1)
    {
        Numeric angle = divide<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, Numeric(pi)), static_cast<Numeric>(length));
        ComplexType wlen(cosine<Numeric, Safe>(angle), sine<Numeric, Safe>(angle));
        for (size_t i = 0; i < size; i += length)
        {
            ComplexType w(Numeric(1), Numeric(0));
            for (size_t k = 0; k < length / 2; k++)
            {
                ComplexType evenPart = input[i + k];
                ComplexType oddPart = multiply<Numeric, Safe>(input[i + k + length / 2], w);
                input[i + k] = add<Numeric, Safe>(evenPart, oddPart);
                input[i + k + length / 2] = subtract<Numeric, Safe>(evenPart, oddPart);
                w = multiply<Numeric, Safe>(w, wlen);
            }
        }
    }
    if (inverse)
    {
        Numeric scale = divide<Numeric, Safe>(Numeric(1), static_cast<Numeric>(size));
        for (size_t i = 0; i < size; i++)
        {
            input[i] = multiply<Numeric, Safe>(input[i], scale);
        }
    }
    return input;
}

template <typename Container, typename Numeric, bool Safe = true>
inline Container radix3(Container input, bool inverse = false)
{
    using ComplexType = std::complex<Numeric>;
    size_t size = input.size();
    if (size == 0)
    {
        return input;
    }
    if (!isPowerOf<Numeric, Safe>(static_cast<Numeric>(size), Numeric(3)))
    {
        throw std::domain_error("radix3: input size must be a power of 3");
    }
    Numeric angleSign = inverse ? Numeric(2) : negative<Numeric, Safe>(Numeric(2));
    Numeric sqrt3div2 = Numeric(0.86602540378443864676372317075294);
    ComplexType omega0(Numeric(1), Numeric(0));
    ComplexType omega1(Numeric(-0.5), multiply<Numeric, Safe>(angleSign, sqrt3div2));
    ComplexType omega2(Numeric(-0.5), negative<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, sqrt3div2)));
    for (size_t n = 3; n <= size; n = multiply<Numeric, Safe>(static_cast<Numeric>(n), Numeric(3)))
    {
        size_t m = divide<Numeric, Safe>(static_cast<Numeric>(n), Numeric(3));
        for (size_t k = 0; k < size; k += n)
        {
            for (size_t j = 0; j < m; j++)
            {
                ComplexType a0 = input[k + j];
                ComplexType a1 = input[k + j + m];
                ComplexType a2 = input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(2))];
                ComplexType b0 = add<Numeric, Safe>(add<Numeric, Safe>(a0, a1), a2);
                ComplexType b1 = add<Numeric, Safe>(add<Numeric, Safe>(a0, multiply<Numeric, Safe>(a1, omega1)), multiply<Numeric, Safe>(a2, omega2));
                ComplexType b2 = add<Numeric, Safe>(add<Numeric, Safe>(a0, multiply<Numeric, Safe>(a1, omega2)), multiply<Numeric, Safe>(a2, omega1));
                input[k + j] = b0;
                input[k + j + m] = b1;
                input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(2))] = b2;
            }
        }
        if (n < size)
        {
            for (size_t k = 0; k < size; k += n)
            {
                for (size_t j = 0; j < m; j++)
                {
                    for (size_t t = 1; t < 3; t++)
                    {
                        Numeric exponent = divide<Numeric, Safe>(multiply<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, Numeric(2)), multiply<Numeric, Safe>(Numeric(pi), static_cast<Numeric>(j * t))), static_cast<Numeric>(n));
                        ComplexType twiddle(cosine<Numeric, Safe>(exponent), sine<Numeric, Safe>(exponent));
                        size_t index = k + j + multiply<Numeric, Safe>(static_cast<Numeric>(t), static_cast<Numeric>(m));
                        input[index] = multiply<Numeric, Safe>(input[index], twiddle);
                    }
                }
            }
        }
    }
    if (inverse)
    {
        Numeric scale = divide<Numeric, Safe>(Numeric(1), static_cast<Numeric>(size));
        for (size_t i = 0; i < size; i++)
        {
            input[i] = multiply<Numeric, Safe>(input[i], scale);
        }
    }
    return input;
}

template <typename Container, typename Numeric, bool Safe = true>
inline Container radix5(Container input, bool inverse = false)
{
    using ComplexType = std::complex<Numeric>;
    size_t size = input.size();
    if (size == 0)
    {
        return input;
    }
    if (!isPowerOf<Numeric, Safe>(static_cast<Numeric>(size), Numeric(5)))
    {
        throw std::domain_error("radix5: input size must be a power of 5");
    }
    Numeric angleSign = inverse ? Numeric(2) : negative<Numeric, Safe>(Numeric(2));
    Numeric twoPiDiv5 = divide<Numeric, Safe>(multiply<Numeric, Safe>(Numeric(2), Numeric(pi)), Numeric(5));
    Numeric fourPiDiv5 = divide<Numeric, Safe>(multiply<Numeric, Safe>(Numeric(4), Numeric(pi)), Numeric(5));
    Numeric cos72 = cosine<Numeric, Safe>(twoPiDiv5);
    Numeric cos144 = cosine<Numeric, Safe>(fourPiDiv5);
    Numeric sin72 = sine<Numeric, Safe>(twoPiDiv5);
    Numeric sin144 = sine<Numeric, Safe>(fourPiDiv5);
    ComplexType omega0(Numeric(1), Numeric(0));
    ComplexType omega1(cos72, multiply<Numeric, Safe>(angleSign, sin72));
    ComplexType omega2(cos144, multiply<Numeric, Safe>(angleSign, sin144));
    ComplexType omega3(cos144, negative<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, sin144)));
    ComplexType omega4(cos72, negative<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, sin72)));
    for (size_t n = 5; n <= size; n = multiply<Numeric, Safe>(static_cast<Numeric>(n), Numeric(5)))
    {
        size_t m = divide<Numeric, Safe>(static_cast<Numeric>(n), Numeric(5));
        for (size_t k = 0; k < size; k += n)
        {
            for (size_t j = 0; j < m; j++)
            {
                ComplexType a0 = input[k + j];
                ComplexType a1 = input[k + j + m];
                ComplexType a2 = input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(2))];
                ComplexType a3 = input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(3))];
                ComplexType a4 = input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(4))];
                ComplexType b0 = add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(a0, a1), a2), a3), a4);
                ComplexType b1 = add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(a0, multiply<Numeric, Safe>(a1, omega1)), multiply<Numeric, Safe>(a2, omega2)), multiply<Numeric, Safe>(a3, omega3)), multiply<Numeric, Safe>(a4, omega4));
                ComplexType b2 = add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(a0, multiply<Numeric, Safe>(a1, omega2)), multiply<Numeric, Safe>(a2, omega4)), multiply<Numeric, Safe>(a3, omega1)), multiply<Numeric, Safe>(a4, omega3));
                ComplexType b3 = add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(a0, multiply<Numeric, Safe>(a1, omega3)), multiply<Numeric, Safe>(a2, omega1)), multiply<Numeric, Safe>(a3, omega4)), multiply<Numeric, Safe>(a4, omega2));
                ComplexType b4 = add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(add<Numeric, Safe>(a0, multiply<Numeric, Safe>(a1, omega4)), multiply<Numeric, Safe>(a2, omega3)), multiply<Numeric, Safe>(a3, omega2)), multiply<Numeric, Safe>(a4, omega1));
                input[k + j] = b0;
                input[k + j + m] = b1;
                input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(2))] = b2;
                input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(3))] = b3;
                input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(m), Numeric(4))] = b4;
            }
        }
        if (n < size)
        {
            for (size_t k = 0; k < size; k += n)
            {
                for (size_t j = 0; j < m; j++)
                {
                    for (size_t t = 1; t < 5; t++)
                    {
                        Numeric exponent = divide<Numeric, Safe>(multiply<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, Numeric(2)), multiply<Numeric, Safe>(Numeric(pi), static_cast<Numeric>(j * t))), static_cast<Numeric>(n));
                        ComplexType twiddle(cosine<Numeric, Safe>(exponent), sine<Numeric, Safe>(exponent));
                        size_t index = k + j + multiply<Numeric, Safe>(static_cast<Numeric>(t), static_cast<Numeric>(m));
                        input[index] = multiply<Numeric, Safe>(input[index], twiddle);
                    }
                }
            }
        }
    }
    if (inverse)
    {
        Numeric scale = divide<Numeric, Safe>(Numeric(1), static_cast<Numeric>(size));
        for (size_t i = 0; i < size; i++)
        {
            input[i] = multiply<Numeric, Safe>(input[i], scale);
        }
    }
    return input;
}

template <typename Container, typename Numeric, bool Safe = true>
inline Container radix7(Container input, bool inverse = false)
{
    using ComplexType = std::complex<Numeric>;
    size_t size = input.size();
    if (size == 0)
    {
        return input;
    }
    if (!isPowerOf<Numeric, Safe>(static_cast<Numeric>(size), Numeric(7)))
    {
        throw std::domain_error("radix7: input size must be a power of 7");
    }
    Numeric angleSign = inverse ? Numeric(2) : negative<Numeric, Safe>(Numeric(2));
    std::complex<Numeric> omega[7];
    for (size_t t = 0; t < 7; t++)
    {
        Numeric angle = divide<Numeric, Safe>(multiply<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, Numeric(2)), multiply<Numeric, Safe>(Numeric(pi), static_cast<Numeric>(t))), Numeric(7));
        omega[t] = ComplexType(cosine<Numeric, Safe>(angle), sine<Numeric, Safe>(angle));
    }
    for (size_t n = 7; n <= size; n = multiply<Numeric, Safe>(static_cast<Numeric>(n), Numeric(7)))
    {
        size_t m = divide<Numeric, Safe>(static_cast<Numeric>(n), Numeric(7));
        for (size_t k = 0; k < size; k += n)
        {
            for (size_t j = 0; j < m; j++)
            {
                ComplexType a[7];
                for (size_t r = 0; r < 7; r++)
                {
                    a[r] = input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(r), static_cast<Numeric>(m))];
                }
                ComplexType b[7];
                for (size_t s = 0; s < 7; s++)
                {
                    b[s] = ComplexType(Numeric(0), Numeric(0));
                    for (size_t r = 0; r < 7; r++)
                    {
                        b[s] = add<Numeric, Safe>(b[s], multiply<Numeric, Safe>(a[r], omega[modulus<Numeric, Safe>(static_cast<Numeric>(r * s), Numeric(7))]));
                    }
                }
                for (size_t r = 0; r < 7; r++)
                {
                    input[k + j + multiply<Numeric, Safe>(static_cast<Numeric>(r), static_cast<Numeric>(m))] = b[r];
                }
            }
        }
        if (n < size)
        {
            for (size_t k = 0; k < size; k += n)
            {
                for (size_t j = 0; j < m; j++)
                {
                    for (size_t t = 1; t < 7; t++)
                    {
                        Numeric exponent = divide<Numeric, Safe>(multiply<Numeric, Safe>(multiply<Numeric, Safe>(angleSign, Numeric(2)), multiply<Numeric, Safe>(Numeric(pi), static_cast<Numeric>(j * t))), static_cast<Numeric>(n));
                        ComplexType twiddle(cosine<Numeric, Safe>(exponent), sine<Numeric, Safe>(exponent));
                        size_t index = k + j + multiply<Numeric, Safe>(static_cast<Numeric>(t), static_cast<Numeric>(m));
                        input[index] = multiply<Numeric, Safe>(input[index], twiddle);
                    }
                }
            }
        }
    }
    if (inverse)
    {
        Numeric scale = divide<Numeric, Safe>(Numeric(1), static_cast<Numeric>(size));
        for (size_t i = 0; i < size; i++)
        {
            input[i] = multiply<Numeric, Safe>(input[i], scale);
        }
    }
    return input;
}
} // namespace mathematics
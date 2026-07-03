#pragma once
#include <tuple>
#include <cmath>
#include <vector>
#include <complex>
#include <initializer_list>
#include <stdexcept>
#include <limits>
#include <iostream>
#include "mathematics.h"

namespace mathematics
{
template <typename N>
class Complex;

template <typename N>
class Quaternion;

template <typename T>
struct is_complex : std::false_type
{
};

template <typename N>
struct is_complex<Complex<N>> : std::true_type
{
};

template <typename N>
struct is_complex<std::complex<N>> : std::true_type
{
};

template <typename T>
inline constexpr bool is_complex_v = is_complex<T>::value;

template <typename T>
struct is_quaternion : std::false_type
{
};

template <typename N>
struct is_quaternion<Quaternion<N>> : std::true_type
{
};

template <typename T>
inline constexpr bool is_quaternion_v = is_quaternion<T>::value;

template <typename T, typename = void>
struct pick
{
};

template <typename T>
struct pick<T, std::enable_if_t<std::is_arithmetic_v<T>>>
{
    using type = T;
};

template <typename T>
struct pick<T, std::enable_if_t<is_complex_v<T> || is_quaternion_v<T>>>
{
    using type = std::remove_reference_t<decltype(std::declval<T>().getReal())>;
};

template <typename T>
using pick_t = typename pick<T>::type;

template <typename N, bool Safe>
Complex<N> polar(const N &magnitude, const N &angle);

template <typename N>
class Complex
{
  private:
    N real;
    N imag;

  public:
    Complex() : real(N(0)), imag(N(0)) {}

    explicit Complex(const N &valueReal) : real(valueReal), imag(N(0)) {}

    explicit Complex(const N &valueReal, const N &valueImag) : real(valueReal), imag(valueImag) {}

    explicit Complex(const std::initializer_list<N> &list)
    {
        switch (list.size())
        {
        case 0:
            real = N(0);
            imag = N(0);
            break;
        case 1:
            real = *(list.begin());
            imag = N(0);
            break;
        default:
            real = *(list.begin());
            imag = *(list.begin() + 1);
            break;
        }
    }

    Complex(const std::complex<N> &complexValue) : real(complexValue.real()), imag(complexValue.imag()) {}

    operator std::complex<N>() const
    {
        return std::complex<N>(real, imag);
    }

    const N &getReal() const { return real; }
    N &getReal() { return real; }
    const N &getImaginary() const { return imag; }
    N &getImaginary() { return imag; }

    N realPart() const { return real; }
    void realPart(const N &value) { real = value; }
    N imagPart() const { return imag; }
    void imagPart(const N &value) { imag = value; }

    template <bool Safe = true>
    Complex<N> negative() const
    {
        return Complex<N>(::mathematics::negative<N, Safe>(real), ::mathematics::negative<N, Safe>(imag));
    }

    template <bool Safe = true>
    Complex<N> exact(N epsilon = (is_decimal_v<N> ? std::numeric_limits<N>::epsilon() * N(10) : N(0))) const
    {
        return Complex<N>(::mathematics::exact<N, Safe>(real, epsilon), ::mathematics::exact<N, Safe>(imag, epsilon));
    }

    template <bool Safe = true>
    Complex<N> limit(N start, N end) const
    {
        return Complex<N>(::mathematics::limit<N, Safe>(real, start, end), ::mathematics::limit<N, Safe>(imag, start, end));
    }

    template <bool Safe = true>
    Complex<N> smooth(N threshold, N steepness) const
    {
        return Complex<N>(::mathematics::smooth<N, Safe>(real, threshold, steepness), ::mathematics::smooth<N, Safe>(imag, threshold, steepness));
    }

    template <bool Safe = true>
    Complex<N> conjugate() const
    {
        return Complex<N>(real, ::mathematics::negative<N, Safe>(imag));
    }

    template <bool Safe = true>
    Complex<N> inverse() const
    {
        N denominator = ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, real), ::mathematics::multiply<N, Safe>(imag, imag));
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(denominator) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Complex<N>(N(1), N(0));
            }
        }
        else
        {
            if (denominator == N(0))
            {
                return Complex<N>(N(1), N(0));
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), denominator);
        return Complex<N>(::mathematics::multiply<N, Safe>(real, scale), ::mathematics::multiply<N, Safe>(::mathematics::negative<N, Safe>(imag), scale));
    }

    template <bool Safe = true>
    N module() const
    {
        N maximumValue = std::max(::mathematics::absolute<N, Safe>(real), ::mathematics::absolute<N, Safe>(imag));
        if constexpr (is_decimal_v<N>)
        {
            if (maximumValue < std::numeric_limits<N>::epsilon() * N(10))
            {
                return N(0);
            }
        }
        else
        {
            if (maximumValue < N(1))
            {
                return N(0);
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), maximumValue);
        N scaledReal = ::mathematics::multiply<N, Safe>(real, scale);
        N scaledImag = ::mathematics::multiply<N, Safe>(imag, scale);
        N sumSquares = ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(scaledReal, scaledReal), ::mathematics::multiply<N, Safe>(scaledImag, scaledImag));
        return ::mathematics::multiply<N, Safe>(::mathematics::squareRoot<N, Safe>(sumSquares), maximumValue);
    }

    template <bool Safe = true>
    N argument() const
    {
        return ::mathematics::arctangent2<N, Safe>(imag, real);
    }

    template <bool Safe = true>
    Complex<N> normalize() const
    {
        N magnitude = module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Complex<N>(N(1), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return Complex<N>(N(1), N(0));
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), magnitude);
        return Complex<N>(::mathematics::multiply<N, Safe>(real, scale), ::mathematics::multiply<N, Safe>(imag, scale));
    }

    template <bool Safe = true>
    Complex<N> exponential() const
    {
        N expReal = ::mathematics::exponential<N, Safe>(real);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(imag) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Complex<N>(expReal, N(0));
            }
        }
        else
        {
            if (imag == N(0))
            {
                return Complex<N>(expReal, N(0));
            }
        }
        N cosValue = ::mathematics::cosine<N, Safe>(imag);
        N sinValue = ::mathematics::sine<N, Safe>(imag);
        return Complex<N>(::mathematics::multiply<N, Safe>(expReal, cosValue), ::mathematics::multiply<N, Safe>(expReal, sinValue));
    }

    template <bool Safe = true>
    Complex<N> logarithm() const
    {
        N magnitude = module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Complex<N>(N(0), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return Complex<N>(N(0), N(0));
            }
        }
        N logMagnitude = ::mathematics::logarithm<N, Safe>(magnitude);
        N arg = argument<Safe>();
        return Complex<N>(logMagnitude, arg);
    }

    template <bool Safe = true>
    Complex<N> logarithm(const Complex<N> &base) const
    {
        return logarithm<Safe>() * base.template logarithm<Safe>().template inverse<Safe>();
    }

    template <bool Safe = true>
    Complex<N> power(const Complex<N> &exponent) const
    {
        N magnitude = module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                if (::mathematics::absolute<N, Safe>(exponent.real) < std::numeric_limits<N>::epsilon() * N(10) && ::mathematics::absolute<N, Safe>(exponent.imag) < std::numeric_limits<N>::epsilon() * N(10))
                {
                    return Complex<N>(N(1), N(0));
                }
                return Complex<N>(N(0), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                if (exponent.real == N(0) && exponent.imag == N(0))
                {
                    return Complex<N>(N(1), N(0));
                }
                return Complex<N>(N(0), N(0));
            }
        }
        return (logarithm<Safe>() * exponent).template exponential<Safe>();
    }

    template <bool Safe = true>
    Complex<N> power(const N &exponent) const
    {
        return power<Safe>(Complex<N>(exponent, N(0)));
    }

    template <bool Safe = true>
    Complex<N> squareRoot() const
    {
        N magnitude = module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Complex<N>(N(0), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return Complex<N>(N(0), N(0));
            }
        }
        N arg = argument<Safe>();
        N sqrtMagnitude = ::mathematics::squareRoot<N, Safe>(magnitude);
        N halfArg = ::mathematics::divide<N, Safe>(arg, N(2));
        return Complex<N>(::mathematics::multiply<N, Safe>(sqrtMagnitude, ::mathematics::cosine<N, Safe>(halfArg)), ::mathematics::multiply<N, Safe>(sqrtMagnitude, ::mathematics::sine<N, Safe>(halfArg)));
    }

    template <bool Safe = true>
    Complex<N> sine() const
    {
        N sinReal = ::mathematics::sine<N, Safe>(real);
        N cosReal = ::mathematics::cosine<N, Safe>(real);
        N sinhImag = ::mathematics::hyperbolicSine<N, Safe>(imag);
        N coshImag = ::mathematics::hyperbolicCosine<N, Safe>(imag);
        return Complex<N>(::mathematics::multiply<N, Safe>(sinReal, coshImag), ::mathematics::multiply<N, Safe>(cosReal, sinhImag));
    }

    template <bool Safe = true>
    Complex<N> cosine() const
    {
        N cosReal = ::mathematics::cosine<N, Safe>(real);
        N sinReal = ::mathematics::sine<N, Safe>(real);
        N sinhImag = ::mathematics::hyperbolicSine<N, Safe>(imag);
        N coshImag = ::mathematics::hyperbolicCosine<N, Safe>(imag);
        return Complex<N>(::mathematics::multiply<N, Safe>(cosReal, coshImag), ::mathematics::multiply<N, Safe>(::mathematics::negative<N, Safe>(sinReal), sinhImag));
    }

    template <bool Safe = true>
    Complex<N> tangent() const
    {
        return sine<Safe>() / cosine<Safe>();
    }

    template <bool Safe = true>
    Complex<N> arcsine() const
    {
        Complex<N> i(N(0), N(1));
        return ::mathematics::negative<N, Safe>(i) * ((i * (*this) + (Complex<N>(N(1), N(0)) - (*this) * (*this)).template squareRoot<Safe>()).template logarithm<Safe>());
    }

    template <bool Safe = true>
    Complex<N> arccosine() const
    {
        Complex<N> i(N(0), N(1));
        return ::mathematics::negative<N, Safe>(i) * ((*this + i * (Complex<N>(N(1), N(0)) - (*this) * (*this)).template squareRoot<Safe>()).template logarithm<Safe>());
    }

    template <bool Safe = true>
    Complex<N> arctangent() const
    {
        Complex<N> i(N(0), N(1));
        return ::mathematics::multiply<N, Safe>(i, N(0.5)) * ((Complex<N>(N(1), N(0)) - i * (*this)).template logarithm<Safe>() - (Complex<N>(N(1), N(0)) + i * (*this)).template logarithm<Safe>());
    }

    template <bool Safe = true>
    Complex<N> rotate(const N &angle) const
    {
        Complex<N> rotation(::mathematics::cosine<N, Safe>(angle), ::mathematics::sine<N, Safe>(angle));
        return (*this) * rotation;
    }

    template <bool Safe = true>
    Complex<N> rotate(const Complex<N> &rotation) const
    {
        return (*this) * rotation.template normalize<Safe>();
    }

    template <bool Safe = true>
    Complex<N> refract(const Complex<N> &normal, N refractionRatio) const
    {
        Complex<N> unitNormal = normal.template normalize<Safe>();
        Complex<N> unitIncident = this->template normalize<Safe>();
        N dotProduct = unitIncident.template dot<Safe>(unitNormal);
        N discriminant = ::mathematics::subtract<N, Safe>(N(1), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(refractionRatio, refractionRatio), ::mathematics::subtract<N, Safe>(N(1), ::mathematics::multiply<N, Safe>(dotProduct, dotProduct))));
        if (discriminant < N(0))
        {
            return this->template reflect<Safe>(normal);
        }
        N sqrtDiscriminant = ::mathematics::squareRoot<N, Safe>(discriminant);
        Complex<N> refracted = unitIncident * refractionRatio;
        Complex<N> normalComponent = unitNormal * (::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(refractionRatio, dotProduct), sqrtDiscriminant));
        Complex<N> result = refracted - normalComponent;
        N originalMagnitude = this->template module<Safe>();
        return result.template normalize<Safe>() * originalMagnitude;
    }

    template <bool Safe = true>
    N cross(const Complex<N> &other) const
    {
        return ::mathematics::subtract<N, Safe>(::mathematics::multiply<N, Safe>(real, other.imag), ::mathematics::multiply<N, Safe>(imag, other.real));
    }

    template <bool Safe = true>
    N dot(const Complex<N> &other) const
    {
        return ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, other.real), ::mathematics::multiply<N, Safe>(imag, other.imag));
    }

    template <bool Safe = true>
    Complex<N> project(const Complex<N> &other) const
    {
        N denom = other.template dot<Safe>(other);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(denom) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Complex<N>(N(0), N(0));
            }
        }
        else
        {
            if (denom == N(0))
            {
                return Complex<N>(N(0), N(0));
            }
        }
        N scalar = ::mathematics::divide<N, Safe>(this->template dot<Safe>(other), denom);
        return other * scalar;
    }

    template <bool Safe = true>
    Complex<N> reflect(const Complex<N> &normal) const
    {
        N denom = normal.template dot<Safe>(normal);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(denom) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return *this;
            }
        }
        else
        {
            if (denom == N(0))
            {
                return *this;
            }
        }
        N factor = ::mathematics::divide<N, Safe>(::mathematics::multiply<N, Safe>(N(2), this->template dot<Safe>(normal)), denom);
        return Complex<N>(::mathematics::subtract<N, Safe>(real, ::mathematics::multiply<N, Safe>(factor, normal.real)), ::mathematics::subtract<N, Safe>(imag, ::mathematics::multiply<N, Safe>(factor, normal.imag)));
    }

    template <bool Safe = true>
    Complex<N> perpendicular() const
    {
        return Complex<N>(::mathematics::negative<N, Safe>(imag), real);
    }

    template <bool Safe = true>
    bool parallel(const Complex<N> &other) const
    {
        if constexpr (is_decimal_v<N>)
        {
            return ::mathematics::absolute<N, Safe>(cross<Safe>(other)) < std::numeric_limits<N>::epsilon() * N(10);
        }
        else
        {
            return cross<Safe>(other) == N(0);
        }
    }

    template <bool Safe = true>
    Complex<N> lerp(const Complex<N> &other, N t) const
    {
        N oneMinusT = ::mathematics::subtract<N, Safe>(N(1), t);
        return Complex<N>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, oneMinusT), ::mathematics::multiply<N, Safe>(other.real, t)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imag, oneMinusT), ::mathematics::multiply<N, Safe>(other.imag, t)));
    }

    template <bool Safe = true>
    Complex<N> nlerp(const Complex<N> &other, N t) const
    {
        Complex<N> interpolated = lerp<Safe>(other, t);
        N magnitude = interpolated.template module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return *this;
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return *this;
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), magnitude);
        return Complex<N>(::mathematics::multiply<N, Safe>(interpolated.real, scale), ::mathematics::multiply<N, Safe>(interpolated.imag, scale));
    }

    template <bool Safe = true>
    Complex<N> slerp(const Complex<N> &other, N t) const
    {
        Complex<N> normalizedThis = normalize<Safe>();
        Complex<N> normalizedOther = other.template normalize<Safe>();
        N dotProduct = ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(normalizedThis.real, normalizedOther.real), ::mathematics::multiply<N, Safe>(normalizedThis.imag, normalizedOther.imag));
        Complex<N> target = normalizedOther;
        if (dotProduct < N(0))
        {
            dotProduct = ::mathematics::negative<N, Safe>(dotProduct);
            target = target.template negative<Safe>();
        }
        if (dotProduct > N(1))
        {
            dotProduct = N(1);
        }
        N theta = ::mathematics::arccosine<N, Safe>(dotProduct);
        N sinTheta = ::mathematics::sine<N, Safe>(theta);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(sinTheta) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return lerp<Safe>(target, t);
            }
        }
        else
        {
            if (sinTheta == N(0))
            {
                return lerp<Safe>(target, t);
            }
        }
        N factorA = ::mathematics::divide<N, Safe>(::mathematics::sine<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::subtract<N, Safe>(N(1), t), theta)), sinTheta);
        N factorB = ::mathematics::divide<N, Safe>(::mathematics::sine<N, Safe>(::mathematics::multiply<N, Safe>(t, theta)), sinTheta);
        return Complex<N>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(normalizedThis.real, factorA), ::mathematics::multiply<N, Safe>(target.real, factorB)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(normalizedThis.imag, factorA), ::mathematics::multiply<N, Safe>(target.imag, factorB)));
    }

    template <bool Safe = true>
    N distance(const Complex<N> &other) const
    {
        Complex<N> diff = (*this) - other;
        return diff.template module<Safe>();
    }

    template <bool Safe = true>
    N angle(const Complex<N> &other) const
    {
        Complex<N> normalizedThis = normalize<Safe>();
        Complex<N> normalizedOther = other.template normalize<Safe>();
        N dotProduct = ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(normalizedThis.real, normalizedOther.real), ::mathematics::multiply<N, Safe>(normalizedThis.imag, normalizedOther.imag));
        if (dotProduct > N(1))
        {
            dotProduct = N(1);
        }
        if (dotProduct < N(-1))
        {
            dotProduct = N(-1);
        }
        return ::mathematics::arccosine<N, Safe>(dotProduct);
    }

    template <typename T = N, bool Safe = true>
    std::vector<Complex<N>> head(const T &step) const
    {
        int count = 0;
        N angleStep = N(0);
        N fullCircle = N(2) * N(::mathematics::pi);
        if constexpr (is_integer_v<T>)
        {
            count = static_cast<int>(step);
            if (count < 1)
                return {};
            angleStep = ::mathematics::divide<N, Safe>(fullCircle, static_cast<N>(count));
        }
        else if constexpr (std::is_arithmetic_v<T>)
        {
            if constexpr (is_decimal_v<N>)
            {
                if (::mathematics::absolute<N, Safe>(step) < std::numeric_limits<N>::epsilon() * N(10))
                    return {*this};
            }
            else
            {
                if (step == N(0))
                    return {*this};
            }
            angleStep = step;
            N ratio = ::mathematics::divide<N, Safe>(fullCircle, ::mathematics::absolute<N, Safe>(angleStep));
            count = static_cast<int>(::mathematics::round<N, Safe>(ratio));
            if (count < 1)
                count = 1;
        }
        else if constexpr (is_complex_v<T>)
        {
            return head<N, Safe>(step.template argument<Safe>());
        }
        else
        {
            return head<N, Safe>(step.template module<Safe>());
        }
        std::vector<Complex<N>> result;
        result.reserve(static_cast<size_t>(count));
        N moduleVal = module<Safe>();
        N baseAngle = argument<Safe>();
        for (int i = 0; i < count; i++)
        {
            N angle = ::mathematics::add<N, Safe>(baseAngle, ::mathematics::multiply<N, Safe>(angleStep, static_cast<N>(i)));
            result.push_back(polar<N, Safe>(moduleVal, angle));
        }
        return result;
    }

    template <typename T = N, bool Safe = true>
    std::vector<Complex<N>> tail(const T &step) const
    {
        auto heads = head<T, Safe>(step);
        std::vector<Complex<N>> result;
        result.reserve(heads.size());
        N endpointReal = getReal();
        N endpointImag = getImaginary();
        for (auto &h : heads)
        {
            result.push_back(Complex<N>(
                ::mathematics::add<N, Safe>(endpointReal, ::mathematics::negative<N, Safe>(h.getReal())),
                ::mathematics::add<N, Safe>(endpointImag, ::mathematics::negative<N, Safe>(h.getImaginary()))));
        }
        return result;
    }

    std::tuple<std::complex<N>, std::complex<N>> toComplex() const
    {
        return {std::complex<N>(real, imag), std::complex<N>(real, ::mathematics::negative<N, false>(imag))};
    }

    Complex<N> &operator=(const Complex<N> &) = default;

    Complex<N> operator+(const Complex<N> &other) const
    {
        return Complex<N>(::mathematics::add<N, false>(real, other.real), ::mathematics::add<N, false>(imag, other.imag));
    }

    Complex<N> operator-(const Complex<N> &other) const
    {
        return Complex<N>(::mathematics::subtract<N, false>(real, other.real), ::mathematics::subtract<N, false>(imag, other.imag));
    }

    Complex<N> operator*(const Complex<N> &other) const
    {
        return Complex<N>(::mathematics::subtract<N, false>(::mathematics::multiply<N, false>(real, other.real), ::mathematics::multiply<N, false>(imag, other.imag)), ::mathematics::add<N, false>(::mathematics::multiply<N, false>(real, other.imag), ::mathematics::multiply<N, false>(imag, other.real)));
    }

    Complex<N> operator/(const Complex<N> &other) const
    {
        return (*this) * other.template inverse<false>();
    }

    Complex<N> operator*(const N &scalar) const
    {
        return Complex<N>(::mathematics::multiply<N, false>(real, scalar), ::mathematics::multiply<N, false>(imag, scalar));
    }

    Complex<N> operator/(const N &scalar) const
    {
        return Complex<N>(::mathematics::divide<N, false>(real, scalar), ::mathematics::divide<N, false>(imag, scalar));
    }

    Complex<N> &operator+=(const Complex<N> &other)
    {
        real = ::mathematics::add<N, false>(real, other.real);
        imag = ::mathematics::add<N, false>(imag, other.imag);
        return *this;
    }

    Complex<N> &operator-=(const Complex<N> &other)
    {
        real = ::mathematics::subtract<N, false>(real, other.real);
        imag = ::mathematics::subtract<N, false>(imag, other.imag);
        return *this;
    }

    Complex<N> &operator*=(const Complex<N> &other)
    {
        *this = *this * other;
        return *this;
    }

    Complex<N> &operator/=(const Complex<N> &other)
    {
        *this = *this / other;
        return *this;
    }

    Complex<N> &operator*=(const N &scalar)
    {
        real = ::mathematics::multiply<N, false>(real, scalar);
        imag = ::mathematics::multiply<N, false>(imag, scalar);
        return *this;
    }

    Complex<N> &operator/=(const N &scalar)
    {
        real = ::mathematics::divide<N, false>(real, scalar);
        imag = ::mathematics::divide<N, false>(imag, scalar);
        return *this;
    }

    Complex<N> operator-() const
    {
        return Complex<N>(::mathematics::negative<N, false>(real), ::mathematics::negative<N, false>(imag));
    }

    bool operator==(const Complex<N> &other) const
    {
        if constexpr (is_decimal_v<N>)
        {
            return ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(real, other.real)) < std::numeric_limits<N>::epsilon() * N(10) && ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(imag, other.imag)) < std::numeric_limits<N>::epsilon() * N(10);
        }
        else
        {
            return real == other.real && imag == other.imag;
        }
    }

    bool operator!=(const Complex<N> &other) const { return !(*this == other); }

    bool operator<(const Complex<N> &other) const
    {
        N magnitude1 = module<false>();
        N magnitude2 = other.template module<false>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(magnitude1, magnitude2)) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return magnitude1 < magnitude2;
            }
            if (::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(real, other.real)) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return real < other.real;
            }
        }
        else
        {
            if (magnitude1 != magnitude2)
            {
                return magnitude1 < magnitude2;
            }
            if (real != other.real)
            {
                return real < other.real;
            }
        }
        return imag < other.imag;
    }

    bool operator<=(const Complex<N> &other) const { return (*this < other) || (*this == other); }
    bool operator>(const Complex<N> &other) const { return !(*this <= other); }
    bool operator>=(const Complex<N> &other) const { return !(*this < other); }

    N &operator[](std::size_t index)
    {
        switch (index)
        {
        case 0:
            return real;
        case 1:
            return imag;
        default:
            throw std::out_of_range("Index out of range");
        }
    }

    const N &operator[](std::size_t index) const
    {
        switch (index)
        {
        case 0:
            return real;
        case 1:
            return imag;
        default:
            throw std::out_of_range("Index out of range");
        }
    }

    friend Complex<N> operator*(const N &scalar, const Complex<N> &complexValue) { return complexValue * scalar; }
    friend Complex<N> operator/(const N &scalar, const Complex<N> &complexValue) { return Complex<N>(scalar, N(0)) / complexValue; }
    friend Complex<N> operator+(const N &scalar, const Complex<N> &complexValue) { return Complex<N>(::mathematics::add<N, false>(scalar, complexValue.real), complexValue.imag); }
    friend Complex<N> operator-(const N &scalar, const Complex<N> &complexValue) { return Complex<N>(::mathematics::subtract<N, false>(scalar, complexValue.real), ::mathematics::negative<N, false>(complexValue.imag)); }

    friend std::ostream &operator<<(std::ostream &output, const Complex<N> &complexValue)
    {
        output << "(" << complexValue.real;
        if (complexValue.imag >= N(0))
        {
            output << " + " << complexValue.imag << "i)";
        }
        else
        {
            output << " - " << ::mathematics::absolute<N, false>(complexValue.imag) << "i)";
        }
        return output;
    }
};

template <typename N, bool Safe = true>
Complex<N> polar(const N &magnitude, const N &angle)
{
    return Complex<N>(::mathematics::multiply<N, Safe>(magnitude, ::mathematics::cosine<N, Safe>(angle)), ::mathematics::multiply<N, Safe>(magnitude, ::mathematics::sine<N, Safe>(angle)));
}

template <typename N>
class Quaternion
{
  private:
    N real;
    N imagI;
    N imagJ;
    N imagK;

  public:
    Quaternion() : real(N(1)), imagI(N(0)), imagJ(N(0)), imagK(N(0)) {}

    explicit Quaternion(const N &valueReal) : real(valueReal), imagI(N(0)), imagJ(N(0)), imagK(N(0)) {}

    explicit Quaternion(const N &valueReal, const N &valueImagI) : real(valueReal), imagI(valueImagI), imagJ(N(0)), imagK(N(0)) {}

    explicit Quaternion(const N &valueReal, const N &valueImagI, const N &valueImagJ) : real(valueReal), imagI(valueImagI), imagJ(valueImagJ), imagK(N(0)) {}

    explicit Quaternion(const N &valueReal, const N &valueImagI, const N &valueImagJ, const N &valueImagK) : real(valueReal), imagI(valueImagI), imagJ(valueImagJ), imagK(valueImagK) {}

    explicit Quaternion(const std::initializer_list<N> &list)
    {
        switch (list.size())
        {
        case 0:
            real = N(1);
            imagI = N(0);
            imagJ = N(0);
            imagK = N(0);
            break;
        case 1:
            real = *(list.begin());
            imagI = N(0);
            imagJ = N(0);
            imagK = N(0);
            break;
        case 2:
            real = *(list.begin());
            imagI = *(list.begin() + 1);
            imagJ = N(0);
            imagK = N(0);
            break;
        case 3:
            real = *(list.begin());
            imagI = *(list.begin() + 1);
            imagJ = *(list.begin() + 2);
            imagK = N(0);
            break;
        default:
            real = *(list.begin());
            imagI = *(list.begin() + 1);
            imagJ = *(list.begin() + 2);
            imagK = *(list.begin() + 3);
            break;
        }
    }

    const N &getReal() const { return real; }
    N &getReal() { return real; }
    const N &getImaginaryI() const { return imagI; }
    N &getImaginaryI() { return imagI; }
    const N &getImaginaryJ() const { return imagJ; }
    N &getImaginaryJ() { return imagJ; }
    const N &getImaginaryK() const { return imagK; }
    N &getImaginaryK() { return imagK; }

    N scalar() const { return real; }
    Quaternion<N> vector() const { return Quaternion<N>(N(0), imagI, imagJ, imagK); }

    template <bool Safe = true>
    Quaternion<N> negative() const
    {
        return Quaternion<N>(::mathematics::negative<N, Safe>(real), ::mathematics::negative<N, Safe>(imagI), ::mathematics::negative<N, Safe>(imagJ), ::mathematics::negative<N, Safe>(imagK));
    }

    template <bool Safe = true>
    Quaternion<N> exact(N epsilon = (is_decimal_v<N> ? std::numeric_limits<N>::epsilon() * N(10) : N(0))) const
    {
        return Quaternion<N>(::mathematics::exact<N, Safe>(real, epsilon), ::mathematics::exact<N, Safe>(imagI, epsilon), ::mathematics::exact<N, Safe>(imagJ, epsilon), ::mathematics::exact<N, Safe>(imagK, epsilon));
    }

    template <bool Safe = true>
    Quaternion<N> cross(const Quaternion<N> &other) const
    {
        return Quaternion<N>(N(0), ::mathematics::subtract<N, Safe>(::mathematics::multiply<N, Safe>(imagJ, other.imagK), ::mathematics::multiply<N, Safe>(imagK, other.imagJ)), ::mathematics::subtract<N, Safe>(::mathematics::multiply<N, Safe>(imagK, other.imagI), ::mathematics::multiply<N, Safe>(imagI, other.imagK)), ::mathematics::subtract<N, Safe>(::mathematics::multiply<N, Safe>(imagI, other.imagJ), ::mathematics::multiply<N, Safe>(imagJ, other.imagI)));
    }

    template <bool Safe = true>
    N module() const
    {
        N maximumValue = std::max({::mathematics::absolute<N, Safe>(real), ::mathematics::absolute<N, Safe>(imagI), ::mathematics::absolute<N, Safe>(imagJ), ::mathematics::absolute<N, Safe>(imagK)});
        if constexpr (is_decimal_v<N>)
        {
            if (maximumValue < std::numeric_limits<N>::epsilon() * N(10))
            {
                return N(0);
            }
        }
        else
        {
            if (maximumValue < N(1))
            {
                return N(0);
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), maximumValue);
        N scaledReal = ::mathematics::multiply<N, Safe>(real, scale);
        N scaledImagI = ::mathematics::multiply<N, Safe>(imagI, scale);
        N scaledImagJ = ::mathematics::multiply<N, Safe>(imagJ, scale);
        N scaledImagK = ::mathematics::multiply<N, Safe>(imagK, scale);
        N sumSquares = ::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(scaledReal, scaledReal), ::mathematics::multiply<N, Safe>(scaledImagI, scaledImagI)), ::mathematics::multiply<N, Safe>(scaledImagJ, scaledImagJ)), ::mathematics::multiply<N, Safe>(scaledImagK, scaledImagK));
        return ::mathematics::multiply<N, Safe>(::mathematics::squareRoot<N, Safe>(sumSquares), maximumValue);
    }

    template <bool Safe = true>
    Quaternion<N> pure() const
    {
        N imaginaryNorm = ::mathematics::squareRoot<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagI, imagI), ::mathematics::multiply<N, Safe>(imagJ, imagJ)), ::mathematics::multiply<N, Safe>(imagK, imagK)));
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(imaginaryNorm) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(1));
            }
        }
        else
        {
            if (imaginaryNorm == N(0))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(1));
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), imaginaryNorm);
        return Quaternion<N>(N(0), ::mathematics::multiply<N, Safe>(imagI, scale), ::mathematics::multiply<N, Safe>(imagJ, scale), ::mathematics::multiply<N, Safe>(imagK, scale));
    }

    template <bool Safe = true>
    Quaternion<N> conjugate() const
    {
        return Quaternion<N>(real, ::mathematics::negative<N, Safe>(imagI), ::mathematics::negative<N, Safe>(imagJ), ::mathematics::negative<N, Safe>(imagK));
    }

    template <bool Safe = true>
    Quaternion<N> inverse() const
    {
        N magnitudeSquared = ::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, real), ::mathematics::multiply<N, Safe>(imagI, imagI)), ::mathematics::multiply<N, Safe>(imagJ, imagJ)), ::mathematics::multiply<N, Safe>(imagK, imagK));
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitudeSquared) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(1), N(0), N(0), N(0));
            }
        }
        else
        {
            if (magnitudeSquared == N(0))
            {
                return Quaternion<N>(N(1), N(0), N(0), N(0));
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), magnitudeSquared);
        return Quaternion<N>(::mathematics::multiply<N, Safe>(real, scale), ::mathematics::multiply<N, Safe>(::mathematics::negative<N, Safe>(imagI), scale), ::mathematics::multiply<N, Safe>(::mathematics::negative<N, Safe>(imagJ), scale), ::mathematics::multiply<N, Safe>(::mathematics::negative<N, Safe>(imagK), scale));
    }

    template <bool Safe = true>
    Quaternion<N> normalize() const
    {
        N magnitude = this->module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(1), N(0), N(0), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return Quaternion<N>(N(1), N(0), N(0), N(0));
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), magnitude);
        return Quaternion<N>(::mathematics::multiply<N, Safe>(real, scale), ::mathematics::multiply<N, Safe>(imagI, scale), ::mathematics::multiply<N, Safe>(imagJ, scale), ::mathematics::multiply<N, Safe>(imagK, scale));
    }

    template <bool Safe = true>
    Quaternion<N> squareRoot() const
    {
        N magnitude = this->module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        N denominator = ::mathematics::add<N, Safe>(magnitude, ::mathematics::absolute<N, Safe>(real));
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(denominator) < std::numeric_limits<N>::epsilon() * N(10))
            {
                N sqrtHalf = ::mathematics::squareRoot<N, Safe>(N(0.5));
                return Quaternion<N>(N(0), ::mathematics::multiply<N, Safe>(sqrtHalf, imagI), ::mathematics::multiply<N, Safe>(sqrtHalf, imagJ), ::mathematics::multiply<N, Safe>(sqrtHalf, imagK));
            }
        }
        else
        {
            if (denominator == N(0))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        N factor = ::mathematics::divide<N, Safe>(N(1), ::mathematics::squareRoot<N, Safe>(::mathematics::multiply<N, Safe>(N(2), denominator)));
        N newReal = ::mathematics::multiply<N, Safe>(::mathematics::squareRoot<N, Safe>(denominator), ::mathematics::squareRoot<N, Safe>(N(0.5)));
        return Quaternion<N>(newReal, ::mathematics::multiply<N, Safe>(imagI, factor), ::mathematics::multiply<N, Safe>(imagJ, factor), ::mathematics::multiply<N, Safe>(imagK, factor));
    }

    template <bool Safe = true>
    N dot(const Quaternion<N> &other) const
    {
        return ::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, other.real), ::mathematics::multiply<N, Safe>(imagI, other.imagI)), ::mathematics::multiply<N, Safe>(imagJ, other.imagJ)), ::mathematics::multiply<N, Safe>(imagK, other.imagK));
    }

    template <bool Safe = true>
    Quaternion<N> project(const Quaternion<N> &other) const
    {
        N denom = other.template dot<Safe>(other);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(denom) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        else
        {
            if (denom == N(0))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        N scalar = ::mathematics::divide<N, Safe>(this->template dot<Safe>(other), denom);
        return other * scalar;
    }

    template <bool Safe = true>
    Quaternion<N> reflect(const Quaternion<N> &normal) const
    {
        N denom = normal.template dot<Safe>(normal);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(denom) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return *this;
            }
        }
        else
        {
            if (denom == N(0))
            {
                return *this;
            }
        }
        N factor = ::mathematics::divide<N, Safe>(::mathematics::multiply<N, Safe>(N(2), this->template dot<Safe>(normal)), denom);
        return Quaternion<N>(::mathematics::subtract<N, Safe>(real, ::mathematics::multiply<N, Safe>(factor, normal.real)), ::mathematics::subtract<N, Safe>(imagI, ::mathematics::multiply<N, Safe>(factor, normal.imagI)), ::mathematics::subtract<N, Safe>(imagJ, ::mathematics::multiply<N, Safe>(factor, normal.imagJ)), ::mathematics::subtract<N, Safe>(imagK, ::mathematics::multiply<N, Safe>(factor, normal.imagK)));
    }

    template <bool Safe = true>
    Quaternion<N> refract(const Quaternion<N> &normal, N refractionRatio) const
    {
        Quaternion<N> unitNormal = normal.template normalize<Safe>();
        Quaternion<N> unitIncident = this->template normalize<Safe>();
        N dotProduct = unitIncident.template dot<Safe>(unitNormal);
        N discriminant = ::mathematics::subtract<N, Safe>(N(1), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(refractionRatio, refractionRatio), ::mathematics::subtract<N, Safe>(N(1), ::mathematics::multiply<N, Safe>(dotProduct, dotProduct))));
        if (discriminant < N(0))
        {
            return this->template reflect<Safe>(normal);
        }
        N sqrtDiscriminant = ::mathematics::squareRoot<N, Safe>(discriminant);
        Quaternion<N> refracted = unitIncident * refractionRatio;
        Quaternion<N> normalComponent = unitNormal * (::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(refractionRatio, dotProduct), sqrtDiscriminant));
        Quaternion<N> result = refracted - normalComponent;
        N originalMagnitude = this->template module<Safe>();
        return result.template normalize<Safe>() * originalMagnitude;
    }

    template <bool Safe = true>
    Quaternion<N> perpendicular() const
    {
        N absI = ::mathematics::absolute<N, Safe>(imagI);
        N absJ = ::mathematics::absolute<N, Safe>(imagJ);
        N absK = ::mathematics::absolute<N, Safe>(imagK);
        if constexpr (is_decimal_v<N>)
        {
            if (absI < std::numeric_limits<N>::epsilon() * N(10) && absJ < std::numeric_limits<N>::epsilon() * N(10) && absK < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(0), N(1), N(0), N(0));
            }
        }
        else
        {
            if (absI == N(0) && absJ == N(0) && absK == N(0))
            {
                return Quaternion<N>(N(0), N(1), N(0), N(0));
            }
        }
        Quaternion<N> result(N(0), N(0), N(0), N(0));
        if (absI >= absJ && absI >= absK)
        {
            result.getImaginaryI() = N(0);
            if constexpr (is_decimal_v<N>)
            {
                if (absJ < std::numeric_limits<N>::epsilon() * N(10) && absK < std::numeric_limits<N>::epsilon() * N(10))
                {
                    result.getImaginaryJ() = N(1);
                    result.getImaginaryK() = N(0);
                }
                else
                {
                    result.getImaginaryJ() = ::mathematics::negative<N, Safe>(imagK);
                    result.getImaginaryK() = imagJ;
                }
            }
            else
            {
                if (absJ == N(0) && absK == N(0))
                {
                    result.getImaginaryJ() = N(1);
                    result.getImaginaryK() = N(0);
                }
                else
                {
                    result.getImaginaryJ() = ::mathematics::negative<N, Safe>(imagK);
                    result.getImaginaryK() = imagJ;
                }
            }
        }
        else if (absJ >= absI && absJ >= absK)
        {
            result.getImaginaryJ() = N(0);
            if constexpr (is_decimal_v<N>)
            {
                if (absI < std::numeric_limits<N>::epsilon() * N(10) && absK < std::numeric_limits<N>::epsilon() * N(10))
                {
                    result.getImaginaryI() = N(0);
                    result.getImaginaryK() = N(1);
                }
                else
                {
                    result.getImaginaryI() = imagK;
                    result.getImaginaryK() = ::mathematics::negative<N, Safe>(imagI);
                }
            }
            else
            {
                if (absI == N(0) && absK == N(0))
                {
                    result.getImaginaryI() = N(0);
                    result.getImaginaryK() = N(1);
                }
                else
                {
                    result.getImaginaryI() = imagK;
                    result.getImaginaryK() = ::mathematics::negative<N, Safe>(imagI);
                }
            }
        }
        else
        {
            result.getImaginaryK() = N(0);
            if constexpr (is_decimal_v<N>)
            {
                if (absI < std::numeric_limits<N>::epsilon() * N(10) && absJ < std::numeric_limits<N>::epsilon() * N(10))
                {
                    result.getImaginaryI() = N(0);
                    result.getImaginaryJ() = N(1);
                }
                else
                {
                    result.getImaginaryI() = ::mathematics::negative<N, Safe>(imagJ);
                    result.getImaginaryJ() = imagI;
                }
            }
            else
            {
                if (absI == N(0) && absJ == N(0))
                {
                    result.getImaginaryI() = N(0);
                    result.getImaginaryJ() = N(1);
                }
                else
                {
                    result.getImaginaryI() = ::mathematics::negative<N, Safe>(imagJ);
                    result.getImaginaryJ() = imagI;
                }
            }
        }
        N norm = ::mathematics::squareRoot<N, Safe>(
            ::mathematics::add<N, Safe>(
                ::mathematics::add<N, Safe>(
                    ::mathematics::multiply<N, Safe>(result.getImaginaryI(), result.getImaginaryI()),
                    ::mathematics::multiply<N, Safe>(result.getImaginaryJ(), result.getImaginaryJ())),
                ::mathematics::multiply<N, Safe>(result.getImaginaryK(), result.getImaginaryK())));
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(norm) > std::numeric_limits<N>::epsilon() * N(10))
            {
                N invNorm = ::mathematics::divide<N, Safe>(N(1), norm);
                result.getImaginaryI() = ::mathematics::multiply<N, Safe>(result.getImaginaryI(), invNorm);
                result.getImaginaryJ() = ::mathematics::multiply<N, Safe>(result.getImaginaryJ(), invNorm);
                result.getImaginaryK() = ::mathematics::multiply<N, Safe>(result.getImaginaryK(), invNorm);
            }
        }
        else
        {
            if (norm != N(0))
            {
                N invNorm = ::mathematics::divide<N, Safe>(N(1), norm);
                result.getImaginaryI() = ::mathematics::multiply<N, Safe>(result.getImaginaryI(), invNorm);
                result.getImaginaryJ() = ::mathematics::multiply<N, Safe>(result.getImaginaryJ(), invNorm);
                result.getImaginaryK() = ::mathematics::multiply<N, Safe>(result.getImaginaryK(), invNorm);
            }
        }
        return result;
    }

    template <bool Safe = true>
    bool parallel(const Quaternion<N> &other) const
    {
        N thisVectorNormSq = ::mathematics::add<N, Safe>(
            ::mathematics::add<N, Safe>(
                ::mathematics::multiply<N, Safe>(imagI, imagI),
                ::mathematics::multiply<N, Safe>(imagJ, imagJ)),
            ::mathematics::multiply<N, Safe>(imagK, imagK));
        N otherVectorNormSq = ::mathematics::add<N, Safe>(
            ::mathematics::add<N, Safe>(
                ::mathematics::multiply<N, Safe>(other.imagI, other.imagI),
                ::mathematics::multiply<N, Safe>(other.imagJ, other.imagJ)),
            ::mathematics::multiply<N, Safe>(other.imagK, other.imagK));
        bool thisIsScalar;
        bool otherIsScalar;
        if constexpr (is_decimal_v<N>)
        {
            thisIsScalar = ::mathematics::absolute<N, Safe>(thisVectorNormSq) < std::numeric_limits<N>::epsilon() * N(10);
            otherIsScalar = ::mathematics::absolute<N, Safe>(otherVectorNormSq) < std::numeric_limits<N>::epsilon() * N(10);
        }
        else
        {
            thisIsScalar = (thisVectorNormSq == N(0));
            otherIsScalar = (otherVectorNormSq == N(0));
        }
        if (thisIsScalar && otherIsScalar)
        {
            return true;
        }
        if (thisIsScalar || otherIsScalar)
        {
            return false;
        }
        Quaternion<N> crossProduct = cross<Safe>(other);
        N crossModule = crossProduct.template module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            return ::mathematics::absolute<N, Safe>(crossModule) < std::numeric_limits<N>::epsilon() * N(10);
        }
        else
        {
            return crossModule == N(0);
        }
    }

    template <bool Safe = true>
    Quaternion<N> lerp(const Quaternion<N> &other, N t) const
    {
        N oneMinusT = ::mathematics::subtract<N, Safe>(N(1), t);
        return Quaternion<N>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, oneMinusT), ::mathematics::multiply<N, Safe>(other.real, t)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagI, oneMinusT), ::mathematics::multiply<N, Safe>(other.imagI, t)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagJ, oneMinusT), ::mathematics::multiply<N, Safe>(other.imagJ, t)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagK, oneMinusT), ::mathematics::multiply<N, Safe>(other.imagK, t)));
    }

    template <bool Safe = true>
    Quaternion<N> nlerp(const Quaternion<N> &other, N t) const
    {
        Quaternion<N> interpolated = lerp<Safe>(other, t);
        N magnitude = interpolated.template module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return *this;
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return *this;
            }
        }
        N scale = ::mathematics::divide<N, Safe>(N(1), magnitude);
        return Quaternion<N>(::mathematics::multiply<N, Safe>(interpolated.real, scale), ::mathematics::multiply<N, Safe>(interpolated.imagI, scale), ::mathematics::multiply<N, Safe>(interpolated.imagJ, scale), ::mathematics::multiply<N, Safe>(interpolated.imagK, scale));
    }

    template <bool Safe = true>
    Quaternion<N> slerp(const Quaternion<N> &other, N t) const
    {
        N dotProduct = this->dot<Safe>(other);
        Quaternion<N> target = other;
        if (dotProduct < N(0))
        {
            dotProduct = ::mathematics::negative<N, Safe>(dotProduct);
            target = target.template negative<Safe>();
        }
        if (dotProduct > N(1))
        {
            dotProduct = N(1);
        }
        N oneMinusDotSquared = ::mathematics::subtract<N, Safe>(N(1), ::mathematics::multiply<N, Safe>(dotProduct, dotProduct));
        N theta = ::mathematics::arctangent2<N, Safe>(::mathematics::squareRoot<N, Safe>(oneMinusDotSquared), dotProduct);
        N sinTheta = ::mathematics::sine<N, Safe>(theta);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(sinTheta) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return lerp<Safe>(target, t);
            }
        }
        else
        {
            if (sinTheta == N(0))
            {
                return lerp<Safe>(target, t);
            }
        }
        N factorA = ::mathematics::divide<N, Safe>(::mathematics::sine<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::subtract<N, Safe>(N(1), t), theta)), sinTheta);
        N factorB = ::mathematics::divide<N, Safe>(::mathematics::sine<N, Safe>(::mathematics::multiply<N, Safe>(t, theta)), sinTheta);
        return Quaternion<N>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(real, factorA), ::mathematics::multiply<N, Safe>(target.real, factorB)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagI, factorA), ::mathematics::multiply<N, Safe>(target.imagI, factorB)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagJ, factorA), ::mathematics::multiply<N, Safe>(target.imagJ, factorB)), ::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagK, factorA), ::mathematics::multiply<N, Safe>(target.imagK, factorB)));
    }

    template <bool Safe = true>
    N distance(const Quaternion<N> &other) const
    {
        Quaternion<N> diff = (*this) - other;
        return diff.template module<Safe>();
    }

    template <bool Safe = true>
    N angle(const Quaternion<N> &other) const
    {
        N moduleA = this->module<Safe>();
        N moduleB = other.template module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(moduleA) < std::numeric_limits<N>::epsilon() * N(10) || ::mathematics::absolute<N, Safe>(moduleB) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return N(0);
            }
        }
        else
        {
            if (moduleA == N(0) || moduleB == N(0))
            {
                return N(0);
            }
        }
        Quaternion<N> normalizedA = *this / moduleA;
        Quaternion<N> normalizedB = other / moduleB;
        N dotProduct = normalizedA.template dot<Safe>(normalizedB);
        if (dotProduct > N(1))
        {
            dotProduct = N(1);
        }
        if (dotProduct < N(-1))
        {
            dotProduct = N(-1);
        }
        return ::mathematics::arccosine<N, Safe>(dotProduct);
    }

    template <typename T = N, bool Safe = true>
    std::vector<Quaternion<N>> head(const T &step) const
    {
        int count = 0;
        N angleStep = N(0);
        N fullCircle = N(2) * N(::mathematics::pi);
        if constexpr (is_integer_v<T>)
        {
            count = static_cast<int>(step);
            if (count < 1)
                return {};
            angleStep = ::mathematics::divide<N, Safe>(fullCircle, static_cast<N>(count));
        }
        else if constexpr (std::is_arithmetic_v<T>)
        {
            if constexpr (is_decimal_v<N>)
            {
                if (::mathematics::absolute<N, Safe>(step) < std::numeric_limits<N>::epsilon() * N(10))
                    return {*this};
            }
            else
            {
                if (step == N(0))
                    return {*this};
            }
            angleStep = step;
            N ratio = ::mathematics::divide<N, Safe>(fullCircle, ::mathematics::absolute<N, Safe>(angleStep));
            count = static_cast<int>(::mathematics::round<N, Safe>(ratio));
            if (count < 1)
                count = 1;
        }
        else if constexpr (is_complex_v<T>)
        {
            return head<N, Safe>(step.template argument<Safe>());
        }
        else
        {
            return head<N, Safe>(step.template module<Safe>());
        }
        std::vector<Quaternion<N>> result;
        result.reserve(static_cast<size_t>(count));
        N vectorNormSq = ::mathematics::add<N, Safe>(
            ::mathematics::add<N, Safe>(
                ::mathematics::multiply<N, Safe>(imagI, imagI),
                ::mathematics::multiply<N, Safe>(imagJ, imagJ)),
            ::mathematics::multiply<N, Safe>(imagK, imagK));
        Quaternion<N> rotationAxis;
        if constexpr (is_decimal_v<N>)
        {
            if (vectorNormSq < std::numeric_limits<N>::epsilon() * N(10))
            {
                rotationAxis = Quaternion<N>(N(0), N(1), N(0), N(0));
            }
            else
            {
                rotationAxis = perpendicular<Safe>();
            }
        }
        else
        {
            if (vectorNormSq == N(0))
            {
                rotationAxis = Quaternion<N>(N(0), N(1), N(0), N(0));
            }
            else
            {
                rotationAxis = perpendicular<Safe>();
            }
        }
        for (int i = 0; i < count; i++)
        {
            N angle = ::mathematics::multiply<N, Safe>(angleStep, static_cast<N>(i));
            N halfAngle = ::mathematics::divide<N, Safe>(angle, N(2));
            N cosHalf = ::mathematics::cosine<N, Safe>(halfAngle);
            N sinHalf = ::mathematics::sine<N, Safe>(halfAngle);
            Quaternion<N> rotation(cosHalf,
                                   ::mathematics::multiply<N, Safe>(sinHalf, rotationAxis.getImaginaryI()),
                                   ::mathematics::multiply<N, Safe>(sinHalf, rotationAxis.getImaginaryJ()),
                                   ::mathematics::multiply<N, Safe>(sinHalf, rotationAxis.getImaginaryK()));
            result.push_back(rotation * (*this) * rotation.template conjugate<Safe>());
        }
        return result;
    }

    template <typename T = N, bool Safe = true>
    std::vector<Quaternion<N>> tail(const T &step) const
    {
        auto heads = head<T, Safe>(step);
        std::vector<Quaternion<N>> result;
        result.reserve(heads.size());
        N endpointReal = getReal();
        N endpointI = getImaginaryI();
        N endpointJ = getImaginaryJ();
        N endpointK = getImaginaryK();
        for (auto &h : heads)
        {
            result.push_back(Quaternion<N>(
                ::mathematics::add<N, Safe>(endpointReal, ::mathematics::negative<N, Safe>(h.getReal())),
                ::mathematics::add<N, Safe>(endpointI, ::mathematics::negative<N, Safe>(h.getImaginaryI())),
                ::mathematics::add<N, Safe>(endpointJ, ::mathematics::negative<N, Safe>(h.getImaginaryJ())),
                ::mathematics::add<N, Safe>(endpointK, ::mathematics::negative<N, Safe>(h.getImaginaryK()))));
        }
        return result;
    }

    std::tuple<std::complex<N>, std::complex<N>, std::complex<N>, std::complex<N>, std::complex<N>, std::complex<N>> toComplex() const
    {
        return {std::complex<N>(real, imagI), std::complex<N>(real, imagJ), std::complex<N>(real, imagK), std::complex<N>(imagI, imagJ), std::complex<N>(imagI, imagK), std::complex<N>(imagJ, imagK)};
    }

    template <bool Safe = true>
    Quaternion<N> exponential() const
    {
        N imaginaryNorm = ::mathematics::squareRoot<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagI, imagI), ::mathematics::multiply<N, Safe>(imagJ, imagJ)), ::mathematics::multiply<N, Safe>(imagK, imagK)));
        N expReal = ::mathematics::exponential<N, Safe>(real);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(imaginaryNorm) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(expReal, N(0), N(0), N(0));
            }
        }
        else
        {
            if (imaginaryNorm == N(0))
            {
                return Quaternion<N>(expReal, N(0), N(0), N(0));
            }
        }
        N sinValue = ::mathematics::sine<N, Safe>(imaginaryNorm);
        N cosValue = ::mathematics::cosine<N, Safe>(imaginaryNorm);
        N factor = ::mathematics::divide<N, Safe>(sinValue, imaginaryNorm);
        return Quaternion<N>(::mathematics::multiply<N, Safe>(expReal, cosValue), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(expReal, factor), imagI), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(expReal, factor), imagJ), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(expReal, factor), imagK));
    }

    template <bool Safe = true>
    Quaternion<N> logarithm() const
    {
        N magnitude = this->module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                return Quaternion<N>(N(0), N(0), N(0), N(0));
            }
        }
        N imaginaryNorm = ::mathematics::squareRoot<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagI, imagI), ::mathematics::multiply<N, Safe>(imagJ, imagJ)), ::mathematics::multiply<N, Safe>(imagK, imagK)));
        N logMagnitude = ::mathematics::logarithm<N, Safe>(magnitude);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(imaginaryNorm) < std::numeric_limits<N>::epsilon() * N(10))
            {
                if (real < N(0))
                {
                    return Quaternion<N>(logMagnitude, N(::mathematics::pi), N(0), N(0));
                }
                return Quaternion<N>(logMagnitude, N(0), N(0), N(0));
            }
        }
        else
        {
            if (imaginaryNorm == N(0))
            {
                if (real < N(0))
                {
                    return Quaternion<N>(logMagnitude, N(::mathematics::pi), N(0), N(0));
                }
                return Quaternion<N>(logMagnitude, N(0), N(0), N(0));
            }
        }
        N theta = ::mathematics::arctangent2<N, Safe>(imaginaryNorm, real);
        N factor = ::mathematics::divide<N, Safe>(theta, imaginaryNorm);
        return Quaternion<N>(logMagnitude, ::mathematics::multiply<N, Safe>(factor, imagI), ::mathematics::multiply<N, Safe>(factor, imagJ), ::mathematics::multiply<N, Safe>(factor, imagK));
    }

    template <bool Safe = true>
    Quaternion<N> logarithm(const Quaternion<N> &base) const
    {
        return this->logarithm<Safe>() * base.template logarithm<Safe>().template inverse<Safe>();
    }

    template <bool Safe = true>
    Quaternion<N> power(N t) const
    {
        N magnitude = this->module<Safe>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(magnitude) < std::numeric_limits<N>::epsilon() * N(10))
            {
                throw std::domain_error("power: zero quaternion cannot be raised to a power");
            }
        }
        else
        {
            if (magnitude == N(0))
            {
                throw std::domain_error("power: zero quaternion cannot be raised to a power");
            }
        }
        N imaginaryNorm = ::mathematics::squareRoot<N, Safe>(::mathematics::add<N, Safe>(::mathematics::add<N, Safe>(::mathematics::multiply<N, Safe>(imagI, imagI), ::mathematics::multiply<N, Safe>(imagJ, imagJ)), ::mathematics::multiply<N, Safe>(imagK, imagK)));
        N logMagnitude = ::mathematics::logarithm<N, Safe>(magnitude);
        N newReal = ::mathematics::multiply<N, Safe>(logMagnitude, t);
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, Safe>(imaginaryNorm) < std::numeric_limits<N>::epsilon() * N(10))
            {
                if (real < N(0))
                {
                    N adjustedAngle = ::mathematics::multiply<N, Safe>(N(::mathematics::pi), t);
                    N expNewReal = ::mathematics::exponential<N, Safe>(newReal);
                    return Quaternion<N>(::mathematics::multiply<N, Safe>(expNewReal, ::mathematics::cosine<N, Safe>(adjustedAngle)), ::mathematics::multiply<N, Safe>(expNewReal, ::mathematics::sine<N, Safe>(adjustedAngle)), N(0), N(0));
                }
                return Quaternion<N>(::mathematics::exponential<N, Safe>(newReal), N(0), N(0), N(0));
            }
        }
        else
        {
            if (imaginaryNorm == N(0))
            {
                if (real < N(0))
                {
                    N adjustedAngle = ::mathematics::multiply<N, Safe>(N(::mathematics::pi), t);
                    N expNewReal = ::mathematics::exponential<N, Safe>(newReal);
                    return Quaternion<N>(::mathematics::multiply<N, Safe>(expNewReal, ::mathematics::cosine<N, Safe>(adjustedAngle)), ::mathematics::multiply<N, Safe>(expNewReal, ::mathematics::sine<N, Safe>(adjustedAngle)), N(0), N(0));
                }
                return Quaternion<N>(::mathematics::exponential<N, Safe>(newReal), N(0), N(0), N(0));
            }
        }
        N theta = ::mathematics::arctangent2<N, Safe>(imaginaryNorm, real);
        N newAngle = ::mathematics::multiply<N, Safe>(theta, t);
        N sinNewAngle = ::mathematics::sine<N, Safe>(newAngle);
        N factor = ::mathematics::divide<N, Safe>(sinNewAngle, imaginaryNorm);
        N expReal = ::mathematics::exponential<N, Safe>(newReal);
        return Quaternion<N>(::mathematics::multiply<N, Safe>(expReal, ::mathematics::cosine<N, Safe>(newAngle)), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(expReal, factor), imagI), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(expReal, factor), imagJ), ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(expReal, factor), imagK));
    }

    template <bool Safe = true>
    Quaternion<N> sine() const
    {
        return Quaternion<N>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::sine<N, Safe>(real), ::mathematics::hyperbolicCosine<N, Safe>(imagI)), ::mathematics::hyperbolicCosine<N, Safe>(imagJ)), ::mathematics::hyperbolicCosine<N, Safe>(imagK)),
                             ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::cosine<N, Safe>(real), ::mathematics::hyperbolicSine<N, Safe>(imagI)), ::mathematics::hyperbolicCosine<N, Safe>(imagJ)), ::mathematics::hyperbolicCosine<N, Safe>(imagK)),
                             ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::cosine<N, Safe>(real), ::mathematics::hyperbolicCosine<N, Safe>(imagI)), ::mathematics::hyperbolicSine<N, Safe>(imagJ)), ::mathematics::hyperbolicCosine<N, Safe>(imagK)),
                             ::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::cosine<N, Safe>(real), ::mathematics::hyperbolicCosine<N, Safe>(imagI)), ::mathematics::hyperbolicCosine<N, Safe>(imagJ)), ::mathematics::hyperbolicSine<N, Safe>(imagK)));
    }

    template <bool Safe = true>
    Quaternion<N> cosine() const
    {
        return Quaternion<N>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::cosine<N, Safe>(real), ::mathematics::hyperbolicCosine<N, Safe>(imagI)), ::mathematics::hyperbolicCosine<N, Safe>(imagJ)), ::mathematics::hyperbolicCosine<N, Safe>(imagK)),
                             ::mathematics::negative<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::sine<N, Safe>(real), ::mathematics::hyperbolicSine<N, Safe>(imagI)), ::mathematics::hyperbolicCosine<N, Safe>(imagJ)), ::mathematics::hyperbolicCosine<N, Safe>(imagK))),
                             ::mathematics::negative<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::sine<N, Safe>(real), ::mathematics::hyperbolicCosine<N, Safe>(imagI)), ::mathematics::hyperbolicSine<N, Safe>(imagJ)), ::mathematics::hyperbolicCosine<N, Safe>(imagK))),
                             ::mathematics::negative<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::multiply<N, Safe>(::mathematics::sine<N, Safe>(real), ::mathematics::hyperbolicCosine<N, Safe>(imagI)), ::mathematics::hyperbolicCosine<N, Safe>(imagJ)), ::mathematics::hyperbolicSine<N, Safe>(imagK))));
    }

    template <bool Safe = true>
    Quaternion<N> around(const Quaternion<N> &rotation) const
    {
        return rotation * (*this) * rotation.template inverse<Safe>();
    }

    template <bool Safe = true>
    Quaternion<N> rotate(const Quaternion<N> &v) const { return (*this) * v * inverse<Safe>(); }

    template <bool Safe = true>
    Quaternion<N> rotate(const std::initializer_list<Quaternion<N>> &rotations) const
    {
        Quaternion<N> result = *this;
        for (const auto &r : rotations)
        {
            result = r * result * r.template inverse<Safe>();
        }
        return result;
    }

    template <bool Safe = true>
    Quaternion<N> rotate(const std::vector<Quaternion<N>> &rotations) const
    {
        Quaternion<N> result = *this;
        for (const auto &r : rotations)
        {
            result = r * result * r.template inverse<Safe>();
        }
        return result;
    }

    template <bool Safe = true>
    Quaternion<N> derivative(const Quaternion<N> &angularVelocity) const
    {
        return Quaternion<N>(N(0.5)) * angularVelocity * (*this);
    }

    template <bool Safe = true>
    Quaternion<N> integrate(const Quaternion<N> &angularVelocity, N timeStep) const
    {
        Quaternion<N> halfAngularVelocity = angularVelocity * N(0.5) * timeStep;
        return (*this) * halfAngularVelocity.template exponential<Safe>();
    }

    Quaternion<N> &operator=(const Quaternion<N> &) = default;

    Quaternion<N> operator+(const Quaternion<N> &q) const
    {
        return Quaternion<N>(::mathematics::add<N, false>(real, q.real), ::mathematics::add<N, false>(imagI, q.imagI), ::mathematics::add<N, false>(imagJ, q.imagJ), ::mathematics::add<N, false>(imagK, q.imagK));
    }

    Quaternion<N> operator-(const Quaternion<N> &q) const
    {
        return Quaternion<N>(::mathematics::subtract<N, false>(real, q.real), ::mathematics::subtract<N, false>(imagI, q.imagI), ::mathematics::subtract<N, false>(imagJ, q.imagJ), ::mathematics::subtract<N, false>(imagK, q.imagK));
    }

    Quaternion<N> operator*(const N &scalar) const
    {
        return Quaternion<N>(::mathematics::multiply<N, false>(real, scalar), ::mathematics::multiply<N, false>(imagI, scalar), ::mathematics::multiply<N, false>(imagJ, scalar), ::mathematics::multiply<N, false>(imagK, scalar));
    }

    Quaternion<N> operator/(const N &scalar) const
    {
        return Quaternion<N>(::mathematics::divide<N, false>(real, scalar), ::mathematics::divide<N, false>(imagI, scalar), ::mathematics::divide<N, false>(imagJ, scalar), ::mathematics::divide<N, false>(imagK, scalar));
    }

    Quaternion<N> operator*(const Quaternion<N> &q) const
    {
        return Quaternion<N>(::mathematics::subtract<N, false>(::mathematics::subtract<N, false>(::mathematics::subtract<N, false>(::mathematics::multiply<N, false>(real, q.real), ::mathematics::multiply<N, false>(imagI, q.imagI)), ::mathematics::multiply<N, false>(imagJ, q.imagJ)), ::mathematics::multiply<N, false>(imagK, q.imagK)), ::mathematics::add<N, false>(::mathematics::add<N, false>(::mathematics::subtract<N, false>(::mathematics::multiply<N, false>(real, q.imagI), ::mathematics::multiply<N, false>(imagK, q.imagJ)), ::mathematics::multiply<N, false>(imagI, q.real)), ::mathematics::multiply<N, false>(imagJ, q.imagK)), ::mathematics::add<N, false>(::mathematics::subtract<N, false>(::mathematics::add<N, false>(::mathematics::multiply<N, false>(real, q.imagJ), ::mathematics::multiply<N, false>(imagK, q.imagI)), ::mathematics::multiply<N, false>(imagI, q.imagK)), ::mathematics::multiply<N, false>(imagJ, q.real)), ::mathematics::add<N, false>(::mathematics::subtract<N, false>(::mathematics::add<N, false>(::mathematics::multiply<N, false>(real, q.imagK), ::mathematics::multiply<N, false>(imagI, q.imagJ)), ::mathematics::multiply<N, false>(imagJ, q.imagI)), ::mathematics::multiply<N, false>(imagK, q.real)));
    }

    Quaternion<N> operator/(const Quaternion<N> &q) const
    {
        return (*this) * q.template inverse<false>();
    }

    Quaternion<N> &operator+=(const Quaternion<N> &q)
    {
        real = ::mathematics::add<N, false>(real, q.real);
        imagI = ::mathematics::add<N, false>(imagI, q.imagI);
        imagJ = ::mathematics::add<N, false>(imagJ, q.imagJ);
        imagK = ::mathematics::add<N, false>(imagK, q.imagK);
        return *this;
    }

    Quaternion<N> &operator-=(const Quaternion<N> &q)
    {
        real = ::mathematics::subtract<N, false>(real, q.real);
        imagI = ::mathematics::subtract<N, false>(imagI, q.imagI);
        imagJ = ::mathematics::subtract<N, false>(imagJ, q.imagJ);
        imagK = ::mathematics::subtract<N, false>(imagK, q.imagK);
        return *this;
    }

    Quaternion<N> &operator*=(const Quaternion<N> &q)
    {
        *this = *this * q;
        return *this;
    }

    Quaternion<N> &operator*=(const N &scalar)
    {
        real = ::mathematics::multiply<N, false>(real, scalar);
        imagI = ::mathematics::multiply<N, false>(imagI, scalar);
        imagJ = ::mathematics::multiply<N, false>(imagJ, scalar);
        imagK = ::mathematics::multiply<N, false>(imagK, scalar);
        return *this;
    }

    Quaternion<N> &operator/=(const N &scalar)
    {
        real = ::mathematics::divide<N, false>(real, scalar);
        imagI = ::mathematics::divide<N, false>(imagI, scalar);
        imagJ = ::mathematics::divide<N, false>(imagJ, scalar);
        imagK = ::mathematics::divide<N, false>(imagK, scalar);
        return *this;
    }

    Quaternion<N> operator-() const
    {
        return Quaternion<N>(::mathematics::negative<N, false>(real), ::mathematics::negative<N, false>(imagI), ::mathematics::negative<N, false>(imagJ), ::mathematics::negative<N, false>(imagK));
    }

    bool operator==(const Quaternion<N> &q) const
    {
        if constexpr (is_decimal_v<N>)
        {
            return ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(real, q.real)) < std::numeric_limits<N>::epsilon() * N(10) && ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(imagI, q.imagI)) < std::numeric_limits<N>::epsilon() * N(10) && ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(imagJ, q.imagJ)) < std::numeric_limits<N>::epsilon() * N(10) && ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(imagK, q.imagK)) < std::numeric_limits<N>::epsilon() * N(10);
        }
        else
        {
            return real == q.real && imagI == q.imagI && imagJ == q.imagJ && imagK == q.imagK;
        }
    }

    bool operator!=(const Quaternion<N> &q) const { return !(*this == q); }

    bool operator<(const Quaternion<N> &q) const
    {
        N magnitude1 = module<false>();
        N magnitude2 = q.template module<false>();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(magnitude1, magnitude2)) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return magnitude1 < magnitude2;
            }
            if (::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(real, q.real)) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return real < q.real;
            }
            if (::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(imagI, q.imagI)) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return imagI < q.imagI;
            }
            if (::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(imagJ, q.imagJ)) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return imagJ < q.imagJ;
            }
        }
        else
        {
            if (magnitude1 != magnitude2)
            {
                return magnitude1 < magnitude2;
            }
            if (real != q.real)
            {
                return real < q.real;
            }
            if (imagI != q.imagI)
            {
                return imagI < q.imagI;
            }
            if (imagJ != q.imagJ)
            {
                return imagJ < q.imagJ;
            }
        }
        return imagK < q.imagK;
    }

    bool operator<=(const Quaternion<N> &q) const { return (*this < q) || (*this == q); }
    bool operator>(const Quaternion<N> &q) const { return !(*this <= q); }
    bool operator>=(const Quaternion<N> &q) const { return !(*this < q); }

    N &operator[](std::size_t index)
    {
        switch (index)
        {
        case 0:
            return real;
        case 1:
            return imagI;
        case 2:
            return imagJ;
        case 3:
            return imagK;
        default:
            throw std::out_of_range("Index out of range");
        }
    }

    const N &operator[](std::size_t index) const
    {
        switch (index)
        {
        case 0:
            return real;
        case 1:
            return imagI;
        case 2:
            return imagJ;
        case 3:
            return imagK;
        default:
            throw std::out_of_range("Index out of range");
        }
    }

    friend Quaternion<N> operator*(const N &scalar, const Quaternion<N> &q) { return q * scalar; }
    friend Quaternion<N> operator/(const N &scalar, const Quaternion<N> &q) { return scalar * q.template inverse<false>(); }
    friend Quaternion<N> operator+(const N &scalar, const Quaternion<N> &q) { return Quaternion<N>(::mathematics::add<N, false>(scalar, q.real), q.imagI, q.imagJ, q.imagK); }
    friend Quaternion<N> operator-(const N &scalar, const Quaternion<N> &q) { return Quaternion<N>(::mathematics::subtract<N, false>(scalar, q.real), ::mathematics::negative<N, false>(q.imagI), ::mathematics::negative<N, false>(q.imagJ), ::mathematics::negative<N, false>(q.imagK)); }

    friend std::ostream &operator<<(std::ostream &output, const Quaternion<N> &q)
    {
        output << "(" << q.real << ", " << q.imagI << ", " << q.imagJ << ", " << q.imagK << ")";
        return output;
    }
};

template <typename Container, typename N, std::enable_if_t<is_complex_v<typename Container::value_type>, int> = 0>
inline bool headsAtOrigin(const Container &vectors)
{
    if (vectors.empty())
    {
        return true;
    }
    for (const auto &v : vectors)
    {
        N moduleVal = v.template module();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, false>(moduleVal) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return false;
            }
        }
        else
        {
            if (moduleVal != N(0))
            {
                return false;
            }
        }
    }
    return true;
}

template <typename Container, typename N, std::enable_if_t<is_quaternion_v<typename Container::value_type>, int> = 0>
inline bool headsAtOrigin(const Container &vectors)
{
    if (vectors.empty())
    {
        return true;
    }
    for (const auto &v : vectors)
    {
        N moduleVal = v.template module();
        if constexpr (is_decimal_v<N>)
        {
            if (::mathematics::absolute<N, false>(moduleVal) >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return false;
            }
        }
        else
        {
            if (moduleVal != N(0))
            {
                return false;
            }
        }
    }
    return true;
}

template <typename Container, typename N, std::enable_if_t<is_complex_v<typename Container::value_type>, int> = 0>
inline bool tailsAtOrigin(const Container &vectors)
{
    if (vectors.size() < 2)
    {
        return true;
    }
    const auto &firstEndpoint = vectors[0];
    for (size_t i = 1; i < vectors.size(); i++)
    {
        if constexpr (is_decimal_v<N>)
        {
            N diffReal = ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(vectors[i].getReal(), firstEndpoint.getReal()));
            N diffImag = ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(vectors[i].getImaginary(), firstEndpoint.getImaginary()));
            if (diffReal >= std::numeric_limits<N>::epsilon() * N(10) || diffImag >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return false;
            }
        }
        else
        {
            if (vectors[i].getReal() != firstEndpoint.getReal() || vectors[i].getImaginary() != firstEndpoint.getImaginary())
            {
                return false;
            }
        }
    }
    return true;
}

template <typename Container, typename N, std::enable_if_t<is_quaternion_v<typename Container::value_type>, int> = 0>
inline bool tailsAtOrigin(const Container &vectors)
{
    if (vectors.size() < 2)
    {
        return true;
    }
    const auto &firstEndpoint = vectors[0];
    for (size_t i = 1; i < vectors.size(); i++)
    {
        if constexpr (is_decimal_v<N>)
        {
            N diffReal = ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(vectors[i].getReal(), firstEndpoint.getReal()));
            N diffImagI = ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(vectors[i].getImaginaryI(), firstEndpoint.getImaginaryI()));
            N diffImagJ = ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(vectors[i].getImaginaryJ(), firstEndpoint.getImaginaryJ()));
            N diffImagK = ::mathematics::absolute<N, false>(::mathematics::subtract<N, false>(vectors[i].getImaginaryK(), firstEndpoint.getImaginaryK()));
            if (diffReal >= std::numeric_limits<N>::epsilon() * N(10) || diffImagI >= std::numeric_limits<N>::epsilon() * N(10) || diffImagJ >= std::numeric_limits<N>::epsilon() * N(10) || diffImagK >= std::numeric_limits<N>::epsilon() * N(10))
            {
                return false;
            }
        }
        else
        {
            if (vectors[i].getReal() != firstEndpoint.getReal() || vectors[i].getImaginaryI() != firstEndpoint.getImaginaryI() || vectors[i].getImaginaryJ() != firstEndpoint.getImaginaryJ() || vectors[i].getImaginaryK() != firstEndpoint.getImaginaryK())
            {
                return false;
            }
        }
    }
    return true;
}

template <typename Container, typename N, std::enable_if_t<is_complex_v<typename Container::value_type> || is_quaternion_v<typename Container::value_type>, int> = 0>
inline bool headToTail(const Container &vectors)
{
    if (vectors.size() < 2)
    {
        return false;
    }
    auto sum = vectors[0];
    for (size_t i = 1; i < vectors.size(); i++)
    {
        sum = sum + vectors[i];
    }
    N moduleVal = sum.template module();
    if constexpr (is_decimal_v<N>)
    {
        return ::mathematics::absolute<N, false>(moduleVal) < std::numeric_limits<N>::epsilon() * N(10);
    }
    else
    {
        return moduleVal == N(0);
    }
}

template <typename Container, typename N, std::enable_if_t<is_complex_v<typename Container::value_type> || is_quaternion_v<typename Container::value_type>, int> = 0>
inline N headToTailGap(const Container &vectors)
{
    if (vectors.empty())
    {
        return N(0);
    }
    if (vectors.size() == 1)
    {
        return vectors[0].template module();
    }
    auto sum = vectors[0];
    for (size_t i = 1; i < vectors.size(); i++)
    {
        sum = sum + vectors[i];
    }
    return sum.template module();
}

} // namespace mathematics

namespace std
{
template <typename N>
struct hash<mathematics::Complex<N>>
{
    size_t operator()(const mathematics::Complex<N> &complexValue) const
    {
        size_t seed = 0;
        seed ^= hash<N>{}(complexValue.getReal()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<N>{}(complexValue.getImaginary()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template <typename N>
struct less<mathematics::Complex<N>>
{
    bool operator()(const mathematics::Complex<N> &left, const mathematics::Complex<N> &right) const
    {
        if (left.getReal() != right.getReal())
        {
            return left.getReal() < right.getReal();
        }
        return left.getImaginary() < right.getImaginary();
    }
};

template <typename N>
struct hash<mathematics::Quaternion<N>>
{
    size_t operator()(const mathematics::Quaternion<N> &q) const
    {
        size_t seed = 0;
        seed ^= hash<N>{}(q.getReal()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<N>{}(q.getImaginaryI()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<N>{}(q.getImaginaryJ()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<N>{}(q.getImaginaryK()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template <typename N>
struct less<mathematics::Quaternion<N>>
{
    bool operator()(const mathematics::Quaternion<N> &left, const mathematics::Quaternion<N> &right) const
    {
        if (left.getReal() != right.getReal())
        {
            return left.getReal() < right.getReal();
        }
        if (left.getImaginaryI() != right.getImaginaryI())
        {
            return left.getImaginaryI() < right.getImaginaryI();
        }
        if (left.getImaginaryJ() != right.getImaginaryJ())
        {
            return left.getImaginaryJ() < right.getImaginaryJ();
        }
        return left.getImaginaryK() < right.getImaginaryK();
    }
};

} // namespace std
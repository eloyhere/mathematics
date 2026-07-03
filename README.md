# Maths Library

A modern C++ maths library focused on **type safety**, **numerical stability**, and **geometric intuition**. It provides a complete mathematical toolchain from basic arithmetic to complex numbers and quaternions, suitable for game development, computer graphics, physics simulation, and scientific computing.

## ✨ Key Features

- **Type Traits System**: Automatically identifies integer, floating‑point, complex, and quaternion types, delivering optimal implementations for each.
- **Optional Safe Mode**: Core functions enable safety checks (division by zero, NaN, Inf, overflow) by default, with the option to disable them for maximum performance.
- **Complete Complex Number Support**: The `Complex<T>` class encapsulates real and imaginary components, supporting polar conversion, complex analysis, and vector operations.
- **Robust Quaternion Implementation**: The `Quaternion<T>` class supports 3D rotations, spherical linear interpolation (SLERP), angular velocity integration, and other essential operations.
- **Rich Mathematical Toolkit**: Covers trigonometric, hyperbolic, power, exponential, logarithmic, statistical distributions, and FFT transforms.
- **Geometric Algorithms**: Includes projection, reflection, refraction, cross and dot products, linear/spherical interpolation, and more.
- **Zero External Dependencies**: Relies solely on the C++ Standard Library for cross‑platform compatibility.

## 📦 Installation & Usage

### Requirements
- C++17 compliant compiler (GCC 8+, Clang 7+, MSVC 2019+)
- No third‑party dependencies

### Quick Integration
Copy `mathematics.h` and `quaternion.h` into your project’s include path and include them as follows:

```cpp
#include "mathematics.h"
#include "quaternion.h"
```

## 🛠️ Core Functions in `mathematics.h`

`mathematics.h` provides a wide range of free functions covering fundamental arithmetic to advanced algorithms. **All functions enable safe mode by default** (the `Safe` template parameter may be omitted).

### Basic Arithmetic
```cpp
#include <iostream>
#include "mathematics.h"

using namespace mathematics;

int main() {
    // Addition, subtraction, multiplication, division
    double a = 10.0, b = 3.0;
    std::cout << "10 / 3 = " << divide(a, b) << std::endl;  // Safe by default

    // Modulo operation (works with integers and floating‑point)
    int mod_int = modulus(10, 3);          // 1
    double mod_double = modulus(10.5, 3.2); // 0.9

    // Absolute value and sign
    std::cout << "|-5| = " << absolute(-5) << std::endl;  // 5
    std::cout << "sign(-3.2) = " << sign(-3.2) << std::endl; // -1

    return 0;
}
```

### Elementary Functions
```cpp
// Square root, exponential, logarithm, power
double sqrt_val = squareRoot(25.0);       // 5.0
double exp_val = exponential(1.0);         // e ≈ 2.718
double log_val = logarithm(10.0);          // ln(10) ≈ 2.302
double pow_val = power(2.0, 3.0);          // 8.0

// Rounding functions
double floor_val = floor(3.7);             // 3.0
double ceil_val = ceil(3.2);               // 4.0
double round_val = round(3.5);             // 4.0
```

### Trigonometric & Hyperbolic Functions
```cpp
// Trigonometric (arguments in radians)
double sin_val = sine(pi / 2);              // 1.0
double cos_val = cosine(pi);                // -1.0
double tan_val = tangent(pi / 4);           // 1.0

// Inverse trigonometric
double asin_val = arcsine(1.0);            // π/2 ≈ 1.5708
double acos_val = arccosine(0.0);           // π/2 ≈ 1.5708
double atan_val = arctangent(1.0);         // π/4 ≈ 0.7854

// Hyperbolic functions
double sinh_val = hyperbolicSine(1.0);      // ≈ 1.1752
double cosh_val = hyperbolicCosine(1.0);    // ≈ 1.5431
double tanh_val = hyperbolicTangent(1.0);   // ≈ 0.7616
```

### Complex Number Utilities
```cpp
// Modulus and argument
std::complex<double> z(3.0, 4.0);
double mod = module(z);                     // 5.0
double arg = arctangent2(z.imag(), z.real()); // ≈ 0.9273 rad

// Complex exponential and logarithm
std::complex<double> exp_z = exponential(z);  // e^(3+4i)
std::complex<double> log_z = logarithm(z);    // ln(5) + i·0.9273

// Conjugate and multiplicative inverse
std::complex<double> conj_z = conjugate(z);   // (3.0, -4.0)
std::complex<double> inv_z = inverse(z);       // (0.12, -0.16)
```

### Geometric Operations
```cpp
// Distance and angle between two points
std::complex<double> p1(1.0, 2.0), p2(4.0, 6.0);
double dist = distance(p1, p2);               // 5.0
double angle_between = angle(p1, p2);         // Angle in radians

// Projection, reflection, refraction
std::complex<double> v(1.0, 1.0), normal(0.0, 1.0);
auto proj = project(v, normal);               // Project onto normal
auto refl = reflect(v, normal);               // Reflection vector
auto refrac = refract(v, normal, 1.5);        // Refraction vector (index 1.5)
```

### Interpolation & Transforms
```cpp
// Linear interpolation (LERP)
double lerp_val = lerp(0.0, 10.0, 0.5);       // 5.0

// Spherical linear interpolation (SLERP, for rotations)
std::complex<double> c1(1.0, 0.0), c2(0.0, 1.0);
auto slerp_c = slerp(c1, c2, 0.5);            // Intermediate complex number

// Radix‑2 FFT
std::vector<std::complex<double>> signal = {{1,0}, {2,0}, {3,0}, {4,0}};
auto fft_result = radix2(signal);              // Forward FFT
auto ifft_result = radix2(signal, true);       // Inverse FFT
```

## 🔢 The `Complex` Class

`Complex<T>` encapsulates complex numbers with operator overloading and method chaining. Safe mode is enabled by default.

### Construction & Access
```cpp
#include <iostream>
#include "quaternion.h"

using namespace mathematics;

int main() {
    // Various constructors
    Complex<double> z1;                         // 0 + 0i
    Complex<double> z2(3.0);                    // 3 + 0i
    Complex<double> z3(3.0, 4.0);               // 3 + 4i
    Complex<double> z4({3.0, 4.0});             // Initialiser list
    Complex<double> z5(std::complex<double>(3,4)); // From std::complex

    // Access real and imaginary parts
    double real = z3.getReal();                 // 3.0
    double imag = z3.getImaginary();            // 4.0
    z3.getReal() = 5.0;                         // Modify real part via reference

    return 0;
}
```

### Core Methods
```cpp
Complex<double> z(3.0, 4.0);

// Modulus and argument
double mod = z.module();                        // 5.0
double arg = z.argument();                      // ≈ 0.9273 rad

// Conjugate, inverse, normalisation
auto conj = z.conjugate();                      // 3 - 4i
auto inv = z.inverse();                         // 0.12 - 0.16i
auto norm = z.normalize();                      // 0.6 + 0.8i (unit modulus)

// Complex analysis
auto exp_z = z.exponential();                   // e^(3+4i)
auto log_z = z.logarithm();                      // ln(5) + i·0.9273
auto sqrt_z = z.squareRoot();                    // √(3+4i) ≈ (2,1)

// Geometric operations
auto rotated = z.rotate(pi / 2);                // Rotate by 90° about origin
auto reflected = z.reflect(Complex<double>(1,0)); // Reflect across real axis
auto projected = z.project(Complex<double>(1,1)); // Project onto (1,1)
```

### Operator Overloads
```cpp
Complex<double> a(1,2), b(3,4);

auto sum = a + b;          // (4,6)
auto diff = a - b;         // (-2,-2)
auto prod = a * b;         // (-5,10)
auto quot = a / b;         // (0.44,0.08)
auto neg = -a;             // (-1,-2)

// Comparison (floating‑point tolerance applied)
bool eq = (a == Complex<double>(1,2));  // true
bool lt = (a < b);                      // Compared by modulus
```

## 🌀 The `Quaternion` Class

`Quaternion<T>` represents 3D rotations without gimbal lock. Safe mode is enabled by default.

### Construction & Access
```cpp
#include <iostream>
#include "quaternion.h"

using namespace mathematics;

int main() {
    // Various constructors
    Quaternion<double> q1;                        // Identity (1,0,0,0)
    Quaternion<double> q2(2.0);                   // Scalar part only
    Quaternion<double> q3(1.0, 2.0, 3.0, 4.0);    // 1 + 2i + 3j + 4k
    Quaternion<double> q4({1,2,3,4});             // Initialiser list

    // Access components
    double scalar = q3.getReal();                 // 1.0
    double i_part = q3.getImaginaryI();           // 2.0
    double j_part = q3.getImaginaryJ();           // 3.0
    double k_part = q3.getImaginaryK();           // 4.0

    return 0;
}
```

### Rotation Operations
```cpp
// Create a rotation quaternion (90° about Y‑axis)
double angle = pi / 2;
Quaternion<double> rot_q(
    std::cos(angle / 2),  // Scalar
    0,                    // i
    std::sin(angle / 2),  // j
    0                     // k
);

// Rotate a vector (pure quaternion)
Quaternion<double> v(0, 1, 0, 0);  // x‑axis vector
auto v_rotated = rot_q * v * rot_q.inverse();  // Rotated vector

// Convenience methods
auto v_rot = v.rotate(rot_q);                  // Same as above
auto v_around = v.around(rot_q);               // Rotate about rot_q axis
```

### Interpolation & Other Methods
```cpp
Quaternion<double> q_start(1,0,0,0);           // No rotation
Quaternion<double> q_end(0.707, 0.707,0,0);    // 90° about X‑axis

// Linear interpolation (LERP)
auto lerp_q = q_start.lerp(q_end, 0.5);

// Normalised linear interpolation (NLERP)
auto nlerp_q = q_start.nlerp(q_end, 0.5);

// Spherical linear interpolation (SLERP – preferred for rotations)
auto slerp_q = q_start.slerp(q_end, 0.5);

// Angular velocity integration (physics)
Quaternion<double> orientation = q_start;
Quaternion<double> ang_vel(0, 1, 0, 0);         // Angular velocity about X‑axis
double dt = 0.016;                             // Timestep (~60 FPS)
auto new_orient = orientation.integrate(ang_vel, dt);
```

### Operator Overloads
```cpp
Quaternion<double> a(1,2,3,4), b(5,6,7,8);

auto sum = a + b;          // Component‑wise addition
auto diff = a - b;         // Component‑wise subtraction
auto prod = a * b;         // Quaternion multiplication (rotation composition)
auto quot = a / b;         // Equivalent to a * b.inverse()
auto neg = -a;             // Negation
```

## 🔧 Safe Mode Explained

All core functions and class methods provide an **optional `Safe` template parameter** (enabled by default):
- **Default Safe Mode**: Validates inputs (e.g. division by zero, NaN, Inf) and throws exceptions to prevent numerical errors.
- **Disabled Safe Mode**: Skips validation for maximum performance (use only when inputs are guaranteed valid).

```cpp
// Default safe mode (recommended)
double safe = divide(10.0, 0.0);  // Throws domain_error

// Disabled safe mode (high performance)
double fast = divide<false>(10.0, 0.0);  // Undefined behaviour if input is invalid
```

## 📄 Licence

This project is licensed under the **Apache Licence 2.0**.

## 🤝 Contributing

Contributions are welcome! Please ensure:
1. Code complies with C++17 standards.
2. New features include unit tests.
3. API consistency and backwards compatibility are maintained.
4. Relevant documentation is updated.

---

**Maths Library** — Making C++ mathematical computation safer, more elegant, and more efficient.

# Mathematics 数学库

一个现代 C++ 数学库，专注于**类型安全**、**数值稳定**与**几何直观**。提供从基础算术到复数、四元数的完整数学工具链，适用于游戏开发、图形学、物理模拟及科学计算场景。

## ✨ 核心特性

- **类型萃取系统**：自动识别整数、浮点数、复数与四元数类型，为不同数值类型提供最优实现
- **可选安全模式**：核心函数默认启用安全校验（防除零、NaN、Inf、溢出），也可按需关闭以提升性能
- **完整的复数支持**：`Complex<T>` 类封装实部/虚部运算，支持极坐标转换、复变函数与向量操作
- **专业的四元数实现**：`Quaternion<T>` 类支持 3D 旋转、球面插值（SLERP）、角速度积分等核心功能
- **丰富的数学工具**：涵盖三角函数、双曲函数、幂指对运算、统计分布、FFT 变换等
- **几何算法集合**：投影、反射、折射、叉积、点积、线性/球面插值等常用几何操作
- **零外部依赖**：仅依赖 C++ 标准库，跨平台兼容

## 📦 安装与使用

### 环境要求
- C++17 及以上编译器（GCC 8+, Clang 7+, MSVC 2019+）
- 无第三方库依赖

### 快速集成
将 `mathematics.h` 和 `quaternion.h` 复制到项目 include 路径，直接包含即可：

```cpp
#include "mathematics.h"
#include "quaternion.h"
```

## 🛠️ mathematics.h 核心函数

`mathematics.h` 提供了大量自由函数，覆盖基础数学运算与高级算法，**所有函数默认启用安全模式**（可省略 `Safe` 模板参数）。

### 基础运算
```cpp
#include <iostream>
#include "mathematics.h"

using namespace mathematics;

int main() {
    // 加减乘除（自动处理除零等异常）
    double a = 10.0, b = 3.0;
    std::cout << "10 / 3 = " << divide(a, b) << std::endl;  // 默认安全模式
    
    // 取模（整数/浮点数通用）
    int mod_int = modulus(10, 3);          // 1
    double mod_double = modulus(10.5, 3.2); // 0.9
    
    // 绝对值与符号
    std::cout << "|-5| = " << absolute(-5) << std::endl;  // 5
    std::cout << "sign(-3.2) = " << sign(-3.2) << std::endl; // -1
    
    return 0;
}
```

### 初等函数
```cpp
// 平方根、指数、对数
double sqrt_val = squareRoot(25.0);       // 5.0
double exp_val = exponential(1.0);         // e ≈ 2.718
double log_val = logarithm(10.0);          // ln(10) ≈ 2.302
double pow_val = power(2.0, 3.0);          // 8.0

// 取整与舍入
double floor_val = floor(3.7);             // 3.0
double ceil_val = ceil(3.2);               // 4.0
double round_val = round(3.5);             // 4.0
```

### 三角函数与双曲函数
```cpp
// 三角函数（参数为弧度）
double sin_val = sine(pi/2);              // 1.0
double cos_val = cosine(pi);               // -1.0
double tan_val = tangent(pi/4);            // 1.0

// 反三角函数
double asin_val = arcsine(1.0);            // π/2 ≈ 1.5708
double acos_val = arccosine(0.0);           // π/2 ≈ 1.5708
double atan_val = arctangent(1.0);          // π/4 ≈ 0.7854

// 双曲函数
double sinh_val = hyperbolicSine(1.0);      // ≈ 1.1752
double cosh_val = hyperbolicCosine(1.0);    // ≈ 1.5431
double tanh_val = hyperbolicTangent(1.0);   // ≈ 0.7616
```

### 复数运算
```cpp
// 复数模长与辐角
std::complex<double> z(3.0, 4.0);
double mod = module(z);                     // 5.0
double arg = arctangent2(z.imag(), z.real()); // ≈ 0.9273（弧度）

// 复指数与复对数
std::complex<double> exp_z = exponential(z);  // e^(3+4i) ≈ (-13.1288, 15.2008)
std::complex<double> log_z = logarithm(z);    // ln(5) + i*0.9273 ≈ (1.6094, 0.9273)

// 共轭与逆
std::complex<double> conj_z = conjugate(z);   // (3.0, -4.0)
std::complex<double> inv_z = inverse(z);      // (0.12, -0.16)
```

### 几何操作
```cpp
// 距离与角度
std::complex<double> p1(1.0, 2.0), p2(4.0, 6.0);
double dist = distance(p1, p2);               // 5.0
double angle_between = angle(p1, p2);          // 两点夹角（弧度）

// 投影、反射、折射
std::complex<double> v(1.0, 1.0), normal(0.0, 1.0);
auto proj = project(v, normal);                // 投影到法线
auto refl = reflect(v, normal);                // 反射向量
auto refrac = refract(v, normal, 1.5);         // 折射向量（折射率1.5）
```

### 插值与变换
```cpp
// 线性插值（LERP）
double lerp_val = lerp(0.0, 10.0, 0.5);        // 5.0

// 球面插值（SLERP，用于旋转）
std::complex<double> c1(1.0, 0.0), c2(0.0, 1.0);
auto slerp_c = slerp(c1, c2, 0.5);             // 中间复数

// FFT变换（基2）
std::vector<std::complex<double>> signal = {{1,0}, {2,0}, {3,0}, {4,0}};
auto fft_result = radix2(signal);               // 正向FFT
auto ifft_result = radix2(signal, true);        // 逆向FFT（第二个参数为true）
```

## 🔢 Complex 类

`Complex<T>` 封装了复数的实部与虚部，支持运算符重载与链式调用，默认启用安全模式。

### 构造与访问
```cpp
#include <iostream>
#include "quaternion.h"

using namespace mathematics;

int main() {
    // 多种构造方式
    Complex<double> z1;                         // 0 + 0i
    Complex<double> z2(3.0);                     // 3 + 0i
    Complex<double> z3(3.0, 4.0);                // 3 + 4i
    Complex<double> z4({3.0, 4.0});              // 初始化列表构造（同z3）
    Complex<double> z5(std::complex<double>(3,4));// 从std::complex转换
    
    // 访问实部/虚部
    double real = z3.getReal();                  // 3.0
    double imag = z3.getImaginary();             // 4.0
    z3.getReal() = 5.0;                          // 修改实部（返回引用）
    
    return 0;
}
```

### 核心方法
```cpp
Complex<double> z(3.0, 4.0);

// 模长与辐角
double mod = z.module();                         // 5.0
double arg = z.argument();                        // ≈ 0.9273（弧度）

// 共轭、逆、归一化
auto conj = z.conjugate();                        // 3 - 4i
auto inv = z.inverse();                           // 0.12 - 0.16i
auto norm = z.normalize();                         // 0.6 + 0.8i（模长为1）

// 复变函数
auto exp_z = z.exponential();                     // e^(3+4i)
auto log_z = z.logarithm();                        // ln(5) + i*0.9273
auto sqrt_z = z.squareRoot();                      // √(3+4i) ≈ (2,1)

// 几何操作
auto rotated = z.rotate(pi/2);                     // 绕原点旋转90度（≈ -4+3i）
auto reflected = z.reflect(Complex<double>(1,0));   // 关于实轴反射（3-4i）
auto projected = z.project(Complex<double>(1,1));  // 投影到(1,1)
```

### 运算符重载
```cpp
Complex<double> a(1,2), b(3,4);

auto sum = a + b;          // 加法：(4,6)
auto diff = a - b;         // 减法：(-2,-2)
auto prod = a * b;         // 乘法：(-5,10)
auto quot = a / b;         // 除法：(0.44,0.08)
auto neg = -a;             // 取负：(-1,-2)

// 比较（浮点数考虑精度误差）
bool eq = (a == Complex<double>(1,2));  // true
bool lt = (a < b);                      // 按模长比较
```

## 🌀 Quaternion 类

`Quaternion<T>` 用于表示 3D 旋转，避免万向锁问题，默认启用安全模式。

### 构造与访问
```cpp
#include <iostream>
#include "quaternion.h"

using namespace mathematics;

int main() {
    // 多种构造方式
    Quaternion<double> q1;                        // 单位四元数（1,0,0,0）
    Quaternion<double> q2(2.0);                   // 标量部分2，向量部分0
    Quaternion<double> q3(1.0, 2.0, 3.0, 4.0);    // 1 + 2i + 3j + 4k
    Quaternion<double> q4({1,2,3,4});             // 初始化列表构造（同q3）
    
    // 访问分量
    double scalar = q3.getReal();                  // 1.0（标量部分）
    double i_part = q3.getImaginaryI();            // 2.0（i分量）
    double j_part = q3.getImaginaryJ();            // 3.0（j分量）
    double k_part = q3.getImaginaryK();            // 4.0（k分量）
    
    return 0;
}
```

### 旋转操作
```cpp
// 创建旋转四元数（绕Y轴旋转90度）
double angle = pi/2;
Quaternion<double> rot_q(
    std::cos(angle/2),  // 标量部分
    0,                  // i分量
    std::sin(angle/2),  // j分量
    0                   // k分量
);

// 旋转向量（纯四元数表示向量）
Quaternion<double> v(0, 1, 0, 0);  // x轴向量（0 + 1i + 0j + 0k）
auto v_rotated = rot_q * v * rot_q.inverse();  // 旋转后向量（≈ 0 + 0i + 1j + 0k）

// 便捷旋转方法
auto v_rot = v.rotate(rot_q);                  // 同上
auto v_around = v.around(rot_q);               // 绕rot_q轴旋转
```

### 插值与其他方法
```cpp
Quaternion<double> q_start(1,0,0,0);  // 无旋转
Quaternion<double> q_end(0.707, 0.707,0,0);  // 绕X轴旋转90度

// 线性插值（LERP）
auto lerp_q = q_start.lerp(q_end, 0.5);

// 归一化线性插值（NLERP）
auto nlerp_q = q_start.nlerp(q_end, 0.5);

// 球面插值（SLERP，旋转插值首选）
auto slerp_q = q_start.slerp(q_end, 0.5);

// 角速度积分（物理模拟）
Quaternion<double> orientation = q_start;
Quaternion<double> ang_vel(0, 1, 0, 0);  // 绕X轴角速度
double dt = 0.016;                       // 时间步长
auto new_orient = orientation.integrate(ang_vel, dt);
```

### 运算符重载
```cpp
Quaternion<double> a(1,2,3,4), b(5,6,7,8);

auto sum = a + b;          // 分量相加
auto diff = a - b;         // 分量相减
auto prod = a * b;         // 四元数乘法（旋转组合）
auto quot = a / b;         // 除以b（等价于a*b.inverse()）
auto neg = -a;             // 取负
```

## 🔧 安全模式说明

所有核心函数与类方法均提供**可选的 `Safe` 模板参数**（默认启用）：
- **默认安全模式**：自动检查输入有效性（如除零、NaN、Inf），抛出异常防止数值错误
- **关闭安全模式**：跳过校验，最大化性能（适合已知输入安全的场景）

```cpp
// 默认安全模式（推荐）
double safe = divide(10.0, 0.0);  // 抛出 domain_error

// 关闭安全模式（高性能）
double fast = divide<false>(10.0, 0.0);  // 未定义行为（需确保输入安全）
```

## 📄 许可证

本项目采用 **Apache License 2.0** 开源协议。

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！请确保：
1. 代码符合 C++17 标准
2. 新功能包含单元测试
3. 保持 API 一致性和向后兼容性
4. 更新相关文档

---

**Mathematics** —— 让 C++ 数学计算更安全、更优雅、更高效。

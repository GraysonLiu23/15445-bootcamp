# 备注

## CMakeLists.txt CMAKE_CXX_COMPILER_ID

在 CMake 中，CMAKE_CXX_COMPILER_ID 用于标识当前使用的 C++ 编译器类型，例如 GNU、Clang、MSVC 等。这一变量在配置阶段由 CMake 自动检测，可用于根据编译器类型执行条件编译逻辑。

指定 CXX 语言编译器，需要设置 CXX 环境变量，CMake 将仅在首次配置时使用它来确定 CXX 编译器，之后 CXX 的值将存储在缓存中，作为 CMAKE_CXX_COMPILER。

export CXX="clang++"

> 储存在缓存中，要使得修改生效，需要删除当前的构建目录重新生成

## 使用 cmake 配置构建的方法

cmake -B build

会在当前目录下创建 build 目录作为构建目录

-B <path-to-build>           = Explicitly specify a build directory.

## 使用 cmake 执行构建的方法

cmake --build build

在 build 目录执行构建

## 移动语义 move semantics

转移所有权，比深拷贝效率要高

### 左值与右值 lvalue and rvalue

A simplified definition of lvalues is that lvalues are objects that refer to a location in memory. Rvalues are anything that is not a lvalue. 

左值，本质上是地址；右值，本质上是数据

举例：

```
int a = 10;
```

这里 `a` 是左值，`10` 是右值，`a` 是左值的意思就是它指代一个特定的内存空间，这个内存空间存储了一个数据（即 10）

`std::move` is the most common way of moving an object from one lvalue to another. `std::move` casts an expression to a rvalue. This allows for us to interact with a lvalue as a rvalue, and allows for the ownership to be transferred from one lvalue to another. 

> 要使用 `std::move`，引入头文件 #include <utility>

举例：

```
std::vector<int> int_array = {1, 2, 3, 4};
std::vector<int> stealing_ints = std::move(int_array);
```

int_array 是左值，经过 std::move 后变成右值引用（即数据 {1,2,3,4} 的引用，类型是 std::vector<int> &&），因为是右值，所以可以赋给其他左值（在这里赋给了 stealing_ints）

### 右值引用 rvalue reference

Rvalue references are references that refer to the data itself, as opposed to a lvalue. Calling std::move on a lvalue (such as stealing_ints) will result in the expression being cast to a rvalue reference.
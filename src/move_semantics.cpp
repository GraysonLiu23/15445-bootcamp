/**
 * @file move_semantics.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for move semantics.
 */

// Move semantics in C++ are a useful concept that allows for the efficient
// and optimized transfer of ownership of data between objects. One of the
// main goals of move semantics is to increase performance, since moving an
// object is faster and more efficient than deep copying the object.

// To understand move semantics, one must understand the concept of lvalues
// and rvalues. A simplified definition of lvalues is that lvalues are objects
// that refer to a location in memory. Rvalues are anything that is not a
// lvalue.

// std::move is the most common way of moving an object from one lvalue to
// another. std::move casts an expression to a rvalue. This allows for us to
// interact with a lvalue as a rvalue, and allows for the ownership to be
// transferred from one lvalue to another.

// In the code below, we include some examples for identifying whether
// expressions in C++ are lvalues or rvalues, how to use std::move, and passing
// rvalues references into functions.

// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the utility header for std::move.
#include <utility>
// Includes the header for std::vector. We'll cover vectors more in
// containers.cpp, but what suffices to know for now is that vectors are
// essentially dynamic arrays, and the type std::vector<int> is an array of
// ints. Mainly, vectors take up a non-negligible amount of memory, and are here
// to show the performance benefits of using std::move.
#include <vector>

// Function that takes in a rvalue reference as an argument.
// It seizes ownership of the vector passed in, appends 3 to
// the back of it, and prints the values in the vector.
void move_add_three_and_print(std::vector<int> &&vec) {
  std::vector<int> vec1 = std::move(vec);    // 需要注意，vec 的类型是右值引用，但是其变量本身是左值，需要再使用 std::move 后才能转为右值
  // 这里必需使用 std::move(vec)，若使用 std::vector<int> vec1 = vec; 则会发生拷贝构造
  // 对于接受右值引用参数的函数时，函数内部**必需**使用 std::move 来执行实际移动
  vec1.push_back(3);
  for (const int &item : vec1) {
    std::cout << item << " ";
  }
  std::cout << "\n";
}

// Function that takes in a rvalue reference as an argument.
// It appends 3 to the back of the vector passed in as an argument,
// and prints the values in the vector. Notably, it does not seize
// ownership of the vector. Therefore, the argument passed in would
// still be usable in the callee context.
void add_three_and_print(std::vector<int> &&vec) {
  vec.push_back(3);
  for (const int &item : vec) {
    std::cout << item << " ";
  }
  std::cout << "\n";
}

int main() {
  // Take this expression. Note that 'a' is a lvalue, since it's a variable that
  // refers to a specific space in memory (where 'a' is stored). 10 is a rvalue.
  int a = 10;

  // Let's see a basic example of moving data from one lvalue to another.
  // We define a vector of integers here.
  std::vector<int> int_array = {1, 2, 3, 4};

  // Now, we move the values of this array to another lvalue.
  std::vector<int> stealing_ints = std::move(int_array);

  // 问题：std::move 之后，还能够通过 int_array 访问数据吗？
  // std::cout << "Printing from int_array: " << int_array[1] << std::endl;
  // 不可以，会触发 Segmentation fault
  // 可以理解这种情况下数据 {1, 2, 3, 4} 的所有权已经从 int_array 转移给了 stealing_ints
  // 【本质：std::move 是做强制类型转换，将 std::vector<int> 转换为 std::vector<int> && 右值引用类型，相当于“请把我当成临时值处理”，接着，用 std::move(int_array) 初始化 stealing_ints 时候，会触发移动构造函数，移动构造函数会接受一个该类型的右值引用，类似于 vector& operator=(vector&& other)，在这个移动构造函数中，会窃取 other 的内部资源，如 data_ptr, size, capacity 等赋给当前对象，此外还有一步关键操作就是需要防止 other 指向原有对象析构重复释放内存，还需要将 other 置为空状态，例如 data_ptr 指向 nullptr，size, capacity 清零，这就是无法通过 int_array 访问数据的原因。但是这个 int_array 仍然是一个有效的 std::vector<int> 对象，可以继续使用，例如 int_array.clear(); int_array.push_back(10); 重新添加一个数据】
  // 附注：默认情况下，发生 SegFault 之后并不会生成 core 文件，需要先行设置，另外为了方便起见，core 文件最好生成在当前目录
  // ulimit -c unlimited
  // sudo sh -c 'echo "./core-%e-%p-%s" > /proc/sys/kernel/core_pattern'
  // 这样操作之后，会在当前目录生成名为 core-move_semantics-1262711-11 的文件
  // 使用 gdb 调试生成的 core 文件
  // gdb ./move_semantics core-move_semantics-1262711-11
  // 可以直接切换到段错误的发生现场和
  // 问题：为什么一定需要 core 文件？不使用 core 文件不是也能定位到发生端错误的地点吗？
  // 有时候崩溃并非稳定复现，如果不使用 core 文件，需要通过 run 命令触发程序执行，这要求崩溃必须能在调试时重现

  // Rvalue references are references that refer to the data itself, as opposed
  // to a lvalue. Calling std::move on a lvalue (such as stealing_ints) will
  // result in the expression being cast to a rvalue reference.
  std::vector<int> &&rvalue_stealing_ints = std::move(stealing_ints);

  // However, note that after this, it is still possible to access the data in
  // stealing_ints, since that is the lvalue that owns the data, not
  // rvalue_stealing_ints.
  std::cout << "Printing from stealing_ints: " << stealing_ints[1] << std::endl;
  // 见上面的解释，因为虽然使用了 std::move 做了转换，但是这个时候没有触发移动构造函数，stealing_ints 的资源还没有被窃取

  // It is possible to pass in a rvalue reference into a function. However,
  // once the rvalue is moved from the lvalue in the caller context to a lvalue
  // in the callee context, it is effectively unusable to the caller.
  // Essentially, after move_add_three_and_print is called, we cannot use the
  // data in int_array2. It no longer belongs to the int_array2 lvalue.
  std::vector<int> int_array2 = {1, 2, 3, 4};
  std::cout << "Calling move_add_three_and_print...\n";
  move_add_three_and_print(std::move(int_array2));
  // 该函数接受一个右值引用类型的参数，先使用 std::move 将 int_array2 转为右值引用类型，作为参数传递
  // 在 move_add_three_and_print 函数内，首先使用 std::vector<int> vec1 = std::move(vec);
  // 完成了所有权转换，vec 引用指向的 int_array2 被置空，进而无法在该函数内通过 vec 访问数据，也无法在 main 函数中通过 int_array2 访问数据

  // It would be unwise to try to do anything with int_array2 here. Uncomment
  // the code to try it out! (On my machine, this segfaults...) NOTE: THIS MIGHT
  // WORK FOR YOU. THIS DOES NOT MEAN THAT THIS IS WISE TO DO! 
  // std::cout << int_array2[1] << std::endl;

  // If we don't move the lvalue in the caller context to any lvalue in the
  // callee context, then effectively the function treats the rvalue reference
  // passed in as a reference, and the lvalue in this context still owns the
  // vector data.
  std::vector<int> int_array3 = {1, 2, 3, 4};
  std::cout << "Calling add_three_and_print...\n";
  add_three_and_print(std::move(int_array3));
  // 在 add_three_and_print 函数虽然接受的参数是一个右值引用 vec，但是在函数内没有赋给某个对象，也就没有进行所有权转换
  // 因此在函数内对右值引用 vec 的操作直接作用于 main 函数的 int_array3，换言之，这里就是当作一个正常的引用处理
  // 【注意】add_three_and_print 函数这样的实现不好，虽然语法没有错但是不符合通常的开发规范

  // As seen here, we can print from this array.
  std::cout << "Printing from int_array3: " << int_array3[1] << std::endl;

  return 0;
}

/**
 * @file vectors.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for C++ Standard Library (STL) vectors.
 */

// The C++ STL contains a container library, which is a generic collection of
// data structure and algorithm implementations that allow users to manipulate
// data structures like stacks, queues, and hash tables easily. Each container
// has its own header and usage. In the C++ standard (up to C++ 23), there are
// currently 20 containers, which is far too many to cover here well. In this
// file, we will introduce the container std::vector. The std::vector container
// is essentially a generic dynamic array (or unbounded array). We won't be
// able to cover every function in this container, but we will try to cover the
// basics of using this container.

// There is documentation on all the other functions, and other containers on
// https://en.cppreference.com/w/cpp/container. You will definitely need this
// resource as you complete the assignments in this class, so you should check
// it out!

// Includes std::remove_if to remove elements from vectors.
#include <algorithm>
// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the vector container library header.
#include <vector>

// Basic point class. (Will use later)
class Point {
public:
  Point() : x_(0), y_(0) {
    std::cout << "Default constructor for the Point class is called.\n";
  }

  Point(int x, int y) : x_(x), y_(y) {
    std::cout << "Custom constructor for the Point class is called.\n";
  }

  inline int GetX() const { return x_; }
  inline int GetY() const { return y_; }
  inline void SetX(int x) { x_ = x; }
  inline void SetY(int y) { y_ = y; }
  void PrintPoint() const {
    std::cout << "Point value is (" << x_ << ", " << y_ << ")\n";
  }

private:
  int x_;
  int y_;
};

// A utility function to print the elements of an int vector. The code for this
// should be understandable and similar to the code iterating through elements
// of a vector in the main function.
// 【注意】这个函数的形参，其类型是常量左值引用类型，它可以接受左值也能接受右值的实参
// print_int_vector(v); OK
// print_int_vector({1, 2, 3, 4}); OK，接受能够被转为 std::vector<int> 类型的值（会创建临时对象，纯右值）
// print_int_vector(get_vector()); OK，接受某个函数返回的临时对象（纯右值）
void print_int_vector(const std::vector<int> &vec) {
  for (const int &elem : vec) {
    std::cout << elem << " ";
  }
  std::cout << "\n";
}

std::vector<int> get_vector() {
  return std::vector<int>{1,2,3,4};
}

int main() {
  std::vector<int> v{1,2,3,4};
  print_int_vector(v);

  auto a = get_vector();
  print_int_vector(std::move(a));

  // We can declare a Point vector with the following syntax.
  std::vector<Point> point_vector;

  // It is also possible to initialize the vector via an initializer list.
  std::vector<int> int_vector = {0, 1, 2, 3, 4, 5, 6};
  // 注：两种列表初始化方法 复制列表初始化 直接列表初始化 本质相同
  // std::vector<int> int_vector {0, 1, 2, 3, 4, 5, 6}; 这个也是可以的，但是类型检查会更严格，禁止窄化转换

  // There are two functions for appending data to the back of the vector. They
  // are push_back and emplace_back. Generally, emplace_back is slightly faster,
  // since it forwards the constructor arguments to the object's constructor and
  // constructs the object in place, while push_back constructs the object, then
  // moves it to the memory in the vector. We can see this here where we add two
  // Point objects to our vector.
  std::cout << "Appending to the point_vector via push_back:\n";
  point_vector.push_back(Point(35, 36));
  std::cout << "Appending to the point_vector via emplace_back:\n";
  point_vector.emplace_back(37, 38);
  // emplace_back 直接传入 vector 容器元素构造函数的参数，直接在容器内原地构造
  // push_back 先构造一个元素（临时对象），接着移动到容器内（如果这个对象有资源，必须实现移动构造函数与移动赋值函数）
  // 从这个角度来说 emplace_back 更快一些

  // Let's just add more items to the back of our point_vector.
  point_vector.emplace_back(39, 40);
  point_vector.emplace_back(41, 42);

  // There are many ways to iterate through a vector. For instance, we can
  // iterate through it's indices via the following for loop. Note that it is
  // good practice to use an unsigned int type for array or vector indexes.
  std::cout << "Printing the items in point_vector:\n";
  for (size_t i = 0; i < point_vector.size(); ++i) {
    point_vector[i].PrintPoint();
  }

  // We can also iterate through it via a for-each loop. Note that I use
  // references to iterate through it so that the items we iterate through are
  // the items in the original vector. If we iterate through references of the
  // vector elements, we can also modify the data in the vector.
  for (Point &item : point_vector) {
    item.SetY(445);
  }

  // Let's see if our changes went through. Note that I use the const reference
  // syntax to ensure that the data I'm accessing is read only.
  for (const Point &item : point_vector) {
    item.PrintPoint();
  }

  // Now, we show how to erase elements from a vector. First, we can erase
  // elements by their position via the erase function. For instance, if we want
  // to delete int_vector[2], we can call the following function with the
  // following arguments. The argument passed into this erase function has
  // the type std::vector<int>::iterator. An iterator for a C++ STL container
  // is an object that points to an element within the container. For instance,
  // int_vector.begin() is an iterator object that points to the first element
  // in the vector. The vector iterator also has a plus operator that takes
  // a vector iterator and an integer. The plus operator will increase the 
  // index of the element that the iterator is pointing to by the number passed
  // in. Therefore, int_vector.begin() + 2 is pointing to the third element in
  // the vector, or the element at int_vector[2].
  // If you are confused about iterators, it may be helpful to read the header of
  // iterator.cpp.
  int_vector.erase(int_vector.begin() + 2);
  // int_vector.begin() 返回的是一个 std::vector<int>::interator 迭代器对象，迭代器对象本质是一个指向 vector 中某一个元素的指针，这里返回的是指向第一个元素的迭代器
  // 这个迭代器类型重载了 + 运算符，因此能够通过 +2 实现迭代器往后推进 2 个位置的操作
  std::cout << "Printing the elements of int_vector after erasing "
               "int_vector[2] (which is 2)\n";
  print_int_vector(int_vector);

  // We can also erase elements in a range via the erase function. If we want to
  // delete elements starting from index 1 to the end of the array, then we can
  // do so the following. Note that int_vector.end() is an iterator pointing to
  // the end of the vector. It does not point to the last valid index of the
  // vector. It points to the end of a vector and cannot be accessed for data.
  int_vector.erase(int_vector.begin() + 1, int_vector.end());
  std::cout << "Printing the elements of int_vector after erasing all elements "
               "from index 1 through the end\n";
  print_int_vector(int_vector);

  // We can also erase values via filtering, i.e. erasing values if they meet a
  // conditional. We can do so by importing another library, the algorithm
  // library, which gives us the std::remove_if function, which removes all
  // elements meeting a conditional from an iterator range. This does seem
  // awfully complicated, but the code can be summarized as follows.
  // std::remove_if takes in 3 arguments. Two of those arguments indicate the
  // range of elements that we should filter. These are given by
  // point_vector.begin() and point_vector.end(), which are iterators that point
  // to the beginning and the end of a vector respectively. Therefore, when we
  // pass these in, we are implying that we want the whole vector filtered.
  // The third argument is a conditional lambda type (see the std::function
  // library in C++, or at 
  // https://en.cppreference.com/w/cpp/utility/functional/function), that takes
  // in one argument, which is supposed to represent each element in the vector
  // that we are filtering. This function should return a boolean that is true
  // if the element is to be filtered out and false otherwise. std::remove_if
  // returns an iterator pointing to the first element in the container that
  // should be eliminated. Keep in mind that it swaps elements as needed,
  // partitioning the elements that need to be deleted after the iterator value
  // it returns. When erase is called, it deletes only the elements that
  // remove_if has partitioned away to be deleted, up to the end of the vector.
  // This outer erase takes a range argument, as we saw in the previous example.
  // remove_if 函数**不会**真正删除元素，而是将要删除的元素移动到容器末尾，并返回一个指向新范围末尾的迭代器
  // 例如，当前容器 [o x o x o o]，需要删除其中 x 的元素，经过 remove_if 处理之后，容器会变为 [o o o o x x]
  // 这就是将元素划分（partition）成两部分了
  // 该函数返回指向容器内下标为 4 的元素的迭代器，接着作为 erase 函数的第一个参数，表示从这里开始删除直到最后一个
  point_vector.erase(
      std::remove_if(point_vector.begin(), point_vector.end(),
                     [](const Point &point) { return point.GetX() == 37; }),
      point_vector.end());
  // Lambda 表达式          | captures 子句：在函数体中引入/捕获新的变量，可以加&前缀引用访问或者值访问，[] 表示不捕获
  //                         |                 | 参数列表 |                            | Lambda 函数体
  // After calling remove here, we should see that three elements remain in our
  // point vector. Only the one with value (37, 445) is deleted.
  std::cout << "Printing the point_vector after (37, 445) is erased:\n";
  for (const Point &item : point_vector) {
    item.PrintPoint();
  }

  // We discuss more stylistic and readable ways of iterating through C++ STL
  // containers in auto.cpp! Check it out if you are interested.

  return 0;
}

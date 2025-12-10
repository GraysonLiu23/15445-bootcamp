#include <cstddef>
#include <iostream>

struct Node {
  Node(int val) : next_(nullptr), prev_(nullptr), value_(val) {}

  Node *next_;
  Node *prev_;
  int value_;
};

class DLL {
public:
  DLL() : head_(nullptr), size_(0) {}

  ~DLL() {
    Node *current = head_;
    while (current != nullptr) {
      Node *next = current->next_;
      delete current;
      current = next;
    }
    head_ = nullptr;
  }

  // 1. 嵌套实现一个迭代器类，它维护一个数据成员用于指向可迭代对象的一个元素
  // 在这个类中，需要实现：a. 重载前缀++运算符，用于推进迭代器，b. 重载!=运算符，用于判断是否到达可迭代对象的末尾
  // c. 重载*解引用运算符，进而通过迭代器对象获得其指向的可迭代对象的对应元素
  class DLLIterator {
  public:
    explicit DLLIterator(Node *head) : curr_(head) {}

    DLLIterator &operator++() {
      curr_ = curr_->next_;
      return *this;
    }

    bool operator!=(const DLLIterator &itr) const {
      return itr.curr_ != this->curr_;
    }

    int operator*() { return curr_->value_; }

    DLLIterator &operator+(size_t offset) {
      for (size_t i = 0; i < offset; ++i) {
        curr_ = curr_->next_;
      }
      return *this;
    }

  private:
    Node *curr_;
  };

  // 2. 在可迭代类中，定义 begin() 方法与 end() 方法，这两个方法返回一个迭代器对象，分别指向开头元素与最后一个元素之后位置（不指向任何元素，仅作为迭代结束的标志）
  DLLIterator begin() { return DLLIterator(head_); }

  DLLIterator end() { return DLLIterator(nullptr); }

  void InsertAtHead(int val) {
    Node *new_node = new Node(val);
    new_node->next_ = head_;

    if (head_ != nullptr) {
      head_->prev_ = new_node;
    }

    head_ = new_node;
    size_ += 1;
  }

private:
  Node *head_{nullptr};
  size_t size_;
};

int main() {
  DLL dll;
  dll.InsertAtHead(6);
  dll.InsertAtHead(5);
  dll.InsertAtHead(4);
  dll.InsertAtHead(3);
  dll.InsertAtHead(2);
  dll.InsertAtHead(1);

  std::cout << "Using range-based for statement\n";
  for (const auto &item : dll) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  std::cout << "Using iterator\n";
  for (DLL::DLLIterator iter = dll.begin(); iter != dll.end(); ++iter) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;

  std::cout << "Test the overload plus operator\n";
  DLL::DLLIterator iter = dll.begin();
  std::cout << "The first element: " << *iter << "\n";
  iter = iter + 2;
  std::cout << "The third element: " << *iter << "\n";

  return 0; 
}
### 1.1 了解Visitor Pattern

Visitor Pattern(访问者模式)是一种在LLVM项目源码中被广泛使用的设计模式。在遍历某个数据结构（比如树）时，如果我们需要对每个节点做一些额外的特定操作，Visitor Pattern就是个不错的思路。  
Visitor Pattern是为了解决**稳定的数据结构**和**易变的操作耦合问题**
而产生的一种设计模式。解决方法就是在被访问的类里面加一个对外提供接待访问者的接口，其关键在于在数据基础类里面有一个方法接受访问者，将自身引用传入访问者。这里举一个应用实例来帮助理解访问者模式:
您在朋友家做客，您是访问者；朋友接受您的访问，您通过朋友的描述，然后对朋友的描述做出一个判断，这就是访问者模式。  
有关 Visitor Pattern 的含义、模式和特点，有梯子的同学可参考[维基百科](https://en.wikipedia.org/wiki/Visitor_pattern#C++_example) 。  
下面的例子可以清晰地展示Visitor Pattern的运作方式。这是助教编写的计算表达式 `4 * 2 - 2 / 4 + 5` 结果的C++程序。  
其中较为重要的一点原则在于，C++中对函数重载特性的支持。在代码`treeVisitor.visit(node)`中，根据`node`对象具体类型的不同，编译器会在`visit(AddSubNode& node)`
、`visit(NumberNode& node)`、`visit(MulDivNode& node)`
三者中，选择对应的实现进行调用。你需要理解下面这个例子中tree是如何被遍历的。请在[report.md](./Reports/lab3/report.md)中**回答问题2**。

<details>
  <summary>例子:简单的表达式计算 - visitor.cpp</summary>

``` c++
#include <iostream>
#include <vector>

class TreeVisitor;  // Forward declare TreeVisitor

class Node {  // Parent class for the elements (AddSubNode, NumberNode and
              // MulDivNode)
 public:
  // This function accepts an object of any class derived from
  // TreeVisitor and must be implemented in all derived classes
  virtual int accept(TreeVisitor& treeVisitor) = 0;
};

// Forward declare specific elements (nodes) to be dispatched
class AddSubNode;
class NumberNode;
class MulDivNode;

class TreeVisitor {  // Declares the interface for the treeVisitor
 public:
  // Declare overloads for each kind of a node to dispatch
  virtual int visit(AddSubNode& node) = 0;
  virtual int visit(NumberNode& node) = 0;
  virtual int visit(MulDivNode& node) = 0;
};

class AddSubNode : public Node {  // Specific element class #1
 public:
  // Resolved at runtime, it calls the treeVisitor's overloaded function,
  // corresponding to AddSubNode.
  int accept(TreeVisitor& treeVisitor) override {
    return treeVisitor.visit(*this);
  }
  Node& leftNode;
  Node& rightNode;
  std::string op;
  AddSubNode(Node& left, Node& right, std::string op): leftNode(left), rightNode(right), op(op){}
};

class NumberNode : public Node {  // Specific element class #2
 public:
  // Resolved at runtime, it calls the treeVisitor's overloaded function,
  // corresponding to NumberNode.
  int accept(TreeVisitor& treeVisitor) override {
    return treeVisitor.visit(*this);
  }
  int number;
  NumberNode(int number){
    this->number = number;
  }
};

class MulDivNode : public Node {  // Specific element class #3
 public:
  // Resolved at runtime, it calls the treeVisitor's overloaded function,
  // corresponding to MulDivNode.
  int accept(TreeVisitor& treeVisitor) override {
    return treeVisitor.visit(*this);
  }
  Node& leftNode;
  Node& rightNode;
  std::string op;
  MulDivNode(Node& left, Node& right, std::string op): leftNode(left), rightNode(right), op(op){}
};

class TreeVisitorCalculator : public TreeVisitor {  // Implements triggering of all
                                                // kind of elements (nodes)
 public:
  int visit(AddSubNode& node) override {
    auto right = node.rightNode.accept(*this);
    auto left = node.leftNode.accept(*this);
    if (node.op == "add") {
      return left + right;
    } 
    else {
      return left - right;
    }
  }

  int visit(NumberNode& node) override {
    return node.number;
  }

  int visit(MulDivNode& node) override {
    auto left = node.leftNode.accept(*this);
    auto right = node.rightNode.accept(*this);
    if (node.op == "mul") {
      return left * right;
    } 
    else {
      return left / right;
    }
  }
};

int main() {
  // construct the expression nodes and the tree
  // the expression: 4 * 2 - 2 / 4 + 5
  auto numberA = NumberNode(4);
  auto numberB = NumberNode(2);
  auto exprC = MulDivNode(numberA, numberB, "mul");
  auto exprD = MulDivNode(numberB, numberA, "div");
  auto exprE = AddSubNode(exprC, exprD, "sub");
  auto numberF = NumberNode(5);
  auto exprRoot = AddSubNode(exprE, numberF, "add");

  TreeVisitorCalculator treeVisitor;
  // traverse the tree and calculate
  int result = treeVisitor.visit(exprRoot);
  std::cout << "4 * 2 - 2 / 4 + 5 evaluates: " << result << std::endl;
  return 0;
}
```
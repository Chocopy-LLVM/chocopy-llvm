### 1.1 Understanding the Visitor Pattern

Visitor Pattern is a design pattern that is widely used in LLVM project source code. When traversing some data structure (like a tree), if we need to do some extra specific operations on each node, Visitor Pattern is a good way to think about it.  
Visitor Pattern is a design pattern created to solve the problem of coupling **stable data structures** and **variable operations**.
The Visitor Pattern is a design pattern created to solve the problem of coupling **stable data structures** and **variable operations**. The solution is to add an interface inside the visited class to provide an external reception for visitors. The key is to have a method inside the data base class to accept visitors and pass its own reference to the visitor. Here is an application example to help understand the visitor pattern:
You are a guest at a friend's house and you are the visitor; the friend accepts your visit, you go through the friend's description, and then make a judgment about the friend's description, which is the Visitor Pattern.  
For more information about the meaning, patterns and characteristics of Visitor Pattern, students with ladders can refer to [Wikipedia](https://en.wikipedia.org/wiki/Visitor_pattern#C++_example).  
The following example clearly shows how the Visitor Pattern works. This is a C++ program written by an assistant teacher to compute the result of the expression `4 * 2 - 2 / 4 + 5`.  
One of the more important principles is the support for the function overloading feature in C++. In the code `treeVisitor.visit(node)`, depending on the specific type of the `node` object, the compiler will overload the function `visit(AddSubNode& node)`
, `visit(NumberNode& node)`, `visit(MulDivNode& node)`
Of the three, choose the corresponding implementation to call. You need to understand how the tree is traversed in this example below. Please find it in [report.md](./Reports/lab3/report.md) **answer question 2**.

<details
  <summary>Example:Simple expression calculation - visitor.cpp</summary>

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
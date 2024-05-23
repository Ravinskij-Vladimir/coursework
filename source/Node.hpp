#ifndef NODE_HPP
#define NODE_HPP
#include <cstddef>
#include <memory>

namespace ravinskij
{
  struct Node;

  using nodePtr = std::shared_ptr<Node>;
  using memberPtr = std::shared_ptr<Node>;
  struct Node
  {
    size_t frequency;
    char symbol;
    memberPtr left, right;

    Node() = default;
    Node(const Node &) = default;
    Node(Node &&) = default;
    Node &operator=(const Node &) = default;
    Node &operator=(Node &&) = default;
    ~Node() = default;

    Node(const memberPtr& leftNode, const memberPtr& rightNode);
  private:  
  };
  
  struct NodeComparator
  {
    bool operator()(const memberPtr& lhs, const memberPtr& rhs) const;
  };
}

#endif
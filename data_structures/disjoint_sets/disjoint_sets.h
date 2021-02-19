#include <iostream>
#include <unordered_map>
#include <vector>

class Node {
public:
  int data;
  int parent;
  int rank;
  Node(int d);
  Node();
};
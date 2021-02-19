#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Node
{
	public:
	int data;
	int parent;
    int rank;
  Node(int d)
 {
   data = d;
   rank = 0;
   parent  = d;
   cout<<parent;
 }
 Node()
 {
	 data = 0;
	 rank = 0;
	 parent = 0;
 }
};

std::unordered_map<int,Node> closed_list;

int Find(int d)
{
	Node current = closed_list[d];
	if(current.parent ==d)
	{
		return current.parent;
	}
	current.parent= Find(current.parent);
	closed_list[current.data]= current;
	return current.parent;
}

void Union(int d1,int d2)
{
	Node node1 = closed_list[d1];
	Node node2 = closed_list[d2];
	
	int par1 = Find(node1.parent);
	int par2 = Find(node2.parent);

    Node parent1 = closed_list[par1];
	Node parent2 = closed_list[par2];
	if(parent1.data== parent2.data)
	{
		return;
	}
	
	if(parent1.rank > parent2.rank)
	{
		parent2.parent = parent1.parent;
	}
	else if(parent1.rank < parent2.rank)
	{
		parent1.parent = parent2.parent;
	}
	else
	{
		parent1.parent = parent2.parent;
		parent2.rank++;
	}
	closed_list[parent1.data]= parent1;
	closed_list[parent2.data]= parent2;
}


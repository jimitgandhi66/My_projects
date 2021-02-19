#ifndef TREE_H
#define TREE_H
struct TreeNode
{
  int key;
  TreeNode *left;
  TreeNode *right;
};
class Tree
{
public:
    Tree();
    ~Tree();
        void insert(int key);
        TreeNode *search(int key);
        void destroy_tree();
private:
        void destroy_node(TreeNode *leaf);
        void insert_key(int key, TreeNode *leaf);
        TreeNode *search(int key, TreeNode *leaf);
        
        TreeNode *root;
};


#endif // TREE_H

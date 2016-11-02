#include <iostream>
#include <cstdlib>
#include <Tree.h>



/**
 * @brief Constructor
 * @return tree root
 */
Tree::Tree()
{
    //TreeNode *root = new TreeNode;
    root=NULL;
}
/**
 * @brief destructor
 * @return NULL
 */
void Tree::destroy_tree()
{
   destroy_node(root);
}

/**
 * @brief inserts key in the tree
 * @param key- element you want to insert
 */
void Tree::insert(int key)
{
    if (root!=NULL)
    {
    insert_key(key, root);
    }
    else
    {
        root->key = key;
        root->left = NULL;
        root->right = NULL;
    }
}

/*
 * Public functions
 */
void Tree::insert_key(int key, TreeNode *leaf)
{
    
    if (leaf->key > key)
    {
        if(leaf->left !=NULL)
        {
            insert_key(key,leaf->left);
        }
        else 
        {
         leaf->left->key = key;
         leaf->left->left = NULL;
         leaf->left->right = NULL;
        }
    }
    else 
    {
        if (leaf->right!=NULL)
        {
           insert_key(key,leaf->right); 
        }
        else
        {
            leaf->right->key = key;
            leaf->right->left=NULL;
            leaf->right->right = NULL;
        }
    }
}
void Tree::destroy_node(TreeNode *leaf)
{
   if (leaf!= NULL)
   {
       destroy_node(leaf->left);
       destroy_node(leaf->right);
       delete leaf;
   }
   else 
       delete leaf;
}

TreeNode* Tree::search(int key, TreeNode *leaf)
{
    if (leaf == NULL)
    {
        return NULL;
    }
    else
    {
        if (leaf->key > key)
        {
            return search(key, leaf->left);
        }
        else if(leaf->key==key)
        {
           return leaf;
        }
        else
        {
            return  search(key,leaf->right);
        }
        
    }
}

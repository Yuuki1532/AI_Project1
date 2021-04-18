#include "TreeNode.hpp"
#include "ObjectPool.hpp"

class SearchTree{
public:
    SearchTree(Board, int, int, int, int);

    Move search();



    ~SearchTree();

private:
    ObjectPool<TreeNode> pool; // object pool to reuse dynamically allocated TreeNode(s)

    TreeNode *root;
    int nodesExpanded = 0;


    TreeNode* select();
    // ? expand(TreeNode *leaf);
    //int rollout(TreeNode *leaf);
    // void update(TreeNode* leaf, int winner);

};
#ifndef MCTS_SEARCH_TREE_H
#define MCTS_SEARCH_TREE_H

#include "TreeNode.hpp"
#include "ObjectPool.hpp"

class SearchTree{
public:
    SearchTree(Board&, int, int, int, int);

    Move search(int);

    ~SearchTree();

private:
    ObjectPool<TreeNode> pool; // object pool to reuse dynamically allocated TreeNode(s)

    TreeNode *root;
    int nodesExpanded = 0;
    const int limit_nodesLimit = 12000000; // nodes limit

    // random number generation related
    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_int_distribution<int> dist;
    int randomInt(const int, const int);

    double getTreeNodeWeight(const TreeNode* const&) const; // selection strategy
    void recycleTreeNodes(TreeNode*);

    // MCTS operations
    TreeNode* select() const;
    int expand(TreeNode*);
    int rollout(TreeNode*);
    void update(TreeNode*, int);
};

#endif

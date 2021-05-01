#ifndef MCTS_SEARCH_TREE_H
#define MCTS_SEARCH_TREE_H

#include "TreeNode.hpp"
#include "ObjectPool.hpp"

class SearchTree{
public:
    SearchTree(const std::vector<std::vector<int> >&, const int, const int, const int, const int);

    Move search(const int);

    ~SearchTree();

private:
    ObjectPool<TreeNode> pool; // object pool to reuse dynamically allocated TreeNode(s)

    TreeNode *root;
    int nodesExpanded = 0;
    const int limit_nodesLimit = 1e9; // nodes limit

    // random number generation related
    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_int_distribution<int> dist;
    int randomInt(const int, const int);

    double getTreeNodeWeight(const TreeNode* const&) const; // selection strategy
    void recycleTreeNodes(TreeNode* const&);

    // MCTS operations
    TreeNode* select() const;
    int expand(TreeNode* const&);
    int rollout(const TreeNode* const&);
    void update(TreeNode* const&, const int);
};

#endif

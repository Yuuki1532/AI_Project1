#include "MCTS.hpp"

SearchTree::SearchTree(Board board, int selfColor, int opponentColor, int selfBudget, int opponentBudget){
    // TODO: ask TAs for RAM limitation in execute environment
    // assume we cannot occupied large memory when idling
    // we must build the object pool here
    pool = ObjectPool<TreeNode>();

    // first create root node according to the given game status (parameters)
    root = pool.pop();
    root->resetData(board, selfColor, opponentColor, selfBudget, opponentBudget, nullptr);

    return;
}
#include "MCTS.hpp"
#include <cmath>

SearchTree::SearchTree(Board& board, int selfColor, int opponentColor, int selfBudget, int opponentBudget){
    // TODO: ask TAs for RAM limitation in execute environment
    // assume we cannot occupied large memory when idling
    // we must build the object pool here
    pool = ObjectPool<TreeNode>();

    // first create root node according to the given game status (parameters)
    root = pool.pop();
    root->resetData(board, selfColor, opponentColor, selfBudget, opponentBudget, nullptr);
    
    return;
}

double SearchTree::getTreeNodeWeight(const TreeNode* const& treeNode) const{
    // calculate the weight of `treeNode` for selection
    // weights are calculated by UCB1 formula

    if (treeNode->visits == 0)
        return std::sqrt(2) * std::sqrt(std::log(treeNode->parent->visits));
    return treeNode->value / treeNode->visits + std::sqrt(2) * std::sqrt(std::log(treeNode->parent->visits) / treeNode->visits);
}

Move SearchTree::getTreeNodeRandomMove(const TreeNode *node){
    // generate a random nubmer uniformly distributed in [0, node->validMoves.size() - 1] by RNG `gen`
    // and return the correspoding move
    // node->validMoves must guaranteed to have at least one element
    int randomIndex = dist(gen, std::uniform_int_distribution<int>::param_type(0, node->validMoves.size() - 1));
    return node->validMoves[randomIndex];
}

TreeNode* SearchTree::select() const{
    TreeNode *current = root;

    while (!current->child.empty()){
        // find the child node with highest weight
        // repeat until leaf reached
        TreeNode *maxNode = nullptr;
        double maxWeight = -1e9;
        
        for (const auto& node: current->child){
            auto nodeWeight = getTreeNodeWeight(node);

            if (nodeWeight > maxWeight) {
                maxNode = node;
                maxWeight = nodeWeight;
            }
        }
        current = maxNode;
    }
    
    return current;
}

int SearchTree::expand(TreeNode *leaf) {
    // expand `leaf` by calling its `setValidMoves()` several times
    // note: `validMoves` of `leaf` may be modified in this function
    // please enusre `validMoves` of `leaf` is set correctly before using

    // set valid moves
    leaf->setValidMoves();
    
    // self has no valid moves, check if opponent has any
    if (leaf->validMoves.empty()){
        std::swap(leaf->selfColor, leaf->opponentColor);
        std::swap(leaf->selfBudget, leaf->opponentBudget);
        
        leaf->setValidMoves();
    }

    // record amount of expanded nodes
    nodesExpanded += leaf->validMoves.size();

    // expand all valid moves
    for (const auto& move: leaf->validMoves) {
        auto newChild = pool.pop();
        newChild->resetData(leaf->board, leaf->selfColor, leaf->opponentColor, leaf->selfBudget, leaf->opponentBudget, leaf);
        newChild->step(move);
        leaf->child.push_back(newChild);
    }
    
    return leaf->validMoves.size();
}

int SearchTree::rollout(TreeNode* leaf){
    // rollout `leaf` and return the winner

    // make a copy of `leaf` for simulation
    auto simulateNode = pool.pop();
    simulateNode->resetData(leaf->board, leaf->selfColor, leaf->opponentColor, leaf->selfBudget, leaf->opponentBudget, nullptr);
    
    while (1){ // play randomly until game ends
        
        // set valid moves
        simulateNode->setValidMoves();
        if (simulateNode->validMoves.empty()){ // check if opponent has any valid move
            std::swap(simulateNode->selfColor, simulateNode->opponentColor);
            std::swap(simulateNode->selfBudget, simulateNode->opponentBudget);
            
            simulateNode->setValidMoves();

            if (simulateNode->validMoves.empty()){ // both players have no valid moves
                // find winner
                int winner = simulateNode->getWinner();
                pool.push(simulateNode); // recycle simulateNode
                return winner;
            }
            
        }

        // play randomly
        const auto& randomMove = getTreeNodeRandomMove(simulateNode);
        simulateNode->step(randomMove);
    }

    pool.push(simulateNode); // recycle simulateNode
    return -1; // error (to avoid having a servere runtime error, we treat this unexpected error as draw)
}

void SearchTree::update(TreeNode* current, int winner){
    // backpropagate the (rollout) result
    // please refer to `TreeNode` for what `value` of `leaf` means and how it is calculated

    int selfColorOfParentNode; // leaf->parent->selfColor takes an action and transits to `leaf`
    int opponentColorOfParentNode;

    if (current->parent){
        selfColorOfParentNode = current->parent->selfColor;
        opponentColorOfParentNode = current->parent->opponentColor;
    }
    else{ // for simplicity, root case
        selfColorOfParentNode = current->selfColor;
        opponentColorOfParentNode = current->opponentColor;
    }
    
    // update visits and value
    current->visits++;
    if (winner == selfColorOfParentNode)
        current->value++;
    else if (winner == opponentColorOfParentNode)
        current->value--;

    if (current->parent) // update parent recursively
        update(current->parent, winner);

    return;
}

void SearchTree::recycleTreeNodes(TreeNode *node){
    // push the whole tree nodes used back to the pool by DFS traversal
    for (auto c: node->child){
        recycleTreeNodes(c);
    }
    pool.push(node);
    return;
}

SearchTree::~SearchTree(){
    recycleTreeNodes(root);
    return;
}

Move SearchTree::search(){
    // TODO
}
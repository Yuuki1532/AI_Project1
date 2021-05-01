#include "MCTS.hpp"
#include <cmath>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

SearchTree::SearchTree(const std::vector<std::vector<int> >& board, const int selfColor, const int opponentColor, const int selfBudget, const int opponentBudget){
    // TODO: ask TAs for RAM limitation in execute environment
    // assume we cannot occupied large memory when idling
    // we must build the object pool here
    
    // first create root node according to the given game status (parameters)
    root = pool.pop();

    // create a 2d int array for resetData()
    int tmpBoard[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            tmpBoard[i][j] = board[i][j];
        }
    }

    root->resetData(tmpBoard, selfColor, opponentColor, selfBudget, opponentBudget, nullptr);
    
    return;
}

double SearchTree::getTreeNodeWeight(const TreeNode* const& treeNode) const{
    // calculate the weight of `treeNode` for selection
    // weights are calculated by UCB1 formula
    return 1.0 * treeNode->value / treeNode->visits + std::sqrt(2) * std::sqrt(std::log(treeNode->parent->visits) / treeNode->visits);
}

int SearchTree::randomInt(const int min, const int max){
    // generate a random nubmer uniformly distributed in [min, max] by RNG `gen`
    return dist(gen, std::uniform_int_distribution<int>::param_type(min, max));
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

int SearchTree::expand(TreeNode* const& leaf) {
    // expand `leaf` by calling its `setValidMoves()` several times
    // note: `validMoves` of `leaf` may be modified in this function
    // please enusre `validMoves` of `leaf` is set correctly before using

    // if nodes have already expanded more than `nodes_limit`, stop expanding anymore
    if (nodesExpanded >= limit_nodesLimit)
        return 0;

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

int SearchTree::rollout(const TreeNode* const& leaf){
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
        int randomIndex = randomInt(0, simulateNode->validMoves.size() - 1);
        const auto& randomMove = simulateNode->validMoves[randomIndex];
        simulateNode->step(randomMove);
    }

    pool.push(simulateNode); // recycle simulateNode
    return -1; // error (to avoid having a servere runtime error, we treat this unexpected error as draw)
}

void SearchTree::update(TreeNode* const& current, const int winner){
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

void SearchTree::recycleTreeNodes(TreeNode* const& node){
    // push the whole tree nodes used back to the pool by DFS traversal
    for (const auto& c: node->child){
        recycleTreeNodes(c);
    }
    pool.push(node);
    return;
}

SearchTree::~SearchTree(){
    recycleTreeNodes(root);
    return;
}

Move SearchTree::search(const int _timeLimit){
    // search for a best move with `_timeLimit` seconds limitation
    const auto startTime = std::chrono::steady_clock::now(); // start time
    const std::chrono::seconds timeLimit(_timeLimit); // duration
    
    nodesExpanded = 0;
    int iters = 0;

    root->setValidMoves();
    if (root->validMoves.empty()){
        return Move(-1, -1, -1, -1);
    }

    // simulate until time limit
    while (1){
        if (iters % 10000 == 0)
            if (std::chrono::steady_clock::now() - startTime >= timeLimit)
                break;
        
        iters++;

        auto leaf = select();
        if (leaf->visits > 1){ // visited, expand it and randomly pick one of its child if any
            if (expand(leaf) > 0){
                int randomIndex = randomInt(0, leaf->child.size() - 1);
                leaf = leaf->child[randomIndex];
            }
        }

        int winner = rollout(leaf);
        update(leaf, winner);
    }

    // find the best node
    long long valueOfBestNode = -1e9;
    long long visitsOfBestNode = -1e9;
    Move bestMove;

    for (unsigned int i = 0; i < root->validMoves.size(); i++){
        if (root->child[i]->visits > visitsOfBestNode){
            bestMove = root->validMoves[i];
            visitsOfBestNode = root->child[i]->visits;
            valueOfBestNode = root->child[i]->value;
        }
        else if (root->child[i]->visits == visitsOfBestNode){
            if (root->child[i]->value > valueOfBestNode){
                bestMove = root->validMoves[i];
                visitsOfBestNode = root->child[i]->visits;
                valueOfBestNode = root->child[i]->value;
            }
        }
    }

    // output some statistics
    std::cout << ((root->selfColor == color_black)? "Black": "White") << "'s turn"
              << "\tTime: " << std::setprecision(4) << std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() << " sec"
              << "\nNodes expanded: " << nodesExpanded << "\tIters: " << iters
              << "\nBest move weight (max visited): " << 1.0 * valueOfBestNode / visitsOfBestNode << " (" << valueOfBestNode << "/" << visitsOfBestNode << ")"
              // << "\nMax visits: " << visitsOfBestNode << " (" << valueOfBestNode << "/" << visitsOfBestNode << ")"
              << std::endl;

    return bestMove;
}


#include "TreeNode.hpp"
#include <cmath>

void TreeNode::resetData(const Board& board, const int selfColor, const int opponentColor, const int selfBudget, const int opponentBudget, TreeNode* const parent){
    // clean and set some data to the given ones
    // designed to be called once after reuse an instance from the object pool
    
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            this->board[i][j] = board[i][j];
        }
    }
    
    this->selfColor = selfColor;
    this->opponentColor = opponentColor;
    this->selfBudget = selfBudget;
    this->opponentBudget = opponentBudget;
    this->parent = parent;
    this->visits = 1;
    this->value = 0;
    this->child.clear();

    return;
}

void TreeNode::setValidMoves(){
    // Clear `validMoves` and set it to a vector of valid moves
    validMoves.clear();
    
    if (board[0][4] != chessType_whiteFlag || board[8][4] != chessType_blackFlag) // flag has been taken
        return;
    if (selfBudget <= FIXED_COST) // early stop when no move can be taken
        return;

    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            // a valid move must start from a self soldier
            // check if (i, j) is a self soldier
            if (board[i][j] != chessType_blackPiece + selfColor)
                continue;
            
            // find valid moves which move self soldier at (i, j)
            for (auto& d: chess_moveDirection){ // try all 4 directions
                int r = i, c = j; // (r, c): new position, starting from (i, j)
                
                while (1){
                    r += d.first;
                    c += d.second;

                    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) // out of range
                        break;
                    
                    bool isHindered = false; // indicates whether the loop should break directly after current check
                    if (board[r][c] == chessType_blackPiece + selfColor) // hindered by self soldier
                        break;
                    if (board[r][c] == chessType_blackFlag + selfColor) // hindered by self flag
                        break;
                    if (board[r][c] != chessType_empty)
                        isHindered = true;
                    
                    int moveCost = FIXED_COST + std::abs(r - i) + std::abs(c - j);
                    if (board[r][c] == chessType_blackBarrier + opponentColor) // need to break opponent barrier
                        moveCost += OPPONENT_BARRIER_COST;
                    
                    if (moveCost > selfBudget) // budget not enough
                        break;
                    
                    validMoves.push_back(Move(i, j, r, c)); // push to valid moves

                    if (isHindered) // can not move toward anymore
                        break;
                }
            }
        
        }
    }

    return;
}

void TreeNode::step(const Move& move){
    // step current node by `move`
    // states related to the game, i.e. `board`, `selfBudget`, `opponentBudget` will be update inplace
    // move must guarenteed to be legal

    board[move.i1][move.j1] = chessType_empty;
    
    selfBudget -= FIXED_COST + std::abs(move.i2 - move.i1) + std::abs(move.j2 - move.j1);

    if (board[move.i2][move.j2] == chessType_blackBarrier + selfColor) // break self barrier
        selfBudget -= SELF_BARRIER_COST;
    else if (board[move.i2][move.j2] == chessType_blackBarrier + opponentColor) // break opponent barrier
        selfBudget -= OPPONENT_BARRIER_COST;

    board[move.i2][move.j2] = chessType_blackPiece + selfColor;
    
    std::swap(selfColor, opponentColor);
    std::swap(selfBudget, opponentBudget);

    return;
}

int TreeNode::getWinner() const{
    // `this` must guaranteed to have no more valid moves for both player
    // find the winner according to the rule

    if (board[0][4] != chessType_whiteFlag) // position of white flag
        return color_black; // black win
    if (board[8][4] != chessType_blackFlag) // position of black flag
        return color_white; // white win
    
    // count pieces
    int blackSoldiers = 0, whiteSoldiers = 0, blackBarriers = 0, whiteBarriers = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            switch (board[i][j]){
                case chessType_blackPiece:
                    blackSoldiers++;
                    break;
                case chessType_whitePiece:
                    whiteSoldiers++;
                    break;
                case chessType_blackBarrier:
                    blackBarriers++;
                    break;
                case chessType_whiteBarrier:
                    whiteBarriers++;
                    break;
            }
        }
    }
    
    // 1. player with the most remaining soldiers win
    if (blackSoldiers > whiteSoldiers)
        return color_black;
    if (whiteSoldiers > blackSoldiers)
        return color_white;
    
    // 2. player with the most remaining barriers win
    if (blackBarriers > whiteBarriers)
        return color_black;
    if (whiteBarriers > blackBarriers)
        return color_white;
    
    // 3. draw
    return -1;
}

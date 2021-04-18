#include "TreeNode.hpp"

TreeNode::TreeNode(){
    board = std::vector<std::vector<int> >(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
    validMoves = std::vector<Move>();
    return;
}

// inline bool TreeNode::validateMove(int i1, int j1, int i2, int j2){
//     // check whether a move is valid
//     // (i1, j1): old position
//     // (i2, j2): new position
//     if (board[i1][j1] != selfColor) // a valid move must start from a self soldier
//         return false;
//     if (i1 < 0 || i1 >= 9 || j1 < 0 || j1 >= 9
//         || i2 < 0 || i2 >= 9 || j2 < 0 || j2 >= 9) // index out of range
//         return false;



//     return true;
// }

void TreeNode::setValidMoves(){
    // Clear `validMoves` and set it to a vector of valid moves
    validMoves.clear();
    
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            // a valid move must start from a self soldier
            // check if (i, j) is a self soldier
            if (board[i][j] != chessType_blackPiece + selfColor)
                continue;
            
            int r = i, c = j; // (r, c): new position, starting from (i, j)

            // find valid moves which move self soldier at (i, j)
            for (auto& d: chess_moveDirection){ // try all 4 directions    
                while (1){
                    r += d.first;
                    c += d.second;

                    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) // out of range
                        break;
                    
                    bool isHindered = false; // indicates whether the loop should break directly after current check
                    if (board[r][c] == chessType_blackPiece + selfColor) // hindered by self soldier
                        break;
                    if (board[r][c] != chessType_empty)
                        isHindered = true;
                    
                    int moveCost = FIXED_COST + (r - i) + (c - j);
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

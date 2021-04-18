#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <utility>

#define BOARD_SIZE 9

#define FIXED_COST 1
#define SELF_BARRIER_COST -3
#define OPPONENT_BARRIER_COST 3

const int color_black = 0;
const int color_white = 1;

const int chessType_empty = 0;
const int chessType_blackFlag = 1;
const int chessType_whiteFlag = 2;
const int chessType_blackPiece = 3;
const int chessType_whitePiece = 4;
const int chessType_blackBarrier = 5;
const int chessType_whiteBarrier = 6;

const std::pair<int, int> chess_moveDirection[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

struct Move{
    int i1, j1, i2, j2; // // (i1, j1): old position, (i2, j2): new position
    Move(int i1, int j1, int i2, int j2): i1(i1), j1(j1), i2(i2), j2(j2){}
};

typedef std::vector<std::vector<int> > Board;

class TreeNode{
public:
    Board board;
    int selfBudget, opponentBudget;
    int selfColor, opponentColor;
    TreeNode *parent;
    std::vector<TreeNode*> child; // child
    std::vector<Move> validMoves; // valid moves
    
    int visits; // visits count of the state
    int value; // value of the state

    TreeNode();


    void setValidMoves();
    void step_(Board&, const Move&);

    // ~TreeNode();

};

#endif
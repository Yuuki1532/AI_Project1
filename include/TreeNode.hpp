#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <utility>
#include <functional>
#include <random>

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
    Move(){}
    Move(const int i1, const int j1, const int i2, const int j2): i1(i1), j1(j1), i2(i2), j2(j2){}
};

typedef int Board[BOARD_SIZE][BOARD_SIZE];

class TreeNode{
public:
    Board board;
    
    int selfBudget, opponentBudget;
    int selfColor, opponentColor;
    
    TreeNode *parent;
    std::vector<TreeNode*> child; // child
    std::vector<Move> validMoves; // valid moves
    
    long long value; // value of the node
    // `value` of the node is w.r.t to `parent`
    // e.g. `value` = 10 implies
    // the value for `parent` to take a specific action thus reaching the node is 10

    long long visits; // visit count of the node
    

    void setValidMoves();
    void step(const Move&);
    int getWinner() const;
    

    void resetData(const Board&, const int, const int, const int, const int, TreeNode* const);
    
};

#endif

#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <utility>

// 0: empty, 1: black flag, 2: white flag, 3: black pieces,
//                  // 4: white pieces, 5: black barriers, 6: white barriers

#define SELF_BARRIER_COST -3
#define OPPONENT_BARRIER_COST 3

const int chessType_empty = 0;
const int chessType_blackFlag = 1;
const int chessType_whiteFlag = 2;
const int chessType_blackPiece = 3;
const int chessType_whitePiece = 4;
const int chessType_blackBarrier = 5;
const int chessType_whiteBarrier = 6;


typedef std::vector<std::vector<int> > Board;
typedef std::pair<int, int> Move;

class TreeNode{
public:
    Board board;
    int budget;
    TreeNode *parent;
    std::vector<TreeNode*> child; // child
    std::vector<Move> validMoves; // valid moves
    
    // bool isEnd; // indicate whether the state is of an ended game
    bool blacksTurn; // indicate whether the next move should be taken by black
    int visits; // visits count of the state
    int value; // value of the state

    TreeNode();

    std::vector<Move> getValidMoves();


    // ~TreeNode();

};

#endif
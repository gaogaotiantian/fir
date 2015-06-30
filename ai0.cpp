#include "game.h"
Point ExampleAI(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (board[i][j] == Empty) {
                Point p(i,j);
                return p;
            }
        }
    }
    Point p(0,0);
    return p;
}

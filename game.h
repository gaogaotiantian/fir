#include <stdio.h>
#include <stdlib.h>

#ifndef __GAME_H__
#define __GAME_H__
#define BoardSize 19

enum NodeType {Empty, Black, White};

class Point{
public:
    Point() {x = -1; y = -1;};
    Point(int xx, int yy) {x = xx; y = yy;};
    void set(int xx, int yy);

    int x;
    int y;
};
class Game {
public:
    void Initialize();
    bool Move(Point p);
    NodeType CheckVictory();
    NodeType GetType(Point p);
    void PrintBoard();
    
    bool isBlackPlaying;
    NodeType board[BoardSize][BoardSize];
private:
    NodeType CheckNode(Point p);
};

#endif

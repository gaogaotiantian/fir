#include <stdio.h>
#include <stdlib.h>

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
Point AI1(const NodeType board[BoardSize][BoardSize], NodeType yourType);
Point AI2(const NodeType board[BoardSize][BoardSize], NodeType yourType);

typedef Point (*AIFunc)(const NodeType[BoardSize][BoardSize], NodeType);
AIFunc pAI1 = &AI1;
AIFunc pAI2 = &AI2;

AIFunc AIList[] = {pAI1, pAI2};

Point (*BlackAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);
Point (*WhiteAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);


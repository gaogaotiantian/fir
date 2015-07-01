#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>

#ifndef __GAME_H__
#define __GAME_H__
#define BoardSize 19

enum NodeType {Empty, Black, White};

class Point{
public:
    Point() {x = -1; y = -1;};
    Point(int xx, int yy) {x = xx; y = yy;};
    void Set(int xx, int yy) {x = xx; y = yy;};
    bool Valid() {return (0 <= x && x < BoardSize && 0 <= y && y < BoardSize);};

    int x;
    int y;
};

typedef Point (*AIFunc)(const NodeType[BoardSize][BoardSize], NodeType);
//typedef Point (*AIFunc)(const NodeType**, NodeType);

struct S_AI {
    int id;
    AIFunc func;
    const char* name;
};

typedef struct S_AI S_AI;

typedef std::map<int, struct S_AI> AI_Map;

class Game {
public:
    Game();
    void Initialize();
    bool Move(Point p);
    NodeType Play(S_AI, S_AI, bool);
    NodeType CheckVictory();
    NodeType GetType(Point p);
    void PrintBoard();
    void SetGamerAI(int gamer1ID, int gamer2ID, bool isRand);
    
    bool isBlackPlaying;
    unsigned int sleepTime;
    bool isPrint;
    NodeType board[BoardSize][BoardSize];
    std::string boardChar[BoardSize*2][BoardSize*2];
    AI_Map aiMap;
private:
    NodeType CheckNode(Point p);
};

#endif

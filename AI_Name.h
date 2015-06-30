#include "game.h"
#ifndef __AI_NAME_H__
#define __AI_NAME_H__

// If you added a new AI, please change this number
#define MAX_AI_NUM 1

typedef Point (*AIFunc)(const NodeType[BoardSize][BoardSize], NodeType);

struct S_AIList {
    int id;
    AIFunc func;
    const char* name;
};

// You need to declare you function here!
Point ExampleAI(const NodeType board[BoardSize][BoardSize], NodeType yourType);

// And add your function here!
S_AIList AIList[MAX_AI_NUM] = {
//  ID | Function Name   | Name
   {0,   ExampleAI       , "Example"}
};

#endif

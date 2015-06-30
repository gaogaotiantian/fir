#include "game.h"
#ifndef __AI_NAME_H__
#define __AI_NAME_H__

// If you added a new AI, please change this number
#define MAX_AI_NUM 1

typedef Point (*AIFunc)(const NodeType[BoardSize][BoardSize], NodeType);

// You need to declare you function here!
Point ExampleAI(const NodeType board[BoardSize][BoardSize], NodeType yourType);

// And add your function here!
AIFunc AIList[MAX_AI_NUM] = {
// Function Name   | ID
    ExampleAI      // 0
};

#endif

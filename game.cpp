#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
#include "AI_Name.h"

Point (*BlackAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);
Point (*WhiteAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);

void Game::Initialize()
{
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            board[i][j] = Empty;
        }
    }
    isBlackPlaying = true;
}
bool Game::Move(Point p)
{
    if (p.x < 0 || p.x >= BoardSize ||
            p.y < 0 || p.y >= BoardSize) {
        return false;
    }
    if (board[p.x][p.y] == Empty) {
        if (isBlackPlaying)
            board[p.x][p.y] = Black;
        else
            board[p.x][p.y] = White;
        isBlackPlaying = !isBlackPlaying;
        return true;
    } else {
        return false;
    }
}
NodeType Game::GetType(Point p)
{
    assert(0 <= p.x && p.x < BoardSize);
    assert(0 <= p.y && p.y < BoardSize);
    return board[p.x][p.y];
}
NodeType Game::CheckVictory()
{
    for (int i = 0; i < BoardSize - 4; i++) {
        for (int j = 0; j < BoardSize - 4; j++) {
            Point p(i,j);
            NodeType winType = CheckNode(p);
            if (winType == Black) 
                return Black;
            else if (winType == White)
                return White;
        }
    }
    return Empty;
}
// We only check right, down and right-down
NodeType Game::CheckNode(Point p)
{
    assert(p.x < BoardSize - 4);
    assert(p.y < BoardSize - 4);
    NodeType curType = GetType(p);
    bool vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x+i, p.y);
        if (GetType(nextP) != curType) {
            vic = false;
            break;
        }
    }
    if (vic == true)
        return curType;
    vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x, p.y+i);
        if (GetType(nextP) != curType) {
            vic = false;
            break;
        }
    }
    if (vic == true)
        return curType;
    vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x+1, p.y+1);
        if (GetType(nextP) != curType) {
            vic = false;
            break;
        }
    }
    if (vic == true)
        return curType;
    return Empty;
}
void Game::PrintBoard()
{
    const char* emptyChar = "- ";
    const char* blackChar = "* ";
    const char* whiteChar = "o ";
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            Point p(i,j);
            if (GetType(p) == Empty)
                printf("%s", emptyChar);
            else if (GetType(p) == White)
                printf("%s", whiteChar);
            else if (GetType(p) == Black)
                printf("%s", blackChar);
        }
        printf("\n");
    }
}
void SetGamerAI(int gamer1ID, int gamer2ID)
{

    AIFunc func1 = NULL;
    AIFunc func2 = NULL;

    for (int i = 0; i < MAX_AI_NUM; i++) {
        if (AIList[i].id == gamer1ID)
            func1 = AIList[i].func;
        if (AIList[i].id == gamer2ID)
            func2 = AIList[i].func;
    }

    if (func1 == NULL) {
        printf("Invalid gamer1ID!\n");
        exit(1);
    }
    if (func2 == NULL) {
        printf("Invalid gamer2ID!\n");
        exit(1);
    }
    
    if (rand() % 2 == 0) {
        BlackAI = func1;
        WhiteAI = func2;
    } else {
        BlackAI = func1;
        WhiteAI = func2;
    }
}
int main(int argc, char* argv[]) 
{
    Game game;
    int gamer1ID = 0;
    int gamer2ID = 0;
    unsigned int sleepTime = 1;

    if (argc >= 3) {
        gamer1ID = atoi(argv[1]);
        gamer2ID = atoi(argv[2]);
        SetGamerAI(gamer1ID, gamer2ID);
    } else {
        printf("Not enough gamers!\n");
        return 1;
    }

    game.Initialize();
    srand(time(NULL));

    while (1) {
        Point p;
        system("clear");
        p = BlackAI(game.board, Black);
        if (game.Move(p) == false) {
            printf("Stupid black AI made a move at (%i, %i)\n", p.x, p.y);
            break;
        }
        game.PrintBoard();
        if (game.CheckVictory() == Black) {
            printf("Black Wins!\n");
            break;
        }
        sleep(sleepTime);

        system("clear");
        p = WhiteAI(game.board, White);
        if (game.Move(p) == false) {
            printf("Stupid white AI made a move at (%i, %i)\n", p.x, p.y);
            break;
        }
        game.PrintBoard();
        if (game.CheckVictory() == White) {
            printf("White Wins!\n");
            break;
        }
        sleep(sleepTime);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include "game.h"
#include "AI_Name.h"

using namespace std;

Point (*BlackAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);
const char* BlackAIName;
Point (*WhiteAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);
const char* WhiteAIName;

S_AI blackAI;
S_AI whiteAI;


GameSettings::GameSettings()
{
    isPrint      = true;
    isRandFirst  = true;
    sleepTime    = 1;
    isNormal     = 1;
}

Game::Game()
{
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            board[i][j] = Empty;
        }
    }

    // Draw board
    int drawSize = BoardSize*2;
    boardChar[1][1]                   = "┌";
    boardChar[1][drawSize-1]          = "┐";
    boardChar[drawSize-1][1]          = "└";
    boardChar[drawSize-1][drawSize-1] = "┘";
    for (int i = 2; i < drawSize-1; ++i) {
        if (i%2 == 1) {
            boardChar[1][i]          = "┬";
            boardChar[drawSize-1][i] = "┴";
            boardChar[i][1]          = "├";
            boardChar[i][drawSize-1] = "┤";
        }
        else {
            boardChar[1][i]          = "─";
            boardChar[drawSize-1][i] = "─";
            boardChar[i][1]          = "│";
            boardChar[i][drawSize-1] = "│";

        }
    }
    for (int i = 2; i < drawSize - 1; ++i) {
        for (int j = 2; j < drawSize - 1; ++j) {
            if (i%2 == 0) {
                if (j%2 == 0)
                    boardChar[i][j] = " ";
                else
                    boardChar[i][j] = "│";
            } else {
                if (j%2 == 0)
                    boardChar[i][j] = "─";
                else
                    boardChar[i][j] = "┼";
            }
        }
    }

    for (int i = 0; i < MAX_AI_NUM; ++i) {
        struct S_AI ai = AIList[i];
        if (ai.id > 0 && ai.id <= MAX_AI_NUM) {
            AI_Map::iterator it = aiMap.find(ai.id);
            if (it == aiMap.end()) {
                printf("Loaded AI: %d. %s\n", ai.id, ai.name);
                aiMap[ai.id] = AIList[i];
            } else {
                printf("AI id conflicts at %d!\n", ai.id);
                exit(0);
            }
        }
    }
}
void Game::Initialize()
{
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            board[i][j] = Empty;
        }
    }
    isBlackPlaying = true;
}
NodeType Game::Play(S_AI blackAI, S_AI whiteAI) {
    while (1) {
        Point p;
        p = blackAI.func(board, Black);
        if (Move(p) == false) {
            if (settings.isPrint)
                printf("Stupid AI %s(%s) made a move at (%i, %i)\n", blackAI.name, blackChar, p.x, p.y);
            return White;
        }
        if (settings.isPrint) {
            PrintBoard();
            sleep(settings.sleepTime);
        }
        if (CheckVictory() == Black) {
            return Black;
        }
        
        p = whiteAI.func(board, White);
        if (Move(p) == false) {
            if (settings.isPrint)
                printf("Stupid AI %s(%s) made a move at (%i, %i)\n", whiteAI.name, whiteChar, p.x, p.y);
            return Black;
        }
        if (settings.isPrint) {
            PrintBoard();
            sleep(settings.sleepTime);
        }
        if (CheckVictory() == White) {
            return White;
        }
    }
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
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
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
    NodeType curType = GetType(p);
    // right
    bool vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x+i, p.y);
        if (p.x+i >= BoardSize || GetType(nextP) != curType) {
            vic = false;
            break;
        }
    }
    if (vic == true)
        return curType;
    // down
    vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x, p.y+i);
        if (p.y+i >= BoardSize || GetType(nextP) != curType) {
            vic = false;
            break;
        }
    }
    if (vic == true)
        return curType;
    // right down
    vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x+i, p.y+i);
        if (p.x+i >= BoardSize || p.y+i >= BoardSize || GetType(nextP) != curType) {
            vic = false;
            break;
        }
    }
    if (vic == true)
        return curType;
    //left down
    vic = true;
    for (int i = 1; i < 5; i++) {
        Point nextP(p.x-i, p.y+i);
        if (p.x-i < 0 || p.y+i >= BoardSize || GetType(nextP) != curType) {
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
    system("clear");
    printf(" ");
    for (int i = 0; i < BoardSize; ++i) {
        printf("%2d", i);
    }
    printf("\n");
    for (int i = 0; i < BoardSize; ++i) {
        printf("%2d", i);
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i,j);
            if (GetType(p) == Empty)
                cout<<boardChar[i*2+1][j*2]<<boardChar[i*2+1][j*2+1];
            else if (GetType(p) == White)
                cout<<boardChar[i*2+1][j*2]<<whiteChar;
            else if (GetType(p) == Black)
                cout<<boardChar[i*2+1][j*2]<<blackChar;
        }
        printf("\n");
    }
    printf("%s: %s\n", blackChar, blackAI.name);
    printf("%s: %s\n", whiteChar, whiteAI.name);
}
void Game::PreSetBoard()
{
    int x = 0;
    int y = 0;
    NodeType t;
    for (int i = 0; i <= 2; ++i) {
        t = (i == 1) ? Black : White;
        x = (BoardSize - 9) / 2 + rand() % 9;
        y = (BoardSize - 9) / 2 + rand() % 9;
        if (board[x][y] == Empty)
            board[x][y] = t;
    }
}
void Game::SetGamerAI(int gamer1ID, int gamer2ID)
{
    S_AI ai1;
    S_AI ai2;
    AI_Map::iterator it = aiMap.find(gamer1ID);
    if (it != aiMap.end()) {
        ai1 = aiMap[gamer1ID];
    } else {
        printf("Gamer1ID: %d does not exist!\n", gamer1ID);
        exit(1);
    }
    it = aiMap.find(gamer2ID);
    if (it != aiMap.end()) {
        ai2 = aiMap[gamer2ID];
    } else {
        printf("Gamer2ID: %d does not exist!\n", gamer2ID);
        exit(1);
    }

    if (!settings.isRandFirst) {
        blackAI = ai1;
        whiteAI = ai2;
    } else {
        if (rand() % 2 == 0) {
            blackAI = ai1;
            whiteAI = ai2;
        } else {
            blackAI = ai2;
            whiteAI = ai1;
        }
    }
}
void CheckBoolArguments(char* arg, char c, bool& dest)
{
    if (arg[1] == c) {
        if (arg[2] == ':') {
            if (arg[3] == '0') {
                dest = false;
            } else if (arg[3] == '1') {
                dest = true;
            } else {
                printf("Unknown argument: %s\n", arg);
                exit(1);
            }
        } else {
            printf("Unknown argument: %s\n", arg);
            exit(1);
        }
    }
}
void CheckIntArguments(char* arg, char c, int& dest)
{
    if (arg[1] == c) {
        if (arg[2] == ':') {
            dest = atoi(arg+3);            
        } else {
            printf("Unknown argument: %s\n", arg);
            exit(1);
        }
    }
}

int main(int argc, char* argv[]) 
{
    Game game;
    int gamer1ID = -1;
    int gamer2ID = -1;
    game.Initialize();
    srand(time(NULL));

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            CheckIntArguments (argv[i], 's', game.settings.sleepTime);
            CheckBoolArguments(argv[i], 'p', game.settings.isPrint);
            CheckBoolArguments(argv[i], 'r', game.settings.isRandFirst);
            CheckBoolArguments(argv[i], 'n', game.settings.isNormal);
        } else {
            if (gamer1ID == -1) {
                gamer1ID = atoi(argv[i]);
            } else if (gamer2ID == -1) {
                gamer2ID = atoi(argv[i]);
            } else {
                printf("Too many IDs!\n");
                exit(1);
            }
        }
    }

    if (gamer1ID == -1 || gamer2ID == -1) {
        printf("Not enough IDs!\n");
        exit(1);
    } else {
        game.SetGamerAI(gamer1ID, gamer2ID);
    }
    if (!game.settings.isNormal)
        game.PreSetBoard();
    NodeType result = game.Play(blackAI, whiteAI);
    if (result == Black) {
        printf("%s Wins! AI: \"%s\" beat AI: \"%s\"\n", blackChar, blackAI.name, whiteAI.name);
    } else if (result == White) {
        printf("%s Wins! AI: \"%s\" beat AI: \"%s\"\n", whiteChar, whiteAI.name, blackAI.name);
    }

    return 0;
}

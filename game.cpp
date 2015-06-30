#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
#include "AI_Name.h"

Point (*BlackAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);
const char* BlackAIName;
Point (*WhiteAI)(const NodeType board[BoardSize][BoardSize], NodeType yourType);
const char* WhiteAIName;

S_AI blackAI;
S_AI whiteAI;

Game::Game()
{
    isBlackPlaying = true;
    sleepTime      = 1;
    isPrint        = true;
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            board[i][j] = Empty;
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
NodeType Game::Play(S_AI blackAI, S_AI whiteAI, bool isPrint) {
    while (1) {
        Point p;
        p = blackAI.func(board, Black);
        if (Move(p) == false) {
            printf("Stupid black AI made a move at (%i, %i)\n", p.x, p.y);
            return White;
        }
        if (isPrint) {
            PrintBoard();
            sleep(sleepTime);
        }
        if (CheckVictory() == Black) {
            return Black;
        }
        
        p = whiteAI.func(board, White);
        if (Move(p) == false) {
            printf("Stupid white AI made a move at (%i, %i)\n", p.x, p.y);
            return Black;
        }
        if (isPrint) {
            PrintBoard();
            sleep(sleepTime);
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
        Point nextP(p.x+i, p.y+i);
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
    system("clear");
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
void Game::SetGamerAI(int gamer1ID, int gamer2ID, bool isRand)
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

    if (!isRand) {
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
int main(int argc, char* argv[]) 
{
    Game game;
    int gamer1ID = -1;
    int gamer2ID = -1;
    game.Initialize();

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 's':
                    if (argv[i][2] == ':') {
                        game.sleepTime = atoi(argv[i]+3);
                    } else {
                        printf("Unknown argument: %s\n", argv[i]);
                        exit(1);
                    }
                    break;
                case 'p':
                    if (argv[i][2] == ':') {
                        if (argv[i][3] == '0')
                            game.isPrint = false;
                        else if (argv[i][3] == '1')
                            game.isPrint = true;
                        else {
                            printf("Unknown argument: %s\n", argv[i]);
                            exit(1);
                        }
                    } else {
                        printf("Unknown argument: %s\n", argv[i]);
                        exit(1);
                    }
                    break;
                default:
                    printf("Unknown argument: %s\n", argv[i]);
                    exit(1);
            }
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
        game.SetGamerAI(gamer1ID, gamer2ID, true);
    }

    
    srand(time(NULL));

    if (game.Play(blackAI, whiteAI, game.isPrint) == Black) {
        printf("Black Wins! AI: \"%s\" beat AI: \"%s\"\n", blackAI.name, whiteAI.name);
    } else if (game.Play(blackAI, whiteAI, game.isPrint) == White) {
        printf("White Wins! AI: \"%s\" beat AI: \"%s\"\n", blackAI.name, whiteAI.name);
    }

    return 0;
}

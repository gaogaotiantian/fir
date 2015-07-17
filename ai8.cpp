#include "game.h"
#include <stdlib.h>
#include <stdio.h>

//enum Direction {};
NodeType p_Anti[BoardSize][BoardSize]; // board to save previous anti board
NodeType p_Mine[BoardSize][BoardSize];
bool Is_FirstStep = true;

class GXY_AI
{
public:
    GXY_AI();
    GXY_AI(const NodeType[BoardSize][BoardSize], NodeType);
    void Defend_Level();
    void Attack_Level();
    Point Defend();
    Point Attack();
    Point Move();
    Point Random_Move();
    bool Defend_Attack();
    void Locate_Anti();
    void Locate_Mine();
    Point Compare_Anti();
    Point Compare_Mine();
    void Save_Board();
    void p_Board_Initial();
    float Defend_Lv;
    float Attack_Lv;

    NodeType myType;
    NodeType antiType;
    NodeType myBoard[BoardSize][BoardSize];  // board to save current board   
    NodeType Anti[BoardSize][BoardSize];
    NodeType Mine[BoardSize][BoardSize];
    Point Defender;
    Point Attacker;
};

GXY_AI::GXY_AI(const NodeType board[BoardSize][BoardSize], NodeType type)
{
    myType = type;
    antiType = myType == Black ? White : Black;
    Defend_Lv = 0;
    Attack_Lv = 0;
    for (int i = 0; i < BoardSize; ++i) 
    {
        for (int j = 0; j < BoardSize; ++j) 
        {
            myBoard[i][j] = board[i][j];
        }
    }
}

void GXY_AI::p_Board_Initial()
{
    if(Is_FirstStep == true)
    {
        for(int i = 0; i < BoardSize; i++)
        {
            for (int j = 0; j < BoardSize; j++)
            {
                p_Anti[i][j] = Empty;
                p_Mine[i][j] = Empty;
                Anti[i][j] = Empty;
                Mine[i][j] = Empty;
            }
        }
        Is_FirstStep = false;      
    }
}

void GXY_AI::Save_Board()
{
    for (int i = 0; i < BoardSize; ++i) 
    {
        for (int j = 0; j < BoardSize; ++j) 
        {
            p_Anti[i][j] = Anti[i][j];
            p_Mine[i][j] = Mine[i][j];
        }
    }
}

void GXY_AI::Locate_Anti()
{
    for(int i = 0; i < BoardSize; i++) // draw out Anti Board
    {
        for(int j = 0; j < BoardSize; j++)
        {
            if(myBoard[i][j] == antiType)
            {
                Anti[i][j] = antiType;
            }
            else
            {
                Anti[i][j] = Empty;
            }
        }
    }
}

void GXY_AI::Locate_Mine()
{
    for(int i = 0; i < BoardSize; i++) // draw out Anti Board
    {
        for(int j = 0; j < BoardSize; j++)
        {
            if(myBoard[i][j] == myType)
            {
                Mine[i][j] = myType;
            }
            else 
            {
                Mine[i][j] = Empty;
            }
        }
    }
}

Point GXY_AI::Compare_Anti()
{
    for(int i = 0; i < BoardSize; i++) // compare Anti with p_Anti
    {
        for(int j = 0; j < BoardSize; j++)
        {
            if(Anti[i][j] != p_Anti[i][j])
            {
                Point p(i,j);
                return p;
            }
        }
    }
    Point p1(-1,-1);
    return p1;
}

Point GXY_AI::Compare_Mine()
{
    for(int i = 0; i < BoardSize; i++) // compare Anti with p_Anti
    {
        for(int j = 0; j < BoardSize; j++)
        {
            if(Mine[i][j] != p_Mine[i][j])
            {
                Point p(i,j);
                return p;
            }
        }
    }
    Point p1(-1,-1);
    return p1;
}

void GXY_AI::Defend_Level()
{
    Locate_Anti();
    Point p_A = Compare_Anti();
    Point UU, RU, RR, RD, DD, LD, LL, LU;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;
    int x = 9, y = 9;
    if (p_A.Valid() == true && myBoard[p_A.x][p_A.y] == antiType)
    {    
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check up
        {
            y -= 1;
            if(0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_UD++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_UD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    UU.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check right-up
        {
            x += 1;
            y -= 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_RULD++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_RULD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    RU.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check right
        {
            x += 1;
            if(0 <= x && x < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_LR++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_LR -= 0.25;
                else if(myBoard[x][y] == Empty)
                    RR.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check right-down
        {
            x += 1;
            y += 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_RDLU++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_RDLU -= 0.25;
                else if(myBoard[x][y] == Empty)
                    RD.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check down
        {
            y += 1;
            if(0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_UD++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_UD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    DD.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check left-down
        {
            x -= 1;
            y += 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_RULD++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_RULD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    LD.Set(x,y);
                break;
            }
            else 
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check left
        {
            x -= 1;
            if(0 <= x && x < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_LR++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_LR -= 0.25;
                else if(myBoard[x][y] == Empty)
                    LL.Set(x,y);
                break;
            }
            else 
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check left-up
        {
            x -= 1;
            y -= 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == antiType)
                {
                    Lv_RDLU++;
                    continue;
                }
                else if(myBoard[x][y] == myType)
                    Lv_RDLU -= 0.25;
                else if(myBoard[x][y] == Empty)
                    LU.Set(x,y);
                break;
            }
            else
                break;
        }

        // find out max in Lv_UD , Lv_LR , Lv_RULD , Lv_RDLU 
        int max = Lv_UD;
        int flag = 0;
        if(Lv_LR > max)
        {
           max = Lv_LR;
           flag = 1;   // flag == 1
        }
        if(Lv_RULD > max)
        {
           max = Lv_RULD;
           flag = 2;   // flag == 2
        }           
        if(Lv_RDLU > max)
        {
           max = Lv_RDLU;
           flag = 3;   // flag == 3
        }

        if(Lv_UD == 2 || Lv_UD == 2.75 || Lv_UD == 3)   
        {
            max = Lv_UD;   
            flag = 0; 
        }
        else if(Lv_LR == 2 || Lv_LR == 2.75 || Lv_LR == 3)
        {
            max = Lv_LR;   
            flag = 1; 
        }
        else if(Lv_RULD == 2 || Lv_RULD == 2.75 || Lv_RULD == 3)
        {
            max = Lv_RULD;
            flag = 2;
        }
        else if(Lv_RDLU == 2 || Lv_RDLU == 2.75 || Lv_RDLU == 3)
        {
            max = Lv_RDLU; 
            flag = 3; 
        }

        Defend_Lv = max;
        switch(flag)
        {
            case 0:
            if(UU.Valid() == true)
            {
                Defender.Set(UU.x,UU.y);
            } 
            else if(DD.Valid() == true)
            {
                Defender.Set(DD.x,DD.y);
            }
            else
            {
                Defend_Lv = -1;
            }
            break;
            case 1:
            if(LL.Valid() == true)
            {
                Defender.Set(LL.x,LL.y);
            } 
            else if(RR.Valid() == true)
            {
                Defender.Set(RR.x,RR.y);
            }
            else
            {
                Defend_Lv = -1;
            }
            break;
            case 2:
            if(RU.Valid() == true)
            {
                Defender.Set(RU.x,RU.y);
            } 
            else if(LD.Valid() == true)
            {
                Defender.Set(LD.x,LD.y);
            }
            else
            {
                Defend_Lv = -1;
            }
            break;
            case 3:
            if(RD.Valid() == true)
            {
                Defender.Set(RD.x,RD.y);
            } 
            else if(LU.Valid() == true)
            {
                Defender.Set(LU.x,LU.y);
            }
            else
            {
                Defend_Lv = -1;
            }
            break;
            default:
            break;
        }
    }
    else
        Defend_Lv = 0;
}

void GXY_AI::Attack_Level()
{
    Locate_Mine();
    Point p_A = Compare_Mine();
    Point UU, RU, RR, RD, DD, LD, LL, LU;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;
    int x = 9, y = 9;
    if (p_A.Valid() == true && myBoard[p_A.x][p_A.y] == myType)
    {    
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check up
        {
            y -= 1;
            if(0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_UD++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_UD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    UU.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check right-up
        {
            x += 1;
            y -= 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_RULD++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_RULD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    RU.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check right
        {
            x += 1;
            if(0 <= x && x < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_LR++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_LR -= 0.25;
                else if(myBoard[x][y] == Empty)
                    RR.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check right-down
        {
            x += 1;
            y += 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_RDLU++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_RDLU -= 0.25;
                else if(myBoard[x][y] == Empty)
                    RD.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check down
        {
            y += 1;
            if(0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_UD++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_UD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    DD.Set(x,y);
                break;
            }
            else
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check left-down
        {
            x -= 1;
            y += 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_RULD++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_RULD -= 0.25;
                else if(myBoard[x][y] == Empty)
                    LD.Set(x,y);
                break;
            }
            else 
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check left
        {
            x -= 1;
            if(0 <= x && x < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_LR++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_LR -= 0.25;
                else if(myBoard[x][y] == Empty)
                    LL.Set(x,y);
                break;
            }
            else 
                break;
        }
        x = p_A.x;
        y = p_A.y;
        for(int i = 0; i < BoardSize; i++) // check left-up
        {
            x -= 1;
            y -= 1;
            if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
            {
                if(myBoard[x][y] == myType)
                {
                    Lv_RDLU++;
                    continue;
                }
                else if(myBoard[x][y] == antiType)
                    Lv_RDLU -= 0.25;
                else if(myBoard[x][y] == Empty)
                    LU.Set(x,y);
                break;
            }
            else
                break;
        }

        // find out max in Lv_UD , Lv_LR , Lv_RULD , Lv_RDLU 
        int max = Lv_UD;
        int flag = 0;
        if(Lv_LR > max)
        {
           max = Lv_LR;
           flag = 1;   // flag == 1
        }
        if(Lv_RULD > max)
        {
           max = Lv_RULD;
           flag = 2;   // flag == 2
        }           
        if(Lv_RDLU > max)
        {
           max = Lv_RDLU;
           flag = 3;   // flag == 3
        }

        if(Lv_UD == 2 || Lv_UD == 2.75 || Lv_UD == 3)   
        {
            max = Lv_UD;   
            flag = 0; 
        }
        else if(Lv_LR == 2 || Lv_LR == 2.75 || Lv_LR == 3)
        {
            max = Lv_LR;   
            flag = 1; 
        }
        else if(Lv_RULD == 2 || Lv_RULD == 2.75 || Lv_RULD == 3)
        {
            max = Lv_RULD;
            flag = 2;
        }
        else if(Lv_RDLU == 2 || Lv_RDLU == 2.75 || Lv_RDLU == 3)
        {
            max = Lv_RDLU; 
            flag = 3; 
        }

        Attack_Lv = max;

        switch(flag)
        {
            case 0:
            if(UU.Valid() == true)
            {
                Attacker.Set(UU.x,UU.y);
            } 
            else if(DD.Valid() == true)
            {
                Attacker.Set(DD.x,DD.y);
            }
            else
            {
                Attack_Lv = 0;
            }
            break;
            case 1:
            if(LL.Valid() == true)
            {
                Attacker.Set(LL.x,LL.y);
            } 
            else if(RR.Valid() == true)
            {
                Attacker.Set(RR.x,RR.y);
            }
            else
            {
                Attack_Lv = 0;
            }
            break;
            case 2:
            if(RU.Valid() == true)
            {
                Attacker.Set(RU.x,RU.y);
            } 
            else if(LD.Valid() == true)
            {
                Attacker.Set(LD.x,LD.y);
            }
            else
            {
                Attack_Lv = 0;
            }
            break;
            case 3:
            if(RD.Valid() == true)
            {
                Attacker.Set(RD.x,RD.y);
            } 
            else if(LU.Valid() == true)
            {
                Attacker.Set(LU.x,LU.y);
            }
            else
            {
                Attack_Lv = 0;
            }
            break;
            default:
            break;
        }
    }
    else
        Attack_Lv = 0;
}

Point GXY_AI::Random_Move()
{
    int random_number = (rand()%4+1);
    if(random_number == 1)
    {
        for (int i = 9; i >= 0; i--) 
        {
            for (int j = 9; j >= 0; j--) 
            {
                if (myBoard[i][j] == Empty) 
                {
                    Point p(i,j);
                    return p;
                }
            }
        }
    }
    
    else if(random_number == 2)
    {
        for (int i = 9; i < BoardSize; i++) 
        {
            for (int j = 9; j >= 0; j--) 
            {
                if (myBoard[i][j] == Empty) 
                {
                    Point p(i,j);
                    return p;
                }
            }
        }
    }

    else if(random_number == 3)
    {
        for (int i = 9; i >= 0; i--) 
        {
            for (int j = 9; j < BoardSize; j++) 
            {
                if (myBoard[i][j] == Empty) 
                {
                    Point p(i,j);
                    return p;
                }
            }
        }
    }
    
    else
    {
        for (int i = 9; i < BoardSize; i++) 
        {
            for (int j = 9; j < BoardSize; j++) 
            {
                if (myBoard[i][j] == Empty) 
                {
                    Point p(i,j);
                    return p;
                }
            }
        }
    }
}

Point GXY_AI::Move()
{
    p_Board_Initial();
    Defend_Level();
    Attack_Level();
    Save_Board();
    if(Defend_Lv == 2 || Defend_Lv == 2.75 || Defend_Lv == 3)
    {
        if(Attack_Lv >= Defend_Lv)
        {
            if(Attacker.Valid() == true)
                return Attacker;
            else if(Defender.Valid() == true)
                return Defender;
            else
                return Random_Move();
        }
        else
        {
            if(Defender.Valid() == true)
                return Defender;
            else if(Attacker.Valid() == true)
                return Attacker;
            else
                return Random_Move();
        }
                 
    }
    else
    {
        if(Attacker.Valid() == true)
            return Attacker;
        else
            return Random_Move();
    }       
}

Point Michael_AI(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    GXY_AI gxy_AI(board, yourType);
    Point p = gxy_AI.Move();
    return p;
}
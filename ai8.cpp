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
    void Attack_Final();
    void Defend_Level();
    float Attack_Level(int i);
    int Df_Level_Check(Point p, int x1, int y1, float Lv);
    int At_Level_Check(Point p, int x1, int y1, float Lv);
    Point Df_Level_Check(Point p, int x1, int y1);
    Point At_Level_Check(Point p, int x1, int y1);
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
    float Attack_Lv[3];
    float Attack_Lv_Final;

    NodeType myType;
    NodeType antiType;
    NodeType myBoard[BoardSize][BoardSize];  // board to save current board   
    NodeType Anti[BoardSize][BoardSize];
    NodeType Mine[BoardSize][BoardSize];
    Point p_A;
    Point p_M[3];
    Point Defender;
    Point Attacker[3];
    Point Attacker_Final;
};

GXY_AI::GXY_AI(const NodeType board[BoardSize][BoardSize], NodeType type)
{
    myType = type;
    antiType = myType == Black ? White : Black;
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
    p_M[0] = p_M[1];
    p_M[1] = p_M[2];
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

int GXY_AI::Df_Level_Check(Point p, int x1, int y1, float Lv)
{
    int x = p.x;
    int y = p.y;
    for(int i = 0; i < BoardSize; i++) // check up
    {
        x += x1;
        y += y1;
        if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
        {
            if(myBoard[x][y] == antiType)
            {
                Lv++;
                continue;
            }
            else if(myBoard[x][y] == myType)
                Lv -= 0.25;
            break;
        }
        else
             break;
    }
    return Lv;
}

Point GXY_AI::Df_Level_Check(Point p, int x1, int y1)
{
    int x = p.x;
    int y = p.y;
    Point pp;
    for(int i = 0; i < BoardSize; i++) // check up
    {
        x += x1;
        y += y1;
        if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
        {
            if(myBoard[x][y] == antiType)
            {
                continue;
            }
            else if(myBoard[x][y] == Empty)
            {
                pp.Set(x,y);
                return pp;                
            }   
            break;
        }
        else
             break;
    }
    return pp;
}

void GXY_AI::Defend_Level()
{
    Locate_Anti();
    p_A = Compare_Anti();
    Point UU, RU, RR, RD, DD, LD, LL, LU;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;

    if (p_A.Valid() == true && myBoard[p_A.x][p_A.y] == antiType)
    {    
        int tpLv_UD    =    Df_Level_Check(p_A,  0, -1, 0 );
        int tpLv_RULD  =    Df_Level_Check(p_A,  1, -1, 0 );
        int tpLv_LR    =    Df_Level_Check(p_A,  1,  0, 0 );
        int tpLv_RDLU  =    Df_Level_Check(p_A,  1,  1, 0 );
        Lv_UD    =    Df_Level_Check(p_A,  0,  1, tpLv_UD   );
        Lv_RULD  =    Df_Level_Check(p_A, -1,  1, tpLv_RULD );      
        Lv_LR    =    Df_Level_Check(p_A, -1,  0, tpLv_LR   );     
        Lv_RDLU  =    Df_Level_Check(p_A, -1, -1, tpLv_RDLU );

        UU  =    Df_Level_Check(p_A,  0, -1 );
        RU  =    Df_Level_Check(p_A,  1, -1 );
        RR  =    Df_Level_Check(p_A,  1,  0 );
        RD  =    Df_Level_Check(p_A,  1,  1 );
        DD  =    Df_Level_Check(p_A,  0,  1 );
        LD  =    Df_Level_Check(p_A, -1,  1 );      
        LL  =    Df_Level_Check(p_A, -1,  0 );     
        LU  =    Df_Level_Check(p_A, -1, -1 );

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

int GXY_AI::At_Level_Check(Point p, int x1, int y1, float Lv)
{
    int x = p.x;
    int y = p.y;
    for(int i = 0; i < BoardSize; i++) // check up
    {
        x += x1;
        y += y1;
        if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
        {
            if(myBoard[x][y] == myType)
            {
                Lv++;
                continue;
            }
            else if(myBoard[x][y] == antiType)
                Lv -= 0.25;
            break;
        }
        else
             break;
    }
    return Lv;
}

Point GXY_AI::At_Level_Check(Point p, int x1, int y1)
{
    int x = p.x;
    int y = p.y;
    Point pp;
    for(int i = 0; i < BoardSize; i++) // check up
    {
        x += x1;
        y += y1;
        if(0 <= x && x < BoardSize && 0 <= y && y < BoardSize)
        {
            if(myBoard[x][y] == myType)
            {
                continue;
            }
            else if(myBoard[x][y] == Empty)
            {
                pp.Set(x,y);
                return pp;                
            }   
            break;
        }
        else
             break;
    }
    return pp;
}

void GXY_AI::Attack_Final()
{
    for(int i = 0; i < 3; i++)
    {
        Attack_Lv[i] = 0;
    }
    Locate_Mine();
    p_M[2] = Compare_Mine();
    for(int i =2; i > 0; i--)
    {
        Attack_Lv[i] = Attack_Level(i);
    }
    
    int Attack_Lv_Final = Attack_Lv[2];
    Attacker_Final = Attacker[2];
    
    if(Attack_Lv[1] > Attack_Lv_Final)
    {
        Attack_Lv_Final = Attack_Lv[1];
        Attacker_Final = Attacker[1];
    }
    if(Attack_Lv[0] > Attack_Lv_Final)
    {
        Attack_Lv_Final = Attack_Lv[0];
        Attacker_Final = Attacker[0];
    }
    
    for(int i = 0; i < 3; i++)
    {
        if(Attack_Lv[i] == 2 || Attack_Lv[i] == 2.75 || Attack_Lv[i] == 3)   
        {
            Attack_Lv_Final = Attack_Lv[i];
            Attacker_Final = Attacker[i];   
        }
    }    

}

float GXY_AI::Attack_Level(int i)
{
    Point UU, RU, RR, RD, DD, LD, LL, LU;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;
    int x = 9, y = 9;
    if (p_M[i].Valid() == true && myBoard[p_M[i].x][p_M[i].y] == myType)
    {    
        int tpLv_UD    =    At_Level_Check(p_M[i],  0, -1, 0 );
        int tpLv_RULD  =    At_Level_Check(p_M[i],  1, -1, 0 );
        int tpLv_LR    =    At_Level_Check(p_M[i],  1,  0, 0 );
        int tpLv_RDLU  =    At_Level_Check(p_M[i],  1,  1, 0 );
        Lv_UD    =    At_Level_Check(p_M[i],  0,  1, tpLv_UD   );
        Lv_RULD  =    At_Level_Check(p_M[i], -1,  1, tpLv_RULD );      
        Lv_LR    =    At_Level_Check(p_M[i], -1,  0, tpLv_LR   );     
        Lv_RDLU  =    At_Level_Check(p_M[i], -1, -1, tpLv_RDLU );

        UU  =    At_Level_Check(p_M[i],  0, -1 );
        RU  =    At_Level_Check(p_M[i],  1, -1 );
        RR  =    At_Level_Check(p_M[i],  1,  0 );
        RD  =    At_Level_Check(p_M[i],  1,  1 );
        DD  =    At_Level_Check(p_M[i],  0,  1 );
        LD  =    At_Level_Check(p_M[i], -1,  1 );      
        LL  =    At_Level_Check(p_M[i], -1,  0 );     
        LU  =    At_Level_Check(p_M[i], -1, -1 );

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

        Attack_Lv[i] = max;

        switch(flag)
        {
            case 0:
            if(UU.Valid() == true)
            {
                Attacker[i].Set(UU.x,UU.y);
            } 
            else if(DD.Valid() == true)
            {
                Attacker[i].Set(DD.x,DD.y);
            }
            else
            {
                Attack_Lv[i] = 0;
            }
            break;
            case 1:
            if(LL.Valid() == true)
            {
                Attacker[i].Set(LL.x,LL.y);
            } 
            else if(RR.Valid() == true)
            {
                Attacker[i].Set(RR.x,RR.y);
            }
            else
            {
                Attack_Lv[i] = 0;
            }
            break;
            case 2:
            if(RU.Valid() == true)
            {
                Attacker[i].Set(RU.x,RU.y);
            } 
            else if(LD.Valid() == true)
            {
                Attacker[i].Set(LD.x,LD.y);
            }
            else
            {
                Attack_Lv[i] = 0;
            }
            break;
            case 3:
            if(RD.Valid() == true)
            {
                Attacker[i].Set(RD.x,RD.y);
            } 
            else if(LU.Valid() == true)
            {
                Attacker[i].Set(LU.x,LU.y);
            }
            else
            {
                Attack_Lv[i] = 0;
            }
            break;
            default:
            break;
        }
    }
    else
        Attack_Lv[i] = 0;
    return Attack_Lv[i];
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

    Point p;
    return p;
}

Point GXY_AI::Move()
{
    p_Board_Initial();
    Defend_Level();
    Attack_Final();
    Save_Board();
    if(Defend_Lv == 2 || Defend_Lv == 2.75 || Defend_Lv == 3)
    {
        if(Attack_Lv_Final >= Defend_Lv)
        {
            if(Attacker_Final.Valid() == true)
                return Attacker_Final;
            else if(Defender.Valid() == true)
                return Defender;
            else
                return Random_Move();
        }
        else
        {
            if(Defender.Valid() == true)
                return Defender;
            else if(Attacker_Final.Valid() == true)
                return Attacker_Final;
            else
                return Random_Move();
        }
                 
    }
    else
    {
        if(Attacker_Final.Valid() == true)
            return Attacker_Final;
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
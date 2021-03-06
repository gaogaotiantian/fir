#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ATTACKRECORD 190
#define DEFENDRECORD 190

class GXY_AI
{
public:
    GXY_AI();
    GXY_AI(const NodeType[BoardSize][BoardSize], NodeType);
    void Locate_Point();
    void Attack_Final();
    void Defend_Final();
    float Defend_Level(int i);
    float Attack_Level(int i);
    float Df_Level_Check(Point p, int x1, int y1, float Lv, bool FLAG, bool CHECK_FLAG);
    float At_Level_Check(Point p, int x1, int y1, float Lv, bool FLAG, bool CHECK_FLAG);
    float Find_Att_Special(float Lv1, float Lv2);
    float Find_Def_Special(float Lv1, float Lv2);
    Point Df_Level_Check(Point p, int x1, int y1);
    Point At_Level_Check(Point p, int x1, int y1, float Lv);
    Point Def_Chk_Next(); // find out next anti step which may cause two attack chains, then block up that position
    Point Att_Chk_Next_33();
    Point Att_Chk_Next_23();
    Point Max_Active_Two();
    Point Move();
    Point Random_Move();

    float Defend_Lv[DEFENDRECORD];
    float Attack_Lv[ATTACKRECORD];
    float Attack_Lv_Final;
    float Defend_Lv_Final;

    NodeType myType;
    NodeType antiType;
    NodeType myBoard[BoardSize][BoardSize];  // board to save current board   
    
    Point p_M[ATTACKRECORD];
    Point p_A[DEFENDRECORD];
    Point Defender[DEFENDRECORD];
    Point Attacker[ATTACKRECORD];
    Point Attacker_Final;
    Point Defender_Final;
    Point Defender_Final_Temp;
    Point Attacker_Final_Temp;
};

GXY_AI::GXY_AI(const NodeType board[BoardSize][BoardSize], NodeType type)
{
    myType = type;
    antiType = myType == Black ? White : Black;
    Attack_Lv_Final = 0;
    Defend_Lv_Final = 0;
    for (int i = 0; i < BoardSize; ++i) 
    {
        for (int j = 0; j < BoardSize; ++j) 
        {
            myBoard[i][j] = board[i][j];
        }
    }
}

void GXY_AI::Locate_Point()
{
    int a = 0;
    int b = 0;
    for(int i = 0; i < BoardSize; i++) // draw out Anti Board
    {
        for(int j = 0; j < BoardSize; j++)
        {
            if(myBoard[i][j] == antiType)
            {
                p_A[a].Set(i,j);
                a++; 
            }
            else if(myBoard[i][j] == myType)
            {
                p_M[b].Set(i,j);
                b++;
            }
        }
    }
}

float GXY_AI::Df_Level_Check(Point p, int x1, int y1, float Lv, bool FLAG, bool CHECK_FLAG)
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
            {
                Lv -= 0.25;
            }          
            else if(myBoard[x][y] == Empty)
            {
                if(CHECK_FLAG == false) // used for defending sleep 3 or 4, block up them to becom connected 5
                {
                    if(fabs(Lv - 0.75)<0.000001 || fabs(Lv - 1)<0.000001 || fabs(Lv - 1.75)<0.000001 || fabs(Lv - 2)<0.000001)
                    {
                        if(0 <= (x+x1) && (x+x1) < BoardSize && 0 <= (y+y1) && (y+y1) < BoardSize && myBoard[x+x1][y+y1] == antiType)
                        {     
                            Lv += 0.1;
                                 
                            if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == antiType)
                            {
                                Lv += 0.5;
                            }
                            else if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == myType)
                            {
                                Lv -= 0.25;
                            }
                            else if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == Empty)
                            {
                                Lv += 0;
                            }    
                            else // outside of the board;
                            {
                                Lv -= 0.5;
                            }

                            if(fabs(Lv - 1.1)<0.000001 || fabs(Lv - 1.35)<0.000001 || fabs(Lv - 1.6)<0.000001 || fabs(Lv - 1.85)<0.000001 || fabs(Lv - 2.1)<0.000001 || fabs(Lv - 2.35)<0.000001 || fabs(Lv - 2.6)<0.000001)
                            {
                                if(FLAG == true)
                                {
                                    Defender_Final_Temp.Set(x,y);
                                }                         
                            }  
                        }
                    }
                }
                else // used for check next step defense trends 0.65 1.3 1.05
                {
                    if(fabs(Lv - 0)<0.000001 || fabs(Lv - -0.25)<0.000001)
                    {
                         if(0 <= (x+x1) && (x+x1) < BoardSize && 0 <= (y+y1) && (y+y1) < BoardSize && myBoard[x+x1][y+y1] == antiType)
                        {     
                            Lv += 0.1;    
                            if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == antiType)
                            {
                                Lv += 0.5;
                                if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize && myBoard[x+3*x1][y+3*y1] == antiType)
                                {
                                    Lv += 0.7;
                                }
                                else if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize && myBoard[x+3*x1][y+3*y1] == myType)
                                {
                                    Lv -= 0.25;
                                }
                                else if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize && myBoard[x+3*x1][y+3*y1] == Empty)
                                {
                                    Lv += 0.05;
                                }    
                                else // outside of the board;
                                {
                                    Lv -= 0.5;
                                }
                            }
                        }
                    }
                }
            }   
            break;
        }
        else // outside of the board
        {
            Lv -= 0.25; // just like blocked up by myType chess.
            break;
        }     
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

void GXY_AI::Defend_Final()
{
    for(int i = 0; i < DEFENDRECORD; i++)
    {
        Defend_Lv[i] = 0;
    }

    for(int i = 0; i < DEFENDRECORD; i++)
    {
        Defend_Lv[i] = Defend_Level(i);
    }

    for(int i = 0; i < DEFENDRECORD; i++)
    {
        if(fabs(Defend_Lv[i] - 1.1)<0.000001 || fabs(Defend_Lv[i] - 1.35)<0.000001 || fabs(Defend_Lv[i] - 1.6)<0.000001 || fabs(Defend_Lv[i] - 1.85)<0.000001 || fabs(Defend_Lv[i] - 2.1)<0.000001 || fabs(Defend_Lv[i] - 2.35)<0.000001 || fabs(Defend_Lv[i] - 2.6)<0.000001)   
        {
            Defend_Lv_Final = Defend_Lv[i];
            Defender_Final = Defender_Final_Temp;
            return;
        }
    }

    Defend_Lv_Final = Defend_Lv[0];
    Defender_Final = Defender[0];

    for(int i = 1; i < DEFENDRECORD; i++)
    {
        if(Defend_Lv[i] > Defend_Lv_Final)
        {
            Defend_Lv_Final = Defend_Lv[i];
            Defender_Final = Defender[i];
        }     
    }
    
    for(int i = 0; i < DEFENDRECORD; i++)
    {
        if(fabs(Defend_Lv[i] - 2)<0.000001 || fabs(Defend_Lv[i] - 2.75)<0.000001 || fabs(Defend_Lv[i] - 3)<0.000001)   
        {
            Defend_Lv_Final = Defend_Lv[i];
            Defender_Final = Defender[i];   
        }
    }    

}

float GXY_AI::Defend_Level(int i)
{
    Point UU, RU, RR, RD, DD, LD, LL, LU;
    float Lv_UD1 = 0, Lv_LR1 = 0, Lv_RULD1 = 0, Lv_RDLU1 = 0;
    float Lv_UD2 = 0, Lv_LR2 = 0, Lv_RULD2 = 0, Lv_RDLU2 = 0;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;

    if (p_A[i].Valid() == true && myBoard[p_A[i].x][p_A[i].y] == antiType)
    {    
        // first up then down, first ru then ld, so on
        float tpLv_UD1    =    Df_Level_Check(p_A[i],  0, -1, 0, false, false);
        Lv_UD1    =    Df_Level_Check(p_A[i],  0,  1, tpLv_UD1, true, false);
        float tpLv_RULD1  =    Df_Level_Check(p_A[i],  1, -1, 0, false, false);
        Lv_RULD1  =    Df_Level_Check(p_A[i], -1,  1, tpLv_RULD1, true, false);
        float tpLv_LR1    =    Df_Level_Check(p_A[i],  1,  0, 0, false, false);
        Lv_LR1    =    Df_Level_Check(p_A[i], -1,  0, tpLv_LR1, true, false);
        float tpLv_RDLU1  =    Df_Level_Check(p_A[i],  1,  1, 0, false, false);
        Lv_RDLU1  =    Df_Level_Check(p_A[i], -1, -1, tpLv_RDLU1, true, false);
        // first down then up, first ld then ru, so on
        float tpLv_UD2    =    Df_Level_Check(p_A[i],  0, 1, 0, false, false);
        Lv_UD2    =    Df_Level_Check(p_A[i],  0,  -1, tpLv_UD2, true, false);
        float tpLv_RULD2  =    Df_Level_Check(p_A[i],  -1, 1, 0, false, false);
        Lv_RULD2  =    Df_Level_Check(p_A[i], 1, -1, tpLv_RULD2, true, false);
        float tpLv_LR2    =    Df_Level_Check(p_A[i],  -1, 0, 0, false, false);
        Lv_LR2    =    Df_Level_Check(p_A[i], 1,  0, tpLv_LR2, true, false);
        float tpLv_RDLU2  =    Df_Level_Check(p_A[i],  -1,  -1, 0, false, false);
        Lv_RDLU2  =    Df_Level_Check(p_A[i], 1, 1, tpLv_RDLU2, true, false);

        Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
        Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
        Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
        Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);
        UU  =    Df_Level_Check(p_A[i],  0, -1 );
        RU  =    Df_Level_Check(p_A[i],  1, -1 );
        RR  =    Df_Level_Check(p_A[i],  1,  0 );
        RD  =    Df_Level_Check(p_A[i],  1,  1 );
        DD  =    Df_Level_Check(p_A[i],  0,  1 );
        LD  =    Df_Level_Check(p_A[i], -1,  1 );      
        LL  =    Df_Level_Check(p_A[i], -1,  0 );     
        LU  =    Df_Level_Check(p_A[i], -1, -1 );
        if(fabs(Lv_UD - 1.1)<0.000001 || fabs(Lv_UD - 1.35)<0.000001 || fabs(Lv_UD - 1.6)<0.000001 || fabs(Lv_UD - 1.85)<0.000001 || fabs(Lv_UD - 2.1)<0.000001 || fabs(Lv_UD - 2.35)<0.000001 || fabs(Lv_UD - 2.6)<0.000001)   
        {
            Defend_Lv[i] = Lv_UD;
            return Defend_Lv[i]; 
        }
        if(fabs(Lv_LR - 1.1)<0.000001 || fabs(Lv_LR - 1.35)<0.000001 || fabs(Lv_LR - 1.6)<0.000001 || fabs(Lv_LR - 1.85)<0.000001 || fabs(Lv_LR - 2.1)<0.000001 || fabs(Lv_LR - 2.35)<0.000001 || fabs(Lv_LR - 2.6)<0.000001)   
        {
            Defend_Lv[i] = Lv_LR;
            return Defend_Lv[i]; 
        }
        if(fabs(Lv_RULD - 1.1)<0.000001 || fabs(Lv_RULD - 1.35)<0.000001 || fabs(Lv_RULD - 1.6)<0.000001 || fabs(Lv_RULD - 1.85)<0.000001 || fabs(Lv_RULD - 2.1)<0.000001 || fabs(Lv_RULD - 2.35)<0.000001 || fabs(Lv_RULD - 2.6)<0.000001)   
        {
            Defend_Lv[i] = Lv_RULD;
            return Defend_Lv[i]; 
        }
        if(fabs(Lv_RDLU - 1.1)<0.000001 || fabs(Lv_RDLU - 1.35)<0.000001 || fabs(Lv_RDLU - 1.6)<0.000001 || fabs(Lv_RDLU - 1.85)<0.000001 || fabs(Lv_RDLU - 2.1)<0.000001 || fabs(Lv_RDLU - 2.35)<0.000001 || fabs(Lv_RDLU - 2.6)<0.000001)   
        {
            Defend_Lv[i] = Lv_RDLU;
            return Defend_Lv[i]; 
        }
        // find out max in Lv_UD , Lv_LR , Lv_RULD , Lv_RDLU 
        float max = Lv_UD;
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

        if(fabs(Lv_UD - 2)<0.000001 || fabs(Lv_UD - 2.75)<0.000001 || fabs(Lv_UD - 3)<0.000001)   
        {
            max = Lv_UD;   
            flag = 0; 
        }
        else if(fabs(Lv_LR - 2)<0.000001 || fabs(Lv_LR - 2.75)<0.000001 || fabs(Lv_LR - 3)<0.000001)  
        {
            max = Lv_LR;   
            flag = 1; 
        }
        else if(fabs(Lv_RULD - 2)<0.000001 || fabs(Lv_RULD - 2.75)<0.000001 || fabs(Lv_RULD - 3)<0.000001)  
        {
            max = Lv_RULD;
            flag = 2;
        }
        else if(fabs(Lv_RDLU - 2)<0.000001 || fabs(Lv_RDLU - 2.75)<0.000001 || fabs(Lv_RDLU - 3)<0.000001)  
        {
            max = Lv_RDLU; 
            flag = 3; 
        }


        Defend_Lv[i] = max;
        switch(flag)
        {
            case 0:
            if(UU.Valid() == true)
            {
                Defender[i].Set(UU.x,UU.y);
            } 
            else if(DD.Valid() == true)
            {
                Defender[i].Set(DD.x,DD.y);
            }
            else
            {
                Defend_Lv[i] = 0;
            }
            break;
            case 1:
            if(LL.Valid() == true)
            {
                Defender[i].Set(LL.x,LL.y);
            } 
            else if(RR.Valid() == true)
            {
                Defender[i].Set(RR.x,RR.y);
            }
            else
            {
                Defend_Lv[i] = 0;
            }
            break;
            case 2:
            if(RU.Valid() == true)
            {
                Defender[i].Set(RU.x,RU.y);
            } 
            else if(LD.Valid() == true)
            {
                Defender[i].Set(LD.x,LD.y);
            }
            else
            {
                Defend_Lv[i] = 0;
            }
            break;
            case 3:
            if(RD.Valid() == true)
            {
                Defender[i].Set(RD.x,RD.y);
            } 
            else if(LU.Valid() == true)
            {
                Defender[i].Set(LU.x,LU.y);
            }
            else
            {
                Defend_Lv[i] = 0;
            }
            break;
            default:
            break;
        }
    }
    else
        Defend_Lv[i] = 0;
    return Defend_Lv[i];
}

Point GXY_AI::Def_Chk_Next()
{
    float Lv_UD1 = 0, Lv_LR1 = 0, Lv_RULD1 = 0, Lv_RDLU1 = 0;
    float Lv_UD2 = 0, Lv_LR2 = 0, Lv_RULD2 = 0, Lv_RDLU2 = 0;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;
    int flag = 0;
    for(int x = 0; x < BoardSize; x++)
    {
        for(int y = 0; y < BoardSize; y++)
        {
            if (myBoard[x][y] == Empty)
            {   
                Point point;
                point.Set(x,y); 
                // first up then down, first ru then ld, so on
                float tpLv_UD1    =    Df_Level_Check(point,  0, -1, 0, false, false);
                Lv_UD1    =    Df_Level_Check(point,  0,  1, tpLv_UD1, false, false);
                float tpLv_RULD1  =    Df_Level_Check(point,  1, -1, 0, false, false);
                Lv_RULD1  =    Df_Level_Check(point, -1,  1, tpLv_RULD1, false, false);
                float tpLv_LR1    =    Df_Level_Check(point,  1,  0, 0, false, false);
                Lv_LR1    =    Df_Level_Check(point, -1,  0, tpLv_LR1, false, false);
                float tpLv_RDLU1  =    Df_Level_Check(point,  1,  1, 0, false, false);
                Lv_RDLU1  =    Df_Level_Check(point, -1, -1, tpLv_RDLU1, false, false);
                // first down then up, first ld then ru, so on
                float tpLv_UD2    =    Df_Level_Check(point,  0, 1, 0, false, false);
                Lv_UD2    =    Df_Level_Check(point,  0,  -1, tpLv_UD2, false, false);
                float tpLv_RULD2  =    Df_Level_Check(point,  -1, 1, 0, false, false);
                Lv_RULD2  =    Df_Level_Check(point, 1, -1, tpLv_RULD2, false, false);
                float tpLv_LR2    =    Df_Level_Check(point,  -1, 0, 0, false, false);
                Lv_LR2    =    Df_Level_Check(point, 1,  0, tpLv_LR2, false, false);
                float tpLv_RDLU2  =    Df_Level_Check(point, -1, -1, 0, false, false);
                Lv_RDLU2  =    Df_Level_Check(point, 1, 1, tpLv_RDLU2, false, false);

                Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
                Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
                Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
                Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);

                flag = 0;
                if(fabs(Lv_UD - 1.1)<0.000001 || fabs(Lv_UD - 1.35)<0.000001 || fabs(Lv_UD - 1.6)<0.000001 || fabs(Lv_UD - 1.85)<0.000001 || fabs(Lv_UD - 2.1)<0.000001 || fabs(Lv_UD - 2.35)<0.000001 || fabs(Lv_UD - 2.6)<0.000001 || fabs(Lv_UD - 2)<0.000001 || fabs(Lv_UD - 2.75)<0.000001 || fabs(Lv_UD - 3)<0.000001)
                {
                    flag++;
                }
                if(fabs(Lv_LR - 1.1)<0.000001 || fabs(Lv_LR - 1.35)<0.000001 || fabs(Lv_LR - 1.6)<0.000001 || fabs(Lv_LR - 1.85)<0.000001 || fabs(Lv_LR - 2.1)<0.000001 || fabs(Lv_LR - 2.35)<0.000001 || fabs(Lv_LR - 2.6)<0.000001 || fabs(Lv_LR - 2)<0.000001 || fabs(Lv_LR - 2.75)<0.000001 || fabs(Lv_LR - 3)<0.000001)
                {
                    flag++;
                }
                if(fabs(Lv_RULD - 1.1)<0.000001 || fabs(Lv_RULD - 1.35)<0.000001 || fabs(Lv_RULD - 1.6)<0.000001 || fabs(Lv_RULD - 1.85)<0.000001 || fabs(Lv_RULD - 2.1)<0.000001 || fabs(Lv_RULD - 2.35)<0.000001 || fabs(Lv_RULD - 2.6)<0.000001 || fabs(Lv_RULD - 2)<0.000001 || fabs(Lv_RULD - 2.75)<0.000001 || fabs(Lv_RULD - 3)<0.000001)
                {
                    flag++;
                }
                if(fabs(Lv_RDLU - 1.1)<0.000001 || fabs(Lv_RDLU - 1.35)<0.000001 || fabs(Lv_RDLU - 1.6)<0.000001 || fabs(Lv_RDLU - 1.85)<0.000001 || fabs(Lv_RDLU - 2.1)<0.000001 || fabs(Lv_RDLU - 2.35)<0.000001 || fabs(Lv_RDLU - 2.6)<0.000001 || fabs(Lv_RDLU - 2)<0.000001 || fabs(Lv_RDLU - 2.75)<0.000001 || fabs(Lv_RDLU - 3)<0.000001)
                {
                    flag++;
                }

                tpLv_UD1    =    Df_Level_Check(point,  0, -1, 0, false, true);
                Lv_UD1      =    Df_Level_Check(point,  0,  1, tpLv_UD1, false, true);
                tpLv_RULD1  =    Df_Level_Check(point,  1, -1, 0, false, true);
                Lv_RULD1    =    Df_Level_Check(point, -1,  1, tpLv_RULD1, false, true);
                tpLv_LR1    =    Df_Level_Check(point,  1,  0, 0, false, true);
                Lv_LR1      =    Df_Level_Check(point, -1,  0, tpLv_LR1, false, true);
                tpLv_RDLU1  =    Df_Level_Check(point,  1,  1, 0, false, true);
                Lv_RDLU1    =    Df_Level_Check(point, -1, -1, tpLv_RDLU1, false, true);
                // first down then up, first ld then ru, so on
                tpLv_UD2    =    Df_Level_Check(point,  0, 1, 0, false, true);
                Lv_UD2      =    Df_Level_Check(point,  0,  -1, tpLv_UD2, false, true);
                tpLv_RULD2  =    Df_Level_Check(point,  -1, 1, 0, false, true);
                Lv_RULD2    =    Df_Level_Check(point, 1, -1, tpLv_RULD2, false, true);
                tpLv_LR2    =    Df_Level_Check(point,  -1, 0, 0, false, true);
                Lv_LR2      =    Df_Level_Check(point, 1,  0, tpLv_LR2, false, true);
                tpLv_RDLU2  =    Df_Level_Check(point, -1, -1, 0, false, true);
                Lv_RDLU2    =    Df_Level_Check(point, 1, 1, tpLv_RDLU2, false, true);

                Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
                Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
                Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
                Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);

                if(fabs(Lv_UD - 0.65)<0.000001 || fabs(Lv_UD - 1.05)<0.000001 || fabs(Lv_UD - 1.3)<0.000001)
                {
                    flag++;
                }
                if(fabs(Lv_LR - 0.65)<0.000001 || fabs(Lv_LR - 1.05)<0.000001 || fabs(Lv_LR - 1.3)<0.000001)
                {
                    flag++;
                }
                if(fabs(Lv_RULD - 0.65)<0.000001 || fabs(Lv_RULD - 1.05)<0.000001 || fabs(Lv_RULD - 1.3)<0.000001)
                {
                    flag++;
                }
                if(fabs(Lv_RDLU - 0.65)<0.000001 || fabs(Lv_RDLU - 1.05)<0.000001 || fabs(Lv_RDLU - 1.3)<0.000001)
                {
                    flag++;
                }

                if(flag >= 2)
                {
                    Point p;
                    p.Set(x,y);
                    return p;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    Point p;
    return p;
}

Point GXY_AI::Att_Chk_Next_33()
{
    float Lv_UD1 = 0, Lv_LR1 = 0, Lv_RULD1 = 0, Lv_RDLU1 = 0;
    float Lv_UD2 = 0, Lv_LR2 = 0, Lv_RULD2 = 0, Lv_RDLU2 = 0;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;
    int flagg = 0;
    for(int x = 0; x < BoardSize; x++)
    {
        for(int y = 0; y < BoardSize; y++)
        {
            if (myBoard[x][y] == Empty)
            {   
                Point point;
                point.Set(x,y); 
                // first up then down, first ru then ld, so on
                float tpLv_UD1    =    At_Level_Check(point,  0, -1, 0, false, false);
                Lv_UD1    =    At_Level_Check(point,  0,  1, tpLv_UD1, false, false);
                float tpLv_RULD1  =    At_Level_Check(point,  1, -1, 0, false, false);
                Lv_RULD1  =    At_Level_Check(point, -1,  1, tpLv_RULD1, false, false);
                float tpLv_LR1    =    At_Level_Check(point,  1,  0, 0, false, false);
                Lv_LR1    =    At_Level_Check(point, -1,  0, tpLv_LR1, false, false);
                float tpLv_RDLU1  =    At_Level_Check(point,  1,  1, 0, false, false);
                Lv_RDLU1  =    At_Level_Check(point, -1, -1, tpLv_RDLU1, false, false);
                // first down then up, first ld then ru, so on
                float tpLv_UD2    =    At_Level_Check(point,  0, 1, 0, false, false);
                Lv_UD2    =    At_Level_Check(point,  0,  -1, tpLv_UD2, false, false);
                float tpLv_RULD2  =    At_Level_Check(point,  -1, 1, 0, false, false);
                Lv_RULD2  =    At_Level_Check(point, 1, -1, tpLv_RULD2, false, false);
                float tpLv_LR2    =    At_Level_Check(point,  -1, 0, 0, false, false);
                Lv_LR2    =    At_Level_Check(point, 1,  0, tpLv_LR2, false, false);
                float tpLv_RDLU2  =    At_Level_Check(point, -1, -1, 0, false, false);
                Lv_RDLU2  =    At_Level_Check(point, 1, 1, tpLv_RDLU2, false, false);

                Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
                Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
                Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
                Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);

                flagg = 0;
                if(fabs(Lv_UD - 1.1)<0.000001 || fabs(Lv_UD - 1.35)<0.000001 || fabs(Lv_UD - 1.6)<0.000001 || fabs(Lv_UD - 1.85)<0.000001 || fabs(Lv_UD - 2.1)<0.000001 || fabs(Lv_UD - 2.35)<0.000001 || fabs(Lv_UD - 2.6)<0.000001 || fabs(Lv_UD - 2)<0.000001 || fabs(Lv_UD - 2.75)<0.000001 || fabs(Lv_UD - 3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_LR - 1.1)<0.000001 || fabs(Lv_LR - 1.35)<0.000001 || fabs(Lv_LR - 1.6)<0.000001 || fabs(Lv_LR - 1.85)<0.000001 || fabs(Lv_LR - 2.1)<0.000001 || fabs(Lv_LR - 2.35)<0.000001 || fabs(Lv_LR - 2.6)<0.000001 || fabs(Lv_LR - 2)<0.000001 || fabs(Lv_LR - 2.75)<0.000001 || fabs(Lv_LR - 3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RULD - 1.1)<0.000001 || fabs(Lv_RULD - 1.35)<0.000001 || fabs(Lv_RULD - 1.6)<0.000001 || fabs(Lv_RULD - 1.85)<0.000001 || fabs(Lv_RULD - 2.1)<0.000001 || fabs(Lv_RULD - 2.35)<0.000001 || fabs(Lv_RULD - 2.6)<0.000001 || fabs(Lv_RULD - 2)<0.000001 || fabs(Lv_RULD - 2.75)<0.000001 || fabs(Lv_RULD - 3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RDLU - 1.1)<0.000001 || fabs(Lv_RDLU - 1.35)<0.000001 || fabs(Lv_RDLU - 1.6)<0.000001 || fabs(Lv_RDLU - 1.85)<0.000001 || fabs(Lv_RDLU - 2.1)<0.000001 || fabs(Lv_RDLU - 2.35)<0.000001 || fabs(Lv_RDLU - 2.6)<0.000001 || fabs(Lv_RDLU - 2)<0.000001 || fabs(Lv_RDLU - 2.75)<0.000001 || fabs(Lv_RDLU - 3)<0.000001)
                {
                    flagg++;
                }

                tpLv_UD1    =    At_Level_Check(point,  0, -1, 0, false, true);
                Lv_UD1      =    At_Level_Check(point,  0,  1, tpLv_UD1, false, true);
                tpLv_RULD1  =    At_Level_Check(point,  1, -1, 0, false, true);
                Lv_RULD1    =    At_Level_Check(point, -1,  1, tpLv_RULD1, false, true);
                tpLv_LR1    =    At_Level_Check(point,  1,  0, 0, false, true);
                Lv_LR1      =    At_Level_Check(point, -1,  0, tpLv_LR1, false, true);
                tpLv_RDLU1  =    At_Level_Check(point,  1,  1, 0, false, true);
                Lv_RDLU1    =    At_Level_Check(point, -1, -1, tpLv_RDLU1, false, true);
                // first down then up, first ld then ru, so on
                tpLv_UD2    =    At_Level_Check(point,  0, 1, 0, false, true);
                Lv_UD2      =    At_Level_Check(point,  0,  -1, tpLv_UD2, false, true);
                tpLv_RULD2  =    At_Level_Check(point,  -1, 1, 0, false, true);
                Lv_RULD2    =    At_Level_Check(point, 1, -1, tpLv_RULD2, false, true);
                tpLv_LR2    =    At_Level_Check(point,  -1, 0, 0, false, true);
                Lv_LR2      =    At_Level_Check(point, 1,  0, tpLv_LR2, false, true);
                tpLv_RDLU2  =    At_Level_Check(point, -1, -1, 0, false, true);
                Lv_RDLU2    =    At_Level_Check(point, 1, 1, tpLv_RDLU2, false, true);

                Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
                Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
                Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
                Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);

                if(fabs(Lv_UD - 0.65)<0.000001 || fabs(Lv_UD - 1.05)<0.000001 || fabs(Lv_UD - 1.3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_LR - 0.65)<0.000001 || fabs(Lv_LR - 1.05)<0.000001 || fabs(Lv_LR - 1.3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RULD - 0.65)<0.000001 || fabs(Lv_RULD - 1.05)<0.000001 || fabs(Lv_RULD - 1.3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RDLU - 0.65)<0.000001 || fabs(Lv_RDLU - 1.05)<0.000001 || fabs(Lv_RDLU - 1.3)<0.000001)
                {
                    flagg++;
                }

                if(flagg >= 2)
                {
                    Point pp;
                    pp.Set(x,y);
                    return pp;
                }
            }
        }
    }
    Point p;
    return p;
}

Point GXY_AI::Att_Chk_Next_23()
{
    float Lv_UD1 = 0, Lv_LR1 = 0, Lv_RULD1 = 0, Lv_RDLU1 = 0;
    float Lv_UD2 = 0, Lv_LR2 = 0, Lv_RULD2 = 0, Lv_RDLU2 = 0;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;
    int flagg = 0;
    int flag[BoardSize][BoardSize];
    for(int x = 0; x < BoardSize; x++)
    {
        for(int y = 0; y < BoardSize; y++)
        {
            if (myBoard[x][y] == Empty)
            {   
                Point point;
                point.Set(x,y); 
                // first up then down, first ru then ld, so on
                float tpLv_UD1    =    At_Level_Check(point,  0, -1, 0, false, false);
                Lv_UD1    =    At_Level_Check(point,  0,  1, tpLv_UD1, false, false);
                float tpLv_RULD1  =    At_Level_Check(point,  1, -1, 0, false, false);
                Lv_RULD1  =    At_Level_Check(point, -1,  1, tpLv_RULD1, false, false);
                float tpLv_LR1    =    At_Level_Check(point,  1,  0, 0, false, false);
                Lv_LR1    =    At_Level_Check(point, -1,  0, tpLv_LR1, false, false);
                float tpLv_RDLU1  =    At_Level_Check(point,  1,  1, 0, false, false);
                Lv_RDLU1  =    At_Level_Check(point, -1, -1, tpLv_RDLU1, false, false);
                // first down then up, first ld then ru, so on
                float tpLv_UD2    =    At_Level_Check(point,  0, 1, 0, false, false);
                Lv_UD2    =    At_Level_Check(point,  0,  -1, tpLv_UD2, false, false);
                float tpLv_RULD2  =    At_Level_Check(point,  -1, 1, 0, false, false);
                Lv_RULD2  =    At_Level_Check(point, 1, -1, tpLv_RULD2, false, false);
                float tpLv_LR2    =    At_Level_Check(point,  -1, 0, 0, false, false);
                Lv_LR2    =    At_Level_Check(point, 1,  0, tpLv_LR2, false, false);
                float tpLv_RDLU2  =    At_Level_Check(point, -1, -1, 0, false, false);
                Lv_RDLU2  =    At_Level_Check(point, 1, 1, tpLv_RDLU2, false, false);

                Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
                Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
                Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
                Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);

                flagg = 0;
                if(fabs(Lv_UD - 1.1)<0.000001 || fabs(Lv_UD - 1.35)<0.000001 || fabs(Lv_UD - 1.6)<0.000001 || fabs(Lv_UD - 1.85)<0.000001 || fabs(Lv_UD - 2.1)<0.000001 || fabs(Lv_UD - 2.35)<0.000001 || fabs(Lv_UD - 2.6)<0.000001 || fabs(Lv_UD - 2)<0.000001 || fabs(Lv_UD - 2.75)<0.000001 || fabs(Lv_UD - 3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_LR - 1.1)<0.000001 || fabs(Lv_LR - 1.35)<0.000001 || fabs(Lv_LR - 1.6)<0.000001 || fabs(Lv_LR - 1.85)<0.000001 || fabs(Lv_LR - 2.1)<0.000001 || fabs(Lv_LR - 2.35)<0.000001 || fabs(Lv_LR - 2.6)<0.000001 || fabs(Lv_LR - 2)<0.000001 || fabs(Lv_LR - 2.75)<0.000001 || fabs(Lv_LR - 3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RULD - 1.1)<0.000001 || fabs(Lv_RULD - 1.35)<0.000001 || fabs(Lv_RULD - 1.6)<0.000001 || fabs(Lv_RULD - 1.85)<0.000001 || fabs(Lv_RULD - 2.1)<0.000001 || fabs(Lv_RULD - 2.35)<0.000001 || fabs(Lv_RULD - 2.6)<0.000001 || fabs(Lv_RULD - 2)<0.000001 || fabs(Lv_RULD - 2.75)<0.000001 || fabs(Lv_RULD - 3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RDLU - 1.1)<0.000001 || fabs(Lv_RDLU - 1.35)<0.000001 || fabs(Lv_RDLU - 1.6)<0.000001 || fabs(Lv_RDLU - 1.85)<0.000001 || fabs(Lv_RDLU - 2.1)<0.000001 || fabs(Lv_RDLU - 2.35)<0.000001 || fabs(Lv_RDLU - 2.6)<0.000001 || fabs(Lv_RDLU - 2)<0.000001 || fabs(Lv_RDLU - 2.75)<0.000001 || fabs(Lv_RDLU - 3)<0.000001)
                {
                    flagg++;
                }

                tpLv_UD1    =    At_Level_Check(point,  0, -1, 0, false, true);
                Lv_UD1      =    At_Level_Check(point,  0,  1, tpLv_UD1, false, true);
                tpLv_RULD1  =    At_Level_Check(point,  1, -1, 0, false, true);
                Lv_RULD1    =    At_Level_Check(point, -1,  1, tpLv_RULD1, false, true);
                tpLv_LR1    =    At_Level_Check(point,  1,  0, 0, false, true);
                Lv_LR1      =    At_Level_Check(point, -1,  0, tpLv_LR1, false, true);
                tpLv_RDLU1  =    At_Level_Check(point,  1,  1, 0, false, true);
                Lv_RDLU1    =    At_Level_Check(point, -1, -1, tpLv_RDLU1, false, true);
                // first down then up, first ld then ru, so on
                tpLv_UD2    =    At_Level_Check(point,  0, 1, 0, false, true);
                Lv_UD2      =    At_Level_Check(point,  0,  -1, tpLv_UD2, false, true);
                tpLv_RULD2  =    At_Level_Check(point,  -1, 1, 0, false, true);
                Lv_RULD2    =    At_Level_Check(point, 1, -1, tpLv_RULD2, false, true);
                tpLv_LR2    =    At_Level_Check(point,  -1, 0, 0, false, true);
                Lv_LR2      =    At_Level_Check(point, 1,  0, tpLv_LR2, false, true);
                tpLv_RDLU2  =    At_Level_Check(point, -1, -1, 0, false, true);
                Lv_RDLU2    =    At_Level_Check(point, 1, 1, tpLv_RDLU2, false, true);

                Lv_UD = Find_Def_Special(Lv_UD1, Lv_UD2);
                Lv_LR = Find_Def_Special(Lv_LR1, Lv_LR2);
                Lv_RULD = Find_Def_Special(Lv_RULD1, Lv_RULD2);
                Lv_RDLU = Find_Def_Special(Lv_RDLU1, Lv_RDLU2);

                if(fabs(Lv_UD - 0.65)<0.000001 || fabs(Lv_UD - 1.05)<0.000001 || fabs(Lv_UD - 1.3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_LR - 0.65)<0.000001 || fabs(Lv_LR - 1.05)<0.000001 || fabs(Lv_LR - 1.3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RULD - 0.65)<0.000001 || fabs(Lv_RULD - 1.05)<0.000001 || fabs(Lv_RULD - 1.3)<0.000001)
                {
                    flagg++;
                }
                if(fabs(Lv_RDLU - 0.65)<0.000001 || fabs(Lv_RDLU - 1.05)<0.000001 || fabs(Lv_RDLU - 1.3)<0.000001)
                {
                    flagg++;
                }
                
                for(int i = 0; i < BoardSize; i++)
                    for(int j = 0; j < BoardSize; j++)
                        flag[i][j] = 0;

                if(flagg == 1)
                {
                    // Up-Down
                    // Type-1
                    if(myBoard[x][y-1] == myType && myBoard[x][y-2] == Empty && myBoard[x][y-3] == Empty && myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y+1] == myType && myBoard[x][y+2] == Empty && myBoard[x][y+3] == Empty && myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-2
                    else if(myBoard[x][y-1] == myType && myBoard[x][y-2] == Empty && myBoard[x][y-3] == Empty && myBoard[x][y-4] == Empty && myBoard[x][y+1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty && myBoard[x][y-3] == Empty && myBoard[x][y+1] == myType && myBoard[x][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y+1] == myType && myBoard[x][y+2] == Empty && myBoard[x][y+3] == Empty && myBoard[x][y+4] == Empty && myBoard[x][y-1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty && myBoard[x][y+3] == Empty && myBoard[x][y-1] == myType && myBoard[x][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-3
                    else if(myBoard[x][y-1] == Empty && myBoard[x][y-2] == myType && myBoard[x][y-3] == Empty && myBoard[x][y-4] == Empty && myBoard[x][y+1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty && myBoard[x][y+1] == Empty && myBoard[x][y+2] == myType && myBoard[x][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y+1] == Empty && myBoard[x][y+2] == myType && myBoard[x][y+3] == Empty && myBoard[x][y+4] == Empty && myBoard[x][y-1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty && myBoard[x][y-1] == Empty && myBoard[x][y-2] == myType && myBoard[x][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-4
                    else if(myBoard[x][y-1] == Empty && myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty && myBoard[x][y+3] == myType && myBoard[x][y+4] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x][y+1] == Empty && myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty && myBoard[x][y-3] == myType && myBoard[x][y-4] == Empty)
                        flag[x][y] ++;

                    // Right-Up-Left-Down
                    // Type-1
                    if(myBoard[x+1][y-1] == myType && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == Empty && myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y+1] == myType && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == Empty && myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-2
                    else if(myBoard[x+1][y-1] == myType && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == Empty && myBoard[x+4][y-4] == Empty && myBoard[x-1][y+1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == Empty && myBoard[x-1][y+1] == myType && myBoard[x-2][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y+1] == myType && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == Empty && myBoard[x-4][y+4] == Empty && myBoard[x+1][y-1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == Empty && myBoard[x+1][y-1] == myType && myBoard[x+2][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-3
                    else if(myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == myType && myBoard[x+3][y-3] == Empty && myBoard[x+4][y-4] == Empty && myBoard[x-1][y+1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty && myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == myType && myBoard[x-2][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == myType && myBoard[x-3][y+3] == Empty && myBoard[x-4][y+4] == Empty && myBoard[x+1][y-1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty && myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == myType && myBoard[x+2][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-4
                    else if(myBoard[x+1][y-1] == Empty && myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == myType && myBoard[x-4][y+4] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y+1] == Empty && myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == myType && myBoard[x+4][y-4] == Empty)
                        flag[x][y] ++;

                    // Left-Right
                    // Type-1
                    if(myBoard[x-1][y] == myType && myBoard[x-2][y] == Empty && myBoard[x-3][y] == Empty && myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y] == myType && myBoard[x+2][y] == Empty && myBoard[x+3][y] == Empty && myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty)
                        flag[x][y] ++;
                    // Type-2
                    else if(myBoard[x-1][y] == myType && myBoard[x-2][y] == Empty && myBoard[x-3][y] == Empty && myBoard[x-4][y] == Empty && myBoard[x+1][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty && myBoard[x-3][y] == Empty && myBoard[x+1][y] == myType && myBoard[x+2][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y] == myType && myBoard[x+2][y] == Empty && myBoard[x+3][y] == Empty && myBoard[x+4][y] == Empty && myBoard[x-1][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty && myBoard[x+3][y] == Empty && myBoard[x-1][y] == myType && myBoard[x-2][y] == Empty)
                        flag[x][y] ++;
                    // Type-3
                    else if(myBoard[x-1][y] == Empty && myBoard[x-2][y] == myType && myBoard[x-3][y] == Empty && myBoard[x-4][y] == Empty && myBoard[x+1][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty && myBoard[x+1][y] == Empty && myBoard[x+2][y] == myType && myBoard[x+2][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y] == Empty && myBoard[x+2][y] == myType && myBoard[x+3][y] == Empty && myBoard[x+4][y] == Empty && myBoard[x-1][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty && myBoard[x-1][y] == Empty && myBoard[x-2][y] == myType && myBoard[x-2][y] == Empty)
                        flag[x][y] ++;
                    // Type-4
                    else if(myBoard[x-1][y] == Empty && myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty && myBoard[x+3][y] == myType && myBoard[x+4][y] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y] == Empty && myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty && myBoard[x-3][y] == myType && myBoard[x-4][y] == Empty)
                        flag[x][y] ++;

                    // Left-Up-Right-Down
                    // Type-1
                    if(myBoard[x-1][y-1] == myType && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == Empty && myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y+1] == myType && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == Empty && myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-2
                    else if(myBoard[x-1][y-1] == myType && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == Empty && myBoard[x-4][y-4] == Empty && myBoard[x+1][y+1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == Empty && myBoard[x+1][y+1] == myType && myBoard[x+2][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y+1] == myType && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == Empty && myBoard[x+4][y+4] == Empty && myBoard[x-1][y-1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == Empty && myBoard[x-1][y-1] == myType && myBoard[x-2][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-3
                    else if(myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == myType && myBoard[x-3][y-3] == Empty && myBoard[x-4][y-4] == Empty && myBoard[x+1][y+1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty && myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == myType && myBoard[x+2][y+2] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == myType && myBoard[x+3][y+3] == Empty && myBoard[x+4][y+4] == Empty && myBoard[x-1][y-1] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty && myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == myType && myBoard[x-2][y-2] == Empty)
                        flag[x][y] ++;
                    // Type-4
                    else if(myBoard[x-1][y-1] == Empty && myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == myType && myBoard[x+4][y+4] == Empty)
                        flag[x][y] ++;
                    else if(myBoard[x+1][y+1] == Empty && myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == myType && myBoard[x-4][y-4] == Empty)
                        flag[x][y] ++;
                }
            }
        }
    }
    int max = flag[0][0];
    Point pp;
    pp.Set(0,0);
    // find MAX!!!
    for(int x = 0; x < BoardSize; x++)
    {
        for(int y = 0; y < BoardSize; y++)
        {
            if(flag[x][y] > max)
            {
                max = flag[x][y];
                pp.Set(x,y);
            }
        }
    }
    if(max >= 1)
    {
        return pp;
    }
    else
    {
        Point p;
        return p;
    }
}

float GXY_AI::At_Level_Check(Point p, int x1, int y1, float Lv, bool FLAG, bool CHECK_FLAG)
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
            {
                Lv -= 0.25;
            }
            else if(myBoard[x][y] == Empty)
            {
                if(CHECK_FLAG == false)
                {
                    if(fabs(Lv - 0.75)<0.000001 || fabs(Lv - 1)<0.000001 || fabs(Lv - 1.75)<0.000001 || fabs(Lv - 2)<0.000001)
                    {
                        if(0 <= (x+x1) && (x+x1) < BoardSize && 0 <= (y+y1) && (y+y1) < BoardSize && myBoard[x+x1][y+y1] == myType)
                        {          
                            Lv += 0.1;
                                
                            if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == myType)
                            {                    
                                Lv += 0.5;
                            }
                            else if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == antiType)
                            {   
                                Lv -= 0.25;
                            }
                            else if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == Empty)
                            {
                                Lv += 0;
                            }    
                            else // outside of the board;
                            {
                                Lv -= 0.5;
                            }
                            if(fabs(Lv - 1.1)<0.000001 || fabs(Lv - 1.35)<0.000001 || fabs(Lv - 1.6)<0.000001 || fabs(Lv - 1.85)<0.000001 || fabs(Lv - 2.1)<0.000001 || fabs(Lv - 2.35)<0.000001 || fabs(Lv - 2.6)<0.000001)
                            {
                                if(FLAG == true)
                                {
                                    Attacker_Final_Temp.Set(x,y); 
                                }
                            }      
                        }
                    }
                    if(fabs(Lv - -0.25)<0.000001 || fabs(Lv - 0.75)<0.000001 || fabs(Lv - 1.75)<0.000001) // 0.75 repeated, couldn't find a valid solution to de-bug!!!
                    {
                        if(0 <= (x+x1) && (x+x1) < BoardSize && 0 <= (y+y1) && (y+y1) < BoardSize)
                        {
                            if(myBoard[x+x1][y+y1] == antiType)
                            {
                                Lv -= 0.1;
                            }          
                            else
                            {
                                if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize)
                                {
                                    if(myBoard[x+2*x1][y+2*y1] == antiType)
                                    {
                                        Lv -= 0.1;
                                    }
                                    else
                                    {
                                        if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize)
                                        {
                                            if(myBoard[x+3*x1][y+3*y1] == antiType)
                                            {
                                                Lv -= 0.1;
                                            }
                                        }
                                        else
                                        {
                                            Lv -= 0.1;
                                        }
                                    }
                                }
                                else
                                {
                                    Lv -= 0.1;
                                }
                            }
                        }
                        else
                        {
                            Lv -= 0.1;
                        }
                    }
                }
                else // used for check next step defense trends 0.65 1.3 1.05
                {
                    if(fabs(Lv - 0)<0.000001 || fabs(Lv - -0.25)<0.000001)
                    {
                         if(0 <= (x+x1) && (x+x1) < BoardSize && 0 <= (y+y1) && (y+y1) < BoardSize && myBoard[x+x1][y+y1] == myType)
                        {     
                            Lv += 0.1;    
                            if(0 <= (x+2*x1) && (x+2*x1) < BoardSize && 0 <= (y+2*y1) && (y+2*y1) < BoardSize && myBoard[x+2*x1][y+2*y1] == myType)
                            {
                                Lv += 0.5;
                                if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize && myBoard[x+3*x1][y+3*y1] == myType)
                                {
                                    Lv += 0.7;
                                }
                                else if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize && myBoard[x+3*x1][y+3*y1] == antiType)
                                {
                                    Lv -= 0.25;
                                }
                                else if(0 <= (x+3*x1) && (x+3*x1) < BoardSize && 0 <= (y+3*y1) && (y+3*y1) < BoardSize && myBoard[x+3*x1][y+3*y1] == Empty)
                                {
                                    Lv += 0.05;
                                }    
                                else // outside of the board;
                                {
                                    Lv -= 0.5;
                                }
                            }
                        }
                    }
                }

            }
            break;
        }
        else // outside of the board
        {
            Lv -= 0.25; // the same as blocked by antiType chess
            break;
        }
    }
    return Lv;
}

Point GXY_AI::At_Level_Check(Point p, int x1, int y1, float Lv)
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
                if(fabs(Lv - 2)<0.000001) // if myType is 3 connected, it'd be better not to move towards antiType chess.
                {
                    if(myBoard[x+x1][y+y1] == antiType)
                    {
                        return pp;
                    }
                }           
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
    for(int i = 0; i < ATTACKRECORD; i++)
    {
        Attack_Lv[i] = 0;
    }

    for(int i = 0; i < ATTACKRECORD; i++)
    {
        Attack_Lv[i] = Attack_Level(i);
    }

    for(int i = 0; i < ATTACKRECORD; i++)
    {
        if(fabs(Attack_Lv[i] - 1.1)<0.000001 || fabs(Attack_Lv[i] - 1.35)<0.000001 || fabs(Attack_Lv[i] - 1.6)<0.000001 || fabs(Attack_Lv[i] - 1.85)<0.000001 || fabs(Attack_Lv[i] - 2.1)<0.000001 || fabs(Attack_Lv[i] - 2.35)<0.000001 || fabs(Attack_Lv[i] - 2.6)<0.000001)   
        {
            Attack_Lv_Final = Attack_Lv[i];
            Attacker_Final = Attacker_Final_Temp;
            return;
        }
    }

    Attack_Lv_Final = Attack_Lv[0];
    Attacker_Final = Attacker[0];

    for(int i = 1; i < ATTACKRECORD; i++)
    {
        if(Attack_Lv[i] > Attack_Lv_Final)
        {
            Attack_Lv_Final = Attack_Lv[i];
            Attacker_Final = Attacker[i];
        }     
    }
    
    for(int i = 0; i < ATTACKRECORD; i++)
    {
        if(fabs(Attack_Lv[i] - 2)<0.000001 || fabs(Attack_Lv[i] - 2.75)<0.000001 || fabs(Attack_Lv[i] - 3)<0.000001)   
        {
            Attack_Lv_Final = Attack_Lv[i];
            Attacker_Final = Attacker[i];   
        }
    }    
}

float GXY_AI::Attack_Level(int i)
{
    Point UU, RU, RR, RD, DD, LD, LL, LU;
    float Lv_UD1 = 0, Lv_LR1 = 0, Lv_RULD1 = 0, Lv_RDLU1 = 0;
    float Lv_UD2 = 0, Lv_LR2 = 0, Lv_RULD2 = 0, Lv_RDLU2 = 0;
    float Lv_UD = 0, Lv_LR = 0, Lv_RULD = 0, Lv_RDLU = 0;

    if (p_M[i].Valid() == true && myBoard[p_M[i].x][p_M[i].y] == myType)
    {   
        // first up then down, first ru then ld, so on
        float tpLv_UD1    =    At_Level_Check(p_M[i],  0, -1, 0, false, false);
        Lv_UD1    =    At_Level_Check(p_M[i],  0,  1, tpLv_UD1, true, false);
        float tpLv_RULD1  =    At_Level_Check(p_M[i],  1, -1, 0, false, false);
        Lv_RULD1  =    At_Level_Check(p_M[i], -1,  1, tpLv_RULD1, true, false);
        float tpLv_LR1    =    At_Level_Check(p_M[i],  1,  0, 0, false, false);
        Lv_LR1    =    At_Level_Check(p_M[i], -1,  0, tpLv_LR1, true, false);
        float tpLv_RDLU1  =    At_Level_Check(p_M[i],  1,  1, 0, false, false);
        Lv_RDLU1  =    At_Level_Check(p_M[i], -1, -1, tpLv_RDLU1, true, false);
        // first down then up, first ld then ru, so on
        float tpLv_UD2    =    At_Level_Check(p_M[i],  0, 1, 0, false, false);
        Lv_UD2    =    At_Level_Check(p_M[i],  0,  -1, tpLv_UD2, true, false);
        float tpLv_RULD2  =    At_Level_Check(p_M[i],  -1, 1, 0, false, false);
        Lv_RULD2  =    At_Level_Check(p_M[i], 1, -1, tpLv_RULD2, true, false);
        float tpLv_LR2    =    At_Level_Check(p_M[i],  -1, 0, 0, false, false);
        Lv_LR2    =    At_Level_Check(p_M[i], 1,  0, tpLv_LR2, true, false);
        float tpLv_RDLU2  =    At_Level_Check(p_M[i], -1, -1, 0, false, false);
        Lv_RDLU2  =    At_Level_Check(p_M[i], 1, 1, tpLv_RDLU2, true, false);

        Lv_UD = Find_Att_Special(Lv_UD1, Lv_UD2);
        Lv_LR = Find_Att_Special(Lv_LR1, Lv_LR2);
        Lv_RULD = Find_Att_Special(Lv_RULD1, Lv_RULD2);
        Lv_RDLU = Find_Att_Special(Lv_RDLU1, Lv_RDLU2);

        UU  =    At_Level_Check(p_M[i],  0, -1, Lv_UD );
        RU  =    At_Level_Check(p_M[i],  1, -1, Lv_RULD );
        RR  =    At_Level_Check(p_M[i],  1,  0, Lv_LR );
        RD  =    At_Level_Check(p_M[i],  1,  1, Lv_RDLU );
        DD  =    At_Level_Check(p_M[i],  0,  1, Lv_UD );
        LD  =    At_Level_Check(p_M[i], -1,  1, Lv_RULD );      
        LL  =    At_Level_Check(p_M[i], -1,  0, Lv_LR );     
        LU  =    At_Level_Check(p_M[i], -1, -1, Lv_RDLU );

        if(fabs(Lv_UD - 1.1)<0.000001 || fabs(Lv_UD - 1.35)<0.000001 || fabs(Lv_UD - 1.6)<0.000001 || fabs(Lv_UD - 1.85)<0.000001 || fabs(Lv_UD - 2.1)<0.000001 || fabs(Lv_UD - 2.35)<0.000001 || fabs(Lv_UD - 2.6)<0.000001)   
        {
            Attack_Lv[i] = Lv_UD;
            return Attack_Lv[i]; 
        }
        if(fabs(Lv_LR - 1.1)<0.000001 || fabs(Lv_LR - 1.35)<0.000001 || fabs(Lv_LR - 1.6)<0.000001 || fabs(Lv_LR - 1.85)<0.000001 || fabs(Lv_LR - 2.1)<0.000001 || fabs(Lv_LR - 2.35)<0.000001 || fabs(Lv_LR - 2.6)<0.000001)   
        {
            Attack_Lv[i] = Lv_LR;
            return Attack_Lv[i]; 
        }
        if(fabs(Lv_RULD - 1.1)<0.000001 || fabs(Lv_RULD - 1.35)<0.000001 || fabs(Lv_RULD - 1.6)<0.000001 || fabs(Lv_RULD - 1.85)<0.000001 || fabs(Lv_RULD - 2.1)<0.000001 || fabs(Lv_RULD - 2.35)<0.000001 || fabs(Lv_RULD - 2.6)<0.000001)   
        {
            Attack_Lv[i] = Lv_RULD;
            return Attack_Lv[i]; 
        }
        if(fabs(Lv_RDLU - 1.1)<0.000001 || fabs(Lv_RDLU - 1.35)<0.000001 || fabs(Lv_RDLU - 1.6)<0.000001 || fabs(Lv_RDLU - 1.85)<0.000001 || fabs(Lv_RDLU - 2.1)<0.000001 || fabs(Lv_RDLU - 2.35)<0.000001 || fabs(Lv_RDLU - 2.6)<0.000001)   
        {
            Attack_Lv[i] = Lv_RDLU;
            return Attack_Lv[i]; 
        }
        // some situations with high lv but could not make contributions to 5 connect!
        if(fabs(Lv_UD - -0.35)<0.000001 || fabs(Lv_UD - 0.65)<0.000001 || fabs(Lv_UD - 1.65)<0.000001 || fabs(Lv_UD - -0.5)<0.000001 || fabs(Lv_UD - 0.5)<0.000001 || fabs(Lv_UD - 1.5)<0.000001 || fabs(Lv_UD - 2.5)<0.000001)
            Lv_UD = -0.9;
        if(fabs(Lv_LR - -0.35)<0.000001 || fabs(Lv_LR - 0.65)<0.000001 || fabs(Lv_LR - 1.65)<0.000001 || fabs(Lv_LR - -0.5)<0.000001 || fabs(Lv_LR - 0.5)<0.000001 || fabs(Lv_LR - 1.5)<0.000001 || fabs(Lv_LR - 2.5)<0.000001)
            Lv_LR = -0.9;
        if(fabs(Lv_RULD - -0.35)<0.000001 || fabs(Lv_RULD - 0.65)<0.000001 || fabs(Lv_RULD - 1.65)<0.000001 || fabs(Lv_RULD - -0.5)<0.000001 || fabs(Lv_RULD - 0.5)<0.000001 || fabs(Lv_RULD - 1.5)<0.000001 || fabs(Lv_RULD - 2.5)<0.000001)
            Lv_RULD = -0.9;
        if(fabs(Lv_RDLU - -0.35)<0.000001 || fabs(Lv_RDLU - 0.65)<0.000001 || fabs(Lv_RDLU - 1.65)<0.000001 || fabs(Lv_RDLU - -0.5)<0.000001 || fabs(Lv_RDLU - 0.5)<0.000001 || fabs(Lv_RDLU - 1.5)<0.000001 || fabs(Lv_RDLU - 2.5)<0.000001)
            Lv_RDLU = -0.9;

        float max = Lv_UD;
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
        if(fabs(Lv_UD - 2)<0.000001 || fabs(Lv_UD - 2.75)<0.000001 || fabs(Lv_UD - 3)<0.000001)   
        {
            max = Lv_UD;   
            flag = 0; 
        }
        else if(fabs(Lv_LR - 2)<0.000001 || fabs(Lv_LR - 2.75)<0.000001 || fabs(Lv_LR - 3)<0.000001)
        {
            max = Lv_LR;   
            flag = 1; 
        }
        else if(fabs(Lv_RULD - 2)<0.000001 || fabs(Lv_RULD - 2.75)<0.000001 || fabs(Lv_RULD - 3)<0.000001)
        {
            max = Lv_RULD;
            flag = 2;
        }
        else if(fabs(Lv_RDLU - 2)<0.000001 || fabs(Lv_RDLU - 2.75)<0.000001 || fabs(Lv_RDLU - 3)<0.000001)
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

float GXY_AI::Find_Att_Special(float Lv1, float Lv2)
{
    if(fabs(Lv1 - 1.1)<0.000001 || fabs(Lv1 - 1.35)<0.000001 || fabs(Lv1 - 1.6)<0.000001 || fabs(Lv1 - 1.85)<0.000001 || fabs(Lv1 - 2.1)<0.000001 || fabs(Lv1 - 2.35)<0.000001 || fabs(Lv1 - 2.6)<0.000001 || fabs(Lv1 - -0.35)<0.000001 || fabs(Lv1 - 0.65)<0.000001 || fabs(Lv1 - 1.65)<0.000001 || fabs(Lv1 - -0.5)<0.000001 || fabs(Lv1 - 0.5)<0.000001 || fabs(Lv1 - 1.5)<0.000001 || fabs(Lv1 - 2.5)<0.000001 || fabs(Lv1 - 2)<0.000001 || fabs(Lv1 - 2.75)<0.000001 || fabs(Lv1 - 3)<0.000001)
    {
        return Lv1;
    }
    else if(fabs(Lv2 - 1.1)<0.000001 || fabs(Lv2 - 1.35)<0.000001 || fabs(Lv2 - 1.6)<0.000001 || fabs(Lv2 - 1.85)<0.000001 || fabs(Lv2 - 2.1)<0.000001 || fabs(Lv2 - 2.35)<0.000001 || fabs(Lv2 - 2.6)<0.000001 || fabs(Lv2 - -0.35)<0.000001 || fabs(Lv2 - 0.65)<0.000001 || fabs(Lv2 - 1.65)<0.000001 || fabs(Lv2 - -0.5)<0.000001 || fabs(Lv2 - 0.5)<0.000001 || fabs(Lv2 - 1.5)<0.000001 || fabs(Lv2 - 2.5)<0.000001 || fabs(Lv2 - 2)<0.000001 || fabs(Lv2 - 2.75)<0.000001 || fabs(Lv2 - 3)<0.000001)
    {
        return Lv2;
    }
    else
    {
        if(Lv1 > Lv2)
            return Lv1;
        else
            return Lv2;
    }
}

float GXY_AI::Find_Def_Special(float Lv1, float Lv2)
{
    if(fabs(Lv1 - 1.1)<0.000001 || fabs(Lv1 - 1.35)<0.000001 || fabs(Lv1 - 1.6)<0.000001 || fabs(Lv1 - 1.85)<0.000001 || fabs(Lv1 - 2.1)<0.000001 || fabs(Lv1 - 2.35)<0.000001 || fabs(Lv1 - 2.6)<0.000001 || fabs(Lv1 - 2)<0.000001 || fabs(Lv1 - 2.75)<0.000001 || fabs(Lv1 - 3)<0.000001 || fabs(Lv1 - 0.65)<0.000001 || fabs(Lv1 - 1.05)<0.000001 || fabs(Lv1 - 1.3)<0.000001)
    {
        return Lv1;
    }
    else if(fabs(Lv2 - 1.1)<0.000001 || fabs(Lv2 - 1.35)<0.000001 || fabs(Lv2 - 1.6)<0.000001 || fabs(Lv2 - 1.85)<0.000001 || fabs(Lv2 - 2.1)<0.000001 || fabs(Lv2 - 2.35)<0.000001 || fabs(Lv2 - 2.6)<0.000001 || fabs(Lv2 - 2)<0.000001 || fabs(Lv2 - 2.75)<0.000001 || fabs(Lv2 - 3)<0.000001 || fabs(Lv1 - 0.65)<0.000001 || fabs(Lv1 - 1.05)<0.000001 || fabs(Lv1 - 1.3)<0.000001)
    {
        return Lv2;
    }
    else
    {
        if(Lv1 > Lv2)
            return Lv1;
        else
            return Lv2;
    }
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

Point GXY_AI::Max_Active_Two()
{
    int flag[BoardSize][BoardSize];
    int max;
    Point pp;
    for(int i = 0; i < BoardSize; i++)
        for(int j = 0; j < BoardSize; j++)
            flag[i][j] = 0;
    for(int x = 0; x < BoardSize; x++)
    {
        for(int y = 0; y < BoardSize; y++)
        {
            if(myBoard[x][y] == Empty)
            {
                // Up-Down
                // Type-1
                if(myBoard[x][y-1] == myType && myBoard[x][y-2] == Empty && myBoard[x][y-3] == Empty && myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y+1] == myType && myBoard[x][y+2] == Empty && myBoard[x][y+3] == Empty && myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty)
                    flag[x][y] ++;
                // Type-2
                else if(myBoard[x][y-1] == myType && myBoard[x][y-2] == Empty && myBoard[x][y-3] == Empty && myBoard[x][y-4] == Empty && myBoard[x][y+1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty && myBoard[x][y-3] == Empty && myBoard[x][y+1] == myType && myBoard[x][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y+1] == myType && myBoard[x][y+2] == Empty && myBoard[x][y+3] == Empty && myBoard[x][y+4] == Empty && myBoard[x][y-1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty && myBoard[x][y+3] == Empty && myBoard[x][y-1] == myType && myBoard[x][y-2] == Empty)
                    flag[x][y] ++;
                // Type-3
                else if(myBoard[x][y-1] == Empty && myBoard[x][y-2] == myType && myBoard[x][y-3] == Empty && myBoard[x][y-4] == Empty && myBoard[x][y+1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty && myBoard[x][y+1] == Empty && myBoard[x][y+2] == myType && myBoard[x][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y+1] == Empty && myBoard[x][y+2] == myType && myBoard[x][y+3] == Empty && myBoard[x][y+4] == Empty && myBoard[x][y-1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty && myBoard[x][y-1] == Empty && myBoard[x][y-2] == myType && myBoard[x][y-2] == Empty)
                    flag[x][y] ++;
                // Type-4
                else if(myBoard[x][y-1] == Empty && myBoard[x][y+1] == Empty && myBoard[x][y+2] == Empty && myBoard[x][y+3] == myType && myBoard[x][y+4] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x][y+1] == Empty && myBoard[x][y-1] == Empty && myBoard[x][y-2] == Empty && myBoard[x][y-3] == myType && myBoard[x][y-4] == Empty)
                    flag[x][y] ++;

                // Right-Up-Left-Down
                // Type-1
                if(myBoard[x+1][y-1] == myType && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == Empty && myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y+1] == myType && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == Empty && myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty)
                    flag[x][y] ++;
                // Type-2
                else if(myBoard[x+1][y-1] == myType && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == Empty && myBoard[x+4][y-4] == Empty && myBoard[x-1][y+1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == Empty && myBoard[x-1][y+1] == myType && myBoard[x-2][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y+1] == myType && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == Empty && myBoard[x-4][y+4] == Empty && myBoard[x+1][y-1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == Empty && myBoard[x+1][y-1] == myType && myBoard[x+2][y-2] == Empty)
                    flag[x][y] ++;
                // Type-3
                else if(myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == myType && myBoard[x+3][y-3] == Empty && myBoard[x+4][y-4] == Empty && myBoard[x-1][y+1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty && myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == myType && myBoard[x-2][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == myType && myBoard[x-3][y+3] == Empty && myBoard[x-4][y+4] == Empty && myBoard[x+1][y-1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty && myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == myType && myBoard[x+2][y-2] == Empty)
                    flag[x][y] ++;
                // Type-4
                else if(myBoard[x+1][y-1] == Empty && myBoard[x-1][y+1] == Empty && myBoard[x-2][y+2] == Empty && myBoard[x-3][y+3] == myType && myBoard[x-4][y+4] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y+1] == Empty && myBoard[x+1][y-1] == Empty && myBoard[x+2][y-2] == Empty && myBoard[x+3][y-3] == myType && myBoard[x+4][y-4] == Empty)
                    flag[x][y] ++;

                // Left-Right
                // Type-1
                if(myBoard[x-1][y] == myType && myBoard[x-2][y] == Empty && myBoard[x-3][y] == Empty && myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y] == myType && myBoard[x+2][y] == Empty && myBoard[x+3][y] == Empty && myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty)
                    flag[x][y] ++;
                // Type-2
                else if(myBoard[x-1][y] == myType && myBoard[x-2][y] == Empty && myBoard[x-3][y] == Empty && myBoard[x-4][y] == Empty && myBoard[x+1][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty && myBoard[x-3][y] == Empty && myBoard[x+1][y] == myType && myBoard[x+2][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y] == myType && myBoard[x+2][y] == Empty && myBoard[x+3][y] == Empty && myBoard[x+4][y] == Empty && myBoard[x-1][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty && myBoard[x+3][y] == Empty && myBoard[x-1][y] == myType && myBoard[x-2][y] == Empty)
                    flag[x][y] ++;
                // Type-3
                else if(myBoard[x-1][y] == Empty && myBoard[x-2][y] == myType && myBoard[x-3][y] == Empty && myBoard[x-4][y] == Empty && myBoard[x+1][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty && myBoard[x+1][y] == Empty && myBoard[x+2][y] == myType && myBoard[x+2][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y] == Empty && myBoard[x+2][y] == myType && myBoard[x+3][y] == Empty && myBoard[x+4][y] == Empty && myBoard[x-1][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty && myBoard[x-1][y] == Empty && myBoard[x-2][y] == myType && myBoard[x-2][y] == Empty)
                    flag[x][y] ++;
                // Type-4
                else if(myBoard[x-1][y] == Empty && myBoard[x+1][y] == Empty && myBoard[x+2][y] == Empty && myBoard[x+3][y] == myType && myBoard[x+4][y] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y] == Empty && myBoard[x-1][y] == Empty && myBoard[x-2][y] == Empty && myBoard[x-3][y] == myType && myBoard[x-4][y] == Empty)
                    flag[x][y] ++;

                // Left-Up-Right-Down
                // Type-1
                if(myBoard[x-1][y-1] == myType && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == Empty && myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y+1] == myType && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == Empty && myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty)
                    flag[x][y] ++;
                // Type-2
                else if(myBoard[x-1][y-1] == myType && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == Empty && myBoard[x-4][y-4] == Empty && myBoard[x+1][y+1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == Empty && myBoard[x+1][y+1] == myType && myBoard[x+2][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y+1] == myType && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == Empty && myBoard[x+4][y+4] == Empty && myBoard[x-1][y-1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == Empty && myBoard[x-1][y-1] == myType && myBoard[x-2][y-2] == Empty)
                    flag[x][y] ++;
                // Type-3
                else if(myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == myType && myBoard[x-3][y-3] == Empty && myBoard[x-4][y-4] == Empty && myBoard[x+1][y+1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty && myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == myType && myBoard[x+2][y+2] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == myType && myBoard[x+3][y+3] == Empty && myBoard[x+4][y+4] == Empty && myBoard[x-1][y-1] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty && myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == myType && myBoard[x-2][y-2] == Empty)
                    flag[x][y] ++;
                // Type-4
                else if(myBoard[x-1][y-1] == Empty && myBoard[x+1][y+1] == Empty && myBoard[x+2][y+2] == Empty && myBoard[x+3][y+3] == myType && myBoard[x+4][y+4] == Empty)
                    flag[x][y] ++;
                else if(myBoard[x+1][y+1] == Empty && myBoard[x-1][y-1] == Empty && myBoard[x-2][y-2] == Empty && myBoard[x-3][y-3] == myType && myBoard[x-4][y-4] == Empty)
                    flag[x][y] ++;
            }
        }
    }
    max = flag[0][0];
    pp.Set(0,0);
    for(int x = 0; x < BoardSize; x++)
    {
        for(int y = 0; y < BoardSize; y++)
        {
            if(flag[x][y] > max)
            {
                max = flag[x][y];
                pp.Set(x,y);
            }
        }
    }
    if(max >= 2)
    {
        return pp;
    }
    else
    {
        Point p;
        return p;
    }
}

Point GXY_AI::Move()
{
    Locate_Point();
    Defend_Final();
    Attack_Final();
    // Defend sleep 4 or connected 4
    if(fabs(Defend_Lv_Final - 1.35)<0.000001 || fabs(Defend_Lv_Final - 1.6)<0.000001 || fabs(Defend_Lv_Final - 1.85)<0.000001 || fabs(Defend_Lv_Final - 2.1)<0.000001 || fabs(Defend_Lv_Final - 2.35)<0.000001 || fabs(Defend_Lv_Final - 2.6)<0.000001 || fabs(Defend_Lv_Final - 2.75)<0.000001 || fabs(Defend_Lv_Final - 3)<0.000001)   
    {
        // if I have sleep 4 or connected 4, then I choose to attack
        if(fabs(Attack_Lv_Final - 1.35)<0.000001 || fabs(Attack_Lv_Final - 1.6)<0.000001 || fabs(Attack_Lv_Final - 1.85)<0.000001 || fabs(Attack_Lv_Final - 2.1)<0.000001 || fabs(Attack_Lv_Final - 2.35)<0.000001 || fabs(Attack_Lv_Final - 2.6)<0.000001 || fabs(Attack_Lv_Final - 2.75)<0.000001 || fabs(Attack_Lv_Final - 3)<0.000001)
        {
            if(Attacker_Final.Valid() == true)
            {
                return Attacker_Final;
            }    
            else if(Defender_Final.Valid() == true)
            {
                return Defender_Final;
            }             
            else
            {
                return Random_Move();
            }     
        }
        // else I choose to defend
        else
        {
            if(Defender_Final.Valid() == true)
            {
                return Defender_Final;
            }   
            else if(Attacker_Final.Valid() == true)
            {
                return Attacker_Final;
            } 
            else 
            {
                return Random_Move();
            }    
        }  
    }     
    // Defend sleep 3 or connected 3
    else if(fabs(Defend_Lv_Final - 1.1)<0.000001 || fabs(Defend_Lv_Final - 2)<0.000001)
    {
        // if I have sleep 3 or connected 3, then I choose to attack
        if(fabs(Attack_Lv_Final - 1.1)<0.000001 || fabs(Attack_Lv_Final - 1.35)<0.000001 || fabs(Attack_Lv_Final - 1.6)<0.000001 || fabs(Attack_Lv_Final - 1.85)<0.000001 || fabs(Attack_Lv_Final - 2)<0.000001 || fabs(Attack_Lv_Final - 2.1)<0.000001 || fabs(Attack_Lv_Final - 2.35)<0.000001 || fabs(Attack_Lv_Final - 2.6)<0.000001 || fabs(Attack_Lv_Final - 2.75)<0.000001 || fabs(Attack_Lv_Final - 3)<0.000001)
        {
            if(Attacker_Final.Valid() == true)
            {
                return Attacker_Final;
            }    
            else if(Defender_Final.Valid() == true)
            {
                return Defender_Final;
            }             
            else
            {
                return Random_Move();
            } 
        }
        // else I choose to defend
        else
        {
            if(Defender_Final.Valid() == true)
            {
                return Defender_Final;
            }   
            else if(Attacker_Final.Valid() == true)
            {
                return Attacker_Final;
            } 
            else 
            {
                return Random_Move();
            }   
        }
    }             
    // if there is no obvious defense to do
    else
    {
        // if I have one step to win, then I choose to move and win
        if(fabs(Attack_Lv_Final - 1.1)<0.000001 || fabs(Attack_Lv_Final - 1.35)<0.000001 || fabs(Attack_Lv_Final - 1.6)<0.000001 || fabs(Attack_Lv_Final - 1.85)<0.000001 || fabs(Attack_Lv_Final - 2)<0.000001 || fabs(Attack_Lv_Final - 2.1)<0.000001 || fabs(Attack_Lv_Final - 2.35)<0.000001 || fabs(Attack_Lv_Final - 2.6)<0.000001 || fabs(Attack_Lv_Final - 2.75)<0.000001 || fabs(Attack_Lv_Final - 3)<0.000001)
        {
            if(Attacker_Final.Valid() == true)
            {
                return Attacker_Final;
            }  
            else if(Defender_Final.Valid() == true)
            {
                return Defender_Final;
            }             
            else
            {
                return Random_Move();
            } 
        }
        else
        {
            // check the whole board to find if there is one move could lead to 2 or more connected or sleep 3 or 4 to prepare for the next attack
            Point att_P_33 = Att_Chk_Next_33();
            if(att_P_33.Valid() == true)
            {
                return att_P_33;
            }
            else
            {
                // else to find one defense move could block up the potential anti move which could cause 2 or more anti-attack
                Point def_P = Def_Chk_Next();
                if(def_P.Valid() == true)
                {
                    return def_P;
                }
                else
                { 
                    // finally I choose to attack
                    Point att_P_23 = Att_Chk_Next_23();
                    if(att_P_23.Valid() == true)
                    {
                        return att_P_23;
                    }
                    else
                    {
                        Point att_P_active_Two = Max_Active_Two();
                        if(att_P_active_Two.Valid() == true)
                        {
                            return att_P_active_Two;
                        }
                        else
                        {
                            if(Attacker_Final.Valid() == true)
                            {
                                return Attacker_Final;
                            }  
                            else if(Defender_Final.Valid() == true)
                            {
                                return Defender_Final;
                            }             
                            else
                            {
                                return Random_Move();
                            } 
                        } 
                    }     
                }
            }
        }     
    }       
}

Point Michael_AI(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    GXY_AI gxy_AI(board, yourType);
    Point p = gxy_AI.Move();
    return p;
}

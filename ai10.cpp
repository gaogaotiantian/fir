#include "game.h"
#include <stdio.h>
Point ActualPlayer(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    Point p(0,0);
    int x;
    int y;
    while (1) {
        printf("You are %s\n", (yourType == Black) ? blackChar : whiteChar);
        printf("Please input your move, colomn(x axis) first, for example:2 3\n");
        scanf("%d %d", &y, &x);
        p.Set(x,y);
        if (p.Valid() && board[x][y] == Empty)
            return p;
    }
}

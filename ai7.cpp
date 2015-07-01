#include "game.h"
#include <iostream>

using namespace std;

class my_Ai {
private:
  NodeType board[BoardSize][BoardSize];
  NodeType myType;
  string myX[BoardSize];
  string myY[BoardSize];

public:
  my_Ai();
  my_Ai(NodeType myType);
  string getX();
  string getY();
  bool setType();
  NodeType getType();
  bool risk();
  Point attack();
};


bool risk(Point p, Nodetype myType){
  if(p

     }
}

Point GaeBolg(const NodeType board[BoardSize][BoardSize], NodeType myType){
  
  

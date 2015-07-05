#include "game.h"
#include <iostream>
#include <cassert>

using namespace std;






//class my_Ai {
//private:
//  NodeType board[BoardSize][BoardSize];
//  NodeType myType;
//  string myX[BoardSize];
//  string myY[BoardSize];
//  Omni omni;

//public:
//  my_Ai(){
//    myX[] = 0;
//    myY[] = 0;
//    myType = Empty;
//    for(int i = 0; i < BoardSize; i++)
//      for(int j = 0; j < BoardSize; j++)
//	board[i][j] = Empty;
//  }
//  my_Ai(NodeType type){
//    myX[] = 0;
//    myY[] = 0;
//    myTYpe = type;
//    for(int i = 0; i < BoardSize; i++)
//      for(int j = 0; j < BoardSize; j++)
//	board[i][j] = type;
//  }
  
//  string getX(){ return myX;}
//  string getY(){ return myY;}
//  bool setPoint(Point p){
//    assert(p.Valid());
//    myX[p.x] = 1;
//    myY[p.y] = 1;
//    board[p.x][p.y] = p.getType();
//    return true;
//  }
//  bool setType(NodeType type){
//    myType = type;
//    return true;
//  }
//  NodeType getType(){ return myType;}
//  Emergency getEmergency(Point pp);

//  Point attack();
//};

//class Position{
//private:
//  int xCor;
//  int yCor;

//public:
//  Position(){ xCor = yCor = 0;}
//  Position(Point p){
//    assert(p.Valid());
//    xCor = p.x; 
//    yCor = p.y;
//  }
//  Position getPosition(Point p){
//    Position pos(p);
//    return pos;
//  }
//};

void fillBoard(NodeType board[BoardSize][BoardSize], NodeType type);
void copyBoard(NodeType scrBoard[BoardSize][BoardSize], NodeType terBoard[BoardSize][BoardSize]);
Point SeraphTheGreat(NodeType board[BoardSize][BoardSize], NodeType myType);
void fillBoard(board[BoardSize][BoardSize], NodeType type){
  for(int i = 0; i < BoardSize; i++)
    for(int j = 0; j < BoardSize; j++)
      board[i][j] = type;
}
void copyBoard(NodeType scrBoard[BoardSize][BoardSize], NodeTYpe terBoard[BoardSize][BoardSize]){
  for(int i = 0; i < BoardSize; i++)
    for(int j = 0; j < BoardSize; j++)
      terBoard[i][j] = scrBoard[i][j];
}
Point SeraphTheGreat(NodeType board[BoardSize][BoardSize], NodeTYpe myType){
  if(myType == White)
    Omni omniknight(Black, White, board);
  else
    Omni omniknight(White, Black, board);
  Point calibur;
  calibur = omniknight(omniknight.getMyType, 5, 4);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getOppoType, 5, 4);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getOppoType, 4, 3);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getMyType, 4, 3);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getOppoType, 3, 2);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getMyType, 3, 2);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getMyType, 2, 1);
  if(calibur.Valid())
    return calibur;
  calibur = omniknight(omniknight.getMyType, 1, 0);
  if(calibur.Valid())
    return calibur;
  Point sad(BoardSize/2, BoardSize/2);
  return sad;
}

class Omni{

private:
  NodeType curBoard[BoardSize][BoardSize];
  NodeType oppoType;
  NodeType myType;

public:
  Omni(){fillBoard(board, Empty); myType = Empty; oppoType = Empty;}
  Omni(NodeType advType, NodeType SeraphType, NodeType board[BoardSize][BoardSize]){
    copyBoard(board, curBoard);
    oppoType = advType;
    myType = SeraphType;
  }
  NodeType getMyType()
    return myType;
  NodeType getOppoType()
    return oppoType;
  Point nextStep(NodeType xType, int num, int threshold){
    Point search(0,0);
    Point hPoint;
    Point vPoint;
    Point rPoint;
    Point lPoint;
    for(int i = 0; i < BoardSize; i++){
      for(int j = 0; j < BoardSize; j++){
	search.Set(i, j);
	hPoint = horizontal(search, xType, num, threshold);
	vPoint = vertical(search, xType, num, threshold);
	rPoint = rightDown(search, xType, num, threshold);
	lPoint = leftDown(search, xType, num, threshold);
	
	if(hPoint.Valid()){
	  return hPoint;
	}
	if(vPoint.Valid()){
	  return vPoint;
	}
	if(rPoint.Valid()){
	  return rPoint;
	}
	if(lPoint.Valid()){
	  return lPoint;
	}
      }
    }
    Point p(0, BoardSize-1)
    return p;
  }
  Point horizontal(Point pp, NodeType xType, int num, int threshold){
    int count = 0;
    Point solution;
    for(int i = 0; i < num; i++){
      if(pp.x+i < BoardSize && curBoard[pp.x+i][pp.y] == xType){
	count ++;
      }
      if(curBoard[pp.x+i][pp.y] == Empty)
	solution.Set(pp.x+i. pp.y);
    }
    if(count >= threshold && solution.Valid())
      return solution;
    Point invalid;
    return invalid;
  }

  Point vertical(Point pp, NodeType xType, int num, int threshold){
    int count = 0;
    Point solution;
    for(int i = 0; i < num; i++){
      if(pp.y+i < BoardSize && curBoard[pp.x][pp.y+i] == xType){
	count ++;
      }
      if(curBoard[pp.x][pp.y+i] == Empty)
	solution.Set(pp.x, pp.y+i);
    }
    if(count >= threshold && solution.Valid())
      return solution;
    Point invalid;
    return invalid;
  }
  Point rightDown(Point pp, NodeType xType, int num, int threshold){
    int count = 0;
    Point solution;
    for(int i = 0; i < num; i++){
      if((pp.x+i < BoardSize) && 
	 (pp.y+i < BoardSize) && 
	 (curBoard[pp.x+i][pp.y+i] == xType))
	count ++;
      if(curBoard[pp.x+i][pp.y+i] == Empty)
	solution.Set(pp.x+i, pp.y+i);
    }
    if(count >= threshold && solution.Valid())
      return solution;
    Point invalid;
    return invalid;
  }
  Point leftDown(Point pp, NodeType xType, int num, int threshold){
    int count = 0;
    Point solution;
    for(int i = 0; i < num; i++){
      if((pp.x-i >= 0) && 
	 (pp.y+i < BoardSize) && 
	 (curBoard[pp.x-i][pp.y+i] == xType))
	count ++;
      if(curBoard[pp.x-i][pp.y+i] == Empty)
	solution.Set(pp.x-i, pp.y+i);
    }
    if(count >= threshold && solution.Valid())
      return solution;
    Point invalid;
    return invalid;
  }
  //bool corner(Point pp, NodeType type);
};




Point GaeBolg(const NodeType board[BoardSize][BoardSize], NodeType myType){
  Point calibur = SeraphTheGreat(board, myType);
  return calibur;
}
  

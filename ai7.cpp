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
void copyBoard(const NodeType scrBoard[BoardSize][BoardSize], NodeType terBoard[BoardSize][BoardSize]);
Point SeraphTheGreat(const NodeType board[BoardSize][BoardSize], NodeType myType);


class Omni{

private:
  NodeType curBoard[BoardSize][BoardSize];
  NodeType oppoType;
  NodeType myType;

public:
  Omni(){fillBoard(curBoard, Empty); myType = Empty; oppoType = Empty;}
  Omni(NodeType advType, NodeType SeraphType, NodeType board[BoardSize][BoardSize]){
    copyBoard(board, curBoard);
    oppoType = advType;
    myType = SeraphType;
  }
  NodeType getMyType(){
    return myType;}
  NodeType getOppoType(){
    return oppoType;}
  void setOmni(NodeType advType, NodeType SeraphType, const NodeType board[BoardSize][BoardSize]){
    copyBoard(board, curBoard);
    oppoType = advType;
    myType = SeraphType;
  }
  
  bool isStepEmergent3(Point pp){
    Point check = pp;
    if(!check.Valid())
      return false;
    else{
      Point test = check;
      Point testR = check;
      Point testL = check;
      testR = check;
      testL = check;
      for(int i = 0; (test.x+i < BoardSize) && (curBoard[test.x+i][test.y] != oppoType); i++){
	if(test.x+i >= BoardSize)
	  return false;
	testR.Set(test.x+i, test.y);
      }
      for(int i = 0; (test.x-i >= 0) && (curBoard[test.x-i][test.y] != oppoType); i++){
	if(test.x-i < 0)
	  return false;
	testL.Set(test.x-i, test.y);
      }
      if(testR.x - testL.x > 4)
	return true;
      if(curBoard[testR.x][testR.y] == myType || curBoard[testL.x][testL.y] == myType)
	return false;
      
      testR = check;
      testL = check;
      for(int i = 0; (test.y+i < BoardSize) && (curBoard[test.x][test.y+i] != oppoType); i++){
	if(test.y+i >= BoardSize)
	  return false;
	testR.Set(test.x, test.y+i);
      }
      for(int i = 0; (test.y-i >= 0) && (curBoard[test.x][test.y-i] != oppoType); i++){
	if(test.y-i < 0)
	  return false;
	testL.Set(test.y-i, test.y);
      }
      if(testR.y - testL.y > 4)
	return true;
      if(curBoard[testR.x][testR.y] == myType || curBoard[testL.x][testL.y] == myType)
	return false;
      
      testR = check;
      testL = check;
      for(int i = 0; (test.x+i < BoardSize && test.y < BoardSize) && (curBoard[test.x+i][test.y+i] != oppoType); i++){
	if(test.x+i >= BoardSize || test.y+i >= BoardSize)
	  return false;
	testR.Set(test.x+i, test.y+i);
      }
      for(int i = 0; (test.x-i >= 0 && test.y-i >= 0) && (curBoard[test.x-i][test.y-i] != oppoType); i++){
	if(test.x-i < 0 || test.y < 0)
	  return false;
	testL.Set(test.x-i, test.y-i);
      }
      if(testR.x - testL.x > 4)
	return true;
      if(curBoard[testR.x][testR.y] == myType || curBoard[testL.x][testL.y] == myType)
	return false;
      
      testR = check;
      testL = check;
      for(int i = 0; (test.x-i >= 0 && test.y+i < BoardSize) && (curBoard[test.x-i][test.y+i] != oppoType); i++){
	if(test.x-i < 0 || test.y+i >= BoardSize)
	  return false;
	testR.Set(test.x-i, test.y+i);
      }
      for(int i = 0; (test.x+i < BoardSize && test.y-i >= 0) && (curBoard[test.x+i][test.y-i] != oppoType); i++){
	if(test.x+i >= BoardSize || test.y-i < 0)
	  return false;
	testL.Set(test.x+i, test.y-i);
      }
      if(testR.y - testL.y > 4)
	return true;
      if(curBoard[testR.x][testR.y] == myType || curBoard[testL.x][testL.y] == myType)
	return false;
    }
    return true;	  
  }
  
  Point nextStep(NodeType xType, int num, int threshold, int startX, int startY){ //还是要另开一个function来搞双3因为单2的优先级很低要用一个special case把它提高
    Point hPoint;
    Point vPoint;
    Point rPoint;
    Point lPoint;
    hPoint = horizontal(search, xType, num, threshold, startX, startY);
    vPoint = vertical(search, xType, num, threshold, startX, startY);
    rPoint = rightDown(search, xType, num, threshold, startX, startY);
    lPoint = leftDown(search, xType, num, threshold, startX, startY);    
    Point hPointT = hPoint;
    Point vPointT = vPoint;
    Point rPointT = rPoint;
    Point lPointT = lPoint;
    while(hPointT.Valid() || vPointT.Valid() || rPointT.Valid() || lPointT.Valid()){
      if(hPointT.Valid() && hPointT == vPointT && vPointT == rPointT && rPointT == lPointT)
	return hPointT;
      if(hPointT.Valid() && hPointT == vPointT && vPointT == rPointT)
	return hPointT;
      if(hPointT.Valid() && hPointT == vPointT && vPointT == lPointT)
	return hPoint;
      if(hPointT.Valid() && hPointT == rPointT && rPointT == lPointT)
	return hPointT;
      if(vPointT.Valid() && vPointT == rPointT && rPointT == lPointT)
	return vPointT;
      if(hPointT.Valid() && hPointT == vPointT)
	return hPointT;
      if(hPointT.Valid() && hPointT == rPointT)
	return hPointT;
      if(hPointT.Valid() && hPointT == lPointT)
	return hPointT;
      if(vPointT.Valid() && vPointT == rPointT)
	return vPointT;
      
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
    Point p(-1, -1);
    return p;
  }
  Point horizontal(Point pp, NodeType xType, int num, int threshold, int startX, int startY){
    int count = 0;
    Point solution;
    for(int i = startX; i < BoardSize; i++){
      for(int j = startY; j < BoardSize; j++){
	for(int i = 0; i < num; i++){
	  if(pp.x+i < BoardSize && curBoard[pp.x+i][pp.y] == xType){
	    count ++;
	  }
	  if(curBoard[pp.x+i][pp.y] == Empty)
	    solution.Set(pp.x+i, pp.y);
	}
	if(count >= threshold && solution.Valid())
	  return solution;
      }
    }
    Point invalid;
    return invalid;
  }

  Point vertical(Point pp, NodeType xType, int num, int threshold, int startX, int startY){
    int count = 0;
    Point solution;
    for(int i = startX; i < BoardSize; i++){
      for(int j = startY; j < BoardSize; j++){
	for(int i = 0; i < num; i++){
	  if(pp.y+i < BoardSize && curBoard[pp.x][pp.y+i] == xType){
	    count ++;
	  }
	  if(curBoard[pp.x][pp.y+i] == Empty)
	    solution.Set(pp.x, pp.y+i);
	}
	if(count >= threshold && solution.Valid())
	  return solution;
      }
    }
    Point invalid;
    return invalid;
  }
  Point rightDown(Point pp, NodeType xType, int num, int threshold, int startX, int startY){
    int count = 0;
    Point solution;
    for(int i = startX; i < BoardSize; i++){
      for(int j = startY; j < BoardSize; j++){
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
      }
    }
    Point invalid;
    return invalid;
  }
  Point leftDown(Point pp, NodeType xType, int num, int threshold, int startX, int startY){
    int count = 0;
    Point solution;
    for(int i = startX; i < BoardSize; i++){
      for(int j = startY; j < BoardSize; j++){
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
      }
    }
    Point invalid;
    return invalid;
  }
  //bool corner(Point pp, NodeType type);
};

void fillBoard(NodeType board[BoardSize][BoardSize], NodeType type){
    for(int i = 0; i < BoardSize; i++)
        for(int j = 0; j < BoardSize; j++)
            board[i][j] = type;
}
void copyBoard(const NodeType scrBoard[BoardSize][BoardSize], NodeType terBoard[BoardSize][BoardSize]){
    for(int i = 0; i < BoardSize; i++)
        for(int j = 0; j < BoardSize; j++)
            terBoard[i][j] = scrBoard[i][j];
}
Point SeraphTheGreat(const NodeType board[BoardSize][BoardSize], NodeType myType){
    Omni omniknight;
    if(myType == White)
      omniknight.setOmni(Black, White, board);
    else
      omniknight.setOmni(White, Black, board);
    Point calibur;
    calibur = omniknight.nextStep(omniknight.getMyType(), 5, 4, 0, 0);
    if(calibur.Valid())
      return calibur;
    calibur = omniknight.nextStep(omniknight.getOppoType(), 5, 4, 0, 0);
    if(calibur.Valid())
      return calibur;
    calibur = omniknight.nextStep(omniknight.getMyType(), 4, 3, 0, 0);
    if(calibur.Valid())
      return calibur;
    calibur = omniknight.nextStep(omniknight.getOppoType(), 4, 3, 0, 0);
    if(calibur.Valid()){
      Point instant = calibur;
      while(calibur.Valid() && !omniknight.isStepEmergent3(calibur)){
	instant = calibur;
	calibur = omniknight.nextStep(omniknight.getOppoType(), 4, 3, calibur.x+1, calibur.y);
      }
      if(calibur.Valid())
	return calibur;
    }
    calibur = omniknight.nextStep(omniknight.getMyType(), 3, 2, 0, 0);
    if(calibur.Valid())
      return calibur;
    calibur = omniknight.nextStep(omniknight.getMyType(), 2, 1, 0, 0);
    if(calibur.Valid())
      return calibur;
    calibur = omniknight.nextStep(omniknight.getOppoType(), 3, 2, 0, 0);
    if(calibur.Valid())
      return calibur;
    calibur = omniknight.nextStep(omniknight.getOppoType(), 2, 1, 0, 0);
    if(calibur.Valid())
      return calibur;
    Point sad((BoardSize+1)/2, (BoardSize+1)/2);
    return sad;
}



Point GaeBolg(const NodeType board[BoardSize][BoardSize], NodeType myType){
  Point calibur = SeraphTheGreat(board, myType);
  return calibur;
}
  

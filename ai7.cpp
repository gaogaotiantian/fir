#include "game.h"
#include <iostream>
#include <map>
#include <cassert>

using namespace std;

void fillBoard(NodeType board[BoardSize][BoardSize], NodeType type);
void copyBoard(const NodeType scrBoard[BoardSize][BoardSize], NodeType terBoard[BoardSize][BoardSize]);
Point SeraphTheGreat(const NodeType board[BoardSize][BoardSize], NodeType myType);
Point getNextPoint(Point pp);

class PointData{

private:
  Point p;
  int count;

public:
  PointData(){p.Set(-1, -1); count = 0;}
  PointData(Point pp, int num){ p.Copy(pp); count = num;}
  Point getPoint(){return p;}
  int getCount(){return count;}
  void Copy(PointData pointdata){p.Copy(pointdata.p); count = pointdata.count;}
  void setPoint(Point pp){p.Copy(pp);}
  void setCount(int num){ count = num;}		
  void setData(Point pp, int num){p.Copy(pp); count = num;}
  bool operator ==(PointData pointData){ return (count == pointData.count && p == pointData.p);} 
  bool operator <(PointData pointData){ return (count < pointData.getCount());}
};

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
  
  bool isStepEmergent(Point pp, int num, NodeType xType, NodeType reverseType){//Check 3 vs. 3+1, num = 4
    Point check = pp;
    if(!check.Valid())
      return false;
    else{
      Point test = check;
      Point testR = check;
      Point testL = check;
      testR = check;
      testL = check;
        for(int hi = 1; (test.x+hi < BoardSize) && (curBoard[testR.x][testR.y] == xType); hi++){
            testR.Set(test.x+hi, test.y);
            cout << hi;}
      for(int hii = 1; (test.x-hii >= 0) && (curBoard[testL.x][testL.y] == xType); hii++)
	testL.Set(test.x-hii, test.y);
      if(testL.Valid() && testR.Valid())
	if(testR.x - testL.x >= num)
	  if(curBoard[testR.x][testR.y] == Empty && curBoard[testL.x][testL.y] == Empty)
	    return true;
      
      testR = check;
      testL = check;
        for(int vi = 1; (test.y+vi < BoardSize) && (curBoard[testR.x][testR.y] == xType); vi++){
            cout << vi;
            testR.Set(test.x, test.y+vi);}
        for(int vii = 1; (test.y-vii >= 0) && (curBoard[testL.x][testL.y] == xType); vii++){
            testL.Set(test.x, test.y-vii);}
        if(testL.Valid() && testR.Valid())
            if(testR.y - testL.y >= num)
                if(curBoard[testR.x][testR.y] == Empty && curBoard[testL.x][testL.y] == Empty)
                    return true;
      
      testR = check;
      testL = check;
      for(int ri = 1; (test.x+ri < BoardSize && test.y+ri < BoardSize) && (curBoard[testR.x][testR.y] == xType); ri++)//改到这，看前面的改
	testR.Set(test.x+ri, test.y+ri);
      for(int rii = 1; (test.x-rii >= 0 && test.y-rii >= 0) && (curBoard[testL.x][testL.y] == xType); rii++)
	testL.Set(test.x-rii, test.y-rii);
      if(testL.Valid() && testR.Valid())
	if(testR.x - testL.x >= num)
	  if(curBoard[testR.x][testR.y] == Empty && curBoard[testL.x][testL.y] == Empty)
	    return true;
      
      testR = check;
      testL = check;
      for(int li = 1; (test.x-li >= 0 && test.y+li < BoardSize) && (curBoard[testR.x][testR.y] == xType); li++)
	testR.Set(test.x-li, test.y+li);
      for(int lii = 1; (test.x+lii < BoardSize && test.y-lii >= 0) && (curBoard[testL.x][testL.y] == xType); lii++)
	testL.Set(test.x+lii, test.y-lii);
      if(testL.Valid() && testR.Valid())
	if(testR.y - testL.y >= num)
	  if(curBoard[testR.x][testR.y] == Empty && curBoard[testL.x][testL.y] == Empty)
	    return true;
    }      
    return false;	  
  }
  Point Foresight(NodeType xType, NodeType reverseType, int num, int threshold, int startX, int startY){
    map <int, PointData> pointList;
    PointData hPoint;
    PointData vPoint;
    PointData rPoint;
    PointData lPoint;
    int count = 1;
    int index = 0;
    Point pp(startX, startY);
    for(Point hp = horizontal(pp, xType, num, threshold); hp.Valid(); hp = horizontal(getNextPoint(hp), xType, num, threshold)){
      if(isStepEmergent(hp, num, xType, reverseType)){
	hPoint.setData(hp, count);
	index = hp.y*100 + hp.x;
	pointList.insert(pair<int, PointData>(index, hPoint));
      }
    }

    for(Point vp = vertical(pp, xType, num, threshold); vp.Valid(); vp = vertical(getNextPoint(vp), xType, num, threshold)){
      if(isStepEmergent(vp, num, xType, reverseType)){
	vPoint.setData(vp, count);
	index = vp.y*100 + vp.x;
	if(pointList.find(index) == pointList.end()){
	  pointList.insert(pair<int, PointData>(index, vPoint));
	}
	else{
	  PointData vInstant = pointList[index];
	  vInstant.setCount(vInstant.getCount()+1);
	  pointList.erase(index);
	  pointList.insert(pair<int, PointData>(index, vInstant));
	}
      }
    }
    
    for(Point rp = rightDown(pp, xType, num, threshold); rp.Valid(); rp = rightDown(getNextPoint(rp), xType, num, threshold)){
      if(isStepEmergent(rp, num, xType, reverseType)){
	rPoint.setData(rp, count);
	index = rp.y*100 + rp.x;
	if(pointList.find(index) == pointList.end()){
	  pointList.insert(pair<int, PointData>(index, rPoint));;
	}
	else{
	  PointData rInstant = pointList[index];
	  rInstant.setCount(rInstant.getCount()+1);
	  pointList.erase(index);
	  pointList.insert(pair<int, PointData>(index, rInstant));;
	}
      }
    }
    
    for(Point lp = leftDown(pp, xType, num, threshold); lp.Valid(); lp = leftDown(getNextPoint(lp), xType, num, threshold)){
      if(isStepEmergent(lp, num, xType, reverseType)){
	lPoint.setData(lp, count);
	index = lp.y*100 + lp.x;
	if(pointList.find(index) == pointList.end()){
	  pointList.insert(pair<int, PointData>(index, lPoint));
	}
	else{
	  PointData lInstant = pointList[index];
	  lInstant.setCount(lInstant.getCount()+1);
	  pointList.erase(index);
	  pointList.insert(pair<int, PointData>(index, lInstant));
	}
      }
    }
   
    if(!pointList.empty()){
      PointData valid = pointList.begin() -> second;
      int key = valid.getPoint().y*100+valid.getPoint().x;
      map <int, PointData>::iterator i = pointList.begin();
      while(curBoard[valid.getPoint().x][valid.getPoint().y] != Empty && i != pointList.end()){
	i++;
	valid = i -> second;
	key = valid.getPoint().y*100+valid.getPoint().x;
      }
      if(curBoard[valid.getPoint().x][valid.getPoint().y] == Empty){
	return valid.getPoint();
      }
    }
    Point invalid;
    return invalid;
  }    

  Point nextStep(NodeType xType, int num, int threshold, int startX, int startY){ //还是要另开一个function来搞双3因为单2的优先级很低要用一个special case把它提高，双3是致死级别的，在nextStep里面也可以用，就直接引用那个funciton就好了。啊啊啊啊找个高效点的方法写啊魂淡！
    Point search(startX, startY);
    Point hPoint;
    Point vPoint;
    Point rPoint;
    Point lPoint;
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
    Point p(-1, -1);
    return p;
  }
  Point horizontal(Point pp, NodeType xType, int num, int threshold){
    if(!pp.Valid()){
      return pp;
    }
    
    int count = 0;
    Point solution;
    for(int ii = 0; ii < num; ii++){
      if(pp.x+ii < BoardSize && curBoard[pp.x+ii][pp.y] == xType){
	count ++;
      }
      if(curBoard[pp.x+ii][pp.y] == Empty)
	solution.Set(pp.x+ii, pp.y);
    }
    if(count >= threshold && solution.Valid())
      return solution;
    
    return horizontal(getNextPoint(pp), xType, num, threshold);
  }

  Point vertical(Point pp, NodeType xType, int num, int threshold){
    if(!pp.Valid()){
      return pp;
    }

    int count = 0;
    Point solution;
    for(int ii = 0; ii < num; ii++){
      if(pp.y+ii < BoardSize && curBoard[pp.x][pp.y+ii] == xType){
	count ++;
      }
      if(curBoard[pp.x][pp.y+ii] == Empty)
	solution.Set(pp.x, pp.y+ii);
    }
    if(count >= threshold && solution.Valid())
      return solution;

    return vertical(getNextPoint(pp), xType, num, threshold);
  }

  Point rightDown(Point pp, NodeType xType, int num, int threshold){
    if(!pp.Valid())
      return pp;
  
    int count = 0;
    Point solution;
    for(int ii = 0; ii < num; ii++){
      if((pp.x+ii < BoardSize) && 
	 (pp.y+ii < BoardSize) && 
	 (curBoard[pp.x+ii][pp.y+ii] == xType))
	count ++;
      if(curBoard[pp.x+ii][pp.y+ii] == Empty)
	solution.Set(pp.x+ii, pp.y+ii);
    }
    if(count >= threshold && solution.Valid())
      return solution;

    return rightDown(getNextPoint(pp), xType, num, threshold);
  }

  Point leftDown(Point pp, NodeType xType, int num, int threshold){
    if(!pp.Valid())
      return pp;

    int count = 0;
    Point solution;
    for(int ii = 0; ii < num; ii++){
      if((pp.x-ii >= 0) && 
	 (pp.y+ii < BoardSize) && 
	 (curBoard[pp.x-ii][pp.y+ii] == xType))
	count ++;
      if(curBoard[pp.x-ii][pp.y+ii] == Empty)
	solution.Set(pp.x-ii, pp.y+ii);
    }
    if(count >= threshold && solution.Valid())
      return solution;

    return leftDown(getNextPoint(pp), xType, num, threshold);
  }

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

Point getEmergent(Omni omniknight, int num, int threshold, int startX, int startY){//wanna get emergent of three? num = 4 and threshold = 3 and xType = oppoType
  Point calibur = omniknight.nextStep(omniknight.getOppoType(), num, threshold, startX, startY);
  while(calibur.Valid() && !omniknight.isStepEmergent(calibur, num, omniknight.getOppoType(), omniknight.getMyType()))
    calibur = omniknight.nextStep(omniknight.getOppoType(), num, threshold, calibur.x+1, calibur.y);
  return calibur;
}
Point getNextPoint(Point pp){
  Point instant = pp;
  if(instant.x + 1 < BoardSize)
    instant.x ++;
  else{
    instant.x = 0;
    instant.y ++;
    }
  return instant;
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
      while(calibur.Valid() && !omniknight.isStepEmergent(calibur, 4, omniknight.getOppoType(), omniknight.getMyType())){
	calibur = omniknight.nextStep(omniknight.getOppoType(), 4, 3, getNextPoint(calibur).x, getNextPoint(calibur).y);
      }
      if(calibur.Valid())
	return calibur;
    }
    //    calibur = getEmergent(omniknight, 4, 3, 0, 0);
    //if(calibur.Valid())
    //  return calibur;

    //    calibur = omniknight.Foresight(omniknight.getOppoType(), omniknight.getMyType(), 3, 2, 0, 0);
    //if(calibur.Valid())
    //  return calibur;

    //    calibur = omniknight.Foresight(omniknight.getMyType(), omniknight.getOppoType(), 3, 2, 0, 0);
    //if(calibur.Valid())
    //  return calibur;

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
  

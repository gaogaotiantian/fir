#include "game.h"
#include <vector>
using namespace std;

Point Ai2(const NodeType board[BoardSize][BoardSize], NodeType yourType);



class c_Ai {
private:
  NodeType *board[BoardSize][BoardSize];
  NodeType type;
  std::vector<Point>Mylocation;
  std::vector<Point>YLocation;
    
public:
  c_Ai(NodeType yourType){type = yourType;};
  Point move(const NodeType board[BoardSize][BoardSize]);
  Point XInHoriz(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type);
  Point XInVerit(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type);
  Point XInLRDCross(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type);
  Point XInLRUCross(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type);
  Point XInARow(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type);
  void addAllPoint(const NodeType board[BoardSize][BoardSize]);
};

Point Ai3(const NodeType board[BoardSize][BoardSize], NodeType yourType){
  c_Ai ai(yourType);
  return ai.move(board);
}

Point c_Ai::XInHoriz(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type){
  int x =P.x; int y = P.y; int tempcount =0, backupstart = 0; int spacex =0, count=0;
  int Xstrat = x;
  bool space = false;
  int Xend = (x>=(BoardSize+1-size))?BoardSize:(x+size-1);
  for(int i = Xstrat; i<=Xend; i++){
    if(board[i][y]==type){
      if(count==0)
	Xstrat = i;
      count++;
    }
    else{
      if(board[i][y]!=Empty){
	count = 0;
        space = false;
        tempcount=0;
        backupstart = 0;
	continue;
      }
      if(count==0)
	continue;
      if(count>0 && !space && (i+1)<BoardSize && board[i+1][y]==type){
	tempcount=count;
	space = true;
	spacex = i;
	backupstart=i+1;
      }
      else if(count>0 && space && (i+1)<BoardSize && board[i+1][y]==type){
	Xstrat = backupstart;
	backupstart = i+1;
	spacex = i;
	count -=tempcount;
	tempcount = count;
      }
      else{
	count = 0;
	space = false;
	tempcount=0;
	backupstart = 0;
      }
    }
    if(count == size){
      if(space) return Point(spacex, y);
      else if (Xstrat!=0&& board[Xstrat-1][y]==Empty)
	return Point(Xstrat-1, y);
      else if ( (Xstrat+size)<=BoardSize && board[Xstrat+size][y]==Empty)
	return Point(Xstrat+size, y);
    }
  }
  return Point(-1,-1);
}


Point c_Ai::XInVerit(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type){
  int x =P.x; int y = P.y; int tempcount =0, backupstart = 0; int spacex =0,count=0;
  int Ystrat = y;
  bool space = false;
  int Yend = (y>=(BoardSize+1-size))?BoardSize:(y+size-1);
  for(int i = Ystrat; i<=Yend; i++){
    if(board[x][i]==type){
      if(count==0)
	Ystrat = i;
      count++;
    }
    else{
      if(board[i][y]!=Empty){
	count = 0;
        space = false;
        tempcount=0;
        backupstart = 0;
        continue;
      }
      if(count==0)
	continue;
      if(count>0 && !space && (i+1)<BoardSize && board[x][i+1]==type){
	tempcount=count;
	space = true;
	spacex = i;
	backupstart=i+1;
      }
      else if(count>0 && space && (i+1)<BoardSize && board[x][i+1]==type){
	Ystrat = backupstart;
	backupstart = i+1;
	spacex = i;
	count -=tempcount;
	tempcount = count;
      }
      else{
	count = 0;
	space = false;
	tempcount=0;
	backupstart = 0;
      }
    }
    if(count == size){
      if(space) return Point(spacex, y);
      else if (Ystrat!=0&& board[x][Ystrat-1]==Empty)
	return Point(x,Ystrat-1);
      else if ( Ystrat+size<BoardSize && board[x][Ystrat+size]==Empty)
	return Point(x, Ystrat+size);
    }
  }
    
  return Point(-1,-1);
}



Point c_Ai::XInLRDCross(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type){
  int x =P.x; int y = P.y; int tempcount =0, backupstartx = 0, backupstarty =0,count=0; int spacex =0,spacey=0;
  int tempx = x; int tempy =y;
  bool space = false;
  Point start = Point(x,y);
  tempx = x; tempy=y;
  for(int i =0; i<size; i++){
    if (tempx<(BoardSize-1)&&tempy>0) {
      tempx++;
      tempy--;
    }
    else break;
  }
  Point end = Point(tempx,tempy);
  tempy = start.y;
  for(int i =start.x; i<end.x; i++){
    if(board[i][tempy]==type){
      if (count==0) {
	start = Point(i,tempy);
      }
      count++;
    }
    else{
      if(board[i][tempy]!=Empty){
	count =0;
	space =false;
	tempcount=0;
	backupstartx=0;
	backupstarty=0;
}
      if (count==0) {
	tempy--;
	continue;
      }
      if (count>0 && !space &&(i+1)<BoardSize && board[i+1][tempy-1]==type) {
	tempcount=count;
	backupstartx = i+1;
	backupstarty = tempy-1;
	spacex = i;
	spacey = tempy;
	space = true;
      }
      else if(count>0 && space && (i+1)<BoardSize&& board[i+1][tempy-1]==type) {
	start = Point(backupstartx,backupstarty);
	backupstartx = i+1;
	backupstarty = tempy-1;
	spacex = i;
	spacey = tempy;
	count-=tempcount;
	tempcount=count;
      }
      else{
	count =0;
	space =false;
	tempcount=0;
	backupstartx=0;
	backupstarty=0;
      }
    }
    if(count==size){
      if (space) {
	return Point(spacex,spacey);
      }
      else if((start.x-1)>=0&&(start.y+1)<BoardSize && board[start.x-1][start.y+1]==Empty)
	return Point(start.x-1,start.y+1);
      else if ((start.x+size)<BoardSize&&(start.y-size)>=0 && board[start.x+size][start.y-size]==Empty )
	return Point(start.x+size,start.y-size);
    }
        
        
    tempy--;
  }
  return Point(-1,-1);
    
}


Point c_Ai::XInLRUCross(int size, const Point& P,const NodeType board[BoardSize][BoardSize],NodeType type){
  int x =P.x; int y = P.y; int tempcount =0, backupstartx = 0, backupstarty =0,count=0; int spacex =0,spacey=0;
  int tempx = x; int tempy =y;
  bool space = false;
  Point start = Point(x,y);
  tempx = x; tempy=y;
  for(int i =0; i<size; i++){
    if (tempx<(BoardSize-1)&&tempy<(BoardSize-1)) {
      tempx++;
      tempy++;
    }
    else break;
  }
  Point end = Point(tempx,tempy);
  tempy = start.y;
  for(int i =start.x; i<end.x;i++){
    if(board[i][tempy]==type){
      if (count==0) {
	start = Point(i,tempy);
      }
      count++;
    }
    else{
      if(board[i][tempy]!=Empty){
	count =0;
	space =false;
	tempcount=0;
	backupstartx=0;
	backupstarty=0;
     }
      if (count==0) {
	tempy++;
	continue;
      }
      if (count>0 && !space &&(i+1)<BoardSize && board[i+1][tempy+1]==type) {
	tempcount=count;
	backupstartx = i+1;
	backupstarty = tempy+1;
	spacex = i;
	spacey = tempy;
	space = true;
      }
      else if(count>0 && space && i+1<BoardSize&& board[i+1][tempy+1]==type) {
	start = Point(backupstartx,backupstarty);
	backupstartx = i+1;
	backupstarty = tempy+1;
	spacex = i;
	spacey = tempy;
	count-=tempcount;
	tempcount=count;
      }
      else{
	count =0;
	space =false;
	tempcount=0;
	backupstartx=0;
	backupstarty=0;
      }
    }
    if(count==size){
      if (space) {
	return Point(spacex,spacey);
      }
      else if((start.x-1)>=0&&(start.y-1)>=0 && board[start.x-1][start.y-1]==Empty)
	return Point(start.x-1,start.y-1);
      else if ((start.x+size)<BoardSize&&(start.y+size)<BoardSize && board[start.x+size][start.y+size]==Empty )
	return Point(start.x+size,start.y+size);
    }
        
        
    tempy++;
  }
  return Point(-1,-1);
    
}

Point c_Ai::XInARow(int size, const Point& P,const NodeType board[BoardSize][BoardSize], NodeType type){
  Point temp = XInHoriz(size,P,board,type);
  if(temp.x!=-1) return temp;
  temp = XInHoriz(size,P,board,type);
  if (temp.x!=-1) {return temp;}
  temp = XInLRUCross(size,P,board,type);
  if(temp.x!=-1) {return temp;}
  temp = XInLRDCross(size,P,board,type);
  if(temp.x!=-1) {return temp;}
  return Point(-1,-1);
}

void c_Ai::addAllPoint(const NodeType board[BoardSize][BoardSize]){
  for(int i =0; i<BoardSize;i++)
    {
      for(int j =0; j<BoardSize; j++){
	if (board[i][j]!=Empty) {
	  if (board[i][j]==type) {
	    Mylocation.push_back(Point(i,j));
	  }
	  else YLocation.push_back(Point(i,j));
	}
      }
    }
}

Point c_Ai::move(const NodeType board[BoardSize][BoardSize]){
  addAllPoint(board);
  int mysize = Mylocation.size();
  int ysize = YLocation.size();
  if (mysize==0) {
    if (board[10][10]==Empty) {
      return Point(10,10);
    }
    else return Point(11,10);
  }
  Point temp(0,0);

  NodeType ytype;
  if(type==Black) ytype = White;
  else ytype = Black;
  for (int i =0; i<mysize; i++) {
    temp = XInARow(4, Mylocation[i], board,type);
    if (temp.x!=-1) {
      return temp;
    }
  }
  for (int i=0; i<ysize; i++) {
    temp = XInARow(4, YLocation[i], board,ytype);
    if (temp.x!=-1) {
      return temp;
    }
  }
  for (int i =0; i<mysize; i++) {
    temp = XInARow(3, Mylocation[i], board,type);
    if (temp.x!=-1) {
      return temp;
    }
  }
  for (int i=0; i<ysize; i++) {
    temp = XInARow(3, YLocation[i], board,ytype);
    if (temp.x!=-1) {
      return temp;
    }
  }
  for (int i =0; i<mysize; i++) {
    temp = XInARow(2, Mylocation[i], board,type);
    if (temp.x!=-1) {
      return temp;
    }
  }
  for (int i =0; i<mysize; i++) {
    temp = Mylocation[i];
    if (board[temp.x+1][temp.y+1]==Empty) {
      return Point(temp.x+1,temp.y+1);
    }
    if (board[temp.x+1][temp.y-1]==Empty) {
      return Point(temp.x+1,temp.y-1);
    }
    if (board[temp.x][temp.y+1]==Empty) {
      return Point(temp.x,temp.y+1);
    }
    if (board[temp.x+1][temp.y]==Empty) {
      return Point(temp.x+1,temp.y);
    }
    if (board[temp.x-1][temp.y+1]==Empty) {
      return Point(temp.x-1,temp.y+1);
    }
    if (board[temp.x-1][temp.y-1]==Empty) {
      return Point(temp.x-1,temp.y-1);
    }
  }
  return Point(Mylocation[0].x+4,Mylocation[0].y+6);
    
    
    
    
}




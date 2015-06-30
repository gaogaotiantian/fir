#include "game.h"
#include <vector>
using namespace std;

Point Ai2(const NodeType board[BoardSize][BoardSize], NodeType yourType);

class c_Ai {
private:
  NodeType board[BoardSize][BoardSize];
  NodeType type;
  std::vector<Point>Mylocation;
  std::vector<Point>YLocation;
    
public:
  c_Ai(const NodeType& board, NodeType yourType): this.board = board, type = yourtype {}; 
  Point move();
  Point ThreeInHoriz();
  Point ThreeInVerit();
  Point ThreeInLRCross();
  Point ThreeInRLCross();
  Point ThreeInARow();
};

Point Ai2(const NodeType board[BoardSize][BoardSize], NodeType yourType){
  C_Ai ai(board,yourtype);
  returm ai.move();
}

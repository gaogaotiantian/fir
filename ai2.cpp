/*
 * This AI is written by Tian Gao
 * github ID: gaogaotiantian
 * ID for the project: 2 
 */
#include <assert.h>
#include <stdlib.h>
#include <map>
#include <iostream>
#include "game.h"
#include <math.h>
#include <vector>
#include <set>
#include <algorithm>

enum Direction {LeftRight = 0, TopBot = 1, LTRB = 2, RTLB = 3};
enum Strategy {Attack = 0, Defend = 1};

void PrintNode(NodeType node) 
{
    if (node == Empty)
        printf("Empty");
    else if (node == Black)
        printf("Black");
    else if (node == White)
        printf("White");
    else
        assert(false);
}

struct PointComparator {
    bool operator() (const Point& l, const Point& r) const {
        int middlediff = abs(l.x - BoardSize/2) + abs(l.y - BoardSize/2) -
            abs(r.x - BoardSize/2) - abs(r.y - BoardSize/2);
        if (middlediff < 0)
            return true;
        if (middlediff == 0 && (l.x < r.x || (l.x == r.x && l.y < r.y)))
            return true;
        return false;
    }
};

inline bool isConnected(const Point& p1, const Point& p2) 
{
    assert(p1.Valid() && p2.Valid());
    if (p1.x == p2.x && p1.y == p2.y)
        return false;
    if (p1.x == p2.x && abs(p1.y - p2.y) <= 4) {
        return true;
    } else if (p1.y == p2.y && abs(p1.x - p2.x) <= 4) {
        return true;
    } else if (abs(p1.x - p2.x) <= 4 && abs(p1.x - p2.x) == abs(p1.y - p2.y)) {
        return true;
    } else {
        return false;
    }
}

class ReqCounts {
public:
    ReqCounts();
    void SetMin();
    void SetMax();
    friend ReqCounts operator + (const ReqCounts &l, const ReqCounts &r);
    friend ReqCounts operator - (const ReqCounts &l, const ReqCounts &r);
    friend ReqCounts operator - (const ReqCounts &l, int r);
    friend ReqCounts operator * (const ReqCounts &l, int r);
    friend ReqCounts operator / (const ReqCounts &l, int r);
    friend bool operator > (const ReqCounts &l, const ReqCounts &r);
    friend bool operator < (const ReqCounts &l, const ReqCounts &r);
    void operator = (const ReqCounts &r);
    int& operator [](int i);

    int  minStep() const;
    bool isClose(const ReqCounts &l) const;
    bool isPositive() const;
    bool isEmpty() const;
    int  GetValue() const;
    void Clear();
    void Print() const;
    
    int counts[6];
};

typedef std::map<Point, ReqCounts, PointComparator> pointContriMap;
typedef std::map<Point, NodeType, PointComparator> assumeNodeMap;
void PrintPointContriMap(pointContriMap m);

ReqCounts::ReqCounts()
{
    for (int i = 0; i <= 5; i++) {
        counts[i] = 0;
    }
}

void ReqCounts::SetMin()
{
    counts[0] = -20;
    counts[1] = -20;
}

void ReqCounts::SetMax()
{
    counts[0] = 20;
    counts[1] = 20;
}
ReqCounts operator + (const ReqCounts &l, const ReqCounts &r)
{
    ReqCounts ret;
    ReqCounts tempr;
    for (int i = 0; i <= 5; i++) {
        ret.counts[i] = l.counts[i] + r.counts[i];
    }
    return ret;
}

ReqCounts operator - (const ReqCounts &l, const ReqCounts &r)
{
    ReqCounts ret;
    ReqCounts tempr;
    for (int i = 0; i <= 5; i++) {
        ret.counts[i] = l.counts[i] - r.counts[i];
    }
    return ret;
}

ReqCounts operator * (const ReqCounts &l, int r)
{
    ReqCounts ret;
    for (int i = 0; i <= 5; i++) {
        ret.counts[i] = l.counts[i] * r;
    }
    return ret;

}

ReqCounts operator / (const ReqCounts &l, int r)
{
    ReqCounts ret;
    for (int i = 0; i <= 5; i++) {
        ret.counts[i] = l.counts[i] / r;
    }
    return ret;

}

ReqCounts operator - (const ReqCounts &l, int r)
{
    ReqCounts ret;
    for (int i = 0; i <= 5; i++) {
        if (l.counts[i] >= r)
            ret.counts[i] = l.counts[i] - r;
        else
            ret.counts[i] = 0;
    }
    return ret;

}

bool operator > (const ReqCounts &l, const ReqCounts &r)
{
    if (l.counts[0] > r.counts[0])
        return true;
    else if (r.counts[0] > l.counts[0])
        return false;
    else if (((l.counts[1] * 2 + l.counts[2]) * 3 + l.counts[3]) * 5 + l.counts[4] > 
            ((r.counts[1] * 2 + r.counts[2]) * 3 + r.counts[3]) * 5 + r.counts[4])
        return true;
    return false;
}

bool operator < (const ReqCounts &l, const ReqCounts &r)
{
    return (r > l);
}

bool BetterForStart(const ReqCounts &l, const ReqCounts &r)
{
    if (l.counts[0] > r.counts[0])
        return true;
    if (r.counts[0] > l.counts[0])
        return false;
    if ((l.counts[1] > 0 || l.counts[2] > 2) && (r.counts[1] == 0 && r.counts[2] <= 2))
        return true;
    if ((r.counts[1] > 0 || r.counts[2] > 2) && (l.counts[1] == 0 && l.counts[2] <= 2))
        return false;
    return l > r;
}

void ReqCounts::operator = (const ReqCounts &r)
{
    for (int i = 0; i <= 5; i++) {
        counts[i] = r.counts[i];
    }

}
int& ReqCounts::operator [](int i)
{
    assert(0 <= i && i <=5);
    return counts[i];
}

int ReqCounts::minStep() const
{
    if (counts[0] != 0)
        return 0;
    if (counts[1] != 0)
        return 1;
    if (counts[2] != 0)
        return 2;
    if (counts[3] != 0)
        return 3;
    if (counts[4] != 0)
        return 4;
    return 5;
}

int ReqCounts::GetValue() const
{
    return ((counts[1]*2 + counts[2])*3 + counts[3])*5 + counts[4];
}
bool ReqCounts::isClose(const ReqCounts &r) const 
{
    if (r.counts[0] == counts[0] && r.counts[1] == counts[1] && abs(r.counts[2] - counts[2]) < 2 &&
            abs(this->GetValue() - r.GetValue()) < 20)
        return true;
    return false;
}

bool ReqCounts::isPositive() const
{
    ReqCounts zero;
    if (*this > zero)
        return true;
    return false;
}

bool ReqCounts::isEmpty() const {
    return (counts[0] == 0 && counts[1] == 0 && counts[2] == 0 && 
            counts[3] == 0 && counts[4] == 0);
}
void ReqCounts::Clear()
{
    for (int i = 0; i <= 5; ++i) {
        counts[i] = 0;
    }
}
void ReqCounts::Print() const
{
    for (int i = 0; i <=5; ++i) {
        printf("Count %d: %d\n", i, counts[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//       PointInfo starts here!                                                   //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

class PointInfo {
public:
    PointInfo(){};
    PointInfo(Point p);
    PointInfo(const PointInfo &);
    friend PointInfo operator + (const PointInfo &l, const PointInfo &r);

    Point     pos;
    ReqCounts rCount;
    pointContriMap pointContri;
    bool valid;
};
PointInfo::PointInfo(Point p)
{
    pos = p;
}
PointInfo::PointInfo(const PointInfo &r)
{
    pos         = r.pos;
    rCount      = r.rCount;
    pointContri = r.pointContri;

    valid       = r.valid;
}

PointInfo operator + (const PointInfo &l, const PointInfo &r)
{
    PointInfo ret(l);
    PointInfo tempr(r);
    ret.rCount = ret.rCount + tempr.rCount;
    pointContriMap::iterator it = tempr.pointContri.begin();
    for (; it != tempr.pointContri.end(); ++it) {
        ret.pointContri[it->first] = ret.pointContri[it->first] + it->second;
    }
    return ret;
}

PointInfo operator * (const PointInfo &l, int r)
{
    PointInfo ret(l);
    ret.rCount = ret.rCount * r;
    pointContriMap::iterator it = ret.pointContri.begin();
    for (; it != ret.pointContri.end(); ++it) {
        ret.pointContri[it->first] = ret.pointContri[it->first] * r;
    }
    return ret;
}

PointInfo operator / (const PointInfo &l, int r)
{
    PointInfo ret(l);
    ret.rCount = ret.rCount / r;
    pointContriMap::iterator it = ret.pointContri.begin();
    for (; it != ret.pointContri.end(); ++it) {
        ret.pointContri[it->first] = ret.pointContri[it->first] / r;
    }
    return ret;
}

bool operator > (const PointInfo &l, const PointInfo &r)
{                          
   if (l.rCount > r.rCount)
       return true;
   return false;

}

void PrintPointContriMap(pointContriMap m)
{
    pointContriMap::iterator it = m.begin();
    for (; it != m.end(); ++it) {
        printf("(%d, %d)\n", it->first.x, it->first.y);
        it->second.Print();
    }
}
////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//       MoveEval starts here!                                                    //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////
class MoveEval {
public:
    MoveEval();
    bool UpdateBetter(const MoveEval&, Strategy);
    //void UpdateDefBetter(MoveEval&);
    void Copy(const MoveEval&);

    Point pos;
    Point antiPos;
    ReqCounts totalCounts;
    int atkConnectNum;
    int defConnectNum;
    bool haveToDeal;
    bool canDeal;
    bool blockOpp;
    bool defOpp;
    int selfBestStep;
    int oppBestStep;
};
MoveEval::MoveEval()
{
    pos.x = -1;
    pos.y = -1;
    antiPos.x = -1;
    antiPos.y = -1;
    totalCounts.SetMin();
    atkConnectNum = -65535;
    defConnectNum = -65535;
    haveToDeal = false;
    canDeal    = false;
    blockOpp   = false;
    defOpp     = false;
    selfBestStep = 4;
    oppBestStep  = 4;
}
void MoveEval::Copy(const MoveEval & newEval)
{
    pos           = newEval.pos;
    antiPos       = newEval.antiPos;
    totalCounts   = newEval.totalCounts;
    atkConnectNum = newEval.atkConnectNum;
    defConnectNum = newEval.defConnectNum;
    haveToDeal    = newEval.haveToDeal;
    canDeal       = newEval.canDeal;
    blockOpp      = newEval.blockOpp;
    defOpp        = newEval.defOpp;
    selfBestStep  = newEval.selfBestStep;
    oppBestStep   = newEval.oppBestStep;
}
bool MoveEval::UpdateBetter(const MoveEval& newEval, Strategy s)
{
    if (!this->pos.Valid()) {
        this->Copy(newEval);
        return true;
    }
        
    if (!haveToDeal && newEval.haveToDeal) {
        if(newEval.totalCounts.isPositive()) {
            this->Copy(newEval);
            return true;
        }
    } else if (haveToDeal == newEval.haveToDeal) {
        if (!haveToDeal) {
            if (!blockOpp && newEval.blockOpp) {
                this->Copy(newEval);
                return true;
            } else if (blockOpp && !newEval.blockOpp) {
                return false;
            }
            if (!defOpp && newEval.defOpp) {
                this->Copy(newEval);
                return true;
            } else if (defOpp && !newEval.defOpp) {
                return false;
            }
        } else {
            if (canDeal && !newEval.canDeal) {
                this->Copy(newEval);
                return true;
            } else if (!canDeal && newEval.canDeal) {
                return false;
            }
            if (s == Attack) {
                if (totalCounts.counts[1] == newEval.totalCounts.counts[1] &&
                        totalCounts.counts[2] < newEval.totalCounts.counts[2]) {
                    this->Copy(newEval);
                    return true;
                }
            }
        }
        if (totalCounts.isClose(newEval.totalCounts)) {
            if (s == Defend) {
                if (newEval.defConnectNum > defConnectNum) {
                    this->Copy(newEval);
                    return true;
                }
            } else if (s == Attack) {
                if (newEval.atkConnectNum > atkConnectNum) {
                    this->Copy(newEval);
                    return true;
                }
            }
        } else {
            if (newEval.totalCounts > totalCounts) {
                this->Copy(newEval);
                return true;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//       GT_FIRAI starts here!                                                    //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////
class GT_FIRAI {
public:
    GT_FIRAI(const NodeType[BoardSize][BoardSize], NodeType);
    Point Move();
    void AssumeMove(const Point& p, const NodeType& t);
    void RemoveAssume(const Point& p);
    void ClearAssume();
    void EvalBoard();
    void UpdateEvalPoint(const Point& p);
    void EvalPoint(PointInfo& retPointInfo, const Point& p, const NodeType& t, const NodeType& selfValue);
    void GetTotalCounts();
    void GetTotalMove();
    Point StartMove();
    MoveEval EvaluateMove(const Point& p); 
    int GetConnectNumber(const Point& p, NodeType t, Strategy s);
    int TestWinMove(const Point& p, const NodeType& t, int step, bool mustFour);
    void UpdateOppWinPoint(const Point& tryp,
                           std::vector<Point> oppWinPointList, 
                           std::set<Point, PointComparator>* checkedPointList,
                           Point* oppWinPoint,
                           MoveEval* oppMaxEval,
                           bool* canStopWin,
                           int* selfBestMove); 
    bool CanStopOppWin(const Point& p, std::vector<Point> oppWinPointList);
    inline bool isEmpty(const Point& p);
    inline NodeType GetType(const Point &p);
   
    NodeType board[BoardSize][BoardSize];
    PointInfo selfptInfo[BoardSize][BoardSize];
    PointInfo oppptInfo[BoardSize][BoardSize];
    NodeType type;
    NodeType oppType;
    int      totalMove;
    ReqCounts totalSelfCounts;
    ReqCounts totalOppCounts;
    PointInfo maxInitSelfInfo;
    PointInfo maxInitOppInfo;
    std::vector<PointInfo> FourPoints;
    assumeNodeMap assumeNodes;

private:
};

GT_FIRAI::GT_FIRAI(const NodeType b[BoardSize][BoardSize], NodeType yourType) {
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            board[i][j] = b[i][j];
        }
    }
    type = yourType;
    oppType = type == Black ? White : Black;
    GetTotalMove();
    EvalBoard();
}

inline bool GT_FIRAI::isEmpty(const Point& p)
{
    if (board[p.x][p.y] == Empty)
        return true;
    return false;
}

inline NodeType GT_FIRAI::GetType(const Point& p)
{
    return board[p.x][p.y];
}
void GT_FIRAI::GetTotalCounts()
{
    totalSelfCounts = totalSelfCounts - totalSelfCounts;
    totalOppCounts  = totalOppCounts  - totalOppCounts;
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i, j);
            if (board[i][j] != Empty)
                continue;
            PointInfo selfInfo = selfptInfo[i][j];
            PointInfo oppInfo  = oppptInfo[i][j];
            totalSelfCounts    = totalSelfCounts + selfInfo.rCount;
            totalOppCounts     = totalOppCounts  + oppInfo.rCount;
        }
    }

}
void GT_FIRAI::GetTotalMove()
{
    totalMove = 0;
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (board[i][j] != Empty)
                totalMove++;
        }
    }
}
Point GT_FIRAI::StartMove()
{
    Point pinvalid(-1, -1);
    if ((maxInitOppInfo.rCount[1] > 0 || maxInitOppInfo.rCount[2] >= 3 || maxInitOppInfo.rCount[3] >= 8) && 
            (maxInitSelfInfo.rCount[2] <= 1 && maxInitSelfInfo.rCount[1] == 0 &&
             maxInitSelfInfo.rCount[0] == 0))
        return maxInitOppInfo.pos;
    return pinvalid;
}
Point GT_FIRAI::Move()
{
    Point retPoint(0,0);
    PointInfo maxSelfInfo;
    PointInfo maxOppInfo;
    PointInfo maxTotalInfo;
    ReqCounts maxTotalCount;
    MoveEval  maxTotalEval;
    //bool firstCount = true;
    Point maxTotalPoint;
    int selfWinStep = 100;
    Point selfWinPoint;
    int oppWinStep = 100;
    Point oppWinPoint;
    bool oppHasFour = false;
    ReqCounts maxOppWinCount;
    bool hasWinPoint = false;
    std::vector<Point> oppWinPointList;
    std::vector<int>   oppWinStepList;
    if (totalMove == 0) {
        retPoint.Set(BoardSize/2, BoardSize/2);
        return retPoint;
    }
    GetTotalCounts();


    // Check for direct win/lose point, speed the algorithm up
    if (maxInitSelfInfo.rCount[0] > 0)
        return maxInitSelfInfo.pos;
    if (maxInitOppInfo.rCount[0] > 0)
        return maxInitOppInfo.pos;

    maxTotalCount.SetMin();
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i, j);
            if (!isEmpty(p))
                continue;

            int thisptSelfWinStep = TestWinMove(p, type, std::min(5, selfWinStep - 1), false);
            int thisptOppWinStep  = TestWinMove(p, oppType, 5, false);

            if (thisptSelfWinStep != 0 && thisptSelfWinStep < selfWinStep) {
                selfWinStep  = thisptSelfWinStep;
                selfWinPoint = p;
                hasWinPoint = true;
            }
            if (thisptOppWinStep != 0) {
                oppWinPointList.push_back(p);
                oppWinStepList.push_back(thisptOppWinStep);
                if (oppptInfo[p.x][p.y].rCount[1] > 0)
                    oppHasFour = true;
                hasWinPoint = true;
                if (thisptOppWinStep < oppWinStep) {
                    oppWinStep = thisptOppWinStep;
                    oppWinPoint = p;
                }
            }

            if (!hasWinPoint) {
                MoveEval me = EvaluateMove(p);
                if (totalMove <= 10 || BetterForStart(totalOppCounts, totalSelfCounts))
                    maxTotalEval.UpdateBetter(me, Defend);
                else
                    maxTotalEval.UpdateBetter(me, Attack);
            }
        }
    }
    if (hasWinPoint) {
        if ((selfWinStep <= 5 && selfptInfo[selfWinPoint.x][selfWinPoint.y].rCount[1] > 0) || 
                (!oppHasFour && selfWinStep <= oppWinStep)) {
            return selfWinPoint;
        } else {
            std::set<Point, PointComparator> checkedPoints;
            ReqCounts oppMaxCounts;
            MoveEval  oppMaxEval;
            bool canStopWin = false;
            int selfBestMove = 5;
            int oppWinStep = 5;
            oppMaxCounts.SetMin();
            // Try all oppWinPoints first, see if there's a way to stop opp
            // to win. Also record the minimum step opp needs to win.
            for (int i = oppWinPointList.size() - 1; i >= 0; --i) {
                Point opppt = oppWinPointList[i];
                ReqCounts oppCounts = oppptInfo[opppt.x][opppt.y].rCount;
                ReqCounts tempCounts;

                // If opp has four, we only need our four to try
                if ((oppCounts[1] > 1 || (oppCounts[1] == 1 && oppCounts[2] > 1)) && selfBestMove > 2 && !canStopWin) {
                    selfBestMove = 2;
                    oppMaxCounts.SetMin();
                }

                UpdateOppWinPoint(opppt, oppWinPointList, &checkedPoints, &oppWinPoint, &oppMaxEval, &canStopWin, &selfBestMove);

                pointContriMap possiblePoints = oppptInfo[opppt.x][opppt.y].pointContri;
                pointContriMap::iterator it = possiblePoints.begin();
                for (; it != possiblePoints.end(); ++it) {
                    Point tryp = it->first;
                    ReqCounts leftCounts = oppCounts - it->second;
                    ReqCounts tryCounts = selfptInfo[tryp.x][tryp.y].rCount;
                    if (leftCounts[0] == 0 && 
                            ((leftCounts[1] == 0 && leftCounts[2] < 4) || 
                             (leftCounts[1] == 1 && leftCounts[2] < 2)) && 
                            (tryCounts[1] > 0 || tryCounts[2] > 1))
                        UpdateOppWinPoint(tryp, oppWinPointList, &checkedPoints, &oppWinPoint, &oppMaxEval, &canStopWin, &selfBestMove);
                }
            }

            if (!canStopWin) {
                MoveEval maxTryEval;
                if (FourPoints.size() > 0 ) { // && selfBestMove >= 2) {
                    for (int i = 0; i < FourPoints.size(); ++i) {
                        MoveEval me = EvaluateMove(FourPoints[i].pos);
                        maxTryEval.UpdateBetter(me, Defend);
                    }
                } 
                if (maxTryEval.pos.Valid())
                    oppWinPoint = maxTryEval.pos;
            }
            return oppWinPoint;
        }
    } else {
        if (totalMove <= 6) {
            Point pstart = StartMove();
            if (pstart.Valid())
                return pstart;
        }
        return maxTotalEval.pos;
    }
}
void GT_FIRAI::UpdateOppWinPoint(const Point& tryp,
                                 std::vector<Point> oppWinPointList, 
                                 std::set<Point, PointComparator>* checkedPointList,
                                 Point* oppWinPoint,
                                 MoveEval* oppMaxEval,
                                 bool* canStopWin, 
                                 int* selfBestMove) {
    if (checkedPointList->find(tryp) == checkedPointList->end()) {
        MoveEval me = EvaluateMove(tryp);
        ReqCounts tempCounts = me.totalCounts;
        ReqCounts selfCounts = selfptInfo[tryp.x][tryp.y].rCount;
        ReqCounts oppCounts  = oppptInfo[tryp.x][tryp.y].rCount;
        if (!(*canStopWin)) {
            if (CanStopOppWin(tryp, oppWinPointList)) {
                *oppWinPoint  = tryp;
                oppMaxEval->Copy(me);
                *canStopWin   = true;
            } else {
                if (oppMaxEval->oppBestStep >= me.oppBestStep && oppMaxEval->UpdateBetter(me, Defend)) {
                    *oppWinPoint = tryp;
                }
            }
        } else {
            if (CanStopOppWin(tryp, oppWinPointList)) {
                if (oppMaxEval->UpdateBetter(me, Defend)) {
                    *oppWinPoint = tryp;
                }
            }
        } 
        // if we can't stop opp to win, check below:
        // 1. selfBestMove == 1, we already has a four to try
        // 2. selfBestMove == 2, opp has three, we need four or three with block
        // 3. selfBestMove == 3, opp has two, we already have three
        // 4. selfBestMove >  3, opp has two, we do not have three
        /*if (!(*canStopWin)) {
            if (*selfBestMove == 1) {
                if (selfCounts[1] > 0 && tempCounts > *oppMaxCounts) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 1;
                }
            } else if (*selfBestMove == 2) {
                if (selfCounts[1] > 0) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 1;
                } else if ((oppCounts[1] >= 2 || (oppCounts[1] == 1 && oppCounts[2] > 1)) && 
                        tempCounts > *oppMaxCounts) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 2;
                }
            } else if (*selfBestMove == 3) {
    	        if (selfCounts[1] > 0) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 1;
                } else if (oppCounts[1] >= 2 || (oppCounts[1] == 1 && oppCounts[2] > 1)) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 2;
                } else if (selfCounts[2] > 1 && tempCounts > *oppMaxCounts) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 3;
                }
            } else {
                if (selfCounts[1] > 0) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 1;
                } else if (oppCounts[1] >= 2 || (oppCounts[1] == 1 && oppCounts[2] > 1)) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 2;
                } else if (selfCounts[2] > 1 ) {
                    *oppWinPoint  = tryp;
                    *oppMaxCounts = tempCounts;
                    *selfBestMove  = 3;
                }
            }
        }*/
        checkedPointList->insert(tryp);
    }
}

bool GT_FIRAI::CanStopOppWin(const Point& p, std::vector<Point> oppWinPointList) {
    bool canStopWin = true;
    AssumeMove(p, type);
    for (int wi = oppWinPointList.size() - 1; wi >= 0; --wi) {
        if (p != oppWinPointList[wi] && TestWinMove(oppWinPointList[wi], oppType, 5, false) > 0) {
            canStopWin = false;
            break;
        }
    }
    RemoveAssume(p);
    return canStopWin;
}
int GT_FIRAI::GetConnectNumber(const Point& p, NodeType t, Strategy s)
{
    int rightInc = 0;
    int botInc = 0;
    NodeType oppt = t == Black ? White : Black;
    int retnum = 0;
    assert(board[p.x][p.y] == Empty);
    for (int dindex = 0; dindex <=3; ++dindex) {
        Direction d = (Direction)dindex;
        switch (d) {
            case LeftRight:
                rightInc = 1;
                botInc   = 0;
                break;
            case TopBot:
                rightInc = 0;
                botInc   = 1;
                break;
            case LTRB:
                rightInc = 1;
                botInc   = 1;
                break;
            case RTLB:
                rightInc = -1;
                botInc   = 1;
                break;
            default:
                assert(false);
        }
        int x = p.x-4*rightInc;
        int y = p.y-4*botInc;
        for (int i = 0; i <= 8; ++i) {
            Point thisp(x+i*rightInc, y+i*botInc);
            if (s == Attack) {
                if (!thisp.Valid() || board[thisp.x][thisp.y] == oppt)
                    retnum -= 5-abs(i-4);
                else if (board[thisp.x][thisp.y] == t)
                    retnum += 5-abs(i-4);
            } else if (s == Defend) {
                if (!thisp.Valid())
                    retnum -= 5-abs(i-4);
                else if (board[thisp.x][thisp.y] == oppt)
                    retnum += 5-abs(i-4);
                else if (board[thisp.x][thisp.y] == Empty)
                    retnum += (5-abs(i-4))/2;
            }
        }
    }
    return retnum;
}
MoveEval GT_FIRAI::EvaluateMove(const Point& p) 
{
    MoveEval retme;
    retme.pos = p;
    ReqCounts selfCounts = selfptInfo[p.x][p.y].rCount;
    ReqCounts oppCounts  = oppptInfo[p.x][p.y].rCount;
    ReqCounts minTotalCounts;
    minTotalCounts.SetMax();
    const pointContriMap& selfInitContri = selfptInfo[p.x][p.y].pointContri;
    bool oppFirst = false;

    pointContriMap::const_iterator selfit;
    pointContriMap::const_iterator oppit;

    retme.atkConnectNum = GetConnectNumber(p, type, Attack);
    retme.defConnectNum = GetConnectNumber(p, type, Defend);
    retme.selfBestStep = selfCounts.minStep();

    if (oppCounts[1] > 0 || oppCounts[2] >= 2) {
        oppFirst = true;
        if (oppCounts[2] != 2)
            retme.defOpp = true;
    }

    // If opp has to deal with self move
    if (selfCounts[1] > 0 || selfCounts[2] >= 2) {
        bool canDeal = false;
        retme.haveToDeal  = true;
        for (selfit = selfInitContri.begin(); selfit != selfInitContri.end(); ++selfit) {
            Point antip;
            antip.Copy(selfit->first);
            ReqCounts tempCounts = selfCounts + oppCounts - selfit->second;
            if (tempCounts[1] == 0 && tempCounts[2] < 2) {
                ReqCounts antiCounts = oppptInfo[antip.x][antip.y].rCount;
                ReqCounts antiSelfCounts = selfptInfo[antip.x][antip.y].rCount;
                tempCounts = tempCounts - antiCounts - antiSelfCounts;
                oppit  = oppptInfo[antip.x][antip.y].pointContri.find(p);
                if (oppit != oppptInfo[antip.x][antip.y].pointContri.end())
                    tempCounts = tempCounts + oppit->second;
                if (minTotalCounts > tempCounts || canDeal == false) {
                    canDeal = true;
                    minTotalCounts = tempCounts;
                    retme.antiPos     = antip;
                    retme.oppBestStep = antiCounts.minStep();
                    if (antiCounts[1] > 0 || antiCounts[2] > 1)
                        retme.haveToDeal = false;
                }
            } else if (canDeal == false) {
                ReqCounts antiCounts = oppptInfo[antip.x][antip.y].rCount;
                ReqCounts antiSelfCounts = selfptInfo[antip.x][antip.y].rCount;
                tempCounts = tempCounts - antiCounts - antiSelfCounts;
                oppit  = oppptInfo[antip.x][antip.y].pointContri.find(p);
                if (oppit != oppptInfo[antip.x][antip.y].pointContri.end())
                    tempCounts = tempCounts + oppit->second;
                if (minTotalCounts > tempCounts) {
                    minTotalCounts = tempCounts;
                    retme.antiPos     = antip;
                    retme.oppBestStep = antiCounts.minStep();
                }
            }
        }
        retme.totalCounts = minTotalCounts;
        retme.canDeal = canDeal;
        retme.atkConnectNum -= GetConnectNumber(retme.antiPos, oppType, Defend);
        retme.defConnectNum -= GetConnectNumber(retme.antiPos, oppType, Attack);
        return retme;
    } else {
        // If opp does not have to deal with self move, they will tend to make
        // three/four first
        retme.haveToDeal = false;
        bool hasThreeFour = false;
        for (int i = 0; i < BoardSize; ++i) {
            for (int j = 0; j < BoardSize; ++j) {
                Point antip(i,j);
                if (isEmpty(antip) && p != antip) {
                    ReqCounts antiCounts = oppptInfo[antip.x][antip.y].rCount;
                    ReqCounts antiSelfCounts = selfptInfo[antip.x][antip.y].rCount;
                    ReqCounts tempCounts = selfCounts + oppCounts - antiCounts - antiSelfCounts;
                    if (isConnected(p, antip)) {
                        selfit = selfInitContri.find(antip);
                        oppit  = oppptInfo[antip.x][antip.y].pointContri.find(p);
                        if (selfit != selfInitContri.end()) {
                            tempCounts = tempCounts - selfit->second;
                        }
                        if (oppit != oppptInfo[antip.x][antip.y].pointContri.end()) {
                            tempCounts = tempCounts + oppit->second;
                            antiCounts = antiCounts - oppit->second;
                        }
                    }
                    if (hasThreeFour == false) {
                        if (antiCounts[1] > 0 || antiCounts[2] > 1) {
                            if (oppFirst == true)
                                retme.blockOpp = false;
                            hasThreeFour = true;
                            minTotalCounts = tempCounts;
                            retme.antiPos = antip;
                            retme.oppBestStep = antiCounts.minStep();
                        } else if (minTotalCounts > tempCounts) {
                            if (oppFirst == true)
                                retme.blockOpp = true;
                            minTotalCounts = tempCounts;
                            retme.antiPos = antip;
                            retme.oppBestStep = antiCounts.minStep();
                        }
                    } else {
                        if (minTotalCounts > tempCounts && (antiCounts[1] > 0 || antiCounts[2] > 1)) {
                            minTotalCounts = tempCounts;
                            retme.antiPos = antip;
                            retme.oppBestStep = antiCounts.minStep();
                        }
                    }
                }
            }
        }
    }
    retme.totalCounts = minTotalCounts;
    // This means we can't find any anti moves, normally the board is full
    if (minTotalCounts[0] > 0)
        return retme;
    
    retme.atkConnectNum -= GetConnectNumber(retme.antiPos, oppType, Defend);
    retme.defConnectNum -= GetConnectNumber(retme.antiPos, oppType, Attack);

    return retme;
}

// Test if I move at p, if I can win in step
// if step == 0, it's impossible
// if step == 1, this step needs to win
// if step == 2, after opponent move, need to win next move
int GT_FIRAI::TestWinMove(const Point& p, const NodeType& t, int step, bool mustFour)
{
    PointInfo ptinfo = (t == type) ? selfptInfo[p.x][p.y] : oppptInfo[p.x][p.y];
    ReqCounts selfCounts = ptinfo.rCount;
    NodeType antit = t == Black ? White: Black;
    bool isPossible = false;
    int  totalUpCount = 0;

    if (ptinfo.valid == false || step <= 0) {
        return 0;
    }
 
    for (int s = 0; s < std::min(3, step); ++s) {
        totalUpCount += selfCounts[s];
        if (totalUpCount > std::min(s, 1)) {
            isPossible = true;
            break;
        }
    }

    if (!isPossible)
        return 0;

    if (mustFour) {
        if (selfCounts[0] == 0 && selfCounts[1] == 0)
            return 0;
    }

    if (ptinfo.rCount.counts[0] > 0)
        return 1;
    else {
        int  maxStep = 0;

        // Here we assume that we already move at p, then check if there
        // is a defense on board that can stop us to win
        pointContriMap antipList = ptinfo.pointContri;
        pointContriMap nextpList = ptinfo.pointContri;
        pointContriMap::iterator antiit = antipList.begin();
        pointContriMap::iterator nextit = nextpList.begin();
        AssumeMove(p, t);
        for (; antiit != antipList.end(); ++antiit) {
            if ((step >= 2 && antiit->second[1] > 0) || 
                    (step >= 3 && antiit->second[2] > 0)) {//|| 
                    //(step >= 4 && antiit->second[3] >= 3)) {
                Point antip(antiit->first);
                if (antip.Valid() && isEmpty(antip)) {
                    PointInfo      antiptInfo    = (antit == type) ? selfptInfo[antip.x][antip.y] : oppptInfo[antip.x][antip.y];
                    ReqCounts      antipCounts   = antiptInfo.rCount;
                    pointContriMap antipNextList = antiptInfo.pointContri;
                    pointContriMap::iterator antinextit = antipNextList.begin();
                    int antiWinSteps = 100;
                    AssumeMove(antip, antit);
                    if ((antipCounts[1] > 1 && selfCounts[1] == 0) ||
                            (antipCounts[1] == 1 && antiit->second[1] == selfCounts[1])) {
                        RemoveAssume(antip);
                        RemoveAssume(p);
                        return 0;
                    } else if (antipCounts[1] == 1 && selfCounts[1] == 0) {
                        for (; antinextit != antipNextList.end(); ++antinextit) {
                            if (antinextit->second[1] == 1) {
                                int winStep = TestWinMove(antinextit->first, t, step-1, mustFour);
                                RemoveAssume(antip);
                                RemoveAssume(p);
                                return winStep;
                            }
                        }
                    } else if (antipCounts[2] > 1) {
                        mustFour = true;
                    }

                    // Check whether this anti move is a win move, if so, how many steps
                    /*for (; antinextit != antipNextList.end(); ++antinextit) {
                        if (antinextit)
                        
                        int tempantiWinSteps = TestWinMove(antinextit->first, antit, std::min(antiWinSteps-1, step - 2));
                        if (tempantiWinSteps != 0 && tempantiWinSteps < antiWinSteps)
                            antiWinSteps = tempantiWinSteps;
                    }*/
                    int minStep = step + 1;
                    // after two moves, check the board for WinMove
                    // record minimum steps to win in minStep
                    for (nextit = nextpList.begin(); nextit != nextpList.end(); ++nextit) {
                        if ((nextit->first != antiit->first) && (
                                (step >= 2 && nextit->second[1] > 0) || 
                                (step >= 3 && nextit->second[2] > 0))) {// ||
                                //(step >= 4 && nextit->second[3] >= 1))) {
                            Point nextp(nextit->first);
                            if (nextp.Valid() && isEmpty(nextp)) {
                                int s = TestWinMove(nextp, t, std::min(antiWinSteps, minStep-2), mustFour);
                                if (s > 0 && s + 1 < minStep) {
                                    minStep = s + 1;
                                    if (minStep == 2)
                                        break;
                                }
                            }
                        }
                    }
                    RemoveAssume(antip);
                    // If there's a way to win the game, see if it's 
                    // larger than maxSteps. If not, it's not a wise
                    // defense and we should ignore it. If it is,
                    // update the maxStep to win.
                    // Otherwise this defense will terminate winMove
                    if (minStep < step + 1) {
                        if (minStep > maxStep) 
                            maxStep = minStep;
                    } else {
                        RemoveAssume(p);
                        return 0;
                    }
                }

            }
        }
        RemoveAssume(p);
        if (maxStep <= step)
            return maxStep;
        assert(false);
    }
    return 0;
}

void GT_FIRAI::AssumeMove(const Point& p, const NodeType& t)
{
    assert(p.Valid());
    assert(board[p.x][p.y] == Empty);
    board[p.x][p.y] = t;
    assumeNodes[p]  = t;
    UpdateEvalPoint(p);
}

void GT_FIRAI::RemoveAssume(const Point& p)
{
    assumeNodeMap::iterator it = assumeNodes.find(p);
    assert(p.Valid());
    assert(it != assumeNodes.end());
    assert(board[p.x][p.y] != Empty);
    board[p.x][p.y] = Empty;
    assumeNodes.erase(it);
    UpdateEvalPoint(p);
}
void GT_FIRAI::ClearAssume()
{
    assumeNodeMap::iterator it = assumeNodes.begin();
    for (; it != assumeNodes.end(); ++it) {
        Point p    = it->first;
        NodeType t = it->second;
        assert(p.Valid());
        board[p.x][p.y] = t;
        UpdateEvalPoint(p);
    }
    assumeNodes.clear();
}
void GT_FIRAI::UpdateEvalPoint(const Point& p)
{
    int rightInc = 0;
    int botInc = 0;
    for (int dindex = 0; dindex <=3; ++dindex) {
        Direction d = (Direction)dindex;
        switch (d) {
            case LeftRight:
                rightInc = 1;
                botInc   = 0;
                break;
            case TopBot:
                rightInc = 0;
                botInc   = 1;
                break;
            case LTRB:
                rightInc = 1;
                botInc   = 1;
                break;
            case RTLB:
                rightInc = -1;
                botInc   = 1;
                break;
            default:
                assert(false);
        }
        int x = p.x-4*rightInc;
        int y = p.y-4*botInc;
        for (int i = 0; i <= 8; ++i) {
            Point thisp(x+i*rightInc, y+i*botInc);
            if (thisp.Valid()) {
                if (isEmpty(thisp)) {
                    EvalPoint(selfptInfo[thisp.x][thisp.y], thisp, type, type);
                    EvalPoint(oppptInfo[thisp.x][thisp.y], thisp, oppType, oppType);
                } else {
                    selfptInfo[thisp.x][thisp.y].valid = false;
                    oppptInfo[thisp.x][thisp.y].valid  = false;
                }
            }
        }
    }
}
void GT_FIRAI::EvalBoard()
{
    NodeType oppType = (type == Black) ? White : Black;
    int maxSelfConnectNum = -65535;
    int maxOppConnectNum  = -65535;
   
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i,j);
            if (isEmpty(p)) {
                EvalPoint(selfptInfo[i][j], p, type, type);
                EvalPoint(oppptInfo[i][j], p, oppType, oppType);
                // for self
                if (selfptInfo[i][j].rCount.isClose(maxInitSelfInfo.rCount)) {
                    int tempNum = GetConnectNumber(p, type, Attack);
                    if (tempNum > maxSelfConnectNum) {
                        maxInitSelfInfo = selfptInfo[i][j];
                        maxSelfConnectNum = tempNum;
                    }
                } else if (BetterForStart(selfptInfo[i][j].rCount, maxInitSelfInfo.rCount)) {
                    maxSelfConnectNum = GetConnectNumber(p, type, Attack);
                    maxInitSelfInfo = selfptInfo[i][j];
                }
                // for opp
                if (oppptInfo[i][j].rCount.isClose(maxInitOppInfo.rCount)) {
                    int tempNum = GetConnectNumber(p, oppType, Attack);
                    if (tempNum > maxOppConnectNum) {
                        maxInitOppInfo = oppptInfo[i][j];
                        maxOppConnectNum = tempNum;
                    }
                } else if (BetterForStart(oppptInfo[i][j].rCount, maxInitOppInfo.rCount)) {
                    maxOppConnectNum = GetConnectNumber(p, oppType, Attack);
                    maxInitOppInfo = oppptInfo[i][j];
                }

                if (selfptInfo[i][j].rCount[1] > 0)
                    FourPoints.push_back(selfptInfo[i][j]);
            } else {
                selfptInfo[i][j].valid = false;
                oppptInfo[i][j].valid  = false;
            }
        }
    }
}
void GT_FIRAI::EvalPoint(PointInfo &retPointInfo, const Point& p, const NodeType& t, const NodeType& selfValue)
{
    retPointInfo.pos.Copy(p);
    if (board[p.x][p.y] != Empty) {
        retPointInfo.valid = false;
        return;
    }
    retPointInfo.valid = true;
    NodeType oppType = t == Black ? White : Black;
    retPointInfo.rCount.Clear();
    retPointInfo.pointContri.clear();
    int rightInc = 0;
    int botInc = 0;

    // assume the point is type t, clear it before return
    board[p.x][p.y] = t;
    for (int dindex = 0; dindex <=3; ++dindex) {
        Direction d = (Direction)dindex;
        switch (d) {
            case LeftRight:
                rightInc = 1;
                botInc   = 0;
                break;
            case TopBot:
                rightInc = 0;
                botInc   = 1;
                break;
            case LTRB:
                rightInc = 1;
                botInc   = 1;
                break;
            case RTLB:
                rightInc = -1;
                botInc   = 1;
                break;
            default:
                assert(false);
        }
        int x = p.x-4*rightInc;
        int y = p.y-4*botInc;
        int conti = 0;
        int empty = 0;
        bool isCounted[5] = {false, false, false, false, false};
        for (int i = 0; i <= 8; ++i) {
            Point thisp(x+i*rightInc, y+i*botInc);
            if (!thisp.Valid() || board[thisp.x][thisp.y] == oppType) {
                conti = 0;
                empty = 0;
                if (i >= 5)
                    break;
            } else if (board[thisp.x][thisp.y] == Empty) {
                conti += 1;
                empty += 1;
            } else {
                conti += 1;
            }
            if (conti == 5) {
                retPointInfo.rCount[empty] += 1;
                for (int j = 0; j <= 4; ++j) {
                    Point lastp(thisp.x-j*rightInc, thisp.y-j*botInc);
                    if (board[lastp.x][lastp.y] == Empty) {
                        retPointInfo.pointContri[lastp][empty] = retPointInfo.pointContri[lastp][empty] + 1;
                        // The point we will throw away is empty;
                        if (j == 4)
                            empty--;
                    }
                }
                isCounted[i-4] = true;
            } else if (conti > 5) {
                Point throwp(thisp.x-5*rightInc, thisp.y-5*botInc);
                // If the point we throw away is empty, the new five should be 
                // counted. Otherwise it's never used.
                // If current p is empty, we add this because either could 
                // happen. If it's occupied, we remove the last info
                if (isEmpty(throwp)) {
		    isCounted[i-4] = true;
                    if (isEmpty(thisp)) {
                        retPointInfo.rCount[empty] += 1;
                        for (int j = 0; j <= 4; ++j) {
                            Point lastp(thisp.x-j*rightInc, thisp.y-j*botInc);
                            if (board[lastp.x][lastp.y] == Empty) {
                                retPointInfo.pointContri[lastp][empty] = retPointInfo.pointContri[lastp][empty] + 1;
                            }
                        }
                    } else {
                        retPointInfo.rCount[empty]   += 1;
                        
                        // we know thisp is not empty here!
                        // we also know throwp is empty!
                        for (int j = 1; j <= 4; ++j) {
                            Point lastp(thisp.x-j*rightInc, thisp.y-j*botInc);
                            if (board[lastp.x][lastp.y] == Empty) {
                                retPointInfo.pointContri[lastp][empty] = retPointInfo.pointContri[lastp][empty] + 1;
                                // only subtract if we counted last one
                                if (isCounted[i-5] == true)
                                    retPointInfo.pointContri[lastp][empty+1] = retPointInfo.pointContri[lastp][empty+1] - 1;
                            }
                        }
                        // only subtract if we counted last one
                        if (isCounted[i-5] == true) {
                            retPointInfo.rCount[empty+1] -= 1;
                            retPointInfo.pointContri[throwp][empty+1] = retPointInfo.pointContri[throwp][empty+1] - 1;
                        }
                        if (retPointInfo.pointContri[throwp][empty+1] < 0)
                            printf("error!\n");
                    }
                }
                if (board[thisp.x-4*rightInc][thisp.y-4*botInc] == Empty)
                    empty--;
            }
        }
    }
    // Clean the map for useless contri points
    pointContriMap::iterator it = retPointInfo.pointContri.begin();
    for (; it != retPointInfo.pointContri.end(); ) {
        if (it->second.isEmpty()) {
            pointContriMap::iterator delit = it;
            ++it;
            retPointInfo.pointContri.erase(delit);
        } else {
            ++it;
        }
    }
    board[p.x][p.y] = Empty;

}

Point GTAIFunc(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    GT_FIRAI gtAI(board, yourType);
    return gtAI.Move();
}

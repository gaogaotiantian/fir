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
        if (l.x < r.x || (l.x == r.x && l.y < r.y))
            return true;
        return false;
    }
};

bool isConnected(Point p1, Point p2) 
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
    friend ReqCounts operator + (const ReqCounts &l, const ReqCounts &r);
    friend ReqCounts operator - (const ReqCounts &l, const ReqCounts &r);
    friend ReqCounts operator - (const ReqCounts &l, int r);
    friend ReqCounts operator * (const ReqCounts &l, int r);
    friend ReqCounts operator / (const ReqCounts &l, int r);
    //friend bool operator > (const ReqCounts &l, const ReqCounts &r);
    void operator = (const ReqCounts &r);
    int& operator [](int i);

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
        if (ret.counts[i] < 0)
            ret.counts[i] = 0;
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
   if (((l.rCount.counts[1] * 2 + l.rCount.counts[2]) * 2 + l.rCount.counts[3]) * 3 + l.rCount.counts[4] > 
           ((r.rCount.counts[1] * 2 + r.rCount.counts[2]) * 2 + r.rCount.counts[3]) * 3 + r.rCount.counts[4])
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
    int TotalMove();
    int TestWinMove(const Point& p, const NodeType& t, int step);
    bool isEmpty(const Point& p);
   
    NodeType board[BoardSize][BoardSize];
    PointInfo selfptInfo[BoardSize][BoardSize];
    PointInfo oppptInfo[BoardSize][BoardSize];
    NodeType type;
    ReqCounts totalSelfCounts;
    ReqCounts totalOppCounts;
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
    EvalBoard();
}

bool GT_FIRAI::isEmpty(const Point& p)
{
    if (board[p.x][p.y] == Empty)
        return true;
    return false;
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
int GT_FIRAI::TotalMove()
{
    int ret = 0;
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (board[i][j] != Empty)
                ret++;
        }
    }
    return ret;
}

Point GT_FIRAI::Move()
{
    Point retPoint(0,0);
    PointInfo maxSelfInfo;
    PointInfo maxOppInfo;
    PointInfo maxTotalInfo;
    int selfWinStep = 100;
    Point selfWinPoint;
    int oppWinStep = 100;
    Point oppWinPoint;
    bool hasWinPoint = false;
    if (TotalMove() == 0) {
        retPoint.Set(BoardSize/2, BoardSize/2);
        return retPoint;
    }
    GetTotalCounts();
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i, j);
            if (!isEmpty(p))
                continue;
            NodeType oppType = (type == Black) ? White : Black;
            PointInfo selfInfo   = selfptInfo[i][j];
            PointInfo oppInfo    = oppptInfo[i][j];

            PointInfo totalInfo;
            totalInfo = selfInfo + oppInfo;

            selfInfo.pos.Set(i, j);
            oppInfo.pos.Set(i, j);
            totalInfo.pos.Set(i, j);

            if (totalInfo > maxTotalInfo) {
                maxTotalInfo = totalInfo;
            }
            int thisptSelfWinStep = TestWinMove(p, type, std::min(4, selfWinStep - 1));
            int thisptOppWinStep  = TestWinMove(p, oppType, std::min(4, oppWinStep - 1));

            if (thisptSelfWinStep != 0 && thisptSelfWinStep < selfWinStep) {
                selfWinStep  = thisptSelfWinStep;
                selfWinPoint = p;
                hasWinPoint = true;
            }
            if (thisptOppWinStep != 0 && thisptOppWinStep < oppWinStep) {
                oppWinStep  = thisptOppWinStep;
                oppWinPoint = p;
                hasWinPoint = true;
            }
        }
    }
    if (hasWinPoint) {
        if (selfWinStep <= oppWinStep) {
            return selfWinPoint;
        } else {
            return oppWinPoint;
        }
    } else {
        return maxTotalInfo.pos;
    }
}

// Test if I move at p, if I can win in step
// if step == 0, it's impossible
// if step == 1, this step needs to win
// if step == 2, after opponent move, need to win next move
int GT_FIRAI::TestWinMove(const Point& p, const NodeType& t, int step)
{
    PointInfo ptinfo = (t == type) ? selfptInfo[p.x][p.y] : oppptInfo[p.x][p.y];
    NodeType antit = t == Black ? White: Black;
    bool isPossible = false;
    int  totalUpCount = 0;

    if (ptinfo.valid == false || step <= 0) {
        return 0;
    }

    if (step >= 5)
        return 5;
    
    for (int s = 0; s < std::min(3, step); ++s) {
        totalUpCount += ptinfo.rCount[s];
        if (totalUpCount > s) {
            isPossible = true;
            break;
        }
    }
    if (!isPossible)
        return 0;

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
                    (step >= 3 && antiit->second[2] > 0) || 
                    (step >= 4 && antiit->second[3] > 0)) {
                Point antip(antiit->first);
                if (antip.Valid() && isEmpty(antip)) {
                    int minStep = step + 1;
                    AssumeMove(antip, antit);
                    // after two moves, check the board for WinMove
                    // record minimum steps to win in minStep
                    for (nextit = nextpList.begin(); nextit != nextpList.end(); ++nextit) {
                        if ((nextit->first != antiit->first) && (
                                (step >= 2 && nextit->second[1] > 0) || 
                                (step >= 3 && nextit->second[2] > 0) ||
                                (step >= 4 && nextit->second[3] > 0))) {
                            Point nextp(nextit->first);
                            if (nextp.Valid() && isEmpty(nextp)) {
                                int s = TestWinMove(nextp, t, minStep-2);
                                if (s > 0 && s + 1 < minStep)
                                    minStep = s + 1;
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
                        if (minStep > maxStep) maxStep = minStep;
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
    NodeType oppType = type == Black ? White : Black;
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
   
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i,j);
            if (isEmpty(p)) {
                EvalPoint(selfptInfo[i][j], p, type, type);
                EvalPoint(oppptInfo[i][j], p, oppType, oppType);
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
    retPointInfo.valid = true;
    NodeType oppType = t == Black ? White : Black;
    retPointInfo.rCount.Clear();
    retPointInfo.pointContri.clear();
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
        int conti = 0;
        int empty = 0;
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
            if (conti >= 5) {
                retPointInfo.rCount[empty-1] += 1;
                for (int j = 0; j <= 4; ++j) {
                    Point lastp(thisp.x-j*rightInc, thisp.y-j*botInc);
                    if (board[lastp.x][lastp.y] == Empty && lastp != p) {
                        retPointInfo.pointContri[lastp][empty-1] = retPointInfo.pointContri[lastp][empty-1] + 1;
                        if (j == 4)
                            empty--;
                    }
                }
            }
        }
    }
}

Point GTAIFunc(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    GT_FIRAI gtAI(board, yourType);
    return gtAI.Move();
}

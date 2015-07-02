/*
 * This AI is written by Tian Gao
 * github ID: gaogaotiantian
 * ID for the project: 2 
 */
#include <assert.h>
#include <stdlib.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include "game.h"

enum Direction {LeftRight, TopBot, LTRB, RTLB};

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

class ReqCounts {
public:
    ReqCounts();
    bool isImportant();
    friend ReqCounts operator + (const ReqCounts &l, const ReqCounts &r);
    friend ReqCounts operator - (const ReqCounts &l, const ReqCounts &r);
    friend ReqCounts operator - (const ReqCounts &l, int r);
    friend ReqCounts operator * (const ReqCounts &l, int r);
    friend ReqCounts operator / (const ReqCounts &l, int r);
    friend bool operator > (const ReqCounts &l, const ReqCounts &r);
    void operator = (const ReqCounts &r);
    void Print() const;
    
    int counts[6];
};

typedef std::map<Point, ReqCounts, PointComparator> pointContriMap;
void PrintPointContriMap(pointContriMap m);

ReqCounts::ReqCounts()
{
    for (int i = 0; i <= 5; i++) {
        counts[i] = 0;
    }
}
bool ReqCounts::isImportant()
{
    exit(1);
    if (counts[0] != 0 || 
           (counts[1] >= 2) || 
           (counts[1] == 1 && counts[2] >= 3 ) ||
           (counts[2] >= 5))
        return true;
    return false;
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
bool operator > (const ReqCounts &l, const ReqCounts &r)
{
    if (l.counts[0] != 0) 
        return true;
    if (r.counts[0] != 0)
        return false;
    if (l.counts[1] > 1 && l.counts[1] > r.counts[1])
        return true;
    if (r.counts[1] > 1 && r.counts[1] > l.counts[1])
        return false;
    if (l.counts[1] == r.counts[1]) {
        if (l.counts[2] * 3 + l.counts[3] >= r.counts[2] * 3 + r.counts[3])
            return true;
        return false;
    } else {
        if (l.counts[1] * 3 + l.counts[2] > r.counts[1] * 3 + r.counts[2])
            return true;
        if (l.counts[1] * 3 + l.counts[2] < r.counts[1] * 3 + r.counts[2])
            return false;
    }
    if (l.counts[3] * 5 + l.counts[4] >= r.counts[3] * 5 + r.counts[4])
        return true;
    return false;
}

void ReqCounts::operator = (const ReqCounts &r)
{
    for (int i = 0; i <= 5; i++) {
        counts[i] = r.counts[i];
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
    bool isImportant() const;
    Point FindMostContriPoint() const;
    friend PointInfo operator + (const PointInfo &l, const PointInfo &r);

    Point     pos;
    ReqCounts rCount;
    pointContriMap pointContri;
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
}

bool PointInfo::isImportant() const
{
    ReqCounts tempReq;
    if (!pos.Valid())
        return false;
    if (rCount.counts[0] > 0)
        return true;
    else {
        Point p = FindMostContriPoint();
        pointContriMap::const_iterator it = pointContri.find(p);
        //assert(it != pointContri.end());
        tempReq = rCount - it->second;
        
        if (tempReq.counts[1] >= 1 || tempReq.counts[2] >= 2) {
            return true;
        }
    }
    return false;
}
Point PointInfo::FindMostContriPoint() const
{
    Point retP;
    ReqCounts retCounts;
    pointContriMap::const_iterator it = pointContri.begin();
    for (; it != pointContri.end(); ++it) {
        if (it->second.counts[1] > retCounts.counts[1]) {
            retP      = it->first;
            retCounts = it->second;
        } else if (it->second.counts[1] == retCounts.counts[1]) {
            if (it->second.counts[2] > retCounts.counts[2]) {
                retP      = it->first;
                retCounts = it->second;
            }
            else if (it->second.counts[2] == retCounts.counts[2]) {
                if (it->second.counts[3] > retCounts.counts[3]) {
                    retP      = it->first;
                    retCounts = it->second;
                } else if (it->second.counts[3] == retCounts.counts[3]) {
                    if (it->second.counts[4] > retCounts.counts[4]) {
                        retP      = it->first;
                        retCounts = it->second;
                    }
                }
            }
        }
    }
    return retP;
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
    Point p;
    if (!r.pos.Valid())
        return true;
    if (!l.pos.Valid())
        return false;
    if (l.rCount.counts[0] >= 1)
        return true;
    if (r.rCount.counts[0] >= 1)
        return false;
    if (l.isImportant() && r.isImportant()) {
        if (l.rCount.counts[1] > r.rCount.counts[1]) 
            return true;
        if (l.rCount.counts[1] < r.rCount.counts[1])
            return false;
        if (l.rCount.counts[2] >= r.rCount.counts[2])
            return true;
        return false;
    } else if (l.isImportant()) { 
        return true;
    } else if (r.isImportant()) {
        return false;
    } else {
        Point pl = l.FindMostContriPoint();
        Point pr = r.FindMostContriPoint();
        pointContriMap::const_iterator itl = l.pointContri.find(pl);
        pointContriMap::const_iterator itr = r.pointContri.find(pr);
        //assert(itl != l.pointContri.end() && itr != r.pointContri.end());
        ReqCounts templ = l.rCount - itl->second;
        ReqCounts tempr = r.rCount - itr->second;
        if (((templ.counts[1] * 3 + templ.counts[2]) * 3 + templ.counts[3]) * 5 + templ.counts[4] >= 
                ((tempr.counts[1] * 3 + tempr.counts[2]) * 3 + tempr.counts[3]) * 5 + tempr.counts[4])
            return true;
        return false;
    }
}


bool FirstBetter(const ReqCounts &li, const ReqCounts &ri);

// This function will only be called if both move are important
bool FirstBetter(const PointInfo &li, const PointInfo &ri)
{
    ReqCounts l = li.rCount;
    ReqCounts r = ri.rCount;
    if (l.counts[0] != 0) 
        return true;
    if (r.counts[0] != 0)
        return false;
    if (l.counts[1] > 1)
        return true;
    if (r.counts[1] > 1)
        return false;
    if (l.counts[1] == 1)
        return true;
    if (r.counts[1] == 1 && r.counts[2] >= 2)
        return false;
    // here, l.counts[0] == l.counts[1] == 0
    return true;
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
    PointInfo EvalPoint(Point p, NodeType t, NodeType selfValue);
    void GetTotalCounts();
    int TotalMove();
   
    NodeType board[BoardSize][BoardSize];
    NodeType type;
    ReqCounts totalSelfCounts;
    ReqCounts totalOppCounts;

private:
    int EvalFive(NodeType[5], NodeType);
    PointInfo EvalPointOneDir(Point p, NodeType t, Direction d, NodeType selfValue);
};

GT_FIRAI::GT_FIRAI(const NodeType b[BoardSize][BoardSize], NodeType yourType) {
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            board[i][j] = b[i][j];
        }
    }
    type = yourType;
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
            NodeType oppType = (type == Black) ? White : Black;
            PointInfo selfInfo = EvalPoint(p, type, type);
            PointInfo oppInfo  = EvalPoint(p, oppType, oppType);
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
    bool importantSelfMove = false;
    bool importantOppMove  = false;
    if (TotalMove() == 0) {
        retPoint.Set(BoardSize/2, BoardSize/2);
        return retPoint;
    }
    GetTotalCounts();
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i, j);
            if (board[i][j] != Empty)
                continue;
            NodeType oppType = (type == Black) ? White : Black;
            PointInfo selfInfo   = EvalPoint(p, type, type);
            PointInfo oppInfo    = EvalPoint(p, oppType, oppType);
            PointInfo totalInfo;
            if (totalSelfCounts > totalOppCounts)
                totalInfo = selfInfo * 3 + oppInfo * 2;
            else
                totalInfo = selfInfo * 2 + oppInfo * 3;

            selfInfo.pos.Set(i, j);
            oppInfo.pos.Set(i, j);
            totalInfo.pos.Set(i, j);

            if (selfInfo.isImportant() && selfInfo > maxSelfInfo) {
                maxSelfInfo = selfInfo;
                importantSelfMove = true;
            }
            if (oppInfo.isImportant() && oppInfo > maxOppInfo) {
                maxOppInfo = oppInfo;
                importantOppMove = true;
            }
            if (!selfInfo.isImportant() && !oppInfo.isImportant()) {
                if (totalInfo > maxTotalInfo) {
                    maxTotalInfo = totalInfo;
                }
            }
        }
    }
    if (importantSelfMove && importantOppMove) {
        if (FirstBetter(maxSelfInfo, maxOppInfo))
            return maxSelfInfo.pos;
        else
            return maxOppInfo.pos;
    } else if (importantSelfMove) {
        return maxSelfInfo.pos;
    } else if (importantOppMove) {
        return maxOppInfo.pos;
    } else {
        return maxTotalInfo.pos;
    }
}
PointInfo GT_FIRAI::EvalPoint(Point p, NodeType t, NodeType selfValue)
{
    PointInfo retPointInfo;
    retPointInfo.pos.Copy(p);
    retPointInfo = EvalPointOneDir(p, t, LeftRight, selfValue) +
            EvalPointOneDir(p, t, TopBot, selfValue) +
            EvalPointOneDir(p, t, LTRB, selfValue) +
            EvalPointOneDir(p, t, RTLB, selfValue);
    return retPointInfo;
}
PointInfo GT_FIRAI::EvalPointOneDir(Point p, NodeType t, Direction d, NodeType selfValue)
{
    PointInfo retPointInfo;
    ReqCounts totalCounts;
    int rightInc = 0;
    int botInc = 0;
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
    for (int i = 0; i <= 4; ++i) {
        Point start(p.x-4*rightInc+i*rightInc, p.y-4*botInc+i*botInc);
        Point end(p.x+i*rightInc, p.y+i*botInc);
        if (start.Valid() && end.Valid()) {
            NodeType nodeList[5];
            Point    pointList[5];
            ReqCounts tempCount;
            int ct = 0;
            for (int j = 0; j <= 4; ++j) {
                pointList[j].Set(start.x+j*rightInc, start.y+j*botInc);
                if (start.x+j*rightInc == p.x && start.y+j*botInc == p.y) {
                    nodeList[j] = selfValue; 
                } else {
                    nodeList[j] = board[start.x+j*rightInc][start.y+j*botInc];
                }
            }
            ct = EvalFive(nodeList, t);
            if (ct >= 0) {
                tempCount.counts[ct] += 1;
                // for each non-self, empty point in the list, increase it's contribution
                for (int j = 0; j <= 4; ++j) {
                    if (pointList[j] != p && nodeList[j] == Empty)
                        retPointInfo.pointContri[pointList[j]] = retPointInfo.pointContri[pointList[j]] + tempCount;
                }
            }
            totalCounts = totalCounts + tempCount;
        }
    }
    retPointInfo.rCount = totalCounts;
    return retPointInfo;
}
int GT_FIRAI::EvalFive(NodeType nodeList[5], NodeType t) 
{
    int num = 5;
    for (int i = 0; i < 5; i++) {
        if (nodeList[i] == t) {
            num--;
        } else if (nodeList[i] != Empty) {
            return -1;
        }
    }
    return num;
}

Point GTAIFunc(const NodeType board[BoardSize][BoardSize], NodeType yourType)
{
    GT_FIRAI gtAI(board, yourType);
    return gtAI.Move();
}

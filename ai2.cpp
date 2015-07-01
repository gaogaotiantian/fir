/*
 * This AI is written by Tian Gao
 * github ID: gaogaotiantian
 * ID for the project: 2 
 */
#include <assert.h>
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
class ReqCounts {
public:
    ReqCounts();
    bool isImportant();
    friend ReqCounts operator + (const ReqCounts &l, const ReqCounts &r);
    friend bool operator > (const ReqCounts &l, const ReqCounts &r);
    void operator = (const ReqCounts &r);
    void Print();
    
    int counts[6];
    
};
ReqCounts::ReqCounts()
{
    for (int i = 0; i <= 5; i++) {
        counts[i] = 0;
    }
}
bool ReqCounts::isImportant()
{
    if (counts[0] != 0 || counts[1] > 1)
        return true;
    return false;
}
ReqCounts operator + (const ReqCounts &l, const ReqCounts &r)
{
    ReqCounts ret;
    for (int i = 0; i <= 5; i++) {
        ret.counts[i] = l.counts[i] + r.counts[i];
    }
    return ret;
}
bool operator > (const ReqCounts &l, const ReqCounts &r)
{
    if (l.counts[0] != 0) 
        return true;
    if (r.counts[0] != 0)
        return false;
    if (l.counts[1] > 1)
        return true;
    if (r.counts[1] > 1)
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
void ReqCounts::Print()
{
    for (int i = 0; i <=5; ++i) {
        printf("Count %d: %d\n", i, counts[i]);
    }
}
class GT_FIRAI {
public:
    GT_FIRAI(const NodeType[BoardSize][BoardSize], NodeType);
    Point Move();
    ReqCounts EvalPoint(Point p, NodeType t, NodeType selfValue);
    int TotalMove();
   
    NodeType board[BoardSize][BoardSize];
    NodeType type;

private:
    int EvalFive(NodeType[5], NodeType);
    ReqCounts EvalPointOneDir(Point p, NodeType t, Direction d, NodeType selfValue);
};

GT_FIRAI::GT_FIRAI(const NodeType b[BoardSize][BoardSize], NodeType yourType) {
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            board[i][j] = b[i][j];
        }
    }
    type = yourType;
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
    ReqCounts maxSelfCount;
    Point selfPoint(0,0);
    ReqCounts maxOppCount;
    Point oppPoint(0,0);
    ReqCounts maxTotalCount;
    Point totalPoint(0,0);
    bool importantMove = false;
    if (TotalMove() == 0) {
        retPoint.Set(BoardSize/2, BoardSize/2);
        return retPoint;
    }
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            Point p(i, j);
            if (board[i][j] != Empty)
                continue;
            NodeType oppType = (type == Black) ? White : Black;
            ReqCounts selfCount  = EvalPoint(p, type, type);
            ReqCounts oppCount   = EvalPoint(p, oppType, oppType);
            ReqCounts totalCount = selfCount + oppCount;
            if (selfCount.isImportant() && selfCount > maxSelfCount) {
                maxSelfCount = selfCount;
                selfPoint.Set(i,j);
                importantMove = true;
            }
            if (oppCount.isImportant() && oppCount > maxOppCount) {
                maxOppCount = oppCount;
                oppPoint.Set(i,j);
                importantMove = true;
            }
            if (!selfCount.isImportant() && !oppCount.isImportant()) {
                if (totalCount > maxTotalCount) {
                    maxTotalCount = totalCount;
                    totalPoint.Set(i,j);
                }
            }
        }
    }
    if (importantMove) {
        if (maxSelfCount > maxOppCount)
            return selfPoint;
        else
            return oppPoint;
    } else
        return totalPoint;
}
ReqCounts GT_FIRAI::EvalPoint(Point p, NodeType t, NodeType selfValue)
{
    ReqCounts totalCounts;
    totalCounts = totalCounts + EvalPointOneDir(p, t, LeftRight, selfValue) +
            EvalPointOneDir(p, t, TopBot, selfValue) +
            EvalPointOneDir(p, t, LTRB, selfValue) +
            EvalPointOneDir(p, t, RTLB, selfValue);
    return totalCounts;
}
ReqCounts GT_FIRAI::EvalPointOneDir(Point p, NodeType t, Direction d, NodeType selfValue)
{
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
            ReqCounts tempCount;
            int ct = 0;
            for (int j = 0; j <= 4; ++j) {
                if (start.x+j*rightInc == p.x && start.y+j*botInc == p.y) {
                    nodeList[j] = selfValue;
                } else {
                    nodeList[j] = board[start.x+j*rightInc][start.y+j*botInc];
                }
            }
            ct = EvalFive(nodeList, t);
            if (ct >= 0) {
                tempCount.counts[ct] += 1;
            }
            totalCounts = totalCounts + tempCount;
        }
    }
    return totalCounts;
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

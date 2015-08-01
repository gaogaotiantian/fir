#include "game.h"
#include <math.h>
#include <vector>
using namespace std;

void buildStatusBoard(const NodeType board[BoardSize][BoardSize], int statusBoard[BoardSize][BoardSize][5], NodeType type) {
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (board[i][j] == Empty) {
                // to left
                int horizonBlock = 2;  // 活: 2, 半活: 1, 死: 0
                int countLeft = i - 1;
                while (countLeft >= 0 && board[countLeft][j] == type) {
                    countLeft--;
                }
                if (countLeft == -1 || board[countLeft][j] != Empty) {
                    horizonBlock -= 1;
                }
                // to right
                int countRight = i + 1;
                while (countRight < BoardSize && board[countRight][j] == type) {
                    countRight++;
                }
                if (countRight == BoardSize || board[countRight][j] != Empty) {
                    horizonBlock -= 1;
                }
                int horizonCount = (i - countLeft - 1) + (countRight - i - 1) + 1;

                // to top
                int verticalBlock = 2;
                int countTop = j - 1;
                while (countTop >= 0 && board[i][countTop] == type) {
                    countTop--;
                }
                if (countTop == -1 || board[i][countTop] != Empty) {
                    verticalBlock -= 1;
                }
                // to bottom
                int countBottom = j + 1;
                while (countBottom < BoardSize && board[i][countBottom] == type) {
                    countBottom++;
                }
                if (countBottom == BoardSize || board[i][countBottom] != Empty) {
                    verticalBlock -= 1;
                }
                int verticalCount = (j - countTop - 1) + (countBottom - j - 1) + 1;

                // to left-top
                int leftDiagnalBlock = 2;
                int offsetLeftTop = 1;
                while (i - offsetLeftTop >= 0 && j - offsetLeftTop >= 0
                    && board[i-offsetLeftTop][j-offsetLeftTop] == type) {
                    offsetLeftTop++;
                }
                if (i - offsetLeftTop == -1 || j - offsetLeftTop == -1 || board[i-offsetLeftTop][j-offsetLeftTop] != Empty) {
                    leftDiagnalBlock -= 1;
                }
                // to right-bottom
                int offsetRightBottom = 1;
                while (i + offsetRightBottom < BoardSize && j + offsetRightBottom < BoardSize
                    && board[i+offsetRightBottom][j+offsetRightBottom] == type) {
                    offsetRightBottom++;
                }
                if (i + offsetRightBottom == BoardSize || j + offsetRightBottom == BoardSize || board[i+offsetRightBottom][j+offsetRightBottom] != Empty) {
                    leftDiagnalBlock -= 1;
                }
                int leftDiagnalCount = (offsetLeftTop - 1) + (offsetRightBottom - 1) + 1;

                // to right-top
                int rightDiagnalBlock = 2;
                int offsetRightTop = 1;
                while (i + offsetRightTop < BoardSize && j - offsetRightTop >= 0 && board[i+offsetRightTop][j-offsetRightTop] == type) {
                    offsetRightTop++;
                }
                if (i + offsetRightTop == BoardSize || j - offsetRightTop == -1 || board[i+offsetRightTop][j-offsetRightTop] != Empty) {
                    rightDiagnalBlock -= 1;
                }
                // to left-bottom
                int offsetLeftBottom = 1;
                while (i - offsetLeftBottom >= 0 && j + offsetLeftBottom < BoardSize && board[i-offsetLeftBottom][j+offsetLeftBottom] == type) {
                    offsetLeftBottom++;
                }
                if (i - offsetLeftBottom == -1 || j + offsetLeftBottom == BoardSize || board[i-offsetLeftBottom][j+offsetLeftBottom] != Empty) {
                    rightDiagnalBlock -= 1;
                }
                int rightDiagnalCount = (offsetRightTop - 1) + (offsetLeftBottom - 1) + 1;


                statusBoard[i][j][0] = (int) pow(horizonCount, 3) * horizonBlock;
                statusBoard[i][j][1] = (int) pow(verticalCount, 3) * verticalBlock;
                statusBoard[i][j][2] = (int) pow(leftDiagnalCount, 3) * leftDiagnalBlock;
                statusBoard[i][j][3] = (int) pow(rightDiagnalCount, 3) * rightDiagnalBlock;
                statusBoard[i][j][4] = statusBoard[i][j][0] + statusBoard[i][j][1] + statusBoard[i][j][2] + statusBoard[i][j][3];
            }
        }
    }
}


Point yhfAI(const NodeType board[BoardSize][BoardSize], NodeType yourType) {
    int myStatusBoard[BoardSize][BoardSize][5] = {0};  // 我的状态表
    int heStatusBoard[BoardSize][BoardSize][5] = {0};  // 对手状态表

    NodeType heType = yourType == Black ? White : Black;

    buildStatusBoard(board, myStatusBoard, yourType);
    buildStatusBoard(board, heStatusBoard, heType);

    bool allEmpty = true;
    int maxMy = 0, maxHe = 0, maxSum = 0, maxMyI = 0, maxMyJ = 0, maxHeI = 0, maxHeJ = 0, maxSumI = 0, maxSumJ = 0;
    int THRESHOLD = 124;
    vector<int> dangerPoints;
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (heStatusBoard[i][j][0] >= THRESHOLD || heStatusBoard[i][j][1] >= THRESHOLD ||
                heStatusBoard[i][j][2] >= THRESHOLD || heStatusBoard[i][j][3] >= THRESHOLD) {
                dangerPoints.push_back(i * BoardSize + j);
            }
            if (board[i][j] == yourType) {
                allEmpty = false;
            }
            if (myStatusBoard[i][j][0] >= THRESHOLD || myStatusBoard[i][j][1] >= THRESHOLD ||
                myStatusBoard[i][j][2] >= THRESHOLD || myStatusBoard[i][j][3] >= THRESHOLD) {
                Point p(i, j);
                return p;
            }
            if (myStatusBoard[i][j][4] > maxMy) {
                maxMy = myStatusBoard[i][j][4];
                maxMyI = i;
                maxMyJ = j;
            }
            if (heStatusBoard[i][j][4] > maxHe) {
                maxHe = heStatusBoard[i][j][4];
                maxHeI = i;
                maxHeJ = j;
            }
            if (myStatusBoard[i][j][4] + heStatusBoard[i][j][4] > maxSum) {
                maxSum = myStatusBoard[i][j][4] + heStatusBoard[i][j][4];
                maxSumI = i;
                maxSumJ = j;
            }
        }
    }

    if (dangerPoints.size() > 0) {
        // find the most dangerous point from dangerPoints.
        int heMaxStatus = 0;
        int heMaxI, heMaxJ;
        for (int &index : dangerPoints) {
            int i = index / BoardSize;
            int j = index % BoardSize;
            if (heStatusBoard[i][j][4] > heMaxStatus) {
                heMaxStatus = heStatusBoard[i][j][4];
                heMaxI = i;
                heMaxJ = j;
            }
        }
        Point p(heMaxI, heMaxJ);
        return p;
    }

    if (allEmpty) {
        if (board[BoardSize/2][BoardSize/2] == Empty) {
            Point p(BoardSize/2, BoardSize/2);
            return p;
        } else {
            Point p(BoardSize/2 - 2, BoardSize/2 - 2);
            return p;
        }
    } else {
        Point p(maxSumI, maxSumJ);
        return p;
    }

    Point p(0, 0);
    return p;
}

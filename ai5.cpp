/*
    @Author: dummy_index

*/
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include "game.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include <assert.h>
using namespace std;
const static double four_score = 10;
const static double three_score = 1.5;
const static double side_three = 0.6;
const static double two_step_three_score = 0.1;
const static double side_two_step_three = 0;
const static double neighbor_score = 0.05;
const static double two_score = 0.7;
const static double side_two = 0.3;
const static double side_xxox_side = 0.3;
const static double side_two_interim = 0.2;
const static int minimax_strategy_num = 4;
const static int minimax_depth = 10;
const static int minimax_strategy_inclass_firstLayer = 5;
class AntiAI{
    struct MyPoint{
        Point p;
        double score;
        MyPoint(int x , int y , int val){
            p.x = x ;
            p.y = y;
            score = val;
        }
        static bool cmp(MyPoint p1 , MyPoint p2){
            return p1.score>p2.score;
        }
    };
private:
    NodeType **board;
    NodeType mytype;
    NodeType othertype;
    double **score;
    int boardsize;
    //int has_three;
    //int has_four;
    bool isAllEmpty;
    vector<Point> max_scores;
    vector<MyPoint> scores;


public:
    double max_score;
/*constructors*/
public:
    AntiAI(const NodeType mboard[BoardSize][BoardSize], NodeType mytype):mytype(mytype){
        isAllEmpty = true;
        boardsize = BoardSize;
        othertype = mytype==White?Black:White;
        board = new NodeType*[BoardSize];
        score = new double*[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)board[i] = new NodeType[BoardSize], score[i] = new double[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)
            for(int j = 0 ; j < BoardSize; j++) {
                board[i][j] = mboard[i][j];
                if(board[i][j]==othertype || board[i][j]==mytype){
                    isAllEmpty = false;
                    int dx[8] = {-1,1,0,0,1,-1,-1,1};
                    int dy[8] = {0,0,1,-1,1,-1,1,-1};
                    for(int k = 0 ; k < 8 ; k++){
                        int nr = dx[k] + i, nc = dy[k]+j;
                        if(!(nr<boardsize && nc < boardsize && nr>=0 && nc>=0))continue;
                        score[nr][nc] = score[nr][nc]==INT_MIN?INT_MIN:neighbor_score;
                    }
                }
                score[i][j] = board[i][j] == Empty?0:INT_MIN;
        }
        evaluate_empty();
        compute_max_scores();
        sort_scores();
    }
    AntiAI( NodeType** mboard, NodeType mytype):mytype(mytype){
        isAllEmpty = true;
        boardsize = BoardSize;
        othertype = mytype==White?Black:White;
        board = new NodeType*[BoardSize];
        score = new double*[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)board[i] = new NodeType[BoardSize], score[i] = new double[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)
            for(int j = 0 ; j < BoardSize; j++) {
                board[i][j] = mboard[i][j];
                if(board[i][j]==othertype || board[i][j]==mytype){
                    isAllEmpty = false;
                    int dx[8] = {-1,1,0,0,1,-1,-1,1};
                    int dy[8] = {0,0,1,-1,1,-1,1,-1};
                    for(int k = 0 ; k < 8 ; k++){
                        int nr = dx[k] + i, nc = dy[k]+j;
                        if(!(nr<boardsize && nc < boardsize && nr>=0 && nc>=0))continue;
                        score[nr][nc] = score[nr][nc]==INT_MIN?INT_MIN:neighbor_score;
                    }
                }
                score[i][j] = board[i][j] == Empty?0:INT_MIN;
        }
        evaluate_empty();
        compute_max_scores();
        sort_scores();
    }


/*destructors*/

    ~AntiAI(){
        for(int i = 0 ; i < boardsize ; i++){
            delete board[i];
            delete score[i];
        }
        delete board;
        delete score;

    }


private:
    bool check_border(int r , int c){
        return r>=0 && c>=0 && r<boardsize && c<boardsize;
    }
    void evaluate_empty(){
        //cout<<"evaling"<<endl;
        max_score = 0 ;
        checkRight2Left();
        checkUpDown();
        check_rightdown();
        check_leftup();
        for(int i = 0 ; i < boardsize ; i++)
            for(int j = 0 ; j < boardsize ; j++) max_score = (max_score<score[i][j])?score[i][j]:max_score ;
    }
    void checkRight2Left(){
        for(int i = 0 ; i < boardsize ; i++){
            check_dir(i,0,0,1);
        }
    }
    void checkUpDown(){
        for(int i = 0 ; i < boardsize ; i ++){
            check_dir(0,i,1,0);
        }
    }

    void check_rightdown(){
        for(int i = 0 ; i < boardsize ; i++){
            check_dir(i,0,1,1);
            if(i!=0)check_dir(0,i,1,1);
        }
    }

    /*the side case in this function needs improvement.*/

    void check_leftup(){
        for(int i = 0 ; i < boardsize ; i++){
            check_dir(i,0,-1,1);
            if(i!=0)check_dir(boardsize - 1,i,-1,1);
        }
    }


    void check_dir(int sr,int sc, int dr, int dc){
        int r = sr , c = sc ;
        while(r >= 0 && c < boardsize && r < boardsize && c >= 0){
            //cout<<"r="<<r<<"c="<<c<<endl;
            int segment = 0;
            int last_r =  r , last_c = c ;
            while(last_r<boardsize && last_c >= 0 && last_r >= 0 && last_c < boardsize && board[last_r][last_c]==othertype){
                segment++;
                last_r+=dr;
                last_c+=dc;
            }
            //if(segment==4)cout<<"4!"<<endl;
            if(segment == 4){
                if(r-dr > boardsize-1 || c-dc<0 || r-dr<0 || c-dc>boardsize-1){
                    if(last_r<boardsize && last_c >=0 && last_r>=0 && last_c < boardsize && board[last_r][last_c] == Empty)
                        score[last_r][last_c] += four_score ;
                }
                else
                if(last_r <= 0 || last_c >= boardsize || last_r>= boardsize || last_c<0){
                    if(r>=0 && c < boardsize && r < boardsize - 1   && c > 0  && board[r-dr][c-dc]==Empty){
                        score[r-dr][c-dc] += four_score;
                    }
                }
                else{
                    if(board[r-dr][c-dc]==Empty)score[r-dr][c-dc]+=four_score;
                    if(board[last_r][last_c]==Empty) score[last_r][last_c]+=four_score;
                }
            }
            //if(segment==3)cout<<"3!"<<endl;
            if(segment==3){
                bool side = false;
                if( (r-dr > boardsize-1 || c-dc<0 || r-dr<0 || c-dc>boardsize-1 || board[r-dr][c-dc]== mytype) ||
                    (last_r < 0 || last_c > boardsize - 1 || last_r > boardsize -1 || last_c < 0 || board[last_r][last_c]==mytype))
                    side = true;
                //if(r-dr >= 0 && c-dc >= 0 && r-dr < boardsize && c-dc < boardsize){
                if(check_border(r-dr,c-dc)){
                    if(board[r-dr][c-dc]==Empty){
                        score[r-dr][c-dc]+= side?side_three:three_score;
                        //cout<<"side="<< (side?"TRUE":"FALSE")<<endl;
                        //cout<<"three score added"<<endl;
                    }
                    //if(r-2*dr>=0 && c-2*dc >= 0 && r-2*dr < boardsize && c-2*dc < boardsize && board[r-2*dr][c-2*dc]==Empty)
                    if(check_border(r-2*dr,c-2*dc)&& board[r-2*dr][c-2*dc]==Empty)
                        score[r-2*dr][c-2*dc]+=side?side_two_step_three:two_step_three_score;
                }

               // if( last_r < boardsize && last_c < boardsize && last_r >=0 && last_c>=0){
                if(check_border(last_r,last_c)){
                        if(board[last_r][last_c]==Empty)score[last_r][last_c]+=side?side_three:three_score;;
                        if(last_r + dr < boardsize && last_c + dc < boardsize && last_r+dr >= 0 && last_c+dc >=0 && board[last_r+dr][last_c+dc]==Empty)
                            score[last_r+dr][last_c+dc]+=side?side_two_step_three:two_step_three_score;
                }
                //check interim


                //if(r-2*dr>=0 && c-2*dc >= 0 && r-2*dr < boardsize && c-2*dc < boardsize && board[r-2*dr][c-2*dc]==othertype)
                if(check_border(r-2*dr,c-2*dc) && board[r-2*dr][c-2*dc]==othertype)
                        score[r-dr][c-dc] += board[r-dr][c-dc]!=Empty?0:four_score;
                //if(last_r + dr < boardsize && last_c + dc < boardsize && last_r+dr >= 0 && last_c+dc >=0 && board[last_r+dr][last_c+dc]==othertype)
                if(check_border(last_r + dr, last_c + dc) && board[last_r+dr][last_c+dc]==othertype)
                        score[last_r][last_c]+= board[last_r][last_c]!=Empty?0:four_score;

            }
            if(segment == 2){
                bool side = false;
                if( (r-dr > boardsize-1 || c-dc<0 || r-dr<0 || c-dc>boardsize-1 || board[r-dr][c-dc]== mytype) ||
                    (last_r < 0 || last_c > boardsize - 1 || last_r > boardsize -1 || last_c < 0 || board[last_r][last_c]==mytype))
                    side = true;
                if(r-dr >= 0 && c-dc >= 0 && r-dr < boardsize && c-dc <boardsize){
                    if(board[r-dr][c-dc]==Empty){
                        score[r-dr][c-dc]+= side?side_two:two_score;
                    }
                }
                if( last_r < boardsize && last_c < boardsize && last_r>=0 && last_c>=0){
                        if(board[last_r][last_c]==Empty)
                            score[last_r][last_c]+=side?side_two:two_score;;
                }
                // check if there is xxox
                if(r-2*dr>=0 && c-2*dc >= 0 && r-2*dr < boardsize && c-2*dc < boardsize && board[r-2*dr][c-2*dc]==othertype && board[r-dr][c-dc]==Empty){
                        score[r-dr][c-dc] += side?side_two_interim:three_score;
                        if(check_border(r-3*dr,c-3*dc) && board[r-3*dr][c-3*dc] == Empty){
                            score[r-3*dr][c-3*dc] += side?side_xxox_side:three_score;
                        }
                }
                if(last_r + dr < boardsize && last_c + dc < boardsize && last_r+dr >= 0 && last_c+dc >=0 && board[last_r+dr][last_c+dc]==othertype && board[last_r][last_c]==Empty){
                        score[last_r][last_c]+= side?side_two_interim:three_score;
                        if(check_border(last_r+2*dr,last_c+2*dc) && board[last_r+2*dr][last_c+2*dc]==Empty){
                            score[last_r+2*dr][last_c+2*dc] += side? side_xxox_side:three_score;
                        }
                }


                // check if there is x(r,c)xo(last_r,c)xx  only check one side to prevent double adding
                if(check_border(last_r + 2*dr, last_c + 2*dc) && board[last_r+dr][last_c+dc] == othertype && board[last_r + 2*dr][last_c + 2*dc]==othertype)
                    score[last_r][last_c]+=four_score;
            }

            r=last_r==r?r+dr:last_r;
            c=last_c==c?c+dc:last_c;
        }
    }
    Point getFirstStep(){
        Point p(boardsize/2,boardsize/2);
        return p;
    }
    void compute_max_scores(){
        int mr=0,mc=0;
        vector<Point> &vec = max_scores;
        Point p(0,0);
        vec.push_back(p);
        for(int i = 0 ; i < boardsize ; i++)
            for(int j = 0 ; j < boardsize ; j++)
                if(i==0 && j==0)continue;
                else
                if(score[mr][mc]<score[i][j]){
                    mr = i;
                    mc = j;
                    Point p(i,j);
                    vec.clear();
                    vec.push_back(p);
                }
                else
                if(score[mr][mc]==score[i][j]){
                    Point p(i,j);
                    vec.push_back(p);
                }
    }

    void sort_scores(){
        for(int i = 0 ; i < boardsize ; i++)
            for(int j = 0 ; j < boardsize ; j++){
                if(board[i][j]!=Empty)continue;
                MyPoint p(i,j,score[i][j]);
                scores.push_back(p);
            }
        sort(scores.begin() , scores.end(),  MyPoint::cmp);
    }

/*codes from game.cpp*/
private:
    NodeType CheckVictory()
    {
        for (int i = 0; i < BoardSize; ++i) {
            for (int j = 0; j < BoardSize; ++j) {
                Point p(i,j);
                NodeType winType = CheckNode(p);
                if (winType == Black)
                    return Black;
                else if (winType == White)
                    return White;
            }
        }
        return Empty;
    }
    // We only check right, down and right-down
    NodeType CheckNode(Point p)
    {
        NodeType curType = GetType(p);
        // right
        bool vic = true;
        for (int i = 1; i < 5; i++) {
            Point nextP(p.x+i, p.y);
            if (p.x+i >= BoardSize || GetType(nextP) != curType) {
                vic = false;
                break;
            }
        }
        if (vic == true)
            return curType;
        // down
        vic = true;
        for (int i = 1; i < 5; i++) {
            Point nextP(p.x, p.y+i);
            if (p.y+i >= BoardSize || GetType(nextP) != curType) {
                vic = false;
                break;
            }
        }
        if (vic == true)
            return curType;
        // right down
        vic = true;
        for (int i = 1; i < 5; i++) {
            Point nextP(p.x+i, p.y+i);
            if (p.x+i >= BoardSize || p.y+i >= BoardSize || GetType(nextP) != curType) {
                vic = false;
                break;
            }
        }
        if (vic == true)
            return curType;
        //left down
        vic = true;
        for (int i = 1; i < 5; i++) {
            Point nextP(p.x-i, p.y+i);
            if (p.x-i < 0 || p.y+i >= BoardSize || GetType(nextP) != curType) {
                vic = false;
                break;
            }
        }
        if (vic == true)
            return curType;

        return Empty;
    }
    NodeType GetType(Point p)
    {
        assert(0 <= p.x && p.x < BoardSize);
        assert(0 <= p.y && p.y < BoardSize);
        return board[p.x][p.y];
    }
private:
    NodeType** generate_new_board(){
        NodeType** res = new NodeType*[boardsize];
        for(int i = 0 ; i < boardsize ;  i++)
            res[i] = new NodeType[boardsize];
        for(int i = 0 ; i < boardsize ; i++)
            for(int j = 0 ; j < boardsize ; j++)
                res[i][j] = board[i][j];
        return res;
    }

public:
    int minimax(int depth, NodeType type){
        //cout<<"depth="<<depth<<endl;
        if(depth>=minimax_depth)return 0;
        if(CheckVictory()!=Empty)return -1;
        bool flag = false;
        if (mytype==type){
            for(int i = 0 ; i < minimax_strategy_num ; i++){
                if(i>=scores.size())break;
                NodeType** newboard = generate_new_board();
                Point &p = scores[i].p;
                newboard[p.x][p.y] = mytype;
                AntiAI enemy(newboard,othertype);
                int res = enemy.minimax(depth+1,type);
                if(res == 1)return -1;
                if(res == -1) flag = true;
            }
        }
        else{
            Point p = response();
            NodeType** newboard = generate_new_board();
            newboard[p.x][p.y] = mytype;
            AntiAI enemy(newboard,othertype);
            int res = enemy.minimax(depth+1,type);
            if(res == 1)return -1;
            if(res == -1) flag = true;
        }
        return flag?1:0;
    }

    Point minimax_strategy(){
        //cout<<"in mini max strategy!"<<endl;
        if(isAllEmpty)return getFirstStep();
        bool flag = false;
        vector<Point> ties;
        for(int i = 0 ; i < minimax_strategy_num ; i++){
            if(i>=scores.size())break;
            NodeType** newboard = generate_new_board();
            Point &p = scores[i].p;
            newboard[p.x][p.y] = mytype;
            AntiAI enemy(newboard,othertype);
            int res = enemy.minimax(1,mytype);
            if(res == -1){
                return scores[i].p;
            }
            else
            if(res == 0){
                ties.push_back(scores[i].p);
            }
        }
        //if(ties.empty())return response();
        //else return ties[rand()%ties.size()];
        return response();
    }


    int minimax2(int depth , NodeType curtype){
        if(depth>=minimax_depth)return 0;
        if(CheckVictory()!=Empty)return -1;
        bool flag = false;
        if (mytype==curtype){
            for(int i = 0 ; i < minimax_strategy_num ; i++){
                if(i>=scores.size())break;
                Point &p = scores[i].p;
                board[p.x][p.y] = mytype;
                int res = minimax(depth+1, othertype);
                board[p.x][p.y] = Empty;
                if(res == 1)return -1;
                if(res == -1) flag = true;
            }
        }
        else{
            AntiAI enemy(board,othertype);
            Point p = enemy.response();
            board[p.x][p.y] = othertype;
            int res = minimax(depth+1,mytype);
            board[p.x][p.y] = Empty;
            if(res == 1)return -1;
            if(res == -1) flag = true;
        }
        return flag?1:0;
    }
    Point minimax_strategy_inclass(){
        if(isAllEmpty)return getFirstStep();
        for(int i  = 0 ; i < minimax_strategy_inclass_firstLayer ; i++){
            if(i>=scores.size())break;
            Point &p = scores[i].p;
            board[p.x][p.y] = mytype;
            int res = minimax2(1,othertype);
            board[p.x][p.y] = Empty;
            if(res==-1)return p;
        }
        return response();
    }
    int minimax3(int depth , NodeType curtype){
        if(depth>=minimax_depth)return 0;
        if(CheckVictory()!=Empty)return -1;
        bool flag = false;
        if (mytype==curtype){
            Point p = response();
            board[p.x][p.y] = mytype;
            int res = minimax(depth+1, othertype);
            board[p.x][p.y] = Empty;
            if(res == 1)return -1;
            if(res == -1) flag = true;
        }
        else{
            AntiAI enemy(board,othertype);
            Point p = enemy.response();
            board[p.x][p.y] = othertype;
            int res = minimax(depth+1,mytype);
            board[p.x][p.y] = Empty;
            if(res == 1)return -1;
            if(res == -1) flag = true;
        }
        return flag?1:0;
    }
    Point single_child_minimax_strategy(){
        if(isAllEmpty)return getFirstStep();
        for(int i  = 0 ; i < minimax_strategy_inclass_firstLayer ; i++){
            if(i>=scores.size())break;
            Point &p = scores[i].p;
            board[p.x][p.y] = mytype;
            int res = minimax3(1,othertype);
            board[p.x][p.y] = Empty;
            if(res==-1)return p;
        }
        return response();
    }


    Point defend(){
        int randIndex = rand()  % max_scores.size();
        return max_scores[randIndex];
    }
    Point attack(){
            AntiAI enemy(board,othertype);
            return enemy.defend();
       }

    Point response(){
         AntiAI enemy(board,othertype);
         if(isAllEmpty){
            return getFirstStep();
         }
         else
         if(enemy.max_score>=four_score)return enemy.defend();
         else
         if(enemy.max_score>=three_score && max_score < four_score) return enemy.defend();
         else
         if(max_score>=2*two_score)return defend();
         else
         if(enemy.max_score>=three_score)return enemy.defend();
         else
         return enemy.defend();
    }
public:
    void print_board(){
        cout<<endl<<"Current BoardInfo:"<<endl;
        for(int i = 0 ; i < boardsize ; i++){

            for(int j = 0 ; j < boardsize ; j++)cout<<board[i][j]<<' ';
            cout<<endl;

        }

    }

    void print_score(){
        cout<<endl<<"Current Score:"<<endl;
        for(int i = 0 ; i < boardsize ; i++){

            for(int j = 0 ; j < boardsize ; j++){
                if (score[i][j]==INT_MIN)cout<<"- ";
                else cout<<(int)score[i][j]<<' ';
            }
            cout<<endl;

        }
    }
};


Point dummyProcedure(const NodeType board[BoardSize][BoardSize], NodeType mytype){
    AntiAI mashiro(board,mytype);
    //mashiro.print_score();
    //return mashiro.response();
    //return mashiro.minimax_strategy();
    return mashiro.response();
    //return mashiro.minimax_strategy_inclass();
}

NodeType** read_board(string file_path , int boardsize){
    freopen(file_path.c_str(), "r+", stdin);
    NodeType **board = new NodeType*[boardsize];
    for(int i = 0 ; i < boardsize ; i++) board[i] = new NodeType[boardsize];
    for(int i = 0 ; i < boardsize ; i++)
        for(int j = 0 ; j < boardsize ; j++){
            int x ;
            cin>>x;
            if(x==1){
                board[i][j] = Black;
            }
            else if(x==0){
                board[i][j] = Empty;
            }
            else{
                board[i][j] = White;
            }
        }
    return board;
}
void print_board(NodeType** board, int boardsize){
    for(int i = 0 ; i < boardsize ; i++){
        for(int j = 0 ; j < boardsize ; j++){
            cout<<board[i][j]<<' ';
        }
        cout<<endl;
    }
}
/*
int main(){
    string file_path = "./board";
    NodeType **board = read_board(file_path, BoardSize);
    print_board(board,BoardSize);
    AntiAI mashiro(board,White);
    //mashiro.print_board();
    mashiro.print_score();
    Point p =mashiro.defend();
    cout<<"r="<<p.x;
    cout<<"c="<<p.y;
}

*/

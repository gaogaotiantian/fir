/*
    @Author: dummy_index

*/
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include "game.h"
#include <string.h>
#include <vector>
using namespace std;
const static double four_score = 5;
const static double three_score = 1.5;
const static double side_three = 0.6;
const static double two_step_three_score = 0;
const static double side_two_step_three = 0;
const static double neighbor_score = 0.05;
const static double two_score = 0.7;
const static double side_two = 0.3;
class AntiAI{
private:
    NodeType **board;
    NodeType mytype;
    NodeType othertype;
    double **score;
    int boardsize;
    //int has_three;
    //int has_four;
    bool isAllEmpty;
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
    }


private:
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
                if(r-dr >= 0 && c-dc >= 0 && r-dr < boardsize && c-dc < boardsize){
                    if(board[r-dr][c-dc]==Empty){
                        score[r-dr][c-dc]+= side?side_three:three_score;
                        //cout<<"side="<< (side?"TRUE":"FALSE")<<endl;
                        //cout<<"three score added"<<endl;
                    }
                    if(r-2*dr>=0 && c-2*dc >= 0 && r-2*dr < boardsize && c-2*dc < boardsize && board[r-2*dr][c-2*dc]==Empty)
                        score[r-2*dr][c-2*dc]+=side?side_two_step_three:two_step_three_score;
                }

                if( last_r < boardsize && last_c < boardsize && last_r >=0 && last_c>=0){
                        if(board[last_r][last_c]==Empty)score[last_r][last_c]+=side?side_three:three_score;;
                        if(last_r + dr < boardsize && last_c + dc < boardsize && last_r+dr >= 0 && last_c+dc >=0 && board[last_r+dr][last_c+dc]==Empty)
                            score[last_r+dr][last_c+dc]+=side?side_two_step_three:two_step_three_score;
                }
                //check interim


                if(r-2*dr>=0 && c-2*dc >= 0 && r-2*dr < boardsize && c-2*dc < boardsize && board[r-2*dr][c-2*dc]==othertype)
                        score[r-dr][c-dc] += score[r-dr][c-dc]==INT_MIN?0:four_score;
                if(last_r + dr < boardsize && last_c + dc < boardsize && last_r+dr >= 0 && last_c+dc >=0 && board[last_r+dr][last_c+dc]==othertype)
                        score[last_r][last_c]+= score[last_r][last_c]==INT_MIN?0:four_score;

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
                        if(board[last_r][last_c]==Empty)score[last_r][last_c]+=side?side_two:two_score;;
                }
                // check if there is xxox
                if(r-2*dr>=0 && c-2*dc >= 0 && r-2*dr < boardsize && c-2*dc < boardsize && board[r-2*dr][c-2*dc]==othertype)
                        score[r-dr][c-dc] += score[r-dr][c-dc]==INT_MIN?0:three_score;
                if(last_r + dr < boardsize && last_c + dc < boardsize && last_r+dr >= 0 && last_c+dc >=0 && board[last_r+dr][last_c+dc]==othertype)
                        score[last_r][last_c]+= score[last_r][last_c]==INT_MIN?0:three_score;
            }

            r=last_r==r?r+dr:last_r;
            c=last_c==c?c+dc:last_c;
        }
    }
    Point getFirstStep(){
        Point p(boardsize/2,boardsize/2);
        return p;
    }
public:
    Point defend(){
        int mr=0,mc=0;
        vector<Point> vec;
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
        int randint = rand()%vec.size();
        return vec[randint];
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
    return mashiro.response();
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

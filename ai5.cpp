/*
    @Author: dummy_index

*/
#include <iostream>
#include <stdlib.h>
#include <unordered_map>
#include <limits.h>
#include "game.h"
#include <string.h>
#include <vector>
using namespace std;

class AntiAI{
private:
    NodeType **board;
    NodeType mytype;
    NodeType othertype;
    double **score;
    int boardsize;
    int has_three;
    int has_four;
    double four_score = 4;
    double three_score = 1;
    double side_three = 0.3;
    double two_step_three_score = 0.6;
    double side_two_step_three = 0.4;
    double neighbor_score = 0.05;

/*constructors*/
public:
    AntiAI(const NodeType mboard[BoardSize][BoardSize], NodeType mytype):mytype(mytype){
        boardsize = BoardSize;
        othertype = mytype==White?Black:White;
        board = new NodeType*[BoardSize];
        score = new double*[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)board[i] = new NodeType[BoardSize], score[i] = new double[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)
            for(int j = 0 ; j < BoardSize; j++) {
                board[i][j] = mboard[i][j];
                if(board[i][j]==othertype){
                    int dx[8] = {-1,1,0,0,1,-1,-1,1};
                    int dy[8] = {0,0,1,-1,1,-1,1,-1};
                    for(int k = 0 ; k < 8 ; k++){
                        int nr = dx[k] + i, nc = dy[k]+j;
                        score[nr][nc]+= neighbor_score;
                    }
                }
                score[i][j] = board[i][j] == Empty?0:INT_MIN;
        }
        evaluate_empty();
    }
    AntiAI(const NodeType** mboard, NodeType mytype):mytype(mytype){
        boardsize = BoardSize;
        othertype = mytype==White?Black:White;
        board = new NodeType*[BoardSize];
        score = new double*[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)board[i] = new NodeType[BoardSize], score[i] = new double[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)
            for(int j = 0 ; j < BoardSize; j++) {
                board[i][j] = mboard[i][j];
                if(board[i][j]==othertype){
                    int dx[8] = {-1,1,0,0,1,-1,-1,1};
                    int dy[8] = {0,0,1,-1,1,-1,1,-1};
                    for(int k = 0 ; k < 8 ; k++){
                        int nr = dx[k] + i, nc = dy[k]+j;
                        score[nr][nc]+= neighbor_score;
                    }
                }
                score[i][j] = board[i][j] == Empty?0:INT_MIN;
        }
        evaluate_empty();
    }
    AntiAI( NodeType** mboard, NodeType mytype):mytype(mytype){
        boardsize = BoardSize;
        othertype = mytype==White?Black:White;
        board = new NodeType*[BoardSize];
        score = new double*[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)board[i] = new NodeType[BoardSize], score[i] = new double[BoardSize];
        for(int i = 0 ; i < BoardSize ; i++)
            for(int j = 0 ; j < BoardSize; j++) {
                board[i][j] = mboard[i][j];
                if(board[i][j]==othertype){
                    int dx[8] = {-1,1,0,0,1,-1,-1,1};
                    int dy[8] = {0,0,1,-1,1,-1,1,-1};
                    for(int k = 0 ; k < 8 ; k++){
                        int nr = dx[k] + i, nc = dy[k]+j;
                        score[nr][nc]+= neighbor_score;
                    }
                }
                score[i][j] = board[i][j] == Empty?0:INT_MIN;
        }
        evaluate_empty();
    }


private:
    void evaluate_empty(){
        checkRight2Left();
        checkUpDown();
        check_rightdown();
        check_leftup();
    }
    void checkRight2Left(){
        for(int i = 0 ; i<boardsize; i++){
            int pos = 0 ;
            while(pos<boardsize){
                int segment = 0 ;
                int last = pos;
                while(last<boardsize && board[i][last]!=Empty && board[i][last]!=mytype){
                    segment++;
                    if(segment==3)has_three = true;
                    if(segment==4)has_four = true;
                    last++;
                }
                if(segment==4){
                    has_four = true;
                    if(pos==0 && board[i][last]==Empty){
                        score[i][last]+=four_score;
                    }
                    else
                    if(last==boardsize && board[i][pos-1] == Empty){
                        score[i][pos-1]+=four_score;
                    }
                    else{
                        if(board[i][pos-1] == Empty) score[i][pos-1] += four_score;
                        if(board[i][last] == Empty) score[i][last] += four_score;
                    }
                }
                if(segment==3){
                    has_three = true;
                    bool side = false;
                    if ((pos!=0 && board[i][pos-1]==mytype) || (last!= boardsize-1 && board[i][last]==mytype))
                        side = true;
                    if(pos!=0 && board[i][pos-1]==Empty){
                        score[i][pos-1]+=side?side_three:three_score;
                        if(pos-1!=0 && board[i][pos-2]==Empty){
                            score[i][pos-2]+=side?side_two_step_three:two_step_three_score;
                        }
                    }
                    if(last!= boardsize-1 && board[i][last]==Empty){
                        score[i][last]+=side?side_three:three_score;
                        if(last+1!=boardsize-1 && board[i][last+1]==Empty){
                            score[i][last+1]+=side?side_two_step_three:two_step_three_score;
                        }
                    }
                }
                pos = last==pos?pos+1:last;
            }
        }
    }
    void checkUpDown(){
        for(int i = 0 ; i<boardsize; i++){
            int pos = 0 ;
            while(pos<boardsize){
                int segment = 0 ;
                int last = pos;
                while(last<boardsize && board[last][i]!=Empty && board[last][i]!=mytype){
                    segment++;
                    if(segment==3)has_three = true;
                    if(segment==4)has_four = true;
                    last++;
                }
                if(segment==4){
                    has_four = true;
                    if(pos==0 && board[last][i]==Empty){
                        score[last][i]+=four_score;
                    }
                    else
                    if(last==boardsize && board[pos-1][i] == Empty){
                        score[pos-1][i]+=four_score;
                    }
                    else{
                        if(board[pos-1][i] == Empty) score[pos-1][i] += four_score;
                        if(board[last][i] == Empty) score[last][i] += four_score;
                    }
                }
                if(segment==3){
                    has_three = true;
                    bool side = false;
                    if ( (pos!= 0 && board[pos-1][i]==mytype) || (last!= boardsize-1 && board[last][i]==mytype) )
                        side = true;
                    if(pos!=0 && board[pos-1][i]==Empty){
                        score[pos-1][i]+=side?side_three:three_score;
                        if(pos-1!=0 && board[pos-2][i]==Empty){
                            score[pos-2][i]+=side?side_two_step_three:two_step_three_score;
                        }
                    }
                    if(last!= boardsize-1 && board[last][i]==Empty){
                        score[last][i]+=side?side_three:three_score;
                        if(last+1!=boardsize-1 && board[last+1][i]==Empty){
                            score[last+1][i]+=side?side_two_step_three:two_step_three_score;
                        }
                    }
                }
                pos = last==pos?pos+1:last;
            }
        }
    }

    void check_rightdown(){
        for(int i = 0 ; i < boardsize ; i++){
            check_diagonal_rightdown(i,0);
            if(i!=0)check_diagonal_rightdown(0,i);
        }


    }

    /*the side case in this function needs improvement.*/
    void check_diagonal_rightdown(int sr, int sc){
        int r = sr , c = sc;
        while(r<boardsize && c<boardsize){
            int segment = 0 ;
            int last_r = r , last_c = c;
            while(last_r<boardsize && last_c<boardsize && board[last_r][last_c]==othertype){
                segment++;
                last_r++;
                last_c++;
            }
            if(segment==4)cout<<"segment=4"<<endl;
            if(segment==4){
                if(r==0 || c==0){
                    if (last_r<boardsize && last_c<boardsize && board[last_r][last_c]==Empty)
                        score[last_r][last_c]+=four_score;
                }
                else
                if(last_r>=boardsize || last_c >= boardsize){
                    if(r>0 && c>0 && board[r-1][c-1]== Empty)
                        score[r-1][c-1]+=four_score;
                }
                else{
                    if(board[r-1][c-1]==Empty)score[r-1][c-1]+=four_score;
                    if(board[last_r][last_c]==Empty)score[last_r][last_c]+=four_score;
                }
            }

            if(segment==3){
                bool side = false;
                if(  (r-1 >= 0 && c-1 >= 0 && board[r-1][c-1]== mytype) || (last_r < boardsize && last_c < boardsize && board[last_r][last_c]==mytype))
                    side = true;

                if(r-1 >= 0 && c-1 >= 0){
                    if(board[r-1][c-1]==Empty)score[r-1][c-1]+=side?side_three:three_score;
                    if(r-2>=0 && c-2 >= 0 && board[r-2][c-2]==Empty)
                        score[r-2][c-2]+=side?side_two_step_three:two_step_three_score;
                }

                if( last_r < boardsize && last_c < boardsize){
                        if(board[last_r][last_c]==Empty)score[last_r][last_c]+=side?side_three:three_score;;
                        if(last_r + 1 < boardsize && last_c + 1 < boardsize && board[last_r+1][last_c+1]==Empty)
                            score[last_r+1][last_c+1]+=side?side_two_step_three:two_step_three_score;
                    }

            }

            r = last_r==r?r+1:last_r;
            c = last_c==c?c+1:last_c;

        }

    }

    void check_leftup(){
        for(int i = 0 ; i < boardsize ; i++){
            check_diagonal_leftup(i,0);
            if(i!=0)check_diagonal_leftup(boardsize-1,i);
        }
    }
    void check_diagonal_leftup(int sr, int sc){
        int r = sr , c = sc ;
        while(r >= 0 && c < boardsize){
            int segment = 0;
            int last_r =  r , last_c = c ;
            //cout<<"last_r="<<last_r<<endl;
            //cout<<"last_c="<<last_c<<endl;
            while(last_r >= 0 && last_c < boardsize && board[last_r][last_c]==othertype){
                segment++;
                last_r--;
                last_c++;
            }
            //if(segment==4)cout<<"!"<<endl;
            if(segment == 4){
                if(r == boardsize -1 || c == 0){
                    if(last_r>=0 && last_c < boardsize && board[last_r][last_c] == Empty)
                        score[last_r][last_c] += four_score ;

                }
                else
                if(last_r <= 0 || last_c >= boardsize - 1){
                    if(r < boardsize - 1   && c > 0  && board[r+1][c-1]==Empty){
                        score[r+1][c-1] += four_score;
                    }
                }
                else{
                    if(board[r+1][c-1]==Empty)score[r+1][c-1]+=four_score;
                    if(board[last_r][last_c]==Empty) score[last_r][last_c]+=four_score;
                }
            }

            if(segment==3){
                    if(r + 1 <= boardsize - 1 && c-1 >= 0){
                        if(board[r+1][c-1]==Empty) score[r+1][c-1]+=three_score;
                        if(r + 2 <= boardsize - 1  && c - 2 >= 0 && board[r+2][r-2]==Empty)
                            score[r+2][c-2]+two_step_three_score;
                    }

                    if(last_r >= 0 && last_c < boardsize){
                        if(board[last_r][last_c]==Empty) score[last_r][last_c]+=three_score;
                        if(last_r - 1 >= 0 && last_c + 1 < boardsize && board[last_r-1][last_c+1]==Empty)
                            score[last_r-1][last_c+1]+=two_step_three_score;
                    }

            }
            r=last_r==r?r-1:last_r;
            c=last_c==c?c+1:last_c;
        }

    }

public:
    Point defense(){
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
                else cout<<score[i][j]<<' ';
            }
            cout<<endl;

        }
    }




};



Point dummyProcedure(const NodeType board[BoardSize][BoardSize], NodeType mytype){
    AntiAI mashiro(board,mytype);
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

        for(int j = 0 ; j < boardsize ; j++)cout<<board[i][j]<<' ';
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
    Point p =mashiro.defense();
    cout<<"r="<<p.x;
    cout<<"c="<<p.y;
}
*/

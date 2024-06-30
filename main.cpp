#include <iostream>
#include <ctime>
#include <cstdlib>

#define SIZE 4
#define ANS_LIMIT 2
using namespace std;

void add_result(int board[]);
bool is_duplicated(int board[]);
bool is_valid(int board[], int pos);
bool eight_queen(int board[], int pos);
int remain_value(int board[], bool mem[SIZE][SIZE], int pos);
bool forward_checking(bool mem[SIZE][SIZE], int pos);

int NODE, SUM=0;
int LIST[ANS_LIMIT][SIZE], INDEX;

/* CSP Search, using backtracking */
bool eight_queen(int board[], int pos){
    NODE++;
    if(pos == SIZE){
        // if leaf node is reached and valid, show the solution
        // prevent duplicated solution because the search function always start from initial state
        if( is_valid(board , pos) && is_duplicated(board) == false ){
            add_result(board);
            cout << "Result : ";
            for(int i = 0;i < SIZE;i++){
                cout << board[i] << " ";
            }
            SUM += NODE;
            cout << "Node : " << NODE << " Sum : " << SUM << endl;

            return true;
        }
    }


    int order_table[SIZE];
    int conflict_table[SIZE] = {0}, sum, count = 0;
    for(int r = 0;r < SIZE;r++){
        bool conflict_mem[SIZE][SIZE];
        board[pos] = r;

        // calculate the Minimum Remain Value
        conflict_table[r] = remain_value(board, conflict_mem, pos);
        // use forward checking to check every unassigned column

        // add move if these column have valid assigned value
        if(forward_checking(conflict_mem, pos)){
            order_table[count] = r;
            count++;
        }
    }

    //sort for better move order (based on Minimum Remaining Value)
    int i, j, m, index, tmp;
    for(int i = 0;i < count;i++){
        m = conflict_table[order_table[i]];
        index = i;
        for(int j = i + 1;j < count;j++){
            if(conflict_table[order_table[j]] < m){
                m = conflict_table[order_table[j]];
                index = j;
            }
            if(conflict_table[order_table[j]] == m && ((rand() % 3 > 1 ))) {
                m = conflict_table[order_table[j]];
                index = j;
            }

        }
        tmp = order_table[index];
        order_table[index] = order_table[i];
        order_table[i] = tmp;
    }
    // backtracking, ref : figure 5.3, Artificial Intelligence : A Modern Approach
    for(int i = 0;i < count;i++){
        board[pos] = order_table[i];
        //board[pos] = i;
        if(is_valid(board , pos)){
            if(eight_queen(board, pos + 1)){
                return true;
            }
        }
    }
    return false;
}

/* add solution to answer list */
void add_result(int board[]){
    for(int c = 0;c < SIZE;c++){
        LIST[INDEX][c] = board[c];
    }
    INDEX++;
}

/* check the solution is duplicated from answer list or not */
bool is_duplicated(int board[]){
    for(int i = 0;i < INDEX;i++){
        int dup = 0;
        for(int c = 0;c < SIZE;c++){
            if(board[c] == LIST[i][c]){
                dup++;
            }
        }
        if(dup == SIZE){
            return true;
        }
    }
    return false;
}

/* check state is valid or not */
bool is_valid(int board[], int pos){
    for(int i = 0;i < pos;i++){
        for(int j = i + 1;j < pos;j++){
            if(board[i] == board[j] || board[i] == board[j] - (j - i) || board[i] == board[j] + (j - i)){
                return false;
            }
        }
    }
    return true;
}

/* if there is no legal move in unassigned column, return false */
bool forward_checking(bool mem[SIZE][SIZE], int pos){

    for(int c = pos + 1;c < SIZE;c++){
        int sum = 0;
        for(int r = 0;r < SIZE;r++){
            if(mem[r][c]){
                sum++;
            }
        }
        if(sum == SIZE){
            return false;
        }
    }

    return true;
}

/* minimum remaining value heuristic */
int remain_value(int board[], bool mem[SIZE][SIZE], int pos){
    int sum = 0, i, j;

    for(i = 0;i < SIZE;i++){
        for(j = 0;j < SIZE;j++){
            mem[i][j] = false;
        }
    }
    // mark every position that cannot put queen
    for(i = 0;i <= pos;i++){
        mem[board[i]][i] = true;
        for(j = 1;i + j < SIZE;j++){
            mem[board[i]][i + j] = true;
            if(board[i] + j < SIZE){
                mem[board[i] + j][i + j] = true;
            }
            if(board[i] - j >= 0){
                mem[board[i] - j][i + j] = true;
            }
        }
    }
    // summation of the mark
    for(i = 0;i < SIZE;i++){
        for(j = 0;j < SIZE;j++){
            if(mem[i][j] == true){
                sum++;
            }
        }
    }
    return sum;
}

int main(){
    int board[SIZE] = {0}, ans = 0, sum = 0;
    INDEX = 0;

    srand(time(NULL));
    while(INDEX < ANS_LIMIT){
        NODE = 0;
        eight_queen(board, 0);
    }
    return 0;

}
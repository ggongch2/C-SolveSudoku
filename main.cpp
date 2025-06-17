#include <iostream>
#include <vector> 
#include <algorithm>
#include <set>

using namespace std; 


int get_block(int x, int y) ;

bool solveSudoku(vector <vector<int>> sudokuMap) ; 

bool pairCompare(pair <pair<int, int>, int> a, pair <pair<int, int>, int> b) ;


int main()
{
    // 블록이 어딘지 찾아야함
    vector <vector<int>> sudoku(9, vector(9, 0)) ; 
    
    //sudokuPosNumVecs은 가능성 있는 수 
    vector <vector<vector<bool>>> sudokuPosNumVecs(9, vector<vector<bool>>(9, vector<bool>(10, true)));
    
    //각각의 숫자마다 존재하는 위치 
    vector < set < pair<int, int>>> numLocation(10) ; 
    
    int num ; 
    int remain = 81 ;
    
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            cin>>num ;
            if(num == 0) continue ; 
            sudoku[i][j] = num ;
            for(int k = 0 ; k < 10 ; k++){
                if(k == num) continue ;
                sudokuPosNumVecs[i][j][k] = false ; 
            }
            numLocation[num].emplace(i, j) ; 
            remain -- ; 
        }
    }

    cout<<"-----------------------------------------------------------------"<<"\n" ;
    solveSudoku(sudoku) ;
    cout<<"-----------------------------------------------------------------"<<"\n" ;
    
    for(auto row : sudoku){
        for(auto elem : row){
            cout<<elem<<" " ;
        }
        cout<<"\n" ; 
    }
    return 0 ; 
    
}

int get_block(int x, int y){
    
    if(0 <= x && x <= 2){
        if(0 <= y && y <= 2){
            return 1 ; 
        }
        else if(3 <= y && y <= 5){
            return 2 ;
        }
        else if(6 <= y && y <= 8){
            return 3 ;
        }
    }
    
    else if (3 <= x && x <= 5){
        if(0 <= y && y <= 2){
            return 4 ; 
        }
        else if(3 <= y && y <= 5){
            return 5 ;
        }
        else if(6 <= y && y <= 8){
            return 6 ;
        }
    }
    
    else{
        if(0 <= y && y <= 2){
            return 7 ; 
        }
        else if(3 <= y && y <= 5){
            return 8 ;
        }
        else if(6 <= y && y <= 8){
            return 9 ;
        }
    }
}

bool pairCompare(pair <pair<int, int>, int> a, pair <pair<int, int>, int> b){
    
    return a.second < b.second ; 
    
}


bool solveSudoku(vector <vector<int>> sudokuMap){
    
    // # sudokuPosNumVecs 생성 부분 #
    
    //sudokuPosNumVecs은 가능성 있는 수 
    vector <vector<vector<bool>>> sudokuPosNumVecs(9, vector<vector<bool>>(9, vector<bool>(10, true)));
    
    //각각의 숫자마다 존재하는 위치 
    vector < set < pair<int, int>>> numLocation(10) ; 
    
    int num ; 
    int remain = 81 ;

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            num = sudokuMap[i][j] ; 
            if(num == 0) continue ; 
            for(int k = 1 ; k <= 9 ; k++){
                if(k == num) continue ;
                sudokuPosNumVecs[i][j][k] = false ; 
            }
            numLocation[num].emplace(i, j) ; 
            remain -- ; 
        }
    }
    
    int prevRemain = remain ; 
    
    do{
        prevRemain = remain ;

        // ## logic 부분 ##
            // 같은 줄/칸에 있는숫자  sudokuPosNumVecs 지우기
        for(int i = 1 ; i <= 9 ; i++){
            for(auto it = numLocation[i].begin() ; it != numLocation[i].end() ; it++){
                    
                    //같은 줄(가로/세로) 없애는거
                    int row = it->first ;
                    int col = it->second ;
                    for(int r1 = 0 ; r1 < 9 ; r1++){
                        for(int c1 = 0 ; c1 < 9 ; c1++){
                            if(r1 == row && c1 == col) continue ;
                            if(c1 == col || r1 == row) sudokuPosNumVecs[r1][c1][i] = false ; 
                        }
                    }
                    //같은 칸 없애기
                    int block_loc = get_block(row, col) ; 
                    for(int r1 = 0 ; r1 < 9 ; r1++){
                        for(int c1 = 0 ; c1 < 9 ; c1++){
                            if(r1 == row && c1 == col) continue ;
                            if(get_block(r1, c1) == block_loc) sudokuPosNumVecs[r1][c1][i] = false ; 
                        }
                    }
            }
        }
        
        //같은 칸에 있는 것중 특정한 수 (6) 이런거에 대해 혼자만 sudokuPosNumVecs[6]이 1인경우도 마찬가지
            for(int block = 1 ; block <= 9 ; block ++){
            
                for(int pNum = 1 ; pNum <= 9 ; pNum++){
                    
                    int posCount = 0 ;
                    int posIdx = 0 ;  
                    pair <int, int> p ; 
                    
                    
                    for(int row = 0 ; row < 9 ; row++){
                        for(int col = 0 ; col < 9 ; col++){
                            if(get_block(row, col) == block){
                                //같은 block에 대해서만 sudokuPosNumVecs 숫자 중 혼자만 1인거는 1로 처리
                                if(sudokuPosNumVecs[row][col][pNum] == 1){
                                    posIdx = pNum ; 
                                    posCount++ ;
                                    p.first = row ;
                                    p.second = col ; 
                                }
                            }
                        }
                    }
                    if(posCount == 1){
                            sudokuMap[p.first][p.second] = posIdx ;  
                            auto check = numLocation[posIdx].emplace(p.first, p.second) ; 
                            if(check.second){
                                remain -- ;
                            }
                            
                            for(int p1 = 1 ; p1 <= 9 ; p1++){
                                if(p1 == posIdx) continue ;
                                sudokuPosNumVecs[p.first][p.second][p1] = false ;
                            }
                    }
                }    
            }
            
            //같은 줄에 있는 것에 대해서도 (row, col 모두) 동일하게 
            
            //col에 대해서
            for(int row = 0 ; row < 9 ; row++){
                
                for(int pNum = 1 ; pNum <= 9 ;pNum++){
                    
                    int posCount = 0 ;
                    int posIdx = 0 ;
                    pair <int, int> p ; 
                    
                    for(int col = 0 ; col < 9 ; col++){
                        if(sudokuPosNumVecs[row][col][pNum] == 1){
                            posIdx = pNum ; 
                            posCount++ ;
                            p.first = row ;
                            p.second = col ; 
                        }
                    }
                    if(posCount == 1){
                            sudokuMap[p.first][p.second] = posIdx ;  
                            auto check = numLocation[posIdx].emplace(p.first, p.second) ; 
                            if(check.second){
                                remain -- ;
                            } 
                            
                            for(int p1 = 1 ; p1 <= 9 ; p1++){
                                if(p1 == posIdx) continue ;
                                sudokuPosNumVecs[p.first][p.second][p1] = false ;
                            }
                    }
                }
            }
            
            //row에 대해서
            for(int col = 0 ; col < 9 ; col++){
                
                for(int pNum = 1 ; pNum <= 9 ;pNum++){
                    
                    int posCount = 0 ;
                    int posIdx = 0 ;
                    pair <int, int> p ; 
                    
                    for(int row = 0 ; row < 9 ; row++){
                        if(sudokuPosNumVecs[row][col][pNum] == 1){
                            posIdx = pNum ; 
                            posCount++ ;
                            p.first = row ;
                            p.second = col ; 
                        }
                    }
                    if(posCount == 1){
                            sudokuMap[p.first][p.second] = posIdx ;  
                            auto check = numLocation[posIdx].emplace(p.first, p.second) ; 
                            if(check.second){
                                remain -- ;                               
                            } 
                            
                            for(int p1 = 1 ; p1 <= 9 ; p1++){
                                if(p1 == posIdx) continue ;
                                sudokuPosNumVecs[p.first][p.second][p1] = false ;
                            }
                    }
                }
            }
            
        // 유효하지 않은 경우 
        for(int i = 0 ; i < 9 ; i++){
            for(int j = 0 ; j < 9 ; j++){
                int posCount = 0 ;
                for(int k = 1 ; k<= 9 ; k++){
                    if(sudokuPosNumVecs[i][j][k] == true) posCount ++ ;
                }
                if(posCount == 0){
                    return false ; 
                }
            }
        }

    }while(prevRemain > remain) ; 
    

    if(remain == 0){
        for(auto row : sudokuMap){
            for(auto elem : row){
                cout<<elem<<" " ;
            }
        cout<<"\n" ; 
        }
        return true ; 
    }
    
    // dfs 부분 
    
    // 찍는 경우를 줄여보기 -> 지금까지 나온 숫자 중 제일 많은것(9개 제외)를 포함하는 아직 미정된 위치 찾아보기
    
    // numLoc 중 제일 많은 것을 한번 골라보기 
    int numLocMaxCount = 0 ;
    int numLocMaxIdx = 0 ;
    for(int i = 1 ; i <= 9 ; i++){
        if(numLocation[i].size() > numLocMaxCount){
            if(numLocation[i].size() == 9) continue ;
            numLocMaxCount = numLocation[i].size() ;
            numLocMaxIdx = i ; 
        }
    }
    
    // numLoc중 제일많은것이 존재가능한 sudokuPosNumVecs 중 제일 적은 가능성을 가지는 것 찾고 정렬  
    vector <pair <pair <int, int>, int >> posLocVecs ;
    
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            int posCount = 0 ;
            if(sudokuMap[i][j] != 0 || sudokuPosNumVecs[i][j][numLocMaxIdx] == false) continue ; 
            for(int k = 1 ; k <= 9 ; k++){
                if(sudokuPosNumVecs[i][j][k] == true) posCount ++ ;    
            }
            posLocVecs.push_back({{i, j}, poshttps://www.onlinegdb.com/myfilesCount}) ;
        }
    }
    
    sort(posLocVecs.begin(), posLocVecs.end(), pairCompare) ;
    
    
    // 가능한 경우 모두 대입해보기 (대입 순서는 posCount 가 적은것? posLocVecs에서 second값이 작은 순서 )
    
    for(auto elem : posLocVecs){
        sudokuMap[elem.first.first][elem.first.second] = numLocMaxIdx ;
        if(solveSudoku(sudokuMap) == false){
            sudokuMap[elem.first.first][elem.first.second] = 0 ; 
        } 
        else break ; 
    }
    
    return true ; 
}




/*

2 0 0 6 3 0 0 1 0       
0 5 1 0 2 0 7 9 3
4 0 3 1 9 7 5 0 0
0 0 0 0 0 9 0 3 2
0 6 5 0 7 0 1 4 0
1 3 0 8 0 0 0 0 0
0 0 9 3 6 2 4 0 7
3 7 6 0 8 0 2 5 0
0 2 0 0 5 1 0 0 9


0 7 0 0 2 0 8 0 0
0 0 0 0 0 8 0 0 7
8 6 0 0 4 0 0 2 3
0 0 0 2 0 0 5 0 0
0 0 3 7 0 9 6 0 0
0 0 6 0 0 4 0 0 0
7 1 0 0 3 0 0 6 9
9 0 0 4 0 0 0 0 0
0 0 4 0 9 0 0 7 0


0 6 0 0 7 0 4 1 0 
0 0 0 0 2 9 7 0 0 
0 7 4 0 6 0 9 0 8 
0 0 0 0 0 0 2 0 4
0 0 0 0 0 2 0 5 0 
0 1 8 7 0 0 0 0 0 
5 3 1 9 0 0 0 0 2 
0 0 0 0 0 0 5 0 0 
0 4 0 0 5 7 0 3 1 

0 3 0 9 0 0 1 0 0
0 0 9 0 0 5 0 7 0 
6 0 0 0 0 0 0 9 0 
3 8 0 0 0 0 0 0 0 
0 0 0 2 1 0 0 0 6 
0 0 0 0 0 0 0 0 4 
0 0 4 5 0 0 7 0 2 
0 0 7 0 0 9 0 8 0 
0 0 0 0 0 7 0 0 0


*/



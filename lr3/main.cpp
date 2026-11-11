#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <sstream>
#include <fstream>

//#define DEBUG

using namespace std;

vector<vector<double>> intersect(const vector<vector<double>>& matrix1,const vector<vector<double>>& matrix2){
    vector<vector<double>>ans = matrix1;
    for (int i = 0; i < matrix1.size(); i++)
    {
        for (int j = 0; j < matrix1[0].size(); j++)
        {
            ans[i][j] = min(ans[i][j],matrix2[i][j]);
        }
        
    }
    return ans;
    
}
vector<vector<double>> Q2(const vector<vector<double>>& matrix,double l){
    vector<vector<double>>ans = matrix;
    for (auto &r : ans) {
        for (auto &el : r) {
            el *= l;
        }
    }
    
    return ans;
    
}

vector<vector<double>> summ(const vector<vector<double>>& matrix1,const vector<vector<double>>& matrix2){
    vector<vector<double>>ans = matrix1;
    for (int i = 0; i < ans.size(); i++)
    {
        for (int j = 0; j < matrix1[0].size(); j++)
        {
            ans[i][j]+=matrix2[i][j];
        }
    }
    return ans;
}


vector<vector<double>> FSPR(vector<vector<double>> Q){
    vector<vector<double>> ans = Q;
    for (int i = 0; i < ans.size(); i++)
    {
        for (int j = 0; j < ans[0].size(); j++)
        {
            ans[i][j] = max(0.0,Q[i][j]-Q[j][i]);
        }
        
    }
    return ans;
}
vector<double> mu(vector<vector<double>> Q){
    vector<double> ans(Q.size());
    for (int i = 0; i < Q[0].size(); i++)
    {
        double tmp = 0;
        for (int j = 0; j < Q.size(); j++)
        {
            tmp = max(tmp,Q[j][i]);
        }
        ans[i]=1-tmp;
    }
    return ans;
    
}
void solve() {
    ostringstream out;
    out.precision(10);

    // Вариант 4 (task_lubenets/prun/LR-3.pdf)
    vector<vector<double>> L = {{0.1, 0.2, 0.1, 0.1, 0.3, 0.1, 0.1}};
    vector<vector<double>> R1 = {
        {1, 0.3, 0.8, 0.5, 0.5, 0.6},
        {0.4, 1, 0.4, 0.9, 0.2, 0.2},
        {0.7, 0.7, 1, 0.8, 0.7, 0.7},
        {0.1, 0.3, 0.7, 1, 0.4, 0.7},
        {0.8, 0.9, 0.7, 0.4, 1, 0.4},
        {0.8, 0.4, 0.2, 0.3, 0.2, 1}
    };
    vector<vector<double>> R2 = {
        {1, 0.8, 0.1, 0.6, 0.9, 0.3},
        {0.4, 1, 0.5, 0.3, 0.7, 0.4},
        {0.2, 0.7, 1, 0.7, 0.7, 0.6},
        {0.7, 0.8, 0.8, 1, 0.3, 0.3},
        {0.3, 0.3, 0.6, 0.6, 1, 0.2},
        {0.3, 0.6, 0.8, 0.6, 0.1, 1}
    };
    vector<vector<double>> R3 = {
        {1, 0.4, 0.7, 0.6, 0.3, 0.3},
        {0.3, 1, 0.3, 0.1, 0.5, 0.7},
        {0.3, 0.8, 1, 0.3, 0.9, 0.2},
        {0.2, 0.7, 0.3, 1, 0.7, 0.8},
        {0.3, 0.8, 0.8, 0.6, 1, 0.4},
        {0.4, 0.7, 0.2, 0.9, 0.4, 1}
    };
    vector<vector<double>> R4 = {
        {1, 0.6, 0.3, 0.7, 0.7, 0.2},
        {0.8, 1, 0.7, 0.5, 0.3, 0.3},
        {0.8, 0.1, 1, 0.7, 0.1, 0.7},
        {0.3, 0.8, 0.6, 1, 0.2, 0.2},
        {0.5, 0.5, 0.8, 0.7, 1, 0.2},
        {0.8, 0.5, 0.2, 0.6, 0.1, 1}
    };
    vector<vector<double>> R5 = {
        {1, 0.2, 0.5, 0.5, 0.5, 0.2},
        {0.6, 1, 0.6, 0.1, 0.8, 0.9},
        {0.7, 0.8, 1, 0.3, 0.8, 0.4},
        {0.4, 0.6, 0.6, 1, 0.4, 0.8},
        {0.2, 0.9, 0.1, 0.2, 1, 0.7},
        {0.4, 0.8, 0.3, 0.8, 0.5, 1}
    };
    vector<vector<double>> R6 = {
        {1, 0.7, 0.7, 0.4, 0.6, 0.4},
        {0.2, 1, 0.2, 0.4, 0.1, 0.4},
        {0.3, 0.3, 1, 0.8, 0.2, 0.5},
        {0.8, 0.5, 0.7, 1, 0.8, 0.3},
        {0.7, 0.3, 0.6, 0.6, 1, 0.9},
        {0.4, 0.7, 0.6, 0.7, 0.8, 1}
    };
    vector<vector<double>> R7 = {
        {1, 0.7, 0.8, 0.8, 0.3, 0.7},
        {0.2, 1, 0.7, 0.5, 0.6, 0.8},
        {0.5, 0.8, 1, 0.8, 0.7, 0.9},
        {0.3, 0.9, 0.6, 1, 0.4, 0.9},
        {0.4, 0.5, 0.4, 0.7, 1, 0.1},
        {0.7, 0.3, 0.4, 0.8, 0.6, 1}
    };

    // Q1 — пересечение матриц парных сравнений
    vector<vector<double>> Q = R1;
    for (auto *m : {&R2, &R3, &R4, &R5, &R6, &R7}) {
        Q = intersect(Q, *m);
    }

    out<<"Q1\n";
    for(auto a:(Q)){
        for(auto aa :a){
            out<<aa<<" ";
        }
        out<<"\n";
    }

    out<<"\n Q1s\n";
    for(auto a:FSPR(Q)){
        for(auto aa :a){
            out<<aa<<" ";
        }
        out<<"\n";
    }

    out<<"\nmyu н.д.Q1 ";
    // степени недоминируемости по Q1
    vector<double> m1 = mu(FSPR(Q));
    for(auto a:m1){
        out<<a<<" ";
    }
    // Q2 — свёртка матриц с весами критериев
    vector<vector<double>> Q_2 = Q2(R1,L[0][0]);
    out<<"\n"<<L[0][0]<<"\n";

    for (auto &m : {std::pair{&R2, L[0][1]}, std::pair{&R3, L[0][2]},
                    std::pair{&R4, L[0][3]}, std::pair{&R5, L[0][4]},
                    std::pair{&R6, L[0][5]}, std::pair{&R7, L[0][6]}}) {
        Q_2 = summ(Q_2,Q2(*m.first, m.second));
    }

    out<<"\nQ2\n";
    for(auto a:Q_2){
        for(auto aa :a){
            out<<aa<<" ";
        }
        out<<"\n";
    }

    out<<"\n Q2s\n";
    for(auto a:FSPR(Q_2)){
        for(auto aa :a){
            out<<aa<<" ";
        }
        out<<"\n";
    }

    out<<"\nmyu н.д.Q2 ";
    vector<double> m2 = mu(FSPR(Q_2));
    for(auto a:m2){
        out<<a<<" ";
    }
    // берём альтернативу с лучшим минимумом из двух оценок
    int ind = [&]{ int k = 0; for (int i = 1; i < (int)m1.size(); ++i) if (std::min(m1[i],m2[i]) > std::min(m1[k],m2[k])) k = i; return k; }();

    out << "\n\nОтвет: оптимальная альтернатива x"<<ind+1<<", "<<"мю(x"<<ind+1<<")"<<" = "<<min(m1[ind],m2[ind])<<"\n";

    cout << out.str();
    ofstream fout("output.txt");
    fout << out.str();
}

#ifdef DEBUG
#define HELLO cout << "******DEBUG******\n";
void debug() {
    HELLO;
}
#endif

signed main() {
    solve();

#ifdef DEBUG
    debug();
#endif // !DEBUG

    system("pause");
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <numeric>

//#define DEBUG

using namespace std;

void print_matrix(const vector<vector<double>> &vctr) {
    for (auto r : vctr) {
        for (auto n : r) {
            cout << n << " ";
        }
        cout << '\n';
    }
}

void read_matrix(vector<vector<double>> *vctr) {
    vctr->clear();

    string s;
    while (getline(cin, s)) {
        if (!s.empty() && s.back() == '\r') s.pop_back();

        if (s.find_first_not_of(" \t") == string::npos) {
            if (!vctr->empty()) return;
            else continue;
        }

        if (!s.empty() && s[0] == ':') continue;

        replace(s.begin(), s.end(), ',', '.');

        vctr->push_back(vector<double>());

        stringstream ss(s);
        string token;
        while (ss >> token) {
            size_t slash = token.find('/');
            if (slash != string::npos) {
                double num = stod(token.substr(0, slash));
                double den = stod(token.substr(slash + 1));
                vctr->back().push_back(num / den);
            } else {
                vctr->back().push_back(stod(token));
            }
        }
    }
}
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
    freopen("input.txt", "r", stdin);
    cout.precision(10);

    vector<vector<double>> L;read_matrix(&L);
    vector<vector<double>> R1;read_matrix(&R1);
    vector<vector<double>> R2;read_matrix(&R2);
    vector<vector<double>> R3;read_matrix(&R3);
    vector<vector<double>> R4;read_matrix(&R4);
    vector<vector<double>> R5;read_matrix(&R5);
    vector<vector<double>> R6;read_matrix(&R6);
    vector<vector<double>> R7;read_matrix(&R7);

    vector<vector<double>> Q = R1;
    for (auto *m : {&R2, &R3, &R4, &R5, &R6, &R7}) {
        Q = intersect(Q, *m);
    }

    cout<<"Q1\n";
    for(auto a:(Q)){
        for(auto aa :a){
            cout<<aa<<" ";
        }
        cout<<"\n";
    }

    cout<<"\n Q1s\n";
    for(auto a:FSPR(Q)){
        for(auto aa :a){
            cout<<aa<<" ";
        }
        cout<<"\n";
    }

    cout<<"\nmyu н.д.Q1 ";
    vector<double> m1 = mu(FSPR(Q));
    for(auto a:m1){
        cout<<a<<" ";
    }
    vector<vector<double>> Q_2 = Q2(R1,L[0][0]);
    cout<<"\n"<<L[0][0]<<"\n";

    for (auto &m : {std::pair{&R2, L[0][1]}, std::pair{&R3, L[0][2]},
                    std::pair{&R4, L[0][3]}, std::pair{&R5, L[0][4]},
                    std::pair{&R6, L[0][5]}, std::pair{&R7, L[0][6]}}) {
        Q_2 = summ(Q_2,Q2(*m.first, m.second));
    }

    cout<<"\nQ2\n";
    for(auto a:Q_2){
        for(auto aa :a){
            cout<<aa<<" ";
        }
        cout<<"\n";
    }

    cout<<"\n Q2s\n";
    for(auto a:FSPR(Q_2)){
        for(auto aa :a){
            cout<<aa<<" ";
        }
        cout<<"\n";
    }

    cout<<"\nmyu н.д.Q2 ";
    vector<double> m2 = mu(FSPR(Q_2));
    for(auto a:m2){
        cout<<a<<" ";
    }
    int ind = [&]{ int k = 0; for (int i = 1; i < (int)m1.size(); ++i) if (std::min(m1[i],m2[i]) > std::min(m1[k],m2[k])) k = i; return k; }();

    cout << "\n\nОтвет: оптимальная альтернатива x"<<ind+1<<", "<<"мю(x"<<ind+1<<")"<<" = "<<min(m1[ind],m2[ind])<<"\n";
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
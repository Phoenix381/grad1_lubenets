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
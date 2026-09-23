#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <algorithm>

//#define DEBUG

using namespace std;

bool is_digit(char ch) {
    return ch <= 57 && ch >= 48;
}
void print_matrix(const vector<vector<double>> &vctr){
    for(auto r:vctr){
        for(auto n:r){
            cout<<n<<" ";
        }
        cout<<'\n';
    }
}
void read_matrix(vector<vector<double>> *vctr) {
    vctr->clear();

    string s;
    while (getline(cin, s)) {
        if (s.find_first_not_of(" \t\r") == string::npos) {
            if (!vctr->empty()) return;
            else continue;
        }
        
        if (!s.empty() && s[0] == ':') continue;

        (*vctr).push_back(vector<double>());

        vector<char> curr;
        bool is_dev = false;

        for (int i = 0; i < (int)s.size(); i++)
        {
            if (is_digit(s[i]) || s[i] == '.') curr.push_back(s[i]);

            if (s[i] == '/') {
                (*vctr).back().push_back(stoi(string(curr.begin(), curr.end())));
                is_dev = true;
                curr.clear();
                continue;
            }

            if (s[i] == ' ' || i == (int)s.size() - 1) {
                if (curr.empty()) { is_dev = false; continue; }
                if (is_dev) {
                    is_dev = false;
                    (*vctr).back().back() /= stoi(string(curr.begin(), curr.end()));
                }
                else {
                    (*vctr).back().push_back(stoi(string(curr.begin(), curr.end())));
                }
                curr.clear();
            }
        }
    }
}

void norm(vector<double>& out) {
    double s = accumulate(out.begin(), out.end(), 0.0);
    if (s == 0.0) return;
    for (double& x : out) x /= s;
}

void calc_priorities(const vector<vector<double>> &matrix,vector<double> *out) {
    (*out).clear();
    for (int i = 0; i < matrix.size(); i++)
    {
        double product = accumulate(matrix[i].begin(), matrix[i].end(), 1.0, multiplies<double>());
        (*out).push_back(pow(product,1.0/matrix.size()));
    }
    norm(*out);
}

void print_ans(const vector<double> &ans){
    freopen("output.txt", "w", stdout);
    auto el = (int)(max_element(ans.begin(),ans.end()) - ans.begin())+1;
    cout<<"оптимальная альтернатива x"<<el<<" с приоритетом w"<<el<<" ="<<ans[el-1]<<".";
}

void solve() {
    freopen("input.txt", "r", stdin);
    cout.precision(10);
    vector<vector<double>> E;
    read_matrix(&E);
    cout << "E:\n";
    print_matrix(E);

    vector<vector<double>> W[7];
    for (int i = 0; i < 7; i++) {
        read_matrix(&W[i]);
        cout << "W" << i + 1 << ":\n";
        print_matrix(W[i]);
    }

    vector<double> e;
    calc_priorities(E, &e);
    cout << "\n~e:\n";
    for (auto a : e) cout << a << " ";
    cout << "\n";

    vector<vector<double>> w(7);
    for (int i = 0; i < 7; i++) {
        calc_priorities(W[i], &w[i]);
        cout << "~w" << i + 1 << "\n";
        for (auto a : w[i]) cout << a << " ";
        cout << "\n";
    }

    size_t n = w[0].size();
    vector<double> result(n, 0.0);
    for (size_t i = 0; i < e.size(); i++)
        for (size_t j = 0; j < n; j++)
            result[j] += e[i] * w[i][j];

    cout << "\nПриоритеты:\n";
    for (size_t j = 0; j < n; j++)
        cout << "w " << j + 1 << ": " << result[j] << "\n";

    print_ans(result);
}

#ifdef DEBUG
#define HELLO cout << "******DEBUG******\n";
void debug() {
    HELLO;
}
#endif

signed main(){
    solve();
    
#ifdef DEBUG
    debug();
#endif // !DEBUG

    system("pause");
    return 0;
}
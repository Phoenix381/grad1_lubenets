#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
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
void print_matrix(ostream &out, const vector<vector<double>> &vctr){
    for(auto r:vctr){
        for(size_t j = 0; j < r.size(); ++j){
            if(j) out<<" ";
            out<<r[j];
        }
        out<<'\n';
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

string decimal_comma(double value, int precision) {
    ostringstream ss;
    ss << fixed << setprecision(precision) << value;
    string s = ss.str();
    replace(s.begin(), s.end(), '.', ',');
    return s;
}

void solve() {
    freopen("input.txt", "r", stdin);

    ostringstream report;
    report.precision(10);

    // матрица парных сравнений критериев
    vector<vector<double>> E;
    read_matrix(&E);
    report << "E:\n";
    print_matrix(report, E);

    vector<vector<double>> W[7];
    for (int i = 0; i < 7; i++) {
        read_matrix(&W[i]);
        report << "W" << i + 1 << ":\n";
        print_matrix(report, W[i]);
    }

    vector<double> e;
    calc_priorities(E, &e);
    report << "\n~e:\n";
    for (size_t j = 0; j < e.size(); ++j) {
        if (j) report << " ";
        report << e[j];
    }
    report << "\n";

    vector<vector<double>> w(7);
    for (int i = 0; i < 7; i++) {
        calc_priorities(W[i], &w[i]);
        report << "~w" << i + 1 << "\n";
        for (size_t j = 0; j < w[i].size(); ++j) {
            if (j) report << " ";
            report << w[i][j];
        }
        report << "\n";
    }

    size_t n = w[0].size();
    vector<double> result(n, 0.0);
    for (size_t i = 0; i < e.size(); i++)
        for (size_t j = 0; j < n; j++)
            result[j] += e[i] * w[i][j];

    report << "\nПриоритеты:\n";
    for (size_t j = 0; j < n; j++)
        report << "w " << j + 1 << ": " << result[j] << "\n";

    auto el = (size_t)(max_element(result.begin(), result.end()) - result.begin());
    report << "\nОтвет: оптимальная альтернатива x" << el + 1
           << " с приоритетом w" << el + 1 << " = "
           << decimal_comma(result[el], 6) << ".\n";

    cout << report.str();

    ofstream fout("output.txt");
    fout << report.str();
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
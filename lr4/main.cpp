// ⣿⣿⣿⣿⣿⣿⣿⣿⠟⠛⢉⢉⠉⠉⠻⣿
// ⣿⣿⣿⣿⣿⣿⠟⠠⡰⣕⣗⣷⣧⣀⣅⠘⣿
// ⣿⣿⣿⣿⣿⠃⣠⣳⣟⣿⣷⣿⡿⣜⠄⣿
// ⣿⣿⣿⡿⠁⠄⣳⢷⣿⣿⣿⣿⡿⣝⠖⠄⣿
// ⣿⣿⣿⠃⠄⢢⡹⣿⢷⣯⢿⢷⡫⣗⠍⢰⣿
// ⣿⣿⡏⢀⢄⠤⣁⠋⠿⣗⣟⡯⡏⢎⠁⢸⣿
// ⣿⣿⠄⢔⢕⣯⣿⣿⡲⡤⡄⡤⠄⡀⢠⣿
// ⣿⠇⠠⡳⣯⣿⣿⣾⢵⣫⢎⢎⠆⢀⣿
// ⣿⠄⢨⣫⣿⣿⡿⣿⣻⢎⡗⡕⡅⢸⣿
// ⣿⠄⢜⢾⣾⣿⣿⣟⣗⢯⡪⡳⡀⢸⣿
// ⣿⠄⢸⢽⣿⣷⣿⣻⡮⡧⡳⡱⡁⢸⣿
// ⣿⡄⢨⣻⣽⣿⣟⣿⣞⣗⡽⡸⡐⢸⣿
// ⣿⡇⢀⢗⣿⣿⣿⣿⡿⣞⡵⡣⣊⢸⣿
// ⣿⣿⡀⡣⣗⣿⣿⣿⣿⣯⡯⡺⣼⠎⣿
// ⣿⣿⣧⠐⡵⣻⣟⣯⣿⣷⣟⣝⢞⡿⢹⣿
// ⣿⣿⣿⡆⢘⡺⣽⢿⣻⣿⣗⡷⣹⢩⢃⢿⣿
// ⣿⣿⣿⣷⠄⠪⣯⣟⣿⢯⣿⣻⣜⢎⢆⠜⣿
// ⣿⣿⣿⣿⡆⠄⢣⣻⣽⣿⣿⣟⣾⡮⡺⡸⠸⣿
// ⣿⡿⠛⠉⠁⠄⢕⡳⣽⡾⣿⢽⣯⡿⣮⢚⣅⠹⣿
// ⣿⠂⠈⠐⢕⡳⣽⣾⣿⣿⣿⢽⣯⡿⣮⢚⣅⣿
// ⣿⣷⣤⣀⠀⠈⠙⣽⣾⣿⣿⣿⣿⣯⡿⡮⡺⣿
// ⣿⣿⣿⣷⣦⣀⡀⠀⠉⠻⣿⣿⣿⣿⣟⣾⡿⣿
// ⣿⣿⣿⣿⣿⣷⣦⣄⣀⡀⠀⠉⠛⠿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣷⣦⣤⣤⣀⣀⣤⣤⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿

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

double calc_concordance(const vector<vector<double>>& vctr) {
    int m = vctr.size();
    int n = vctr[0].size();
    
    vector<double> sum_ranks(n, 0.0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            sum_ranks[j] += vctr[i][j];
        }
    }
    
    double avg = m * (n + 1) / 2.0;
    double S = 0.0;
    for (int j = 0; j < n; j++) {
        S += (sum_ranks[j] - avg) * (sum_ranks[j] - avg);
    }
    
    return (12.0 * S) / (m * m * (n * n * n - n));
}

void solve() {
    freopen("input.txt", "r", stdin);
    cout.precision(10);

    vector<vector<double>> E;
    read_matrix(&E);

    double R_all = calc_concordance(E);
    cout << "R = " << R_all << "\n";

    double max_R = -1;
    int best_expert = -1;
    
    for (int skip = 0; skip < (int)E.size(); skip++) {
        vector<vector<double>> E_excl;
        for (int i = 0; i < (int)E.size(); i++) {
            if (i != skip) {
                E_excl.push_back(E[i]);
            }
        }
        
        double R_i = calc_concordance(E_excl);
        cout << "R" << skip + 1 << " = " << R_i << "\n";
        
        if (R_i > max_R) {
            max_R = R_i;
            best_expert = skip + 1;
        }
    }

    cout << "\nОтвет: коэффициент конкордации всей группы R = " << R_all
         << ", наиболее согласованная группа – без эксперта e" << best_expert
         << ", R" << best_expert << " = " << max_R << "\n";
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
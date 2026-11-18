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
#include <numeric>

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

// Печать матрицы в формате принятого примера:
// строки x1..xN, затем max по столбцу и степени недоминируемости mu.
void print_table(ostream& out, const string& title,
                 const vector<vector<double>>& Q,
                 const vector<double>& mu_row) {
    out << title << "\n";
    out << "\t";
    int cols = (int)Q[0].size();
    for (int j = 0; j < cols; ++j) {
        out << "x" << j + 1;
        if (j + 1 < cols) out << "\t";
    }
    out << "\n";

    for (int i = 0; i < (int)Q.size(); ++i) {
        out << "x" << i + 1 << "\t";
        for (int j = 0; j < cols; ++j) {
            out << fixed << setprecision(2) << Q[i][j];
            if (j + 1 < cols) out << "\t";
        }
        out << "\n";
    }

    out << "max\t";
    for (int j = 0; j < cols; ++j) {
        double mx = 0;
        for (int i = 0; i < (int)Q.size(); ++i)
            mx = max(mx, Q[i][j]);
        out << fixed << setprecision(2) << mx;
        if (j + 1 < cols) out << "\t";
    }

    out << "\nmu\t";
    for (int j = 0; j < cols; ++j) {
        out << fixed << setprecision(2) << mu_row[j];
        if (j + 1 < cols) out << "\t";
    }
    out << "\n\n";
}

void solve() {
    cout.precision(10);

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

    vector<vector<double>> Q = R1;
    for (auto *m : {&R2, &R3, &R4, &R5, &R6, &R7}) {
        Q = intersect(Q, *m);
    }

    vector<vector<double>> Q1s = FSPR(Q);
    vector<double> m1 = mu(Q1s);

    vector<vector<double>> Q_2 = Q2(R1, L[0][0]);
    for (auto &m : {std::pair{&R2, L[0][1]}, std::pair{&R3, L[0][2]},
                    std::pair{&R4, L[0][3]}, std::pair{&R5, L[0][4]},
                    std::pair{&R6, L[0][5]}, std::pair{&R7, L[0][6]}}) {
        Q_2 = summ(Q_2, Q2(*m.first, m.second));
    }

    vector<vector<double>> Q2s = FSPR(Q_2);
    vector<double> m2 = mu(Q2s);

    vector<double> mu_total(m1.size());
    for (int j = 0; j < (int)m1.size(); ++j)
        mu_total[j] = min(m1[j], m2[j]);

    int ind = 0;
    for (int i = 1; i < (int)mu_total.size(); ++i)
        if (mu_total[i] > mu_total[ind]) ind = i;

    ostringstream report;
    print_table(report, "Q1 s:", Q1s, m1);
    print_table(report, "Q2 s:", Q2s, m2);

    report << "\t\t";
    int cols = (int)mu_total.size();
    for (int j = 0; j < cols; ++j) {
        report << "x" << j + 1;
        if (j + 1 < cols) report << "\t\t";
    }
    report << "\nmu Q1\t";
    for (int j = 0; j < cols; ++j) {
        report << fixed << setprecision(2) << m1[j];
        if (j + 1 < cols) report << "\t";
    }
    report << "\nmu Q2\t";
    for (int j = 0; j < cols; ++j) {
        report << fixed << setprecision(2) << m2[j];
        if (j + 1 < cols) report << "\t";
    }
    report << "\nmu\t\t";
    for (int j = 0; j < cols; ++j) {
        report << fixed << setprecision(2) << mu_total[j];
        if (j + 1 < cols) report << "\t";
    }

    report << "\n\nОтвет: оптимальная альтернатива - x" << ind + 1
           << ", mu(x" << ind + 1 << ") = "
           << fixed << setprecision(2) << mu_total[ind] << ".\n";

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

signed main() {
    solve();

#ifdef DEBUG
    debug();
#endif // !DEBUG

    system("pause");
    return 0;
}
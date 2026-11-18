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

// Дополняет матрицу строками максимумов по столбцам и степеней недоминируемости
vector<vector<double>> withMaxMu(const vector<vector<double>>& Q) {
    vector<vector<double>> M = Q;
    int n = Q.size();
    int m = Q[0].size();

    vector<double> maxRow(m), muRow(m);
    for (int j = 0; j < m; j++)
    {
        double mx = 0;
        for (int i = 0; i < n; i++)
            mx = max(mx, Q[i][j]);

        maxRow[j] = mx;             // максимум по столбцу
        muRow[j] = 1 - mx;          // степень недоминируемости
    }

    M.push_back(maxRow);
    M.push_back(muRow);
    return M;
}

// Вывод матрицы в формате эталонной программы: шапка x1..xn, строки x1..xn, max, mu
void printMatrix(const vector<vector<double>>& M, ostream& out) {
    int n = M.size();
    int m = M[0].size();

    out << "\t";
    for (int j = 0; j < m; j++)
        out << "x" << j + 1 << "\t\t";
    out << "\n";

    for (int i = 0; i < n; i++)
    {
        if (i == n - 2)       out << "max\t";
        else if (i == n - 1)  out << "mu\t";
        else                  out << "x" << i + 1 << "\t";

        for (int j = 0; j < m; j++)
            out << fixed << setprecision(2) << M[i][j] << "\t";
        out << "\n";
    }
    out << "\n";
}

void solve() {
    ostringstream out;

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

    // Q2 — свёртка матриц с весами критериев
    vector<vector<double>> Q_2 = Q2(R1,L[0][0]);

    for (auto &m : {std::pair{&R2, L[0][1]}, std::pair{&R3, L[0][2]},
                    std::pair{&R4, L[0][3]}, std::pair{&R5, L[0][4]},
                    std::pair{&R6, L[0][5]}, std::pair{&R7, L[0][6]}}) {
        Q_2 = summ(Q_2,Q2(*m.first, m.second));
    }

    // Строгие отношения предпочтения Q1s, Q2s и степени недоминируемости по ним
    vector<vector<double>> Q1s = FSPR(Q);
    vector<vector<double>> Q2s = FSPR(Q_2);
    vector<double> m1 = mu(Q1s);
    vector<double> m2 = mu(Q2s);

    // Матрицы в формате эталонной программы: Q1s и Q2s со строками max и mu
    out << "Q1 s:\n";
    printMatrix(withMaxMu(Q1s), out);

    out << "Q2 s:\n";
    printMatrix(withMaxMu(Q2s), out);

    // Сводная таблица: степени недоминируемости по Q1, по Q2 и итоговые
    out << "\t\t";
    for (int j = 0; j < (int)m1.size(); j++)
        out << "x" << j + 1 << "\t\t";
    out << "\n";

    out << "mu Q1\t";
    for (double v : m1)
        out << fixed << setprecision(2) << v << "\t";
    out << "\n";

    out << "mu Q2\t";
    for (double v : m2)
        out << fixed << setprecision(2) << v << "\t";
    out << "\n";

    // Берём альтернативу с лучшим минимумом из двух оценок; при равенстве — первую
    double maxX = 0;
    int maxI = 0;
    out << "mu\t\t";
    for (int j = 0; j < (int)m1.size(); j++)
    {
        double v = min(m1[j], m2[j]);
        if (v > maxX)
        {
            maxX = v;
            maxI = j;
        }
        out << fixed << setprecision(2) << v << "\t";
    }

    out << "\n\nОтвет: оптимальная альтернатива - x" << maxI + 1
        << ", mu(x" << maxI + 1 << ") = " << maxX << ".\n";

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
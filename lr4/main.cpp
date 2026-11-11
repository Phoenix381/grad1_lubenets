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
#include <numeric>

//#define DEBUG

using namespace std;

// коэффициент конкордации Кендалла
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
    cout.precision(10);

    // Вариант 4 (task_lubenets/prun/LR-4.pdf)
    // ранжирования экспертов
    vector<vector<double>> E = {
        {1, 2, 3, 4, 5, 6, 10, 8, 9, 7},
        {1, 2, 6, 4, 5, 3, 7, 8, 9, 10},
        {1, 4, 2, 5, 3, 6, 7, 9, 8, 10},
        {2, 1, 3, 4, 5, 7, 6, 8, 10, 9},
        {1, 2, 3, 4, 7, 6, 5, 8, 9, 10},
        {1, 2, 3, 4, 5, 6, 8, 7, 9, 10},
        {2, 1, 6, 4, 5, 8, 7, 3, 9, 10}
    };

    double R_all = calc_concordance(E);
    cout << "R = " << R_all << "\n";

    double max_R = -1;
    int best_expert = -1;
    
    // по очереди исключаем каждого эксперта
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
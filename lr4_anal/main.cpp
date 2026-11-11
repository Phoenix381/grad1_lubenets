// ЛР4. Дискретная задача управления запасами, вариант 4.
// DP по месяцам: dp[i][k] — минимум затрат при запасе k в конце i-го месяца.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// стоимости и значения DP
using ll = long long;

// условная бесконечность
const ll INF = (ll)1e18;

// печатает z по удвоенному индексу z2: 5 -> "2.5"
string half(int z2) {
    if (z2 % 2 == 0) return to_string(z2 / 2);
    return to_string(z2 / 2) + ".5";
}

// точка входа
int main() {
    // 1. Исходные данные варианта 4

    // количество месяцев
    const int n = 4;

    // расход по месяцам (нулевой индекс пустой)
    vector<int> y(n + 1, 0);
    y[1] = 3;
    y[2] = 1;
    y[3] = 2;
    y[4] = 2;

    // начальный запас
    const int k0 = 2;

    // требуемый запас после последнего месяца
    const int kn = 0;

    // затраты на хранение: phi[2*z] = φ(z)
    vector<ll> phi = {0, 7, 14, 20, 26, 31, 36, 40, 44, 48, 51, 53, 54};

    // затраты на пополнение: psi[x] = ψ(x)
    vector<ll> psi = {0, 27, 42, 46, 48, 49, 50};

    // 2. Допустимые границы запасов

    // запас не может превышать спрос за оставшиеся месяцы
    vector<int> maxK(n + 1, 0);
    maxK[n] = kn;
    for (int i = n - 1; i >= 1; i--)
        maxK[i] = maxK[i + 1] + y[i + 1];
    maxK[0] = k0;

    // 3. Динамическое программирование

    // dp[i][k] — минимум затрат, parent — для восстановления плана
    vector<vector<ll>> dp(n + 1);
    vector<vector<int>> parent(n + 1);

    // База DP: до начала планирования запас равен k0 и затрат ещё нет.
    dp[0].assign(maxK[0] + 1, INF);
    dp[0][k0] = 0;

    // перебираем предыдущий и текущий запас, x = cur - prev + y[i]
    for (int i = 1; i <= n; i++) {
        dp[i].assign(maxK[i] + 1, INF);
        parent[i].assign(maxK[i] + 1, -1);

        for (int prev = 0; prev <= maxK[i - 1]; prev++) {
            // Недостижимое состояние пропускаем.
            if (dp[i - 1][prev] == INF) continue;

            for (int cur = 0; cur <= maxK[i]; cur++) {
                // Баланс запасов: cur = prev + x - y[i].
                int x = cur - prev + y[i];
                if (x < 0 || x >= (int)psi.size()) continue;

                // Удвоенный средний запас: z2 = 2*z.
                int z2 = prev + cur + y[i];
                if (z2 < 0 || z2 >= (int)phi.size()) continue;

                ll cost = dp[i - 1][prev] + phi[z2] + psi[x];

                // Релаксация: если нашли более дешёвый способ попасть в cur,
                // обновляем значение и запоминаем предка.
                if (cost < dp[i][cur]) {
                    dp[i][cur] = cost;
                    parent[i][cur] = prev;
                }
            }
        }
    }

    // Если конечное состояние недостижимо, допустимого плана нет.
    if (dp[n][kn] == INF) {
        cout << "Решение не найдено.\n";
        return 1;
    }

    // 4. Восстановление оптимального плана

    // запас, пополнение и средний запас по месяцам
    vector<int> planPrev(n + 1), planX(n + 1), planK(n + 1), planZ2(n + 1);

    // Идём от конечного состояния назад по массиву parent.
    int cur = kn;
    for (int i = n; i >= 1; i--) {
        int prev = parent[i][cur];
        int x = cur - prev + y[i];

        planPrev[i] = prev;
        planX[i] = x;
        planK[i] = cur;
        planZ2[i] = prev + cur + y[i];

        cur = prev;
    }
    planK[0] = k0;

    // 5. Формирование отчёта

    // собираем отчёт в строку, чтобы консоль и файл совпали
    ostringstream out;
    out << "ЛР4. Дискретная задача управления запасами. Вариант 4\n";
    out << "k0 = " << k0 << ", k" << n << " = " << kn << "\n";
    out << "Расход по месяцам: ";
    for (int i = 1; i <= n; i++) out << "y" << i << " = " << y[i] << " ";
    out << "\n\n";

    // Печать таблицы φ(z) в исходных обозначениях j(z).
    out << "Таблица j(z):\n";
    for (int z2 = 0; z2 < (int)phi.size(); z2++) {
        out << "j(" << half(z2) << ") = " << phi[z2];
        if (z2 + 1 < (int)phi.size()) out << ", ";
    }
    out << "\n\n";

    // Печать таблицы ψ(x) в исходных обозначениях y(z).
    out << "Таблица y(z):\n";
    for (int x = 0; x < (int)psi.size(); x++) {
        out << "y(" << x << ") = " << psi[x];
        if (x + 1 < (int)psi.size()) out << ", ";
    }
    out << "\n\n";

    // таблица переходов по каждому месяцу
    for (int i = 1; i <= n; i++) {
        out << "Месяц " << i << ":\n";
        out << left
            << setw(10) << "k(i-1)"
            << setw(8) << "x_i"
            << setw(8) << "y_i"
            << setw(10) << "k_i"
            << setw(8) << "z"
            << setw(10) << "j(z)"
            << setw(10) << "y(x_i)"
            << setw(12) << "Q_i(k_i)" << "\n";
        out << string(76, '-') << "\n";

        bool any = false;
        for (int k = 0; k <= maxK[i]; k++) {
            if (dp[i][k] == INF) continue;
            any = true;

            int prev = parent[i][k];
            int x = k - prev + y[i];
            int z2 = prev + k + y[i];

            out << left
                << setw(10) << prev
                << setw(8) << x
                << setw(8) << y[i]
                << setw(10) << k
                << setw(8) << half(z2)
                << setw(10) << phi[z2]
                << setw(10) << psi[x]
                << setw(12) << dp[i][k] << "\n";
        }
        if (!any) out << "(нет допустимых состояний)\n";
        out << "\n";
    }

    // Итоговый оптимальный план.
    out << "Оптимальный план:\n";
    out << left
        << setw(6) << "i"
        << setw(10) << "k(i-1)"
        << setw(8) << "x_i"
        << setw(8) << "y_i"
        << setw(8) << "k_i"
        << setw(8) << "z"
        << setw(10) << "j(z)"
        << setw(10) << "y(x_i)" << "\n";
    out << string(58, '-') << "\n";

    // Суммируем затраты по месяцам и одновременно печатаем строки плана.
    ll total = 0;
    for (int i = 1; i <= n; i++) {
        ll stage = phi[planZ2[i]] + psi[planX[i]];
        total += stage;

        out << left
            << setw(6) << i
            << setw(10) << planPrev[i]
            << setw(8) << planX[i]
            << setw(8) << y[i]
            << setw(8) << planK[i]
            << setw(8) << half(planZ2[i])
            << setw(10) << phi[planZ2[i]]
            << setw(10) << psi[planX[i]] << "\n";
    }

    // Встроенная проверка: сумма затрат должна совпасть с dp[n][kn].
    out << "\nПроверка: сумма затрат = " << total << "\n";
    out << "Ответ: Q" << n << "(" << kn << ") = " << dp[n][kn] << " при ";
    for (int i = 1; i <= n; i++) {
        if (i > 1) out << ", ";
        out << "x" << i << " = " << planX[i];
    }
    out << "\n";

    // 6. Вывод результата

    cout << out.str();

    ofstream fout("output.txt");
    fout << out.str();

    return 0;
}

// ЛР3. Сетевое планирование, вариант 4.
// CPM: топологическая сортировка, ранние/поздние сроки, резервы, критический путь.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <string>
#include <utility>

using namespace std;

// точка входа
int main() {
    // 1. Исходные данные варианта 4

    // количество работ; нулевой индекс пустой, чтобы обращаться по номеру
    const int n = 18;

    // длительности работ (i = 1..18)
    vector<int> dur(n + 1, 0);
    int d[] = {0, 16, 8, 9, 14, 16, 11, 7, 12, 15, 16, 12, 13, 16, 14, 14, 15, 16, 14};
    for (int i = 1; i <= n; i++) dur[i] = d[i];

    // непосредственные предшественники каждой работы
    vector<vector<int>> pred(n + 1);
    pred[1] = {};
    pred[2] = {};
    pred[3] = {1};
    pred[4] = {2};
    pred[5] = {2, 4};
    pred[6] = {3, 4};
    pred[7] = {6, 5};
    pred[8] = {5, 6, 4};
    pred[9] = {6, 4, 5};
    pred[10] = {8, 9};
    pred[11] = {9, 7};
    pred[12] = {9};
    pred[13] = {11, 12};
    pred[14] = {11, 12, 10};
    pred[15] = {11};
    pred[16] = {15, 13};
    pred[17] = {14, 15};
    pred[18] = {13, 15};

    // 2. Построение графа работ

    // списки последователей для обратного прохода
    vector<vector<int>> succ(n + 1);

    // входящие степени для алгоритма Кана
    vector<int> indeg(n + 1, 0);

    for (int v = 1; v <= n; v++) {
        // Входящая степень — число непосредственных предшественников.
        indeg[v] = (int)pred[v].size();

        // Для каждого предшественника p добавляем v в его список потомков.
        for (int p : pred[v]) succ[p].push_back(v);
    }

    // 3. Топологическая сортировка (алгоритм Кана)

    // если в порядок попали не все работы, значит в графе есть цикл
    queue<int> q;
    for (int v = 1; v <= n; v++)
        if (indeg[v] == 0) q.push(v);

    // топологический порядок работ
    vector<int> topo;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        topo.push_back(v);

        for (int s : succ[v])
            if (--indeg[s] == 0) q.push(s);
    }

    // Проверка ацикличности: в корректном проекте все n работ должны
    // попасть в топологический порядок.
    if ((int)topo.size() != n) {
        cout << "Ошибка: в графе есть цикл.\n";
        return 1;
    }

    // 4. Прямой проход: ранние сроки ES / EF

    // es[v] — раннее начало, ef[v] — раннее окончание
    vector<int> es(n + 1, 0), ef(n + 1, 0);

    for (int v : topo) {
        // Работа может начаться только после завершения ВСЕХ предшественников,
        // поэтому берём максимум из их ранних окончаний.
        int start = 0;
        for (int p : pred[v]) start = max(start, ef[p]);

        es[v] = start;
        ef[v] = start + dur[v];
    }

    // критическое время проекта
    int Tkp = 0;
    for (int v = 1; v <= n; v++) Tkp = max(Tkp, ef[v]);

    // 5. Обратный проход: поздние сроки LS / LF

    // ls[v] — позднее начало, lf[v] — позднее окончание
    vector<int> ls(n + 1, 0), lf(n + 1, 0);

    for (int idx = n - 1; idx >= 0; idx--) {
        int v = topo[idx];

        // По умолчанию работа может закончиться в момент Tкр.
        int finish = Tkp;

        // Если есть последователи, нужно закончить до самого раннего
        // из их поздних начал.
        for (int s : succ[v]) finish = min(finish, ls[s]);

        lf[v] = finish;
        ls[v] = finish - dur[v];
    }

    // 6. Резервы времени

    // полный резерв: на сколько работу можно задержать без роста Tкр
    vector<int> reserve(n + 1, 0);
    for (int v = 1; v <= n; v++) reserve[v] = ls[v] - es[v];

    // 7. Восстановление критического пути

    // идём назад от работы, заканчивающейся в Tкр, по связям без зазора
    int end = -1;
    for (int v = 1; v <= n; v++) {
        if (reserve[v] == 0 && ef[v] == Tkp) {
            end = v;
            break;
        }
    }

    // один критический путь от начала к концу
    vector<int> path;

    int cur = end;
    while (cur != -1) {
        path.push_back(cur);

        // Ищем критического предшественника, который заканчивается
        // ровно в момент начала текущей работы.
        int next = -1;
        for (int p : pred[cur]) {
            if (reserve[p] == 0 && ef[p] == es[cur]) {
                next = p;
                break;
            }
        }
        cur = next;
    }

    // Шли от конца к началу — разворачиваем путь.
    reverse(path.begin(), path.end());

    // 8. Формирование отчёта

    // собираем отчёт в строку, чтобы консоль и файл совпали
    ostringstream out;
    out << "ЛР3. Сетевое планирование. Вариант 4\n";
    out << "Количество работ: " << n << "\n\n";

    // Заголовок таблицы работ.
    out << left
        << setw(6) << "№"
        << setw(6) << "t_i"
        << setw(24) << "Опирается на"
        << setw(6) << "ES"
        << setw(6) << "EF"
        << setw(6) << "LS"
        << setw(6) << "LF"
        << setw(6) << "R" << "\n";
    out << string(56, '-') << "\n";

    // Таблица по каждой работе: длительность, предшественники и сроки.
    for (int v = 1; v <= n; v++) {
        string p;
        for (int x : pred[v]) p += to_string(x) + " ";

        out << left
            << setw(6) << v
            << setw(6) << dur[v]
            << setw(24) << p
            << setw(6) << es[v]
            << setw(6) << ef[v]
            << setw(6) << ls[v]
            << setw(6) << lf[v]
            << setw(6) << reserve[v] << "\n";
    }

    // Служебная информация о ходе решения.
    out << "\nТопологический порядок: ";
    for (int v : topo) out << v << " ";
    out << "\n";

    out << "Критическое время Tкр = " << Tkp << "\n";

    out << "Критические работы (R = 0): ";
    for (int v = 1; v <= n; v++)
        if (reserve[v] == 0) out << v << " ";
    out << "\n";

    out << "Критический путь: ";
    for (int i = 0; i < (int)path.size(); i++) {
        if (i) out << " -> ";
        out << path[i];
    }
    out << "\n";

    // Сумма длительностей работ критического пути должна совпадать с Tкр.
    // Это удобная встроенная проверка корректности.
    int pathLen = 0;
    for (int v : path) pathLen += dur[v];
    out << "Длина критического пути: " << pathLen << "\n";

    // 9. Вывод результата

    cout << out.str();

    ofstream fout("output.txt");
    fout << out.str();

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <climits>
#include <cstdint>
#include <unordered_set>

using namespace std;

const int ROWS = 3;
const int COLS = 5;

// false — "не меньше", true — "ровно"
const bool EXACT = false;

struct Figure {
    string name;
    int w, h;
    char symbol;
    int need;
};

vector<Figure> figures = {
    {"A", 3, 2, 'A', 21},
    {"B", 2, 2, 'B', 15},
    {"C", 1, 3, 'C', 22},
    {"D", 1, 2, 'D', 31}
};

const string COLORS[] = {
    "\033[41m", "\033[42m", "\033[43m",
    "\033[44m", "\033[45m", "\033[46m", "\033[47m"
};
const int NUM_COLORS = 7;
const string BG_BLACK = "\033[40m";
const string RESET = "\033[0m";

struct Card {
    vector<vector<int>> grid;
    vector<int> count;
};

// ключ — количество фигур каждого типа; храним самый заполненный вариант
map<string, Card> uniqueCards;
vector<Card> cards;

// мемоизация состояний сетки: 4 бита на клетку, 15 клеток
unordered_set<uint64_t> visitedStates;

uint64_t stateKey(const vector<vector<int>>& g) {
    uint64_t h = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            h <<= 4;
            h |= (uint64_t)(g[i][j] + 1);
        }
    }
    return h;
}

bool canPlace(const vector<vector<int>>& g, int x, int y, int w, int h) {
    if (x < 0 || y < 0) return false;
    if (x + w > ROWS || y + h > COLS) return false;
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            if (g[i][j] != -1) return false;
    return true;
}

void placeFigure(vector<vector<int>>& g, int x, int y, int w, int h, int id) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            g[i][j] = id;
}

void removeFigure(vector<vector<int>>& g, int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            g[i][j] = -1;
}

// ни одна фигура (в любом повороте) больше не помещается
bool isMaximal(const vector<vector<int>>& g) {
    for (int f = 0; f < (int)figures.size(); f++) {
        int w1 = figures[f].w, h1 = figures[f].h;
        int w2 = figures[f].h, h2 = figures[f].w;

        for (int x = 0; x + w1 <= ROWS; x++)
            for (int y = 0; y + h1 <= COLS; y++)
                if (canPlace(g, x, y, w1, h1)) return false;

        if (w1 != w2 || h1 != h2) {
            for (int x = 0; x + w2 <= ROWS; x++)
                for (int y = 0; y + h2 <= COLS; y++)
                    if (canPlace(g, x, y, w2, h2)) return false;
        }
    }
    return true;
}

string countKey(const vector<int>& cnt) {
    string s;
    for (int c : cnt) s += to_string(c) + ",";
    return s;
}

int usedArea(const vector<vector<int>>& g) {
    int area = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (g[i][j] != -1) area++;
    return area;
}

// перебор всех карт раскроя
void genCards(vector<vector<int>>& g, vector<int>& cnt) {
    uint64_t key = stateKey(g);
    if (visitedStates.count(key)) return;
    visitedStates.insert(key);

    bool any = false;
    for (int c : cnt) if (c > 0) { any = true; break; }

    if (any && (EXACT || isMaximal(g))) {
        string ck = countKey(cnt);
        auto it = uniqueCards.find(ck);
        if (it == uniqueCards.end()) {
            Card card;
            card.grid = g;
            card.count = cnt;
            uniqueCards[ck] = card;
        } else if (usedArea(g) > usedArea(it->second.grid)) {
            Card card;
            card.grid = g;
            card.count = cnt;
            uniqueCards[ck] = card;
        }
    }

    for (int f = 0; f < (int)figures.size(); f++) {
        int w1 = figures[f].w, h1 = figures[f].h;
        int w2 = figures[f].h, h2 = figures[f].w;

        for (int x = 0; x + w1 <= ROWS; x++) {
            for (int y = 0; y + h1 <= COLS; y++) {
                if (canPlace(g, x, y, w1, h1)) {
                    placeFigure(g, x, y, w1, h1, f);
                    cnt[f]++;
                    genCards(g, cnt);
                    cnt[f]--;
                    removeFigure(g, x, y, w1, h1);
                }
            }
        }
        if (w1 != w2 || h1 != h2) {
            for (int x = 0; x + w2 <= ROWS; x++) {
                for (int y = 0; y + h2 <= COLS; y++) {
                    if (canPlace(g, x, y, w2, h2)) {
                        placeFigure(g, x, y, w2, h2, f);
                        cnt[f]++;
                        genCards(g, cnt);
                        cnt[f]--;
                        removeFigure(g, x, y, w2, h2);
                    }
                }
            }
        }
    }
}

void printColor(const Card& c) {
    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i < ROWS; i++) {
            int id = c.grid[i][j];
            if (id == -1) cout << BG_BLACK << "  " << RESET;
            else          cout << COLORS[id % NUM_COLORS]
                               << figures[id].symbol << " " << RESET;
        }
        cout << "\n";
    }
}

void writeCard(ofstream& fout, const Card& c, int idx) {
    fout << "Карта " << idx << ":\n";
    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i < ROWS; i++) {
            int id = c.grid[i][j];
            fout << (id == -1 ? '.' : figures[id].symbol) << " ";
        }
        fout << "\n";
    }
    fout << "Фигур: ";
    for (int f = 0; f < (int)figures.size(); f++)
        if (c.count[f] > 0)
            fout << figures[f].name << "=" << c.count[f] << " ";
    fout << "\n\n";
}

int main() {
    vector<vector<int>> grid(ROWS, vector<int>(COLS, -1));
    vector<int> cnt(figures.size(), 0);
    genCards(grid, cnt);

    for (auto& kv : uniqueCards)
        cards.push_back(kv.second);

    sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        return usedArea(a.grid) > usedArea(b.grid);
    });

    const int N = figures.size();

    cout << "Лист: " << ROWS << "x" << COLS << "\n";
    cout << "Режим: " << (EXACT ? "ровно" : "не меньше") << "\n";
    cout << "Фигуры:\n";
    for (const auto& f : figures)
        cout << "  " << f.name << ": " << f.w << "x" << f.h
             << ", нужно " << f.need << " шт\n";
    cout << "Карт раскроя: " << cards.size() << "\n\n";

    for (int i = 0; i < (int)cards.size(); i++) {
        cout << "Карта " << i + 1 << ":\n";
        printColor(cards[i]);
        cout << "\n";
    }

    ofstream fout("output.txt");
    fout << "Задача раскроя. Лист " << ROWS << "x" << COLS << "\n";
    fout << "Режим: " << (EXACT ? "ровно" : "не меньше") << "\n";
    for (const auto& f : figures)
        fout << f.name << "(" << f.w << "x" << f.h
             << ", нужно " << f.need << ")\n";
    fout << "Всего карт: " << cards.size() << "\n\n";

    for (int i = 0; i < (int)cards.size(); i++)
        writeCard(fout, cards[i], i + 1);

    const int A = figures[0].need;
    const int B = figures[1].need;
    const int C = figures[2].need;
    const int D = figures[3].need;

    const int INF = 1e9;

    const int SA = A + 1, SB = B + 1, SC = C + 1, SD = D + 1;
    auto idx = [&](int a, int b, int c, int d) {
        return ((a * SB + b) * SC + c) * SD + d;
    };

    vector<int> dp(SA * SB * SC * SD, INF);
    vector<int> from(SA * SB * SC * SD, -1);
    vector<int> pa(SA * SB * SC * SD, -1);
    vector<int> pb(SA * SB * SC * SD, -1);
    vector<int> pc(SA * SB * SC * SD, -1);
    vector<int> pd(SA * SB * SC * SD, -1);

    dp[idx(0, 0, 0, 0)] = 0;

    for (int a = 0; a <= A; a++)
    for (int b = 0; b <= B; b++)
    for (int c = 0; c <= C; c++)
    for (int d = 0; d <= D; d++) {
        int cur = idx(a, b, c, d);
        if (dp[cur] == INF) continue;

        for (int j = 0; j < (int)cards.size(); j++) {
            int ca = cards[j].count[0];
            int cb = cards[j].count[1];
            int cc = cards[j].count[2];
            int cd = cards[j].count[3];

            int na, nb, nc, nd;
            if (EXACT) {
                na = a + ca; nb = b + cb; nc = c + cc; nd = d + cd;
                if (na > A || nb > B || nc > C || nd > D) continue;
            } else {
                na = min(A, a + ca);
                nb = min(B, b + cb);
                nc = min(C, c + cc);
                nd = min(D, d + cd);
            }

            int ni = idx(na, nb, nc, nd);
            if (dp[ni] > dp[cur] + 1) {
                dp[ni] = dp[cur] + 1;
                from[ni] = j;
                pa[ni] = a; pb[ni] = b; pc[ni] = c; pd[ni] = d;
            }
        }
    }

    int ansIdx = idx(A, B, C, D);
    int ans = dp[ansIdx];

    if (ans == INF) {
        cout << "\nРешение не найдено.\n";
        fout << "Решение не найдено.\n";
        fout.close();
        return 0;
    }

    cout << "\nМинимум листов: " << ans << "\n";
    fout << "Минимум листов: " << ans << "\n\n";

    vector<int> x(cards.size(), 0);
    int a = A, b = B, c = C, d = D;
    while (!(a == 0 && b == 0 && c == 0 && d == 0)) {
        int cur = idx(a, b, c, d);
        int j = from[cur];
        if (j == -1) break;
        x[j]++;
        int na = pa[cur], nb = pb[cur], nc = pc[cur], nd = pd[cur];
        if (na < 0) break;
        a = na; b = nb; c = nc; d = nd;
    }

    for (int j = 0; j < (int)x.size(); j++) {
        if (x[j] > 0) {
            cout << "x" << j + 1 << " = " << x[j] << " (";
            for (int f = 0; f < N; f++)
                if (cards[j].count[f] > 0)
                    cout << figures[f].name << "=" << cards[j].count[f] << " ";
            cout << ")\n";

            fout << "x" << j + 1 << " = " << x[j] << " (";
            for (int f = 0; f < N; f++)
                if (cards[j].count[f] > 0)
                    fout << figures[f].name << "=" << cards[j].count[f] << " ";
            fout << ")\n";
        }
    }

    vector<int> total(N, 0);
    for (int j = 0; j < (int)cards.size(); j++)
        for (int f = 0; f < N; f++)
            total[f] += x[j] * cards[j].count[f];

    cout << "\nВыпуск (нужно / получилось):\n";
    fout << "\nВыпуск (нужно / получилось):\n";
    for (int f = 0; f < N; f++) {
        cout << "  " << figures[f].name << ": "
             << figures[f].need << " / " << total[f] << "\n";
        fout << "  " << figures[f].name << ": "
             << figures[f].need << " / " << total[f] << "\n";
    }

    cout << "Всего листов: " << ans << "\n";
    fout << "Всего листов: " << ans << "\n";

    fout.close();
    cout << "Записано в result.txt\n";
    return 0;
}
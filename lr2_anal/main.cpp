#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1e18;

struct Edge { int to, rev; ll cap, cost; };
vector<vector<Edge>> g;

void addEdge(int u, int v, ll cap, ll cost) {
    g[u].push_back({v, (int)g[v].size(), cap, cost});
    g[v].push_back({u, (int)g[u].size()-1, 0, -cost});
}

pair<ll,ll> minCostFlow(int s, int t, ll maxFlow) {
    int n = g.size();
    vector<ll> pot(n,0), dist(n);
    vector<int> pv(n), pe(n);
    ll flow=0, cost=0;
    while (flow < maxFlow) {
        fill(dist.begin(), dist.end(), INF);
        dist[s]=0;
        priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;
        pq.push({0,s});
        while(!pq.empty()){
            auto [d,u]=pq.top(); pq.pop();
            if(d!=dist[u]) continue;
            for(int i=0;i<(int)g[u].size();i++){
                auto &e=g[u][i];
                if(e.cap<=0) continue;
                ll nd = d + e.cost + pot[u] - pot[e.to];
                if(nd < dist[e.to]){
                    dist[e.to]=nd; pv[e.to]=u; pe[e.to]=i;
                    pq.push({nd,e.to});
                }
            }
        }
        if(dist[t]==INF) break;
        for(int v=0;v<n;v++) if(dist[v]<INF) pot[v]+=dist[v];
        ll add = maxFlow - flow;
        for(int v=t; v!=s; v=pv[v]) add = min(add, g[pv[v]][pe[v]].cap);
        for(int v=t; v!=s; v=pv[v]){
            auto &e = g[pv[v]][pe[v]];
            e.cap -= add;
            g[v][e.rev].cap += add;
            cost += add * e.cost;
        }
        flow += add;
    }
    return {flow, cost};
}

int main(){
    vector<ll> a = {0,19,13,14,13,12,14};
    vector<ll> b = {0,12,14,12,13,11,13,10};
    vector<ll> capD = {0,18,16,20,18,17};
    vector<vector<ll>> cAD = {
        {}, // 0
        {0,3,6,3,3,7},
        {0,5,4,7,6,5},
        {0,6,3,6,3,4},
        {0,9,6,7,2,3},
        {0,3,2,4,7,8},
        {0,5,3,9,7,3}
    };
    vector<vector<ll>> cDB = {
        {},
        {0,4,5,3,6,4,7,5},
        {0,4,6,4,6,3,8,4},
        {0,7,6,5,2,4,6,7},
        {0,5,8,2,4,7,6,4},
        {0,9,4,7,9,2,3,8}
    };

    int S=0, T=24;
    int A0=1, D0in=7, D0out=12, B0=17;
    g.assign(25, {});

    ll total=0;
    for(int i=1;i<=6;i++){ addEdge(S, A0+i-1, a[i], 0); total+=a[i]; }
    for(int j=1;j<=5;j++){
        addEdge(D0in+j-1, D0out+j-1, capD[j], 0);
        for(int i=1;i<=6;i++)
            addEdge(A0+i-1, D0in+j-1, INF, cAD[i][j]);
    }
    for(int j=1;j<=5;j++)
        for(int k=1;k<=7;k++)
            addEdge(D0out+j-1, B0+k-1, INF, cDB[j][k]);
    for(int k=1;k<=7;k++) addEdge(B0+k-1, T, b[k], 0);

    auto [flow, cost] = minCostFlow(S, T, total);
    cout << "Поток = " << flow << " / " << total << "\n";
    cout << "Минимальная стоимость = " << cost << "\n\n";

    cout << "Матрица X (A->D), 6x5:\n";
    for(int i=1;i<=6;i++){
        for(int j=1;j<=5;j++){
            int u = A0+i-1, v = D0in+j-1;
            ll used = 0;
            for(auto &e : g[v]) if(e.to==u && e.cap>0) used = e.cap;
            cout << used << "\t";
        }
        cout << "\n";
    }
    cout << "\nМатрица Y (D->B), 5x7:\n";
    for(int j=1;j<=5;j++){
        for(int k=1;k<=7;k++){
            int u = D0out+j-1, v = B0+k-1;
            ll used = 0;
            for(auto &e : g[v]) if(e.to==u && e.cap>0) used = e.cap;
            cout << used << "\t";
        }
        cout << "\n";
    }
    return 0;
}
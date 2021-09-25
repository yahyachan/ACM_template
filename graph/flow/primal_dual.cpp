// LOJ 102
#include <bits/stdc++.h>
using ll = long long;
const int maxn = 405;
const int maxm = 40005;
int first[maxn];
int next[maxm], to[maxm], residue[maxm]; ll cost[maxm];
int ecnt = 1;
void add_edge(int u, int v, int f, ll c) {
  ecnt ++;
  next[ecnt] = first[u];
  first[u] = ecnt;
  to[ecnt] = v;
  residue[ecnt] = f;
  cost[ecnt] = c;
}
void ins_edge(int u, int v, int f, ll c) {
  add_edge(u, v, f, c);
  add_edge(v, u, 0, -c);
}

int n, m;
struct rec {
  int u; ll d;
  bool operator <(const rec &r) const {
    return d > r.d;
  }
};
const ll INF = 0x3f3f3f3f3f3fLL;
int S, T; ll h[maxn], d[maxn];
bool vis[maxn]; int prev[maxn];
bool dij() {
  std::fill(d + 1, d + 1 + n, INF);
  std::fill(vis + 1, vis + 1 + n, false);
  std::fill(prev + 1, prev + 1 + n, 0);
  d[S] = 0;
  std::priority_queue<rec> Q;
  Q.push((rec){S, 0LL});
  while(!Q.empty()) {
    const int u = Q.top().u; Q.pop();
    if(vis[u]) continue;
    vis[u] = true;
    for(int i = first[u]; i; i = next[i]) {
      const int v = to[i];
      const ll nc = cost[i] + h[u] - h[v];
      if(residue[i] > 0 && d[u] + nc < d[v]) {
        d[v] = d[u] + nc;
        prev[v] = i;
        Q.push((rec){v, d[v]});
      }
    }
  }
  for(int i = 1; i <= n; i ++) {
    h[i] += d[i];
  }
  return vis[T];
}

void primal_dual(int &flow, ll &co) {
  dij(); // as bootstrap
  flow = 0; co = 0;
  while(dij()) {
    int th_f = INT_MAX;
    int u = T;
    while(u != S) {
      const int e = prev[u];
      th_f = std::min(th_f, residue[e]);
      u = to[e ^ 1];
    }
    flow += th_f;
    u = T;
    while(u != S) {
      const int e = prev[u];
      residue[e] -= th_f;
      residue[e ^ 1] += th_f;
      co += cost[e] * th_f;
      u = to[e ^ 1];
    }
  }
}

int main() {
  scanf("%d%d", &n, &m);
  S = 1; T = n;
  while(m --) {
    int s, t, c, w;
    scanf("%d%d%d%d", &s, &t, &c, &w);
    ins_edge(s, t, c, w);
  }
  int flow; ll co;
  primal_dual(flow, co);
  printf("%d %lld\n", flow, co);
  return 0;
}
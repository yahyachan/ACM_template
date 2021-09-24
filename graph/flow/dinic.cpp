#include <bits/stdc++.h>
typedef long long ll;
const int maxn = 10005;
const int maxm = 200005;
int first[maxn];
int next[maxm], to[maxm]; ll cap[maxm], flow[maxm];
void ins_edge(int u, int v, ll w) {
  static int cnt = 0; ++ cnt;
  next[cnt] = first[u]; first[u] = cnt;
  to[cnt] = v; cap[cnt] = w; flow[cnt] = 0;
  
  ++ cnt;
  next[cnt] = first[v]; first[v] = cnt;
  to[cnt] = u; cap[cnt] = flow[cnt] = 0;
}
int rev(int i) {
  return ((i - 1) ^ 1) + 1;
}

int n;
int d[maxn]; bool vis[maxn];
int s, t;
bool bfs() {
  std::fill(d, d + 1 + n, -1);
  std::fill(vis, vis + 1 + n, false);
  std::queue<int> Q; Q.push(s);
  d[s] = 1; vis[s] = true;
  while(!Q.empty()) {
    int u = Q.front(); Q.pop();
    for(int i = first[u]; i; i = next[i]) {
      int v = to[i];
      if(!vis[v] && cap[i] > flow[i]) {
        d[v] = d[u] + 1;
        Q.push(v); vis[v] = true;
      }
    }
  }
  return vis[t];
}
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
int cur[maxn];
ll dfs(int x, ll a) {
  if(a == 0LL || x == t) return a;
  ll ans = 0;
  for(int &i = cur[x]; i; i = next[i]) {
    int v = to[i];
    if(d[x] + 1 == d[v]) {
      ll fl = dfs(v, std::min(a, cap[i] - flow[i]));
      if(fl > 0) {
        ans += fl; a -= fl;
        flow[i] += fl; flow[rev(i)] -= fl;
        if(!a) break;
      }
    }
  }
  if(a > 0LL) d[x] = -1;
  return ans;
}
ll dinic(int S, int T) {
  s = S; t = T;
  ll ans = 0;
  while(bfs()) {
    std::copy(first + 1, first + 1 + n, cur + 1);
    ans += dfs(s, INF);
  }
  return ans;
}

int main() {
  int m, S, T; scanf("%d%d%d%d", &n, &m, &S, &T);
  while(m --) {
    int u, v; ll w; scanf("%d%d%lld", &u, &v, &w);
    ins_edge(u, v, w);
  }
  printf("%lld\n", dinic(S, T));
  return 0;
}
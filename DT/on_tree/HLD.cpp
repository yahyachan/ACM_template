#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
const int maxn = 100005;
std::vector<int> G[maxn];
inline void ins_edge(int u, int v) {
  G[u].push_back(v); G[v].push_back(u);
}

const int maxno = maxn << 2;
using ll = long long;
ll sumv[maxno], addv[maxno];
ll plc[maxno];
void maintain(int o) {
  sumv[o] = sumv[o << 1] + sumv[o << 1 | 1];
}
void paint(int o, int L, int R, ll v) {
  sumv[o] += (ll(R - L + 1)) * v;
  addv[o] += v;
}
void pushdown(int o, int L, int R) {
  if(addv[o] != 0LL) {
    int M = (L + R) / 2;
    paint(o << 1, L, M, addv[o]);
    paint(o << 1 | 1, M + 1, R, addv[o]);
    addv[o] = 0;
  }
}
void build_tree(int o, int L, int R) {
  if(L == R) {
    sumv[o] = plc[L];
  } else {
    int M = (L + R) / 2;
    build_tree(o << 1, L, M);
    build_tree(o << 1 | 1, M + 1, R);
    maintain(o);
  }
}
ll query(int o, int L, int R, int ql, int qr) {
  if(ql <= L && R <= qr) {
    return sumv[o];
  } else {
    int M = (L + R) / 2;
    pushdown(o, L, R);
    ll ans = 0;
    if(ql <= M) ans += query(o << 1, L, M, ql, qr);
    if(qr > M) ans += query(o << 1 | 1, M + 1, R, ql, qr);
    return ans;
  }
}
void modify(int o, int L, int R, int ql, int qr, ll v) {
  if(ql <= L && R <= qr) {
    paint(o, L, R, v);
  } else {
    int M = (L + R) / 2; pushdown(o, L, R);
    if(ql <= M) modify(o << 1, L, M, ql, qr, v);
    if(qr > M) modify(o << 1 | 1, M + 1, R, ql, qr, v);
    maintain(o);
  }
}

int anc[maxn][17];
int siz[maxn], dep[maxn], hson[maxn];
void dfs_1(int x, int fa = -1, int depth = 0) {
  siz[x] = 1; dep[x] = depth; anc[x][0] = fa;
  int max_siz = 0;
  for(int v : G[x]) {
    if(v != fa) {
      dfs_1(v, x, depth + 1);
      siz[x] += siz[v];
      if(siz[v] > max_siz) {
        max_siz = siz[v]; hson[x] = v;
      }
    }
  }
}
int top[maxn], tid[maxn], dfn[maxn]; ll A[maxn];
void dfs_2(int x, int a = 1) {
  static int cnt = 0; cnt ++;
  dfn[x] = cnt; tid[cnt] = x; plc[cnt] = A[x]; top[x] = a;
  if(hson[x]) {
    dfs_2(hson[x], a);
  } else {
    return;
  }
  for(int v : G[x]) {
    if(v != anc[x][0] && v != hson[x]) {
      dfs_2(v, v);
    }
  }
}
int n;
void process() {
  memset(anc, -1, sizeof(anc));
  dfs_1(1); dfs_2(1); build_tree(1, 1, n);
  for(int j = 1; (1 << j) < n; j ++) {
    for(int i = 1; i <= n; i ++) {
      int a = anc[i][j - 1];
      if(a != -1) anc[i][j] = anc[a][j - 1];
    }
  }
}
int get_up(int v, int u) {
  for(int j = 16; j >= 0; j --) {
    int a = anc[u][j];
    if(a != -1 && dep[a] > dep[v]) u = a;
  }
  return u;
}
bool is_anc(int v, int u) {
  for(int j = 16; j >= 0; j --) {
    int a = anc[u][j];
    if(a != -1 && dep[a] >= dep[v]) u = a;
  }
  return (u == v);
}
int now_rt;
ll query_tree(int x) {
  if(x == now_rt) {
    return query(1, 1, n, 1, n);
  } else {
    if(is_anc(x, now_rt)) {
      int g = get_up(x, now_rt);
      return query(1, 1, n, 1, n) - query(1, 1, n, dfn[g], dfn[g] + siz[g] - 1);
    } else {
      return query(1, 1, n, dfn[x], dfn[x] + siz[x] - 1);
    }
  }
}
void modify_tree(int x, ll v) {
  if(x == now_rt) {
    modify(1, 1, n, 1, n, v);
  } else {
    if(is_anc(x, now_rt)) {
      int g = get_up(x, now_rt);
      modify(1, 1, n, 1, n, v);
      modify(1, 1, n, dfn[g], dfn[g] + siz[g] - 1, -v);
    } else {
      modify(1, 1, n, dfn[x], dfn[x] + siz[x] - 1, v);
    }
  }
}
ll query_chain(int u, int v) {
  if(top[u] == top[v]) {
    int l = dfn[u], r = dfn[v];
    if(l > r) std::swap(l, r);
    return query(1, 1, n, l, r);
  }
  if(dep[top[u]] < dep[top[v]]) std::swap(u, v);
  return query(1, 1, n, dfn[top[u]], dfn[u]) + query_chain(anc[top[u]][0], v);
}
void modify_chain(int u, int v, ll delta) {
  if(top[u] == top[v]) {
    int l = dfn[u], r = dfn[v];
    if(l > r) std::swap(l, r);
    modify(1, 1, n, l, r, delta); return;
  }
  if(dep[top[u]] < dep[top[v]]) std::swap(u, v);
  modify(1, 1, n, dfn[top[u]], dfn[u], delta);
  modify_chain(anc[top[u]][0], v, delta);
}

int main() {
  now_rt = 1;
  scanf("%d", &n);
  for(int i = 1; i <= n; i ++) scanf("%lld", &A[i]);
  for(int i = 2; i <= n; i ++) {
    int f; scanf("%d", &f);
    ins_edge(i, f);
  }
  process();
  int q; scanf("%d", &q);
  while(q --) {
    int op, u; scanf("%d%d", &op, &u);
    if(op == 1) {
      now_rt = u;
    } else if(op == 2) {
      int v; ll k; scanf("%d%lld", &v, &k);
      modify_chain(u, v, k);
    } else if(op == 4) {
      int v; scanf("%d", &v);
      printf("%lld\n", query_chain(u, v));
    } else if(op == 3) {
      ll k; scanf("%lld", &k) ;
      modify_tree(u, k);
    } else if(op == 5) {
      printf("%lld\n", query_tree(u));
    }
  }
  return 0;
}
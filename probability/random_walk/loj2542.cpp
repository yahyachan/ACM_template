#include <bits/stdc++.h>
using ll = long long;
const ll ha = 998244353LL;
ll inv(ll x);

struct Rec {
  ll x, y;
  Rec(ll a = 0, ll b = 0) { x = a; y = b; }
};
Rec operator +(const Rec &a, const Rec &b) {
  return Rec((a.x + b.x) % ha, (a.y + b.y) % ha);
}
Rec operator -(const Rec &a, const Rec &b) {
  return Rec((a.x - b.x + ha) % ha, (a.y - b.y + ha) % ha);
}
Rec operator *(const Rec &a, const ll &v) {
  return Rec(a.x * v % ha, a.y * v % ha);
}
Rec operator *(const ll &v, const Rec &a) {
  return Rec(a.x * v % ha, a.y * v % ha);
}

const int maxn = 20;
std::vector<int> G[maxn]; int deg[maxn];
Rec dfs(int s, int x, int fa) {
  if((1 << (x - 1)) & s) return Rec();
  Rec lt(deg[x], ha - deg[x]);
  for(int v : G[x]) {
    if(v != fa) {
      lt = lt - dfs(s, v, x);
    }
  }
  ll inv_A = inv(lt.x);
  return Rec(inv_A, ((ha - lt.y) % ha) * inv_A % ha);
}

// 最终需要的答案就是根节点 dfs 结果的 y 成员
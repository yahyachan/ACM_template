#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <functional>
using ll = long long;
int lowbit(int x) {
  return x & (-x);
}

const int maxn = 1000005;
ll T[maxn];
ll query(int p) {
  ll ans = 0;
  while(p > 0) {
    ans += T[p];
    p -= lowbit(p);
  }
  return ans;
}
ll query(int l, int r) {
  return query(r) - query(l - 1);
}
int n;
void add(int p, ll v) {
  while(p <= n) {
    T[p] += v;
    p += lowbit(p);
  }
}

int main() {
  int q; scanf("%d%d", &n, &q);
  for(int i = 1; i <= n; i ++) {
    ll v; scanf("%lld", &v);
    add(i, v);
  }
  while(q --) {
    int op; scanf("%d", &op);
    if(op == 1) {
      int i; ll x; scanf("%d%lld", &i, &x);
      add(i, x);
    } else {
      int l, r; scanf("%d%d", &l, &r);
      printf("%lld\n", query(l, r));
    }
  }
  return 0;
}
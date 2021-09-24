#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <vector>
const int maxn = 100005;
const int maxk = 200005;
inline int lowbit(int x) {
  return x & (-x);
}

int C[maxk];
int k;
void add(int p, int v = 1) {
  while(p <= k) {
    C[p] += v;
    p += lowbit(p);
  }
}
int sum(int p) {
  int ret = 0;
  while(p > 0) {
    ret += C[p];
    p -= lowbit(p);
  }
  return ret;
}
void clear(int p) {
  while(p <= k) {
    if(C[p] == 0) break;
    C[p] = 0;
    p += lowbit(p);
  }
}

struct Node {
  int id, s;
  int a, b, c;
  bool operator <(const Node &res) const {
    if(a == res.a) {
      if(b == res.b) {
        return c < res.c;
      } else {
        return b < res.b;
      }
    } else {
      return a < res.a;
    }
  }
  bool operator ==(const Node &res) const {
    return (a == res.a && b == res.b && c == res.c);
  }
};
Node P[maxn];
int ans[maxn];
void cdq(int L, int R) {
  static Node tmp[maxn];
  if(L == R) return;
  int M = (L + R) / 2;
  cdq(L, M); cdq(M + 1, R);
  int lp = L, rp = M + 1;
  std::vector<int> V;
  for(int i = L; i <= R; i ++) {
    if(rp > R || (lp <= M && P[lp].b <= P[rp].b)) {
      tmp[i] = P[lp ++];
      V.push_back(tmp[i].c); add(tmp[i].c, tmp[i].s);
    } else {
      tmp[i] = P[rp ++];
      ans[tmp[i].id] += sum(tmp[i].c);
    }
  }
  for(auto p : V) {
    clear(p);
  }
  std::copy(tmp + L, tmp + R + 1, P + L);
}

int main() {
  int n; scanf("%d%d", &n, &k);
  for(int i = 1; i <= n; i ++) {
    P[i].id = i; P[i].s = 1;
    scanf("%d%d%d", &P[i].a, &P[i].b, &P[i].c);
  }
  std::sort(P + 1, P + 1 + n);
  for(int i = 2; i <= n; i ++) {
    if(P[i] == P[i - 1]) {
      P[i].s = P[i - 1].s + 1;
      P[i - 1].a = P[i - 1].b = P[i - 1].c = 0;
    }
  }
  std::sort(P + 1, P + 1 + n);
  int tp = std::unique(P + 1, P + 1 + n) - P - 1;
  int bp = (P[1].a == 0) ? 2 : 1;
  cdq(bp, tp);
  static int d[maxn];
  for(int i = bp; i <= tp; i ++) {
    int id = P[i].id;
    int ret = ans[id] + P[i].s - 1;
    d[ret] += P[i].s;
  }
  for(int i = 0; i < n; i ++) {
    printf("%d\n", d[i]);
  }
  return 0;
}
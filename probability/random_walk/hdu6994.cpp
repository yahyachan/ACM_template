#include <bits/stdc++.h>
typedef long long ll;
const ll ha = 1000000007;
ll inv(ll x);

int n, m, vsiz;
const int maxn = 405;
int idx(int i, int j) {
  if(i <= 0 || j <= 0 || i > n || j > m) return 0;
  return (i - 1) * m + j;
}
ll p[maxn][maxn][4];
ll G[maxn][maxn];
void gen_G();
void eliminate(ll *dest, ll *src, int pv, std::vector<int> &can_use) {
  if(!dest[pv]) return;
  ll rate = dest[pv];
  for(int i : can_use) {
    dest[i] = (dest[i] - src[i] * rate % ha + ha) % ha;
  }
}
std::vector<int> occur[maxn];
ll solve_nm() { // of n < m
  const int bd = n * m;
  for(int y = 1; y <= m; y ++) {
    for(int x = 1; x <= n; x ++) {
      const int th = idx(x, y);
      int r = th;
      assert(G[r][r] != 0);
      ll *row = G[th];
      std::vector<int> tmp;
      tmp.push_back(bd + 1);
      for(int j = 1; j <= bd; j ++) {
        if(row[j] != 0) tmp.push_back(j);
      }
      ll inv_principal = inv(row[th]);
      for(int j : tmp) {
        row[j] = row[j] * inv_principal % ha;
      }
      for(int j = 1; j <= n; j ++) {
        ll *br;
        if(j <= x) {
          br = G[idx(j, y + 1)];
        } else {
          br = G[idx(j, y)];
        }
        eliminate(br, row, th, tmp);
      }
    }
  }
  for(int j = 1; j <= bd; j ++) {
    occur[j].clear();
  }
  for(int j = 1; j <= bd; j ++) {
    for(int i = 1; i <= bd; i ++) {
      if(G[j][i] != 0) occur[i].push_back(j);
    }
  }
  ll ans = 0;
  for(int y = m; y >= 1; y --) {
    for(int x = n; x >= 1; x --) {
      const int i = idx(x, y);
      assert(G[i][i] != 0);
      ll inv_principal = inv(G[i][i]);
      G[i][i] = 1; G[i][bd + 1] = G[i][bd + 1] * inv_principal % ha;
      ans = (ans + G[i][bd + 1]) % ha;
      std::vector<int> go;
      go.push_back(i); go.push_back(bd + 1);
      for(int p : occur[i]) {
        if(p != i) eliminate(G[p], G[i], i, go);
      }
    }
  }
  return ans;
}
ll solve_mn() { // of n > m
  const int bd = n * m;
  for(int x = 1; x <= n; x ++) {
    for(int y = 1; y <= m; y ++) {
      const int th = idx(x, y);
      int r = th;
      assert(G[r][r] != 0);
      ll *row = G[th];
      std::vector<int> tmp;
      tmp.push_back(bd + 1);
      for(int j = 1; j <= bd; j ++) {
        if(row[j] != 0) tmp.push_back(j);
      }
      ll inv_principal = inv(row[th]);
      for(int j : tmp) {
        row[j] = row[j] * inv_principal % ha;
      }
      for(int j = 1; j <= m; j ++) {
        ll *br;
        if(j <= y) {
          br = G[idx(x + 1, j)];
        } else {
          br = G[idx(x, j)];
        }
        eliminate(br, row, th, tmp);
      }
    }
  }
  for(int j = 1; j <= bd; j ++) {
    occur[j].clear();
  }
  for(int j = 1; j <= bd; j ++) {
    for(int i = 1; i <= bd; i ++) {
      if(G[j][i] != 0) occur[i].push_back(j);
    }
  }
  ll ans = 0;
  for(int x = n; x >= 1; x --) {
    for(int y = m; y >= 1; y --) {
      const int i = idx(x, y);
      assert(G[i][i] != 0);
      ll inv_principal = inv(G[i][i]);
      G[i][i] = 1; G[i][bd + 1] = G[i][bd + 1] * inv_principal % ha;
      ans = (ans + G[i][bd + 1]) % ha;
      std::vector<int> go;
      go.push_back(i); go.push_back(bd + 1);
      for(int p : occur[i]) {
        if(p != i) eliminate(G[p], G[i], i, go);
      }
    }
  }
  return ans;
}
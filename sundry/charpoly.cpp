#include <bits/stdc++.h>
const int maxn = 305;
using ll = long long;
const ll ha = 998244353;
int n;
void add_mod(ll &x, ll v) {
  x = (x + v) % ha;
}
void sub_mod(ll &x, ll v) {
  x = (x - v + ha) % ha;
}
ll inv(ll x);

ll B[maxn][maxn], delta;
void to_Hessenberg() {
  for(int i = 1; i < n - 1; i ++) {
    int r = -1;
    for(int j = i + 1; j < n; j ++) {
      if(B[j][i] != 0) {
        r = j; break;
      }
    }
    if(r == -1) continue;
    if(r != i + 1) {
      for(int j = 1; j < n; j ++) std::swap(B[r][j], B[i + 1][j]);
      for(int j = 1; j < n; j ++) std::swap(B[j][r], B[j][i + 1]);
    }
    const ll inv_principal = inv(B[i + 1][i]);
    for(int j = i + 2; j < n; j ++) {
      if(B[j][i] == 0) continue;
      const ll rate = B[j][i] * inv_principal % ha;
      for(int k = 1; k < n; k ++) {
        sub_mod(B[j][k], rate * B[i + 1][k] % ha);
      }
      for(int k = 1; k < n; k ++) {
        add_mod(B[k][i + 1], rate * B[k][j]);
      }
    }
  }
}
ll poly[maxn][maxn];
void calc_charpoly() {
  poly[0][0] = 1;
  for(int i = 1; i < n; i ++) {
    ll *th = poly[i];
    const ll diag = B[i][i];
    for(int j = 0; j <= i; j ++) {
      if(j > 0) {
        th[j] = poly[i - 1][j - 1];
      } else {
        th[j] = 0;
      }
      if(j < i) {
        sub_mod(th[j], poly[i - 1][j] * diag % ha);
      }
    }
    ll prod = 1;
    for(int k = 1; k < i; k ++) {
      prod = prod * B[i - k + 1][i - k] % ha;
      ll t = ha - prod * B[i - k][i] % ha;
      ll *ot = poly[i - k - 1];
      for(int j = 0; j <= i - k - 1; j ++) {
        add_mod(th[j], ot[j] * t);
      }
    }
  }
}
// 最后 poly[n - 1] 存的就是特征多项式的系数表示
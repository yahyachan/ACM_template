#include <bits/stdc++.h>
typedef long long ll;
const ll ha = 998244353;
void add_mod(ll &x, ll v) {
  x = (x + v) % ha;
}
void sub_mod(ll &x, ll v) {
  x = (x - v + ha) % ha;
}
ll pow_mod(ll a, ll b) {
  ll ans = 1, res = a;
  while(b) {
    if(b & 1) ans = ans * res % ha;
    res = res * res % ha; b >>= 1;
  }
  return ans;
}
ll inv(ll x) {
  return pow_mod(x, ha - 2);
}

const int maxn = 505;
int n, d;
ll M[maxn][maxn << 1];
ll delta; int al_shift;
void column_elminate(int dest, int src, ll k) {
  while(dest > 0 && src > 0) {
    for(int i = 1; i <= n; i ++) {
      M[i][dest] = (M[i][dest] + k * M[i][src]) % ha;
    }
    dest -= n; src -= n;
  }
}
void column_shift(int j) {
  while(j > n) {
    for(int i = 1; i <= n; i ++) {
      M[i][j] = M[i][j - n];
    }
    j -= n;
  }
  for(int i = 1; i <= n; i ++) M[i][j] = 0;
}

bool pre_Gauss() {
  delta = 1; al_shift = 0;
  const int bd = n * (d + 1);
  for(int i = 1; i <= n; i ++) {
    const int col = n * d + i;
    int r = -1;
    while(true) {
      for(int j = i; j <= n; j ++) {
        if(M[j][col] != 0) {
          r = j; break;
        }
      }
      if(r != -1) break;
      if((++ al_shift) > n * d) return false;
      for(int j = 1; j < i; j ++) {
        const ll rate = ha - M[j][col];
        column_elminate(col, n * d + j, rate);
      }
      column_shift(col);
    }
    if(r != i) {
      delta = ha - delta;
      for(int j = 1; j <= bd; j ++) {
        std::swap(M[i][j], M[r][j]);
      }
    }
    const ll principal = M[i][col];
    delta = delta * principal % ha;
    const ll inv_principal = inv(principal);
    for(int j = 1; j <= bd; j ++) {
      M[i][j] = M[i][j] * inv_principal % ha;
    }
    for(int j = 1; j <= n; j ++) {
      if(j == i || M[j][col] == 0) continue;
      const ll rate = ha - M[j][col];
      for(int k = 1; k <= bd; k ++) {
        M[j][k] = (M[j][k] + rate * M[i][k]) % ha;
      }
    }
  }
  return true;
}
ll B[maxn << 1][maxn << 1];
void calc_B() {
  for(int i = 1; i <= (n * d - n); i ++) {
    B[i][i + n] = 1;
  }
  const int x_shift = n * d - n;
  for(int i = 1; i <= n; i ++) {
    ll *th = B[x_shift + i], *ot = M[i];
    for(int j = 1; j <= n * d; j ++) {
      th[j] = ha - ot[j];
      if(th[j] >= ha) th[j] -= ha;
    }
  }
}

// 下面两个函数的实现参考上一节
void to_Hessenberg();
ll poly[maxn][maxn];
void calc_charpoly();

int main() {
  scanf("%d%d", &n, &d);
  const int bd = n * d;
  for(int k = 0; k <= d; k ++) {
    for(int i = 1; i <= n; i ++) {
      for(int j = 1; j <= n; j ++) {
        ll x; scanf("%lld", &x);
        M[i][k * n + j] = x;
      }
    }
  }
  if(!pre_Gauss()) {
    for(int i = 0; i <= bd; i ++) {
      printf("0%c", " \n"[i == bd]);
    }
    return 0;
  }
  calc_B();
  to_Hessenberg();
  calc_charpoly();
  for(int i = 0; i <= bd; i ++) {
    int idx = i + al_shift;
    ll th;
    if(idx > bd) {
      th = 0;
    } else {
      th = poly[bd][idx] * delta % ha;
    }
    printf("%lld%c", th, " \n"[i == bd]);
  }
  return 0;
}
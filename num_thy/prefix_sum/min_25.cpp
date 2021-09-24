#include <bits/stdc++.h>
const int maxn = 400005;
using ll = long long;
int prm[maxn]; ll A[maxn], B[maxn];
ll sqr(ll x) {
  ll ret = 1;
  while((ret + 1LL) * (ret + 1LL) <= x) ret ++;
  return ret;
}

ll n, S;
int cnt;
void process() {
  S = sqr(n); cnt = 0;
  for(int i = 1; i <= S; i ++) {
    A[i] = i;
  }
  for(int i = 1; i <= S; i ++) {
    B[i] = n / (ll(i));
  }
  for(int i = 2; i <= S; i ++) {
    if(A[i] == A[i - 1]) continue;
    ll v = A[i - 1], lim = (ll(i)) * (ll(i));
    for(int j = 1; j <= (S / i); j ++) {
      B[j] -= B[j * i] - v;
    }
    for(int j = S / i + 1; j <= S; j ++) {
      ll th = n / (ll(j)); if(th < lim) break;
      B[j] -= A[th / (ll(i))] - v;
    }
    for(int j = S; (ll)j >= lim; j --) {
      A[j] -= A[j / (ll(i))] - v;
    }
    prm[++ cnt] = i;
  }
  prm[++ cnt] = S + 1LL;
}
inline ll query(ll x) {
  if(x <= S) {
    return A[x];
  } else {
    return B[n / x];
  }
}
ll calc(ll m, int x) {
  if(m < (ll)prm[x]) return 0;
  ll ret = 4LL * (query(m) - query(prm[x] - 1));
  for(int i = x; i <= cnt; i ++) {
    ll mul = prm[i];
    if(mul * (ll(prm[i])) > m) break;
    for(int j = 1; ; j ++) {
      if(mul * (ll(prm[i])) > m) break;
      ret += calc(m / mul, i + 1) * (ll(3 * j + 1)) + (ll(3 * j + 4));
      mul *= (ll(prm[i]));
    }
  }
  return ret;
}

int main() {
  int T; scanf("%d", &T);
  while(T --) {
    scanf("%lld", &n);
    process();
    printf("%lld\n", calc(n, 1) + 1LL);
  }
  return 0;
}
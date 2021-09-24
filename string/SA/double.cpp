#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cctype>
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
#include <queue>
const int maxn = 1000005;
inline int idx(char c) {
  return c - 'a' + 1;
}
char S[maxn]; int n;
int sa[maxn], rk[maxn];
int fir[maxn], sec[maxn];
int tmp[maxn], buf[maxn];
inline void build_SA() {
  for(int i = 1; i <= n; i ++) buf[S[i] + 1] ++;
  for(int i = 1; i <= 256; i ++) buf[i] += buf[i - 1];
  for(int i = 1; i <= n; i ++) rk[i] = buf[S[i]] + 1;
  for(int L = 1; L <= n; L <<= 1) {
    for(int i = 1; i <= n; i ++) fir[i] = rk[i];
    for(int i = 1; i <= n; i ++) {
      if(i + L > n) sec[i] = 0;
      else sec[i] = rk[i + L];
    }
    std::fill(buf, buf + 1 + n, 0);
    for(int i = 1; i <= n; i ++) buf[sec[i]] ++;
    for(int i = 1; i <= n; i ++) buf[i] += buf[i - 1];
    for(int i = 1; i <= n; i ++) tmp[buf[sec[i]] --] = i;

    std::fill(buf, buf + 1 + n, 0);
    for(int i = 1; i <= n; i ++) buf[fir[i]] ++;
    for(int i = 1; i <= n; i ++) buf[i] += buf[i - 1];
    for(int i = n; i >= 1; i --) {
      const int th = tmp[i];
      sa[buf[fir[th]] --] = th;
    }
    bool unique = true;
    for(int i = 1, las = 0; i <= n; i ++) {
      int th = sa[i];
      if(!las) {
        rk[th] = 1;
      } else {
        if(sec[th] == sec[las] && fir[th] == fir[las]) {
          unique = false;
          rk[th] = rk[las];
        } else {
          rk[th] = rk[las] + 1;
        }
      }
      las = th;
    }
    if(unique) break;
  }
}

int main() {
  scanf("%s", S + 1); n = strlen(S + 1);
  build_SA();
  for(int i = 1; i <= n; i ++) {
    if(i > 1) putchar(' ');
    printf("%d", sa[i]);
  }
  puts("");
  return 0;
}
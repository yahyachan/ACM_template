#include <bits/stdc++.h>
const int maxn = 407;
int o_deg[maxn], G[maxn][maxn];
void ins_edge(int u, int v) {
  o_deg[u] ++;
  G[u][v] ++;
}
 
typedef double real;
const real eps = 1e-10;
real labs(real x) {
  if(x < 0) return -x;
  else return x;
}
 
typedef real Mat[407][407];
void transform_1(Mat &A, const int n, int x, int y) {
  for(int i = 1; i <= n; i ++) std::swap(A[x][i], A[y][i]);
}
void transform_2(Mat &A, const int n, int dst, int src, real k) {
  for(int i = 1; i <= n; i ++) {
    A[dst][i] += A[src][i] * k;
  }
}
void transform_3(Mat &A, const int n, int x, real k) {
  for(int i = 1; i <= n; i ++) A[x][i] /= k;
}
 
int n;
void gauss(Mat &A, Mat &ret) {
  for(int i = 1; i <= n; i ++) {
    int r = -1;
    for(int j = i; j <= n + 1; j ++) {
      if(labs(A[j][i]) > eps) {
        r = j; break;
      }
    }
    assert(r != -1);
    if(r != i) transform_1(A, n + 1, r, i);
    for(int j = 1; j <= n + 1; j ++) {
      if(j == i) continue;
      if(!(A[j][i])) continue;
      const real k = -A[j][i] / A[i][i];
      transform_2(A, n + 1, j, i, k);
    }
  }
  for(int i = 1; i <= n; i ++) {
    ret[i][i] = A[i][i] / A[i][n + 1];
  }
}
void calc_X(Mat &A, Mat &B, Mat &X) {
  for(int i = 1; i <= n - 1; i ++) {
    int r = -1;
    for(int j = i; j <= n; j ++) {
      if(labs(A[j][i]) > eps) {
        r = j; break;
      }
    }
    if(i == n) {
      assert(r == -1);
      break;
    }
    assert(r != -1);
    if(r != i) {
      transform_1(A, n, r, i);
      transform_1(B, n, r, i);
    }
    real self_k = A[i][i];
    transform_3(A, n, i, self_k);
    transform_3(B, n, i, self_k);
    // assert(labs(A[i][i] - 1) < eps);
    for(int j = 1; j <= n; j ++) {
      if(j == i) continue;
      if(!(A[j][i])) continue;
      const real k = -A[j][i];
      transform_2(A, n, j, i, k);
      transform_2(B, n, j, i, k);
    }
  }
  // for(int j = 1; j <= n; j ++) assert(labs(A[n][j]) < eps);
  for(int j = 1; j <= n; j ++) B[n][j] = 0;
  for(int i = 1; i <= n; i ++) {
    for(int j = 1; j <= n; j ++) {
      X[i][j] = B[i][j] - B[j][j];
    }
  }
}
 
Mat A, B, X;
Mat trans_G, g;
int main() {
  int m, q; scanf("%d%d%d", &n, &m, &q);
  for(int i = 1; i <= m; i ++) {
    int u, v; scanf("%d%d", &u, &v);
    u ++; v ++; ins_edge(u, v);
  }
  for(int i = 1; i <= n; i ++) {
    for(int j = 1; j <= n; j ++) {
      A[i][j] = -(real(G[i][j])) / (real(o_deg[i]));
      if(i == j) A[i][j] += 1.00;
    }
  }
  for(int i = 1; i <= n; i ++) {
    for(int j = 1; j <= n; j ++) {
      real k = -A[j][i];
      trans_G[i][j] = k;
    }
    trans_G[i][n + 1] = 0;
  }
  for(int j = 1; j <= n + 1; j ++) trans_G[n + 1][j] = 1;
  gauss(trans_G, g);
  for(int i = 1; i <= n; i ++) {
    for(int j = 1; j <= n; j ++) {
      B[i][j] = 1.00;
      if(i == j) B[i][j] -= g[i][j];
    }
  }
  calc_X(A, B, X);
  while(q --) {
    int k; scanf("%d", &k);
    int fir; scanf("%d", &fir); k --; fir ++;
    real ans = 0;
    if(k == 0) ans = 1;
    while(k --) {
      int x; scanf("%d", &x); x ++;
      ans += X[fir][x]; fir = x;
    }
    printf("%.9f\n", (double)ans);
  }
  return 0;

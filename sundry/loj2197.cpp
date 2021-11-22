#include <bits/stdc++.h>
using ll = long long;
using T = ll;
struct Point {
  T x, y;
  Point(T qx = 0LL, T qy = 0LL) {
    x = qx; y = qy;
  }
};
using Vector = Point;
Vector operator +(const Vector &a, const Vector &b) {
  return Vector(a.x + b.x, a.y + b.y);
}
Vector operator -(const Point &a, const Point &b) {
  return Vector(a.x - b.x, a.y - b.y);
}
Vector operator *(const Vector &a, T lam) {
  return Vector(a.x * lam, a.y * lam);
}
Vector operator *(T lam, const Vector &a) {
  return Vector(a.x * lam, a.y * lam);
}
inline T dot(const Vector &a, const Vector &b) {
  return (a.x * b.x + a.y * b.y);
}
inline T times(const Vector &a, const Vector &b) {
  return (a.x * b.y - a.y * b.x);
}
inline bool cmp(const Point &a, const Point &b) {
  if((a.x - b.x) == 0LL) {
    return a.y < b.y;
  } else {
    return a.x < b.x;
  }
}
inline void andrew(Point *P, int L, std::vector<Point> &bot, std::vector<Point> &top) {
  std::sort(P + 1, P + 1 + L, cmp);
  for(int i = 1; i <= L; i ++) {
    if(i != 1 && (P[i].x - P[i - 1].x) == 0LL) continue;
    while(bot.size() > 1 && (times(P[i] - bot.back(), bot.back() - bot[bot.size() - 2])) >= 0LL) {
      bot.pop_back();
    }
    bot.push_back(P[i]);
  }
  for(int i = L; i >= 1; i --) {
    if(i != L && (P[i].x - P[i + 1].x) == 0LL) continue;
    while(top.size() > 1 && (times(P[i] - top.back(), top.back() - top[top.size() - 2])) >= 0LL) {
      top.pop_back();
    }
    top.push_back(P[i]);
  }
  std::reverse(top.begin(), top.end());
}
 
const int maxn = 400005;
const int maxno = maxn << 2;
const int N = 400000;
bool zen[maxno];
std::vector<Point> bot[maxno], top[maxno];
Point P[maxn];
inline void maintain(int o, int L, int R) {
  static Point tmp[maxn];
  const int lc = o << 1, rc = o << 1 | 1;
  const bool used = zen[o];
  zen[o] = (zen[lc] && zen[rc]);
  if(zen[o] != used) {
    std::copy(P + L, P + R + 1, tmp + 1);
    int len = R - L + 1;
    andrew(tmp, len, bot[o], top[o]);
  }
}
void modify(int o, int L, int R, const int &p, const Point &v) {
  if(L == R) {
    zen[o] = true;
    P[L] = v;
    bot[o].push_back(v); top[o].push_back(v);
  } else {
    const int M = (L + R) / 2;
    if(p <= M) {
      modify(o << 1, L, M, p, v);
    } else {
      modify(o << 1 | 1, M + 1, R, p, v);
    }
    maintain(o, L, R);
  }
}
inline T search(const std::vector<Point> &vec, const Point &p) {
  int l = 0, r = vec.size() - 1;
  while(r - l > 2) {
    int lm = (l * 2 + r) / 3, rm = (2 * r + l) / 3;
    if(dot(p, vec[lm]) > dot(p, vec[rm])) {
      r = rm;
    } else {
      l = lm;
    }
  }
  T ans = LLONG_MIN;
  for(int i = l; i <= r; i ++) {
    ans = std::max(ans, dot(p, vec[i]));
  }
  return ans;
}
T query(int o, int L, int R, const int &ql, const int &qr, const Point &p) {
  if(ql <= L && R <= qr) {
    if(p.y > 0LL) {
      return search(top[o], p);
    } else {
      return search(bot[o], p);
    }
  } else {
    int M = (L + R) / 2;
    T ans = LLONG_MIN;
    if(ql <= M) {
      ans = std::max(ans, query(o << 1, L, M, ql, qr, p));
    }
    if(qr > M) {
      ans = std::max(ans, query(o << 1 | 1, M + 1, R, ql, qr, p));
    }
    return ans;
  }
}
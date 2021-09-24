#include <bits/stdc++.h>
const int maxn = 5005;
const int maxm = 500005;
using pii = std::pair<int, int>;
struct Node {
  Node *fa; int siz;
  Node() {
    fa = NULL; siz = 1;
  }
  void sc(Node *c) {
    siz += c -> siz;
    c -> fa = this;
  }
  void brk() {
    fa -> siz -= siz;
    fa = NULL;
  }
};
Node *r[maxn];
int n;
void init_set() {
  for(int i = 1; i <= n; i ++) {
    r[i] = new Node();
  }
}
Node *get_fa(Node *x) {
  if(x -> fa == NULL) {
    return x;
  } else {
    return get_fa(x -> fa);
  }
}
Node *link_set(Node *x, Node *y) {
  if(x -> siz < y -> siz) std::swap(x, y);
  x -> sc(y); return y;
}
Node *merge_set(Node *x, Node *y) {
  return link_set(get_fa(x), get_fa(y));
}
bool is_same(Node *x, Node *y) {
  return (get_fa(x) == get_fa(y));
}

const int maxno = maxm << 2;
std::vector<pii> event[maxno];
void add_event(int o, int L, int R, int ql, int qr, const pii &e) {
  if(ql <= L && R <= qr) {
    event[o].push_back(e);
  } else {
    int M = (L + R) / 2;
    if(ql <= M) add_event(o << 1, L, M, ql, qr, e);
    if(qr > M) add_event(o << 1 | 1, M + 1, R, ql, qr, e);
  }
}
pii que[maxno];
void add_query(int o, int L, int R, int p, const pii &e) {
  if(L == R) {
    que[o] = e;
  } else {
    int M = (L + R) / 2;
    if(p <= M) {
      add_query(o << 1, L, M, p, e);
    } else {
      add_query(o << 1 | 1, M + 1, R, p, e);
    }
  }
}
int ret[maxno];
std::stack<std::pair<Node*, int> > S;
void solve(int o, int L, int R) {
  for(auto e : event[o]) {
    int u = e.first, v = e.second;
    if(!is_same(r[u], r[v])) {
      S.push(std::make_pair(merge_set(r[u], r[v]), o));
    }
  }
  if(L == R) {
    int u = que[o].first, v = que[o].second;
    if(u == -1 && v == -1) {
      ret[L] = -1;
    } else {
      if(is_same(r[u], r[v])) {
        ret[L] = 1;
      } else {
        ret[L] = 0;
      }
    }
  } else {
    int M = (L + R) / 2;
    solve(o << 1, L, M); solve(o << 1 | 1, M + 1, R);
  }
  while(!S.empty() && S.top().second >= o) {
    Node *u = S.top().first; S.pop();
    u -> brk();
  }
}

std::map<pii, std::stack<int> > ma;
std::vector<pii> V;
int main() {
  int m; scanf("%d%d", &n, &m);
  init_set();
  pii fl(-1, -1);
  for(int i = 1; i <= m; i ++) {
    pii e; int op;
    scanf("%d%d%d", &op, &e.first, &e.second);
    if(e.first > e.second) {
      std::swap(e.first, e.second);
    }
    if(op == 2) {
      add_query(1, 1, m, i, e);
    } else {
      add_query(1, 1, m, i, fl);
      V.push_back(e);
      if(op == 0) {
        ma[e].push(i);
      } else {
        int last = ma[e].top(); ma[e].pop();
        add_event(1, 1, m, last, i - 1, e);
      }
    }
  }
  for(auto e : V) {
    while(!ma[e].empty()) {
      int g = ma[e].top(); ma[e].pop();
      add_event(1, 1, m, g, m, e);
    }
  }
  solve(1, 1, m);
  for(int i = 1; i <= m; i ++) {
    if(ret[i] != -1) {
      if(ret[i]) {
        puts("Y");
      } else {
        puts("N");
      }
    }
  }
  return 0;
}
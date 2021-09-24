#include <bits/stdc++.h>
const double alpha = 0.75;
struct Node {
  Node *ch[2];
  int v, cnt, s;
  int del_cnt, del_s;
  int cmp(int x) {
    if(x == v) {
      return -1;
    } else {
      if(x < v) {
        return 0;
      } else {
        return 1;
      }
    }
  }
  void maintain() {
    s = cnt + ch[0] -> s + ch[1] -> s;
    del_s = del_cnt + ch[0] -> del_s + ch[1] -> del_s;
  }
  int rs() {
    return s - del_s;
  }
  int rcnt() {
    return cnt - del_cnt;
  }
};
const int maxno = 100005;
Node pool[maxno]; Node *sta[maxno];
Node *nil;
Node *tot; int stot;
void init_pool() {
  tot = nil = pool; stot = 0;
  nil -> ch[0] = nil -> ch[1] = nil;
  nil -> s = nil -> cnt = nil -> del_s = nil -> del_cnt = 0;
}
Node *alloc_node(int x, int cnt = 1) {
  Node *ret;
  if(stot > 0) {
    ret = sta[-- stot];
  } else {
    ret = ++ tot;
  }
  ret -> v = x; ret -> ch[0] = ret -> ch[1] = nil;
  ret -> s = ret -> cnt = cnt;
  ret -> del_s = ret -> del_cnt = 0;
  return ret;
}
void del_node(Node *x) {
  sta[stot ++] = x;
}

Node *build_tree(int *A, int *B, int L, int R) {
  if(L > R) return nil;
  int M = (L + R) / 2;
  Node *ret = alloc_node(A[M], B[M]);
  ret -> ch[0] = build_tree(A, B, L, M - 1);
  ret -> ch[1] = build_tree(A, B, M + 1, R);
  ret -> maintain();
  return ret;
}
void expand(Node *x, int *A, int *B, int &cnt) {
  if(x -> ch[0] != nil) {
    expand(x -> ch[0], A, B, cnt);
  }
  if(x -> cnt > x -> del_cnt) {
    A[++ cnt] = x -> v;
    B[cnt] = x -> rcnt();
  }
  if(x -> ch[1] != nil) {
    expand(x -> ch[1], A, B, cnt);
  }
  del_node(x);
}
Node *refact(Node *x) {
  static int A[maxno], B[maxno];
  int cnt = 0;
  expand(x, A, B, cnt);
  Node *ret = build_tree(A, B, 1, cnt);
  return ret;
}
Node *rt;
Node *check(Node *x, int v) {
  if(x == nil) return x;
  if((double)std::max(x -> ch[0] -> s, x -> ch[1] -> s) > alpha * (double)(x -> s)) {
    return refact(x);
  } else {
    int d = x -> cmp(v);
    if(d == 0) {
      x -> ch[0] = check(x -> ch[0], v);
    }
    if(d == 1) {
      x -> ch[1] = check(x -> ch[1], v);
    }
    x -> maintain();
    return x;
  }
}
void insert(Node* &x, int v) {
  if(x == nil) {
    x = alloc_node(v, 1);
    return;
  }
  int d = x -> cmp(v);
  if(d == -1) {
    x -> cnt ++;
  } else {
    insert(x -> ch[d], v);
  }
  x -> maintain();
}
void ins(int v) {
  insert(rt, v);
  rt = check(rt, v);
}
void erase(Node *x, int v) {
  int d = x -> cmp(v);
  if(d == -1) {
    x -> del_cnt ++;
  } else {
    erase(x -> ch[d], v);
  }
  x -> maintain();
}
void del(int v) {
  erase(rt, v);
  if(rt -> rs() < alpha * rt -> s) {
    rt = refact(rt);
  }
}
int kth(Node *x, int k) {
  if(k <= x -> ch[0] -> rs()) {
    return kth(x -> ch[0], k);
  } else if(k <= x -> ch[0] -> rs() + x -> rcnt()) {
    return x -> v;
  } else {
    return kth(x -> ch[1], k - x -> ch[0] -> rs() - x -> rcnt());
  }
}
int rank(Node *x, int v) {
  if(x == nil) return 1;
  int d = x -> cmp(v);
  if(d == 0) {
    return rank(x -> ch[0], v);
  } else if(d == -1) {
    return x -> ch[0] -> rs() + 1;
  } else {
    return x -> ch[0] -> rs() + x -> rcnt() + rank(x -> ch[1], v);
  }
}
int pre(int v) {
  int r = rank(rt, v);
  return kth(rt, r - 1);
}
int suc(int v) {
  int r = rank(rt, v + 1);
  return kth(rt, r);
}
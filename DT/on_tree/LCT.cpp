#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <functional>
#include <utility>
#include <random>
#include <map>
const int bufsiz = 128 * 1024 * 1024;
char buf[bufsiz]; char *cur = buf;
void *alloc(size_t size) {
  if(cur - buf + size > bufsiz) {
    return malloc(size);
  } else {
    char *ret = cur; cur += size;
    return ret;
  }
}
 
using ull = unsigned long long;
struct Node {
  Node *ch[2], *fa;
  ull val, lz; bool flag;
  int d() {
    return (fa -> ch[1] == this);
  }
  void sc(Node *c, int dir) {
    ch[dir] = c; c -> fa = this;
  }
  void paint(ull v) {
    val ^= v; lz ^= v;
  }
  void paint() {
    flag ^= 1; std::swap(ch[0], ch[1]);
  }
  void pushdown() {
    if(flag) {
      ch[0] -> paint();
      ch[1] -> paint();
      flag = false;
    }
    if(lz != 0ULL) {
      ch[0] -> paint(lz);
      ch[1] -> paint(lz);
      lz = 0;
    }
  }
};
Node *nil;
void init_pool() {
  nil = (Node*)alloc(sizeof(Node));
  nil -> ch[0] = nil -> ch[1] = nil; nil -> fa = nil;
  nil -> lz = nil -> val = 0; nil -> flag = false;
}
Node *alloc_node() {
  Node *ret = (Node*)alloc(sizeof(Node));
  ret -> ch[0] = ret -> ch[1] = ret -> fa = nil;
  ret -> lz = ret -> val = 0; ret -> flag = false;
  return ret;
}
 
bool is_root(Node *o) {
  return (o -> fa == nil || (o -> fa -> ch[0] != o && o -> fa -> ch[1] != o));
}
void zig(Node *x) {
  int d = x -> d(); Node *y = x -> fa;
  if(is_root(y)) {
    x -> fa = y -> fa;
  } else {
    y -> fa -> sc(x, y -> d());
  }
  y -> sc(x -> ch[d ^ 1], d);
  x -> sc(y, d ^ 1);
}
void splay(Node *x) {
  while(!is_root(x)) {
    Node *y = x -> fa;
    if(!is_root(y)) y -> fa -> pushdown();
    y -> pushdown(); x -> pushdown();
    if(!is_root(y)) {
      if((x -> d()) ^ (y -> d())) {
        zig(x);
      } else {
        zig(y);
      }
    }
    zig(x);
  }
}
void access(Node *x) {
  Node *vx = x, *y = nil;
  while(x != nil) {
    splay(x); x -> pushdown();
    x -> sc(y, 1);
    y = x; x = x -> fa;
  }
  splay(vx);
}
void evert(Node *x) {
  access(x); x -> paint();
}
void link(Node *x, Node *y) {
  evert(x); x -> fa = y;
}
void cut(Node *x, Node *y) {
  evert(x); access(y);
  int d = x -> d();
  y -> ch[d] = nil; x -> fa = nil;
}
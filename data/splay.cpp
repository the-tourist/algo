namespace splay_tree {

bool is_bst_root(node* v) {
  if (v == nullptr) {
    return false;
  }
  return (v->p == nullptr || (v->p->l != v && v->p->r != v));
}

void rotate(node* v) {
  node* u = v->p;
  assert(u != nullptr);
  u->push();
  v->push();
  v->p = u->p;
  if (v->p != nullptr) {
    if (v->p->l == u) {
      v->p->l = v;
    }
    if (v->p->r == u) {
      v->p->r = v;
    }
  }
  if (v == u->l) {
    u->l = v->r;
    v->r = u;
  } else {
    u->r = v->l;
    v->l = u;
  }
  u->pull();
  v->pull();
}

void splay(node* v) {
  if (v == nullptr) {
    return;
  }
  while (!is_bst_root(v)) {
    node* u = v->p;
    if (!is_bst_root(u)) {
      if ((u->l == v) ^ (u->p->l == u)) {
        rotate(v);
      } else {
        rotate(u);
      }
    }
    rotate(v);
  }
}

pair<node*, int> find(node* v, const function<int(node*)> &go_to) {
  // go_to returns: 0 -- found; -1 -- go left; 1 -- go right
  // find returns the last vertex on the descent and its go_to
  if (v == nullptr) {
    return {nullptr, 0};
  }
  splay(v);
  int dir;
  while (true) {
    v->push();
    dir = go_to(v);
    if (dir == 0) {
      break;
    }
    node* u = (dir == -1 ? v->l : v->r);
    if (u == nullptr) {
      break;
    }
    v = u;
  }
  splay(v);
  return {v, dir};
}

node* get_leftmost(node* v) {
  return find(v, [&](node*) { return -1; }).first;
}

node* get_rightmost(node* v) {
  return find(v, [&](node*) { return 1; }).first;
}

node* get_kth(node* v, int k) { // 0-indexed
  pair<node*, int> p = find(v, [&](node* u) {
    if (u->l != nullptr) {
      if (u->l->sz > k) {
        return -1;
      }
      k -= u->l->sz;
    }
    if (k == 0) {
      return 0;
    }
    k--;
    return 1;
  });
  return (p.second == 0 ? p.first : nullptr);
}

int get_position(node* v) { // 0-indexed
  splay(v);
  return (v->l != nullptr ? v->l->sz : 0);
}

node* get_bst_root(node* v) {
  splay(v);
  return v;
}

pair<node*, node*> split(node* v, const function<bool(node*)> &is_right) {
  if (v == nullptr) {
    return {nullptr, nullptr};
  }
  pair<node*, int> p = find(v, [&](node* u) { return is_right(u) ? -1 : 1; });
  v = p.first;
  v->push();
  if (p.second == -1) {
    node* u = v->l;
    if (u == nullptr) {
      return {nullptr, v};
    }
    v->l = nullptr;
    u->p = v->p;
    u = get_rightmost(u);
    v->p = u;
    v->pull();
    return {u, v};
  } else {
    node* u = v->r;
    if (u == nullptr) {
      return {v, nullptr};
    }
    v->r = nullptr;
    v->pull();
    return {v, u};
  }
}

pair<node*, node*> split_leftmost_k(node* v, int k) {
  return split(v, [&](node* u) {
    int left_and_me = (u->l != nullptr ? u->l->sz : 0) + 1;
    if (k >= left_and_me) {
      k -= left_and_me;
      return false;
    }
    return true;
  });
}

node* merge(node* v, node* u) {
  if (v == nullptr) {
    return u;
  }
  if (u == nullptr) {
    return v;
  }
  v = get_rightmost(v);
  assert(v->r == nullptr);
  splay(u);
  v->push();
  v->r = u;
  v->pull();
  return v;
}

int count_left(node* v, const function<bool(node*)> &is_right) {
  if (v == nullptr) {
    return 0;
  }
  pair<node*, int> p = find(v, [&](node* u) { return is_right(u) ? -1 : 1; });
  node* u = p.first;
  return (u->l != nullptr ? u->l->sz : 0) + (p.second == 1);
}

node* add(node* r, node* v, const function<bool(node*)> &go_left) {
  pair<node*, node*> p = split(r, go_left);
  return merge(p.first, merge(v, p.second));
}

node* remove(node* v) { // returns the new root
  splay(v);
  v->push();
  node* x = v->l;
  node* y = v->r;
  v->l = v->r = nullptr;
  node* z = merge(x, y);
  if (z != nullptr) {
    z->p = v->p;
  }
  v->p = nullptr;
  v->push();
  v->pull(); // now v might be reusable...
  return z;
}

node* next(node* v) {
  splay(v);
  v->push();
  if (v->r == nullptr) {
    return nullptr;
  }
  v = v->r;
  while (v->l != nullptr) {
    v->push();
    v = v->l;
  }
  splay(v);
  return v;
}

node* prev(node* v) {
  splay(v);
  v->push();
  if (v->l == nullptr) {
    return nullptr;
  }
  v = v->l;
  while (v->r != nullptr) {
    v->push();
    v = v->r;
  }
  splay(v);
  return v;
}

int get_size(node* v) {
  splay(v);
  return (v != nullptr ? v->sz : 0);
}

template<typename... T>
void do_apply(node* v, T... args) {
  splay(v);
  v->unsafe_apply(args...);
}

void reverse(node* v) {
  splay(v);
  v->unsafe_reverse();
}

}  // namespace splay_tree

using namespace splay_tree;

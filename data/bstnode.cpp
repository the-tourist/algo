class node {
 public:
  int id;
  node* l;
  node* r;
  node* p;
  bool rev;
  int sz;
  // declare extra variables:


  node(int _id) {
    id = _id;
    l = r = p = nullptr;
    rev = false;
    sz = 1;
    // init extra variables:

  }

  // push everything else:
  void push_stuff() {
    
  }

  void unsafe_reverse() {
    push_stuff(); // !! edu 112
    rev ^= 1;
    swap(l, r);
    pull();
  }

  // apply changes:
  void unsafe_apply() {
    
  }

  void push() {
    if (rev) {
      if (l != nullptr) {
        l->unsafe_reverse();
      }
      if (r != nullptr) {
        r->unsafe_reverse();
      }
      rev = 0;
    }
    push_stuff();
  }

  void pull() {
    sz = 1;
    // now init from self:

    if (l != nullptr) {
      l->p = this;
      sz += l->sz;
      // now pull from l:

    }
    if (r != nullptr) {
      r->p = this;
      sz += r->sz;
      // now pull from r:

    }
  }
};

void debug_node(node* v, string pref = "") {
  #ifdef LOCAL
    if (v != nullptr) {
      debug_node(v->r, pref + " ");
      cerr << pref << "-" << " " << v->id << '\n';
      debug_node(v->l, pref + " ");
    } else {
      cerr << pref << "-" << " " << "nullptr" << '\n';
    }
  #endif
}

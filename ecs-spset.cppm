export module ecs:spset;

namespace ecs {
export template <typename Tp, unsigned Max> class sparse_set {
  struct dense {
    Tp value;
    unsigned id;
  };

  dense m_dense[Max]{};
  unsigned m_sparse[Max]{}; // contains id+1
  unsigned m_n{};

public:
  constexpr void add(unsigned id, Tp &&v) {
    // TODO: delete "dense" if `id` exists or fail?
    m_dense[m_n++] = {v, id};
    m_sparse[id] = m_n;
  }

  constexpr void set(unsigned id, Tp &&v) {
    // TODO: upsert or fail?
    m_dense[m_sparse[id] - 1].value = v;
  }

  [[nodiscard]] constexpr bool has(unsigned id) const {
    return m_sparse[id] != 0;
  }

  constexpr void remove_if(auto &&fn) {
    for (auto ri = 0U; ri < m_n; ri++) {
      unsigned i = m_n - ri - 1U;
      const auto &[v, id] = m_dense[i];
      if (fn(v, id))
        remove(id);
    }
  }
  constexpr void remove(unsigned id) {
    auto index = m_sparse[id] - 1;
    m_sparse[id] = 0;
    m_n--;
    if (index != m_n) {
      m_dense[index] = m_dense[m_n];
      m_sparse[m_dense[index].id] = index + 1;
    }
  }

  [[nodiscard]] constexpr auto *begin() const { return &m_dense[0]; }
  [[nodiscard]] constexpr auto *end() const { return &m_dense[m_n]; }
};
} // namespace ecs

namespace {
static constexpr auto build_set() {
  ecs::sparse_set<int, 50> set{};
  set.add(20, 99);
  set.add(40, 15);
  set.add(30, 12);
  return set;
}
static constexpr bool check_all(const auto &set) {
  return set.has(20) && set.has(40) && set.has(30);
}

static_assert([] {
  auto set = build_set();
  return check_all(set);
});
static_assert([] {
  auto set = build_set();
  set.remove(40);
  return set.has(20) && !set.has(40) && set.has(30);
}());
static_assert([] {
  auto set = build_set();
  set.remove(30);
  return set.has(20) && set.has(40) && !set.has(30);
}());
static_assert([] {
  auto set = build_set();
  set.remove(20);
  set.remove(40);
  return !set.has(20) && !set.has(40) && set.has(30);
}());
static_assert([] {
  auto set = build_set();
  set.remove_if([](auto v, auto id) { return v == 12 && id == 30; });
  return set.has(20) && set.has(40) && !set.has(30);
}());
} // namespace

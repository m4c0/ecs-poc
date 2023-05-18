export module ecs;

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
    // TODO: delete "dense" if `id` exists?

    m_dense[m_n++] = {v, id};
    m_sparse[id] = m_n;
  }

  [[nodiscard]] constexpr bool has(unsigned id) const {
    return m_sparse[id] != 0;
  }

  constexpr void remove_if(auto &&fn) {
    for (auto ri = 0U; ri < m_n; ri++) {
      unsigned i = m_n - ri - 1U;
      const auto &[v, id] = m_dense[i];
      if (!fn(v, id))
        continue;

      m_sparse[id] = 0;
      m_n--;
      if (i != m_n) {
        m_dense[i] = m_dense[m_n];
        m_sparse[m_dense[i].id] = i;
      }
    }
  }

  [[nodiscard]] constexpr auto *begin() const { return &m_dense[0]; }
  [[nodiscard]] constexpr auto *end() const { return &m_dense[m_n]; }
};
} // namespace ecs

static_assert([] {
  ecs::sparse_set<int, 50> set{};
  set.add(20, 99);
  set.add(40, 15);
  set.add(30, 12);

  if (!set.has(20) || !set.has(40) || !set.has(30))
    return false;

  set.remove_if([](auto v, auto id) { return v == 15 && id == 40; });
  if (!set.has(20) || set.has(40) || !set.has(30))
    return false;

  set.remove_if([](auto v, auto id) { return v == 12 && id == 30; });
  if (!set.has(20) || set.has(40) || set.has(30))
    return false;

  set.remove_if([](auto v, auto id) { return v == 99 && id == 20; });
  if (set.has(20) || set.has(40) || set.has(30))
    return false;

  return true;
}());

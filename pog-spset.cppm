export module pog:spset;
import :eid;

namespace pog {
export template <typename Tp, unsigned Max> class sparse_set {
  struct dense {
    Tp value;
    eid id;
  };

  dense m_dense[Max]{};
  unsigned m_sparse[Max]{}; // contains id+1
  unsigned m_n{};

public:
  constexpr void add(eid id, Tp v) {
    // TODO: delete "dense" if `id` exists or fail?
    m_dense[m_n++] = {v, id};
    m_sparse[id] = m_n;
  }

  constexpr void update(eid id, Tp v) {
    // TODO: upsert or fail?
    m_dense[m_sparse[id] - 1].value = v;
  }

  [[nodiscard]] constexpr Tp get(eid id) const noexcept {
    return has(id) ? m_dense[m_sparse[id] - 1].value : Tp{};
  }
  [[nodiscard]] constexpr bool has(eid id) const { return m_sparse[id] != 0; }

  constexpr void for_each_r(auto &&fn) {
    for (auto ri = 0U; ri < m_n; ri++) {
      unsigned i = m_n - ri - 1U;
      const auto &[v, id] = m_dense[i];
      fn(v, id);
    }
  }

  constexpr void remove_if(auto &&fn) {
    for (auto ri = 0U; ri < m_n; ri++) {
      unsigned i = m_n - ri - 1U;
      const auto &[v, id] = m_dense[i];
      if (fn(v, id))
        remove(id);
    }
  }
  constexpr void remove(eid id) {
    if (!id)
      return;

    auto &sid = m_sparse[id];
    if (!sid)
      return;

    auto index = sid - 1;
    sid = 0;
    m_n--;
    if (index != m_n) {
      m_dense[index] = m_dense[m_n];
      m_sparse[m_dense[index].id] = index + 1;
    }
  }

  [[nodiscard]] constexpr auto *begin() const noexcept { return &m_dense[0]; }
  [[nodiscard]] constexpr auto *end() const noexcept { return &m_dense[m_n]; }
  [[nodiscard]] constexpr auto *begin() noexcept { return &m_dense[0]; }
  [[nodiscard]] constexpr auto *end() noexcept { return &m_dense[m_n]; }
  [[nodiscard]] constexpr auto size() const noexcept { return m_n; }
};
} // namespace pog

namespace {
static constexpr auto build_set() {
  pog::sparse_set<int, 50> set{};
  set.add(pog::eid{20}, 2);
  set.add(pog::eid{40}, 4);
  set.add(pog::eid{30}, 3);
  return set;
}
static constexpr bool set_matches(const pog::sparse_set<int, 50> &set,
                                  auto... ids) {
  if (!(set.has(pog::eid{static_cast<unsigned>(ids)}) && ...))
    throw 0;

  if (set.size() != sizeof...(ids))
    throw 1;

  auto p = set.begin();
  unsigned ns[] = {static_cast<unsigned>(ids)...};
  for (auto n : ns) {
    const auto &[v, id] = *p++;
    if (id != pog::eid{n})
      throw 2;
    if (v != n / 10)
      throw 3;
  }

  return true;
}

static_assert([] {
  auto set = build_set();
  return !set.has(pog::eid{}) && !set.has(pog::eid{1});
}());
static_assert([] {
  auto set = build_set();
  return set_matches(set, 20, 40, 30);
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{40});
  return set_matches(set, 20, 30);
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{30});
  return set_matches(set, 20, 40);
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{20});
  set.remove(pog::eid{40});
  return set_matches(set, 30);
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{20});
  set.remove(pog::eid{40});
  set.remove(pog::eid{30});
  return set_matches(set);
}());
static_assert([] {
  auto set = build_set();
  set.remove_if([](auto v, auto id) { return v == 3 && id == 30; });
  return set_matches(set, 20, 40);
}());
static_assert([] {
  auto set = build_set();
  set.remove_if([](auto v, auto id) { return true; });
  return set_matches(set);
}());
static_assert([] {
  build_set().remove(pog::eid{});
  build_set().remove(pog::eid{1});
  return true;
}());
} // namespace

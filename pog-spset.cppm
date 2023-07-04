export module pog:spset;
import :eid;
import hai;

namespace pog {
// TODO: consider a different structure to store the "sparse index"
export template <typename Tp> class sparse_set {
  static constexpr const auto initial_dense_cap = 16;
  static constexpr const auto dense_cap_increase = 16;

  static constexpr const auto initial_sparse_cap = 16;
  static constexpr const auto sparse_cap_increase = 16;

  struct dense {
    Tp value;
    eid id;
  };

  hai::varray<dense> m_dense;
  hai::array<unsigned> m_sparse;

  static constexpr void swapy(auto &a, auto &b) noexcept {
    auto tmp = a;
    a = b;
    b = tmp;
  }
  constexpr void swap(unsigned a, unsigned b) noexcept {
    if (a == b)
      return;

    swapy(m_dense[a], m_dense[b]);
    swapy(m_sparse[m_dense[a].id], m_sparse[m_dense[b].id]);
  }

public:
  explicit constexpr sparse_set()
      : m_dense{initial_dense_cap}, m_sparse{initial_sparse_cap} {}

  constexpr void add(eid id, Tp v) {
    // TODO: delete "dense" if `id` exists or fail?
    if (m_dense.size() == m_dense.capacity())
      m_dense.add_capacity(dense_cap_increase);
    if (m_sparse.size() < id + 1)
      m_sparse.add_capacity(id - m_sparse.size() + sparse_cap_increase);

    m_dense.push_back(dense{v, id});
    m_sparse[id] = m_dense.size();
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
    for (auto ri = m_dense.size(); ri != 0; ri--) {
      const auto &[v, id] = m_dense[ri - 1];
      fn(v, id);
    }
  }

  constexpr void remove_if(auto &&fn) {
    for_each_r([&](auto v, auto id) {
      if (fn(v, id))
        remove(id);
    });
  }
  constexpr void remove(eid id) {
    if (!id)
      return;

    auto &sid = m_sparse[id];
    if (!sid)
      return;

    swap(sid - 1, m_dense.size() - 1);
    sid = 0;
    m_dense.pop_back();
  }

  constexpr void sort(auto &&fn) {
    for (auto i = 0U; i < m_dense.size(); i++) {
      for (auto j = i + 1; j < m_dense.size(); j++) {
        auto a = m_dense[i].value;
        auto b = m_dense[j].value;
        if (fn(a, b) > 0)
          swap(i, j);
      }
    }
  }

  [[nodiscard]] constexpr auto *begin() const noexcept {
    return m_dense.begin();
  }
  [[nodiscard]] constexpr auto *end() const noexcept { return m_dense.end(); }
  [[nodiscard]] constexpr auto *begin() noexcept { return m_dense.begin(); }
  [[nodiscard]] constexpr auto *end() noexcept { return m_dense.end(); }
  [[nodiscard]] constexpr auto size() const noexcept { return m_dense.size(); }
};
} // namespace pog

namespace {
static constexpr auto build_set() {
  pog::sparse_set<int> set{};
  set.add(pog::eid{20}, 2);
  set.add(pog::eid{40}, 4);
  set.add(pog::eid{30}, 3);
  return set;
}
static constexpr bool set_matches(const pog::sparse_set<int> &set,
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
  set.sort([](auto a, auto b) { return a - b; });
  return set_matches(set, 20, 30, 40);
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{40});
  return set_matches(set, 20, 30);
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{20});
  set.add(pog::eid{10}, 1);
  set.sort([](auto a, auto b) { return a - b; });
  return set_matches(set, 10, 30, 40);
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
  set.sort([](auto a, auto b) { return a - b; });
  return set_matches(set);
}());
static_assert([] {
  auto set = build_set();
  set.remove_if([](auto v, auto id) { return true; });
  set.add(pog::eid{40}, 4);
  set.add(pog::eid{30}, 3);
  set.add(pog::eid{20}, 2);
  set.add(pog::eid{10}, 1);
  set.sort([](auto a, auto b) { return a - b; });
  return set_matches(set, 10, 20, 30, 40);
}());
static_assert([] {
  build_set().add(pog::eid{50}, 5);
  return true;
}());
static_assert([] {
  build_set().remove(pog::eid{});
  build_set().remove(pog::eid{1});
  return true;
}());
} // namespace

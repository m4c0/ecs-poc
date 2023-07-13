export module pog:rtree;
import :eid;
import :spset;
import rtree;

namespace pog {
export using aabb = rtree::aabb;

export class rtree {
  ::rtree::db::storage m_storage{};
  ::rtree::tree m_tree{&m_storage};
  using rid = ::rtree::db::nnid;

  sparse_set<aabb> m_index{};

public:
  constexpr void add(eid id, aabb area) {
    m_tree.insert(rid{id}, area);
    m_index.add(id, area);
  }

  [[nodiscard]] constexpr aabb get(eid id) const { return m_index.get(id); }
  [[nodiscard]] constexpr bool has(eid id) const { return m_index.has(id); }

  constexpr void for_each_in(aabb area, auto &&fn) const {
    m_tree.for_each_in(
        area, [&](auto nnid, auto narea) { fn(eid{nnid.index()}, narea); });
  }

  [[nodiscard]] constexpr bool has_in(aabb area) const noexcept {
    bool found = false;
    m_tree.for_each_in(area, [&](auto id, auto a) { found = true; });
    return found;
  }

  constexpr void remove(eid id) {
    if (has(id) && m_tree.remove(rid{id}, get(id))) {
      m_index.remove(id);
    }
  }
};
} // namespace pog

static_assert([] {
  pog::rtree t{};
  t.add(pog::eid{10}, {{1, 2}, {5, 9}});
  t.add(pog::eid{20}, {{3, 4}, {9, 8}});

  if (!t.has(pog::eid{10}))
    throw 0;
  if (!t.has_in(t.get(pog::eid{10})))
    throw 0;

  auto found = false;
  t.for_each_in({{1, 2}, {5, 9}}, [&](pog::eid id, pog::aabb) {
    if (id == pog::eid{10})
      found = true;
  });
  if (!found)
    throw 0;

  t.remove(pog::eid{10});
  if (!t.has(pog::eid{20}))
    throw 0;
  if (t.has(pog::eid{10}))
    throw 0;

  t.remove(pog::eid{20});
  if (t.has(pog::eid{10}) || t.has(pog::eid{20}))
    throw 0;

  t.remove(pog::eid{40});

  return true;
}());

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
  void add(eid id, aabb area) {
    m_tree.insert(rid{id}, area);
    m_index.add(id, area);
  }

  [[nodiscard]] aabb get(eid id) const { return m_index.get(id); }
  [[nodiscard]] bool has(eid id) const { return m_index.has(id); }

  void for_each_in(aabb area, auto &&fn) const {
    m_tree.for_each_in(
        area, [&](auto nnid, auto narea) { fn(eid{nnid.index()}, narea); });
  }

  [[nodiscard]] bool has_in(aabb area) const noexcept {
    bool found = false;
    m_tree.for_each_in(area, [&](auto id, auto a) { found = true; });
    return found;
  }

  auto remove(eid id) {
    auto area = get(id);
    m_index.remove(id);
    return m_tree.remove(rid{id}, area);
  }
};
} // namespace pog

namespace {
inline void instance_testing() {
  pog::rtree t{};
  t.for_each_in({}, [](pog::eid, pog::aabb) {});
}
} // namespace

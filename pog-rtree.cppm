export module pog:rtree;
import :eid;
import :spset;
import rtree;

namespace pog {
export using aabb = rtree::aabb;

export class rtree {
  ::rtree::db::storage m_storage{};
  ::rtree::tree m_tree{};
  using rid = ::rtree::db::nnid;

  sparse_set<aabb> m_index{};

public:
  void add(eid id, aabb area) {
    ::rtree::db::current() = &m_storage;
    m_tree.insert(rid{id}, area);
    m_index.add(id, area);
  }

  [[nodiscard]] aabb get(eid id) { return m_index.get(id); }
  [[nodiscard]] bool has(eid id) { return m_index.has(id); }

  void for_each_in(aabb area, auto &&fn) {
    ::rtree::db::current() = &m_storage;
    m_tree.for_each_in(
        area, [&](auto nnid, auto narea) { fn(eid{nnid.index()}, narea); });
  }

  [[nodiscard]] bool has_in(aabb area) noexcept {
    ::rtree::db::current() = &m_storage;
    bool found = false;
    m_tree.for_each_in(area, [&](auto id, auto a) { found = true; });
    return found;
  }

  auto remove(eid id) {
    ::rtree::db::current() = &m_storage;
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

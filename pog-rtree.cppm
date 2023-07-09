export module pog:rtree;
import :eid;
import rtree;

namespace pog {
export using aabb = rtree::aabb;

export class rtree {
  ::rtree::db::storage m_storage{};
  ::rtree::tree m_tree{};

  constexpr static auto to_nnid(eid id) noexcept {
    return ::rtree::db::nnid{id};
  }

public:
  void add(eid id, aabb area) {
    ::rtree::db::current() = &m_storage;
    m_tree.insert(to_nnid(id), area);
  }

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

  auto remove(eid id, aabb area) {
    ::rtree::db::current() = &m_storage;
    return m_tree.remove(to_nnid(id), area);
  }
};
} // namespace pog

namespace {
inline void instance_testing() {
  pog::rtree t{};
  t.for_each_in({}, [](pog::eid, pog::aabb) {});
}
} // namespace

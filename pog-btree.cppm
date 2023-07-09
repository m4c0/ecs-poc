export module pog:btree;
import :eid;
import btree;

namespace pog {
export template <typename Tp> class btree {
  ::btree::db::storage m_storage{Tp{}};
  ::btree::tree<Tp> m_tree{};

  constexpr static auto to_nnid(eid id) noexcept {
    return ::btree::db::nnid{id};
  }

public:
  void add(eid id, Tp v) {
    ::btree::db::current() = &m_storage;
    m_tree.insert(to_nnid(id), v);
  }

  void update(eid id, Tp v) {
    ::btree::db::current() = &m_storage;
    m_tree.remove(to_nnid(id));
    m_tree.insert(to_nnid(id), v);
  }
  void remove(eid id) {
    ::btree::db::current() = &m_storage;
    m_tree.remove(to_nnid(id));
  }

  [[nodiscard]] Tp get(eid id) {
    ::btree::db::current() = &m_storage;
    return m_tree.get(to_nnid(id)).unwrap(Tp{});
  }
  [[nodiscard]] bool has(eid id) {
    ::btree::db::current() = &m_storage;
    return m_tree.has(to_nnid(id));
  }
};
} // namespace pog

namespace {
inline void instancing_test() {
  pog::btree<long> tree{};
  tree.add(pog::eid{11}, 10L);
  auto _ = tree.get(pog::eid{11});
  if (tree.has(pog::eid{11}))
    tree.update(pog::eid{11}, 99L);
  tree.remove(pog::eid{11});
}
} // namespace

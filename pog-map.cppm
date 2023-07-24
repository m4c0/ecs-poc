export module pog:map;
import :eid;
import btree;
import traits;

namespace pog {
/// Maps an EID into another EID
export class map {
  btree::db::storage m_storage{};
  btree::tree m_index{&m_storage};

  using nnid = btree::db::nnid;

public:
  constexpr map() = default;

  map(const map &) = delete;
  map &operator=(const map &) = delete;

  constexpr map(map &&o) : m_storage{traits::move(o.m_storage)} {
    m_index.set_root(o.m_index.root());
  }
  constexpr map &operator=(map &&o) {
    m_storage = traits::move(o.m_storage);
    m_index.set_root(o.m_index.root());
    return *this;
  }

  constexpr void set(eid from, eid to) { m_index.insert(nnid{from}, nnid{to}); }

  [[nodiscard]] constexpr eid get(eid id) const {
    return eid{m_index.get(nnid{id}).index()};
  }
  [[nodiscard]] constexpr bool has(eid id) const {
    return m_index.has(nnid{id});
  }

  constexpr void remove(eid id) { m_index.remove(nnid{id}); }
};
} // namespace pog

export module pog:map;
import :eid;

namespace pog {
/// More a kv-map than a hash-map, in case you didn't noticed... :P
export template <typename Tp, unsigned Max> class map {
  struct slot {
    Tp value;
    bool in_use;
  };
  slot m_data[Max]{};

public:
  constexpr void set(eid id, Tp v) { m_data[id] = {v, true}; }

  [[nodiscard]] constexpr Tp get(eid id) const noexcept {
    auto [v, used] = m_data[id];
    return used ? v : Tp{};
  }
  [[nodiscard]] constexpr bool has(eid id) const { return m_data[id].in_use; }

  constexpr void remove(eid id) { m_data[id] = {}; }
};
} // namespace pog

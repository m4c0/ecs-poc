export module ecs:grid;
import :eid;

namespace ecs {
export template <unsigned W, unsigned H, unsigned Max> class grid {
public:
  struct coord {
    unsigned x;
    unsigned y;
  };

  constexpr void add(eid id, unsigned x, unsigned y) {}
  constexpr void set(eid id, unsigned x, unsigned y) {}

  [[nodiscard]] constexpr coord get(eid id) const noexcept { return {}; }
  [[nodiscard]] constexpr bool has(eid id) const noexcept { return false; }

  constexpr void remove_if(auto &&fn) {}
  constexpr void remove(eid id) {}
};
} // namespace ecs

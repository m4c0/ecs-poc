export module pog:ll;

namespace pog::ll {
struct node {
  unsigned id;
  unsigned next;
};
class list {
public:
  [[nodiscard]] constexpr bool empty() const noexcept { return true; }

  constexpr unsigned pop() noexcept { return 1; }
  constexpr void add(unsigned v) noexcept {}
};
} // namespace pog::ll

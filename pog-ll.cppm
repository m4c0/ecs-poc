export module pog:ll;

namespace pog::ll {
struct node {
  unsigned id;
  unsigned next;
};
class list {
public:
  [[nodiscard]] constexpr bool empty() const noexcept { return true; }
};
} // namespace pog::ll

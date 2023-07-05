export module pog:eid;
import :ll;

namespace pog {
export class marker {};

export class eid {
  unsigned m_value;

public:
  constexpr eid() = default;
  explicit constexpr eid(unsigned v) : m_value{v} {}

  [[nodiscard]] constexpr operator unsigned() const noexcept { return m_value; }
};

export class entity_list {
  ll::list m_recycle_list{};
  unsigned m_max{};

public:
  explicit constexpr entity_list() {}

  [[nodiscard]] constexpr eid alloc() {
    if (m_recycle_list.empty()) {
      return eid{++m_max};
    }
    return eid{m_recycle_list.pop()};
  }

  constexpr void dealloc(eid e) { m_recycle_list.add(e); }
};
} // namespace pog

namespace {
static_assert([] {
  pog::entity_list ents{};
  return ents.alloc() == 1 && ents.alloc() == 2 && ents.alloc() == 3;
}());
static_assert([] {
  pog::entity_list ents{};
  auto a = ents.alloc();
  auto b = ents.alloc();
  ents.dealloc(ents.alloc());
  auto c = ents.alloc();
  return c && c != a && c != b;
}());
static_assert([] {
  pog::entity_list ents{};
  auto a = ents.alloc();
  auto b = ents.alloc();
  auto c = ents.alloc();
  ents.dealloc(a);
  ents.dealloc(b);
  ents.dealloc(ents.alloc());
  ents.dealloc(ents.alloc());
  return ents.alloc() == a;
}());
} // namespace

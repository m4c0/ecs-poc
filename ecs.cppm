export module ecs;
export import :spset;

import hai;

namespace ecs {
class eid {
  unsigned m_value;

public:
  constexpr eid() = default;
  explicit constexpr eid(unsigned v) : m_value{v} {}

  [[nodiscard]] constexpr operator unsigned() const noexcept { return m_value; }
};

template <unsigned Max> class entity_list {
  bool m_allocated[Max]{};
  unsigned m_count{};

public:
  [[nodiscard]] constexpr eid alloc() {
    if (m_count == Max)
      return eid{0};

    return eid{++m_count};
  }
};
} // namespace ecs

namespace {
static_assert([] {
  ecs::entity_list<3> ents;
  return ents.alloc() == 1 && ents.alloc() == 2 && ents.alloc() == 3;
}());
static_assert([] {
  ecs::entity_list<3> ents;
  auto a = ents.alloc();
  auto b = ents.alloc();
  auto c = ents.alloc();
  return !ents.alloc();
}());
} // namespace

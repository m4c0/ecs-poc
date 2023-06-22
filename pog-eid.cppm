export module pog:eid;
import hai;

namespace pog {
export class eid {
  unsigned m_value;

public:
  constexpr eid() = default;
  explicit constexpr eid(unsigned v) : m_value{v} {}

  [[nodiscard]] constexpr operator unsigned() const noexcept { return m_value; }
};

export class entity_list {
  hai::varray<bool> m_allocs;

public:
  explicit constexpr entity_list(unsigned max_ents) : m_allocs{max_ents} {}

  [[nodiscard]] constexpr eid alloc() {
    if (m_allocs.size() < m_allocs.capacity()) {
      m_allocs.push_back(true);
      return eid{m_allocs.size()};
    }

    for (auto i = 0U; i < m_allocs.size(); i++) {
      if (m_allocs[i])
        continue;

      m_allocs[i] = true;
      return eid{i + 1};
    }

    return eid{0};
  }

  constexpr void dealloc(eid e) { m_allocs[e - 1] = false; }
};
} // namespace pog

namespace {
static_assert([] {
  pog::entity_list ents{3};
  return ents.alloc() == 1 && ents.alloc() == 2 && ents.alloc() == 3;
}());
static_assert([] {
  pog::entity_list ents{3};
  auto a = ents.alloc();
  auto b = ents.alloc();
  auto c = ents.alloc();
  return !ents.alloc();
}());
static_assert([] {
  pog::entity_list ents{3};
  auto a = ents.alloc();
  auto b = ents.alloc();
  ents.dealloc(ents.alloc());
  return ents.alloc();
}());
static_assert([] {
  pog::entity_list ents{3};
  auto a = ents.alloc();
  auto b = ents.alloc();
  auto c = ents.alloc();
  ents.dealloc(a);
  ents.dealloc(b);
  ents.dealloc(ents.alloc());
  ents.dealloc(ents.alloc());
  return ents.alloc();
}());
} // namespace

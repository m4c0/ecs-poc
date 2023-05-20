export module ecs:eid;

namespace ecs {
export class eid {
  unsigned m_value;

public:
  constexpr eid() = default;
  explicit constexpr eid(unsigned v) : m_value{v} {}

  [[nodiscard]] constexpr operator unsigned() const noexcept { return m_value; }
};

export template <unsigned Max> class entity_list {
  bool m_allocated[Max]{};
  unsigned m_count{};

public:
  [[nodiscard]] constexpr eid alloc() {
    if (m_count < Max) {
      m_allocated[m_count] = true;
      return eid{++m_count};
    }

    for (auto i = 0U; i < Max; i++) {
      if (!m_allocated[i]) {
        m_allocated[i] = true;
        return eid{i + 1};
      }
    }

    return eid{0};
  }

  constexpr void dealloc(eid e) {
    if (e == m_count)
      m_count--;

    m_allocated[e - 1] = false;
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
static_assert([] {
  ecs::entity_list<3> ents;
  auto a = ents.alloc();
  auto b = ents.alloc();
  ents.dealloc(ents.alloc());
  return ents.alloc();
}());
static_assert([] {
  ecs::entity_list<3> ents;
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

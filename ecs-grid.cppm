export module ecs:grid;
import :eid;

namespace ecs {
export template <unsigned W, unsigned H, unsigned Max> class grid {
  struct coord {
    unsigned x;
    unsigned y;
  };
  struct c_map : coord {
    bool filled;
  };

  c_map m_c2i[Max]{};
  eid m_i2c[H][W]{};

public:
  constexpr void set(eid id, unsigned x, unsigned y) {
    m_c2i[id] = {x, y, true};
    m_i2c[y][x] = id;
  }

  [[nodiscard]] constexpr coord get(eid id) const noexcept { return m_c2i[id]; }
  [[nodiscard]] constexpr bool has(eid id) const noexcept {
    return m_c2i[id].filled;
  }

  constexpr void remove(eid id) {
    auto c = m_c2i[id];
    m_i2c[c.y][c.x] = {};
    m_c2i[id] = {};
  }
};
} // namespace ecs

namespace {
static constexpr auto build_set() {
  ecs::grid<8, 5, 50> set{};
  set.set(ecs::eid{20}, 3, 3);
  set.set(ecs::eid{40}, 7, 4);
  set.set(ecs::eid{30}, 1, 2);
  return set;
}

static_assert([] {
  auto set = build_set();
  return set.has(ecs::eid{20}) && set.has(ecs::eid{40}) &&
         set.has(ecs::eid{30});
}());
static_assert([] {
  auto set = build_set();
  set.remove(ecs::eid{40});
  return set.has(ecs::eid{20}) && !set.has(ecs::eid{40}) &&
         set.has(ecs::eid{30});
}());
static_assert([] {
  auto set = build_set();
  set.remove(ecs::eid{30});
  return set.has(ecs::eid{20}) && set.has(ecs::eid{40}) &&
         !set.has(ecs::eid{30});
}());
static_assert([] {
  auto set = build_set();
  set.remove(ecs::eid{20});
  set.remove(ecs::eid{40});
  return !set.has(ecs::eid{20}) && !set.has(ecs::eid{40}) &&
         set.has(ecs::eid{30});
}());
} // namespace

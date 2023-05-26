export module pog:grid;
import :eid;

namespace pog {
export struct grid_coord {
  unsigned x;
  unsigned y;
};
export template <unsigned W, unsigned H, unsigned Max> class grid {
  struct c_map : grid_coord {
    bool filled;
  };

  c_map m_i2c[Max]{};
  eid m_c2i[H][W]{};

public:
  constexpr void put(eid id, grid_coord c) {
    m_i2c[id] = {c.x, c.y, true};
    m_c2i[c.y][c.x] = id;
  }
  constexpr void update(eid id, grid_coord c) {
    remove(id);
    put(id, c);
  }

  [[nodiscard]] constexpr grid_coord get(eid id) const noexcept {
    return m_i2c[id];
  }
  [[nodiscard]] constexpr bool has(eid id) const noexcept {
    return m_i2c[id].filled;
  }

  [[nodiscard]] constexpr eid get(grid_coord c) const noexcept {
    return m_c2i[c.y][c.x];
  }
  [[nodiscard]] constexpr bool has(grid_coord c) const noexcept {
    return m_c2i[c.y][c.x];
  }

  constexpr void remove(eid id) {
    auto c = m_i2c[id];
    m_c2i[c.y][c.x] = {};
    m_i2c[id] = {};
  }
};
} // namespace pog

namespace {
static constexpr auto build_set() {
  pog::grid<8, 5, 50> set{};
  set.put(pog::eid{20}, {3, 3});
  set.put(pog::eid{40}, {7, 4});
  set.put(pog::eid{30}, {1, 2});
  return set;
}

static_assert([] {
  auto set = build_set();
  return set.has(pog::eid{20}) && set.has(pog::eid{40}) &&
         set.has(pog::eid{30}) && set.has({3, 3}) && set.has({7, 4}) &&
         set.has({1, 2});
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{40});
  return set.has(pog::eid{20}) && !set.has(pog::eid{40}) &&
         set.has(pog::eid{30}) && set.has({3, 3}) && !set.has({7, 4}) &&
         set.has({1, 2});
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{30});
  return set.has(pog::eid{20}) && set.has(pog::eid{40}) &&
         !set.has(pog::eid{30}) && set.has({3, 3}) && set.has({7, 4}) &&
         !set.has({1, 2});
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{20});
  set.remove(pog::eid{40});
  return !set.has(pog::eid{20}) && !set.has(pog::eid{40}) &&
         set.has(pog::eid{30}) && !set.has({3, 3}) && !set.has({7, 4}) &&
         set.has({1, 2});
}());
static_assert([] {
  auto set = build_set();
  set.remove(pog::eid{20});
  return !set.get({3, 3});
}());
} // namespace

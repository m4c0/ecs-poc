export module pog:quadtree;
import :eid;

namespace pog {
struct coord {
  float x;
  float y;
};

class quadtree {
public:
  constexpr void add(eid id, coord c) {}
  constexpr void for_each_in(coord aa, coord bb, auto &&fn) {}
  [[nodiscard]] constexpr bool has(eid id) { return false; }
  constexpr void remove(eid id) {}
};
} // namespace pog

namespace {
static_assert([] {
  pog::quadtree q{};
  q.add(pog::eid{10}, {3.5f, -1.2f});
  q.add(pog::eid{20}, {3.5f, 100.f});
  q.add(pog::eid{30}, {300.f, -1.2f});
  q.for_each_in({0.0f, 0.0f}, {300.0f, 200.0f},
                [&](auto eid) { q.remove(eid); });
  return q.has(pog::eid{20}) && !q.has(pog::eid{10});
}());
}

export module pog:quadtree;
import :eid;
import hai;

namespace pog {
struct coord {
  float x;
  float y;
};
struct aabb {
  coord aa;
  coord bb;
};
[[nodiscard]] constexpr auto intersect(aabb n, aabb o) noexcept {
  if (n.bb.x < o.aa.x)
    return false;
  if (o.bb.x < n.aa.x)
    return false;
  if (n.bb.y < o.aa.y)
    return false;
  if (o.bb.y < n.aa.y)
    return false;
  return true;
}

export class quadtree {
  static constexpr const auto chunk_size = 1024;
  struct pair {
    eid id;
    aabb area;
  };
  // TODO: organise chunks into a quad-tree
  hai::varray<pair> m_chunk{chunk_size};

public:
  constexpr void add(eid id, aabb area) { m_chunk.push_back(pair{id, area}); }

  constexpr void for_each_in(aabb area, auto &&fn) {
    for (auto i = 0; i < m_chunk.size(); i++) {
      auto &[id, a] = m_chunk[i];

      if (!intersect(a, area))
        continue;

      fn(id, a);
      i--;
    }
  }

  [[nodiscard]] constexpr bool has(eid id) {
    for (auto &[cid, _] : m_chunk) {
      if (id == cid)
        return true;
    }
    return false;
  }
  [[nodiscard]] constexpr bool has_in(aabb area) {
    for (auto &[_, a] : m_chunk) {
      if (intersect(a, area))
        return true;
    }
    return false;
  }

  constexpr void remove(eid id) {
    for (auto i = 0; i < m_chunk.size(); i++) {
      if (m_chunk[i].id != id)
        continue;

      m_chunk[i] = m_chunk.pop_back();
      break;
    }
  }
};
} // namespace pog

namespace {
static_assert([] {
  pog::quadtree q{};
  q.add(pog::eid{10}, {{3.5f, -1.2f}, {3.5f, -1.2f}});
  q.add(pog::eid{20}, {{3.5f, 100.f}, {3.5f, 100.f}});
  q.add(pog::eid{30}, {{300.f, -1.2f}, {300.f, -1.2f}});
  if (!q.has(pog::eid{20}))
    throw 0;
  if (q.has(pog::eid{}))
    throw 1;

  q.for_each_in({{0.0f, 0.0f}, {300.0f, 200.0f}},
                [&](auto eid, auto c) { q.remove(eid); });
  if (!q.has(pog::eid{10}))
    throw 1;
  if (q.has(pog::eid{20}))
    throw 1;
  if (!q.has(pog::eid{10}))
    throw 1;
  return true;
}());
}

export module pog:quadtree;
import :eid;
import hai;

namespace pog {
struct coord {
  float x;
  float y;
};

class quadtree {
  static constexpr const auto chunk_size = 1024;
  struct pair {
    eid id;
    coord c;
  };
  // TODO: organise chunks into a quad-tree
  hai::varray<pair> m_chunk{chunk_size};

public:
  constexpr void add(eid id, coord c) { m_chunk.push_back(pair{id, c}); }

  constexpr void for_each_in(coord aa, coord bb, auto &&fn) {
    for (auto i = 0; i < m_chunk.size(); i++) {
      auto &[id, c] = m_chunk[i];

      if (c.x < aa.x || c.x > bb.x)
        continue;
      if (c.y < aa.y || c.y > bb.y)
        continue;

      fn(id, c);
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
  q.add(pog::eid{10}, {3.5f, -1.2f});
  q.add(pog::eid{20}, {3.5f, 100.f});
  q.add(pog::eid{30}, {300.f, -1.2f});
  if (!q.has(pog::eid{20}))
    throw 0;
  if (q.has(pog::eid{}))
    throw 1;

  q.for_each_in({0.0f, 0.0f}, {300.0f, 200.0f},
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

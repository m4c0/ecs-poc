export module pog:ll;
import hai;

namespace pog::ll {
class list {
  static constexpr const auto cap_delta = 128;

  hai::varray<unsigned> m_data{cap_delta};

public:
  [[nodiscard]] constexpr bool empty() const noexcept {
    return m_data.size() == 0;
  }

  constexpr unsigned pop() noexcept { return m_data.pop_back(); }
  constexpr void add(unsigned v) noexcept {
    if (m_data.size() == m_data.capacity())
      m_data.add_capacity(cap_delta);

    m_data.push_back(v);
  }
};
} // namespace pog::ll

namespace {
static_assert([] {
  pog::ll::list l{};
  return l.empty();
}());
static_assert([] {
  pog::ll::list l{};
  l.add(99);
  if (l.empty())
    throw 0;
  return l.pop() == 99;
}());
static_assert([] {
  pog::ll::list l{};
  l.add(99);
  l.add(55);
  return l.pop() == 55 && l.pop() == 99;
}());
static_assert([] {
  pog::ll::list l{};
  l.add(99);
  l.pop();
  l.add(55);
  return l.pop() == 55 && l.empty();
}());
} // namespace

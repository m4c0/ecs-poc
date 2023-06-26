export module pog:single;
import :eid;

namespace pog {
export template <typename Tp = marker> class singleton {
  Tp m_value{};
  eid m_id{};

public:
  constexpr eid get_id() const noexcept { return m_id; }

  constexpr auto get(eid id) const noexcept { return has(id) ? m_value : Tp{}; }

  constexpr bool has(eid id) const noexcept { return m_id == id; }

  constexpr void remove(eid id) noexcept {
    if (has(id)) {
      *this = {};
    }
  }

  constexpr void set(eid id, Tp value) noexcept {
    m_id = id;
    m_value = value;
  }
};
} // namespace pog

namespace {
static_assert([] {
  pog::singleton<long> l;
  return !l.has(pog::eid{11});
}());
static_assert([] {
  pog::singleton<long> l;
  l.set(pog::eid{11}, 0L);
  return l.get_id() == pog::eid{11} && l.has(pog::eid{11});
}());
static_assert([] {
  pog::singleton<long> l;
  l.set(pog::eid{3}, 99L);
  return l.get(pog::eid{3}) == 99L && l.get(pog::eid{1}) == 0L &&
         l.get(pog::eid{}) == 0L;
}());
static_assert([] {
  pog::singleton<long> l;
  l.set(pog::eid{11}, 0L);
  l.remove(pog::eid{12});
  return l.has(pog::eid{11}) && !l.has(pog::eid{12});
}());
static_assert([] {
  pog::singleton<long> l;
  l.set(pog::eid{11}, 0L);
  l.remove(pog::eid{11});
  return !l.has(pog::eid{11}) && !l.has(pog::eid{12});
}());
} // namespace

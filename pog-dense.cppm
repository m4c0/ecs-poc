export module pog:dense;
import :eid;
import hai;

namespace pog {
export template <typename Tp> class dense_set {
  static constexpr const auto initial_cap = 16;
  static constexpr const auto cap_increase = 16;

  struct node {
    eid id;
    Tp value;
  };

  hai::varray<node> m_data{initial_cap};

public:
  constexpr auto add(eid id, Tp v) {
    if (m_data.size() == m_data.capacity())
      m_data.add_capacity(cap_increase);

    m_data.push_back(node{id, v});
    return m_data.size() - 1;
  }

  [[nodiscard]] constexpr auto &get(unsigned index) noexcept {
    return m_data[index];
  }
  [[nodiscard]] constexpr const auto &get(unsigned index) const noexcept {
    return m_data[index];
  }

  constexpr void for_each_r(auto &&fn) {
    for (auto ri = m_data.size(); ri != 0; ri--) {
      const auto &[id, v] = m_data[ri - 1];
      fn(id, v);
    }
  }

  constexpr void truncate() { m_data.set_size(0); }
  constexpr auto pop_back() { return m_data.pop_back(); }

  [[nodiscard]] constexpr auto *begin() const noexcept {
    return m_data.begin();
  }
  [[nodiscard]] constexpr auto *end() const noexcept { return m_data.end(); }
  [[nodiscard]] constexpr auto *begin() noexcept { return m_data.begin(); }
  [[nodiscard]] constexpr auto *end() noexcept { return m_data.end(); }

  [[nodiscard]] constexpr auto size() const noexcept { return m_data.size(); }
};
} // namespace pog

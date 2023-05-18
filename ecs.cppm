export module ecs;

namespace ecs {
export template <typename Tp, unsigned Max> class sparse_set {
  struct dense {
    Tp value;
    unsigned id;
  };

  dense m_dense[Max]{};
  unsigned m_sparse[Max]{};
  unsigned m_n{};

public:
  constexpr void add(unsigned id, Tp &&v) {
    // TODO: delete "dense" if `id` exists?

    m_dense[m_n++] = {v, id};
    m_sparse[id] = m_n;
  }

  [[nodiscard]] constexpr bool has(unsigned id) const {
    return m_sparse[id] != 0;
  }

  [[nodiscard]] constexpr auto *begin() const { return &m_dense[0]; }
  [[nodiscard]] constexpr auto *end() const { return &m_dense[m_n]; }
};
} // namespace ecs

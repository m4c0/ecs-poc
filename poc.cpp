import ecs;

template <typename Tp, unsigned Max> class sparse_set {
  struct dense {
    Tp v;
    unsigned s;
  };

  dense m_dense[Max]{};
  unsigned m_sparse[Max]{};
  unsigned m_n{};

public:
};

struct rigid_body {};

constexpr const auto width = 30;
constexpr const auto height = 20;
constexpr const auto max = width * height;

int main() { sparse_set<rigid_body, max> bodies{}; }

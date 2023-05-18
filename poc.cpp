#include <stdio.h>
#include <stdlib.h>

import ecs;

constexpr const auto width = 10;
constexpr const auto height = 5;
constexpr const auto max = width * height;

constexpr const auto mob_count = height;

enum body_type { block, mob };
struct rigid_body {
  body_type type;
};

class coord {
  unsigned m_x;
  unsigned m_y;

public:
  constexpr coord() = default;
  constexpr coord(unsigned x, unsigned y) : m_x{x}, m_y{y} {}

  [[nodiscard]] constexpr operator unsigned() const noexcept {
    return m_y * width + m_x;
  }
};

unsigned random(unsigned n) { return rand() % n; }

int main() {
  srand(69);

  ecs::sparse_set<rigid_body, max> bodies{};

  for (auto x = 0U; x < width; x++) {
    bodies.add(coord{x, 0}, rigid_body{block});
    bodies.add(coord{x, height - 1}, rigid_body{block});
  }
  for (auto y = 0U; y < height; y++) {
    bodies.add(coord{0, y}, rigid_body{block});
    bodies.add(coord{width - 1, y}, rigid_body{block});
  }

  for (auto i = 0; i < mob_count; i++) {
    coord c{};
    do {
      c = {random(width), random(height)};
    } while (bodies.has(c));

    bodies.add(c, rigid_body{mob});
  }

  for (auto [body, id] : bodies) {
    printf("%d %d = %d\n", id % width, id / width, body.type);
  }
}

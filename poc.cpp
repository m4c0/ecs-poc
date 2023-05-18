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

struct lifeness {
  bool alive;
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
  ecs::sparse_set<lifeness, max> alives{};

  // Simulates bodies from a map
  for (auto x = 0U; x < width; x++) {
    bodies.add(coord{x, 0}, rigid_body{block});
    bodies.add(coord{x, height - 1}, rigid_body{block});
  }
  for (auto y = 0U; y < height; y++) {
    bodies.add(coord{0, y}, rigid_body{block});
    bodies.add(coord{width - 1, y}, rigid_body{block});
  }

  // Simulates picking random position for new mobs
  for (auto i = 0; i < mob_count; i++) {
    coord c{};
    do {
      c = {random(width), random(height)};
    } while (bodies.has(c));

    bodies.add(c, rigid_body{mob});
    alives.add(c, lifeness{true});
  }

  // Simulates mob movement
  // All mob wants to go to 0, 0 and kills on contact
  for (auto [alive, id] : alives) {
    // attack target; or
    // remove body
    // add body
  }

  // Simulates output
  char out[height * width];
  for (auto &o : out)
    o = ' ';
  for (auto [body, id] : bodies) {
    if (body.type == block) {
      out[id] = '#';
    } else if (alives.has(id)) {
      out[id] = '@';
    } else {
      out[id] = '*';
    }
  }
  for (auto y = 0; y < height; y++) {
    fwrite(out + y * width, 1, width, stdout);
    puts("");
  }
}

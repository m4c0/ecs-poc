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
using bodies_set = ecs::sparse_set<rigid_body, max>;

struct lifeness {
  bool alive;
};
using alives_set = ecs::sparse_set<lifeness, max>;

class coord {
  unsigned m_x;
  unsigned m_y;

public:
  constexpr coord() = default;
  constexpr coord(unsigned x, unsigned y) : m_x{x}, m_y{y} {}

  explicit constexpr coord(unsigned i) : m_x{i % width}, m_y{i / width} {}

  [[nodiscard]] constexpr operator unsigned() const noexcept {
    return m_y * width + m_x;
  }

  constexpr void decr_y() noexcept { m_y--; }
};

unsigned random(unsigned n) { return rand() % n; }

void gen_map(bodies_set &bodies) {
  // Simulates bodies from a map
  for (auto x = 0U; x < width; x++) {
    bodies.add(coord{x, 0}, rigid_body{block});
    bodies.add(coord{x, height - 1}, rigid_body{block});
  }
  for (auto y = 0U; y < height; y++) {
    bodies.add(coord{0, y}, rigid_body{block});
    bodies.add(coord{width - 1, y}, rigid_body{block});
  }
}
void gen_mobs(bodies_set &bodies, alives_set &alives) {
  // Simulates picking random position for new mobs
  for (auto i = 0; i < mob_count; i++) {
    coord c{};
    do {
      c = {random(width), random(height)};
    } while (bodies.has(c));

    bodies.add(c, rigid_body{mob});
    alives.add(c, lifeness{true});
  }
}

void move_mobs(const bodies_set &bodies, alives_set &alives) {
  // Simulates mob movement
  // All mob wants to go to up and kills on contact
  for (auto [alive, id] : alives) {
    coord tgt{id};
    tgt.decr_y();

    if (bodies.has(tgt) && alives.has(tgt)) {
      // alives--?
      continue;
    }
    // add desired target
  }
}

void output(const bodies_set &bodies, const alives_set &alives) {
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

int main() {
  // This seed gave a good scenario in my system (before mob movement):
  //
  // ##########
  // #@       #
  // #@       #
  // #  @ @  @#
  // ##########
  //
  // One entity can't walk, three can, one will attack
  srand(69);

  ecs::sparse_set<rigid_body, max> bodies{};
  ecs::sparse_set<lifeness, max> alives{};

  gen_map(bodies);
  gen_mobs(bodies, alives);
  move_mobs(bodies, alives);

  // remove dead alives
  // move to targets

  output(bodies, alives);
}

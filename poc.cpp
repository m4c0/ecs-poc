#include <stdio.h>
#include <stdlib.h>

import ecs;

unsigned random(unsigned n) { return rand() % n; }

constexpr const auto width = 10;
constexpr const auto height = 5;
constexpr const auto max = width * height;

constexpr const auto mob_count = height;

//------ "Components"

enum body_type { block, mob };
struct rigid_body {
  body_type type;
};

struct lifeness {
  bool alive;
};

struct coord {
  unsigned x;
  unsigned y;
};

struct ec {
  ecs::entity_list<max> e;
  ecs::sparse_set<coord, max> coords;
  ecs::sparse_set<rigid_body, max> bodies;
  ecs::sparse_set<lifeness, max> alives;
  ecs::eid map[height][width];
};

//------ Entity creation

void add_block(ec &ec, coord c) {
  auto id = ec.e.alloc();
  ec.coords.add(id, c);
  ec.bodies.add(id, rigid_body{block});
  ec.map[c.y][c.x] = id;
}
void add_mob(ec &ec, coord c) {
  auto id = ec.e.alloc();
  ec.coords.add(id, c);
  ec.bodies.add(id, rigid_body{mob});
  ec.alives.add(id, lifeness{true});
  ec.map[c.y][c.x] = id;
}

//------ "Systems"

void gen_map(ec &ec) {
  // Simulates bodies from a map
  for (auto x = 0U; x < width; x++) {
    add_block(ec, coord{x, 0});
    add_block(ec, coord{x, height - 1});
  }
  for (auto y = 0U; y < height; y++) {
    add_block(ec, coord{0, y});
    add_block(ec, coord{width - 1, y});
  }
}

void gen_mobs(ec &ec) {
  // Simulates picking random position for new mobs
  for (auto i = 0; i < mob_count; i++) {
    coord c{};
    do {
      c = {random(width), random(height)};
    } while (ec.map[c.y][c.x]);

    add_mob(ec, c);
  }
}

void move_mobs(ec &ec) {
  // Simulates mob movement
  // All mob wants to go to up and kills on contact
  for (const auto [alive, id] : ec.alives) {
    coord c = ec.coords.get(id);
    ecs::eid tgt = ec.map[c.y - 1][c.x];

    if (ec.bodies.has(tgt) && ec.alives.has(tgt)) {
      ec.alives.set(tgt, lifeness{false});
      continue;
    }
    // add desired target
  }
}

void dead_cleanup(ec &ec) {
  for (const auto [a, id] : ec.alives) {
    if (a.alive)
      continue;

    auto c = ec.coords.get(id);
    ec.map[c.y][c.x] = {};
    ec.coords.remove(id);
    ec.bodies.remove(id);
    ec.e.dealloc(id);
  }
  ec.alives.remove_if([](auto l, auto id) { return !l.alive; });
}

void output(const ec &ec) {
  // Simulates output
  for (auto &row : ec.map) {
    char buf[width + 1];
    for (auto i = 0; i < width; i++) {
      auto id = row[i];
      if (!id) {
        buf[i] = ' ';
      } else if (ec.bodies.get(id).type == block) {
        buf[i] = '#';
      } else if (ec.alives.has(id)) {
        buf[i] = '@';
      } else {
        buf[i] = '*';
      }
    }
    puts(buf);
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

  ec ec{};

  gen_map(ec);
  gen_mobs(ec);
  move_mobs(ec);
  output(ec);

  dead_cleanup(ec);
  // move to targets
  output(ec);
}

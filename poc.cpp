#include <stdio.h>
#include <stdlib.h>

import pog;

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

struct ec {
  pog::entity_list<max> e;
  pog::grid<width, height, max> coords;
  pog::sparse_set<rigid_body, max> bodies;
  pog::sparse_set<lifeness, max> alives;
};

//------ Entity creation

void add_block(ec &ec, pog::grid_coord c) {
  auto id = ec.e.alloc();
  ec.coords.set(id, c);
  ec.bodies.add(id, rigid_body{block});
}
void add_mob(ec &ec, pog::grid_coord c) {
  auto id = ec.e.alloc();
  ec.coords.set(id, c);
  ec.bodies.add(id, rigid_body{mob});
  ec.alives.add(id, lifeness{true});
}

//------ "Systems"

void gen_map(ec &ec) {
  // Simulates bodies from a map
  for (auto x = 0U; x < width; x++) {
    add_block(ec, pog::grid_coord{x, 0});
    add_block(ec, pog::grid_coord{x, height - 1});
  }
  for (auto y = 0U; y < height; y++) {
    add_block(ec, pog::grid_coord{0, y});
    add_block(ec, pog::grid_coord{width - 1, y});
  }
}

void gen_mobs(ec &ec) {
  // Simulates picking random position for new mobs
  for (auto i = 0; i < mob_count; i++) {
    pog::grid_coord c{};
    do {
      c = {random(width), random(height)};
    } while (ec.coords.has(c));

    add_mob(ec, c);
  }
}

void move_mobs(ec &ec) {
  // Simulates mob movement
  // All mob wants to go to up and kills on contact
  for (const auto [alive, id] : ec.alives) {
    pog::grid_coord c = ec.coords.get(id);
    c.y--;

    pog::eid tgt = ec.coords.get(c);

    if (ec.alives.has(tgt)) {
      ec.alives.set(tgt, lifeness{false});
    } else if (!ec.bodies.has(tgt)) {
      ec.coords.remove(id);
      ec.coords.set(id, c);
    }
  }
}

void dead_cleanup(ec &ec) {
  for (const auto [a, id] : ec.alives) {
    if (a.alive)
      continue;

    auto c = ec.coords.get(id);
    ec.coords.remove(id);
    ec.bodies.remove(id);
    ec.e.dealloc(id);
  }
  ec.alives.remove_if([](auto l, auto id) { return !l.alive; });
}

void output(const ec &ec) {
  // Simulates output
  for (auto y = 0U; y < height; y++) {
    char buf[width + 1];
    for (auto x = 0U; x < width; x++) {
      auto id = ec.coords.get({x, y});
      if (!id) {
        buf[x] = ' ';
      } else if (ec.bodies.get(id).type == block) {
        buf[x] = '#';
      } else if (ec.alives.has(id)) {
        buf[x] = '@';
      } else {
        buf[x] = '*';
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
  output(ec);

  // "Game loop"
  for (auto ticks = 0; ticks < 3; ticks++) {
    move_mobs(ec);
    dead_cleanup(ec);
    output(ec);
  }
}

#pragma once

#include "../ecow/ecow.hpp"

auto pog() {
  using namespace ecow;

  auto m = unit::create<mod>("ecs");
  m->add_part("eid");
  m->add_part("grid");
  m->add_part("spset");
  return m;
}

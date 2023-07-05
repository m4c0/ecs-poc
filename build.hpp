#pragma once

#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"

auto pog() {
  using namespace ecow;

  auto m = unit::create<mod>("pog");
  m->add_wsdep("hai", hai());
  m->add_part("ll");
  m->add_part("eid");
  m->add_part("grid");
  m->add_part("map");
  m->add_part("quadtree");
  m->add_part("single");
  m->add_part("spset");
  return m;
}

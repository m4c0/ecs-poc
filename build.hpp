#pragma once

#include "../btree/build.hpp"
#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../rtree/build.hpp"
#include "../traits/build.hpp"

auto pog() {
  using namespace ecow;

  auto m = unit::create<mod>("pog");
  m->add_wsdep("hai", hai());
  m->add_wsdep("btree", btree());
  m->add_wsdep("rtree", rtree());
  m->add_wsdep("traits", traits());
  m->add_part("ll");
  m->add_part("eid");
  m->add_part("grid");
  m->add_part("map");
  m->add_part("single");
  m->add_part("dense");
  m->add_part("spset");
  m->add_part("rtree");
  return m;
}

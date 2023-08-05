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
  return m;
}

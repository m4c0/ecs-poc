#pragma once

#include "../ecow/ecow.hpp"
#include "../silog/build.hpp"

auto pog() {
  using namespace ecow;

  auto m = unit::create<box>("pog");
  m->add_wsdep("silog", silog());
  return m;
}

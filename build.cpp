#include "../ecow/ecow.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto m = unit::create<mod>("ecs");
  m->add_part("spset");

  auto poc = unit::create<tool>("poc");
  poc->add_ref(m);
  poc->add_unit("poc");

  return run_main(poc, argc, argv);
}

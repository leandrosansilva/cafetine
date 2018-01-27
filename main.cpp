#include "person.h"

int main(int, char**)
{
  const auto p = Person{};

  const auto p1 = p;

  return p.age();
}

#include "person.h"
#include <utility>

int main(int, char**)
{
  auto result = int{0};

  {
    auto p = Person{};

    auto p1 = std::move(p);

    result = p.age();
  }

  return result;
}

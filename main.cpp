#include "person.h"
#include <utility>
#include <iostream>

int main(int, char**)
{
  auto result = int{0};

  std::cout << "sizeof person: " << sizeof(Person) << "\n";

  {
    auto p = Person{};

    std::cout << p.name() << '\n';

    auto p1 = std::move(p);

    std::cout << p1.name() << '\n';

    result = p.age();
  }

  return result;
}

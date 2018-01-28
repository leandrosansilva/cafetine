#include "person.h"
#include <utility>
#include <iostream>

int main(int, char**)
{
  auto result = int{0};

  {
    auto p = Person{"Leandro", 30};

    std::cout << p.name() << '\n';

    auto p1 = std::move(p);
    
    // p becomes invalid by the move

    std::cout << p1.name() << '\n';

    result = p.age();
  }

  return result;
}

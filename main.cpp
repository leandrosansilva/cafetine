#include "person.h"
#include <utility>
#include <iostream>

int main(int, char**)
{
  auto result = int{0};

  for (auto i = 0; i < 1000; i++) {
    auto p = Person{"Leandro", 30};
    
    std::cout << p.name() << '\n';
    
    {
      const auto copyP = p;
      std::cout << copyP.name() << '\n';
    }

    const auto p1 = std::move(p);
    
    // p becomes invalid by the move

    std::cout << p1.name() << '\n';

    result += p1.age();
  }

  return result % 128;
}

#include "person.h"
#include <iostream>

void some_function()
{
  std::cout << "Deleted PersonImpl\n";
}

struct Person::PersonImpl
{
  const char* _name;
  int _age;

  const char* name() const
  {
    return _name;
  }

  int age() const
  {
    return _age;
  }

  ~PersonImpl()
  {
    some_function();
  }
};

Person::Person():
  pimp(PimpMeAlloc<Impl>::pimp()),
  impl(PimpMeAlloc<Impl>::alloc(pimp))
{
  impl->_age = 34;
  impl->_name = "Leandro";
}

Person::Person(Person&& other):
  pimp(PimpMeAlloc<Impl>::move(other.pimp)),
  impl(pimp.get())
{
}

const char* Person::name() const
{
  return impl->name();
}

int Person::age() const
{
  return impl->age();
}

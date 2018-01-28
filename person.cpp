#include "person.h"

#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>

struct Person::PersonImpl
{
  int _age;

  std::unique_ptr<std::string> _u;

  const char* name() const
  {
    return _u->c_str();
  }

  int age() const
  {
    return _age;
  }

  PersonImpl(PersonImpl&&) = default;

  PersonImpl(const std::string& name, int age):
    _age(age),
    _u(std::unique_ptr<std::string>(new std::string(name)))
  {
  }

  ~PersonImpl()
  {
    std::cout << "Deleted PersonImpl\n";
  }
};

Person::Person()
{
  assert(!impl);
  PimpMeAlloc<Impl>::alloc(impl, "Leandro", 30);
  assert(impl);
}

Person::Person(Person&& other):
  impl(PimpMeAlloc<Impl>::move(other.impl))
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

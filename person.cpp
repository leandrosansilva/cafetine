#include "person.h"

#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>

// Non-copyable but moveable, because of the unique_ptr
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
  
  PersonImpl& operator=(PersonImpl&) = delete;

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

Person::Person(const char* name, int age)
{
  assert(!impl);
  cafetine::Alloc<Impl>::alloc(impl, name, age);
  assert(impl);
}

Person::Person(Person&& other):
  impl(cafetine::Alloc<Impl>::move(other.impl))
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

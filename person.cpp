#include "person.h"

#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>

// Non-copyable but moveable, because of the unique_ptr
struct Person::PersonImpl final
{
  int16_t _age1;
  std::shared_ptr<std::string> _u;
  int16_t _age;

  const char* name() const
  {
    return _u->c_str();
  }

  int age() const
  {
    return _age;
  }
  
  PersonImpl(const PersonImpl&) = default;
  
  PersonImpl& operator=(PersonImpl&) = default;

  PersonImpl(PersonImpl&&) = default;

  PersonImpl(const std::string& name, int age):
    _age(age),
    _u(std::shared_ptr<std::string>(new std::string(name)))
  {
  }

  ~PersonImpl()
  {
    std::cout << "Deleted PersonImpl\n";
  }
};

Person::Person(const Person& other):
  impl(cafetine::Alloc<Impl>::copy(other.impl))
{
  std::cout << "Copied person\n";
}

Person::Person(const char* name, int age)
{
  std::cout << "Created person\n";
  assert(!impl);
  cafetine::Alloc<Impl>::alloc(impl, name, age);
  assert(impl);
}

Person::Person(Person&& other):
  impl(cafetine::Alloc<Impl>::move(other.impl))
{
  std::cout << "Moved person\n";
}

Person::~Person()
{
  std::cout << "Destroyed person\n";
}

const char* Person::name() const
{
  return impl->name();
}

int Person::age() const
{
  return impl->age();
}

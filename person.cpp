#include "person.h"

#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include <array>

// To prevent the compiler of optimizing it
extern "C" void do_something_special(const uint8_t*, std::size_t);

// Non-copyable but moveable, because of the unique_ptr
template<>
struct VersionedPerson<PersonVersionNumber>::PersonImpl final
{
  int16_t _age;
  std::shared_ptr<std::string> _u;
  
  std::array<uint8_t, 2048> aBigMember;
  
  const char* name() const
  {
    do_something_special(aBigMember.data(), aBigMember.size());
    return _u->c_str();
  }

  int age() const
  {
    do_something_special(aBigMember.data(), aBigMember.size());
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
    do_something_special(aBigMember.data(), aBigMember.size());
    std::cout << "Deleted PersonImpl\n";
  }
};

template<>
VersionedPerson<PersonVersionNumber>::VersionedPerson(const VersionedPerson& other):
  impl(cafetine::Alloc<Impl>::copy(other.impl))
{
  std::cout << "Copied person\n";
}

template<>
VersionedPerson<PersonVersionNumber>::VersionedPerson(const char* name, int age)
{
  std::cout << "Created person\n";
  assert(!impl);
  cafetine::Alloc<Impl>::alloc(impl, name, age);
  assert(impl);
  impl.reset();
  assert(!impl);
  cafetine::Alloc<Impl>::alloc(impl, name, age);
  assert(impl);
}

template<>
VersionedPerson<PersonVersionNumber>::VersionedPerson(VersionedPerson&& other):
  impl(cafetine::Alloc<Impl>::move(other.impl))
{
  std::cout << "Moved person\n";
}

template<>
VersionedPerson<PersonVersionNumber>::~VersionedPerson()
{
  std::cout << "Destroyed person\n";
}

template<>
const char* VersionedPerson<PersonVersionNumber>::name() const
{
  return impl->name();
}

template<>
int VersionedPerson<PersonVersionNumber>::age() const
{
  return impl->age();
}

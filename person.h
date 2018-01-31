#pragma once

#include "cafetine.h"

constexpr std::uint16_t PersonVersionNumber = 2;

template<std::uint16_t Version>
struct VersionedPerson
{
  struct PersonImpl;

  // Allocate 56 bytes, 4-byte aligned in the VersionedPerson Object to store
  // the implementation
  using Impl = cafetine::Pimpl<PersonImpl, 56 + 2048, alignof(std::uint32_t), Version>;

  // impl looks like a pointer, but never does dynamic memory allocation
  mutable Impl impl;

  VersionedPerson(const char* name, int age);
  
  VersionedPerson(const VersionedPerson&);
  
  VersionedPerson& operator=(const VersionedPerson&) = default;

  VersionedPerson(VersionedPerson&&);
  
  ~VersionedPerson();

  const char* name() const;

  int age() const;
};

using Person = VersionedPerson<PersonVersionNumber>;

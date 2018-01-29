#pragma once

#include "cafetine.h"

struct Person
{
  struct PersonImpl;

  // Allocate 16 bytes, 4-byte aligned in the Person Object to store
  // the implementation
  using Impl = cafetine::Pimpl<PersonImpl, 56, alignof(std::uint32_t)>;

  // impl looks like a pointer, but never does dynamic memory allocation
  Impl impl;

  Person(const char* name, int age);
  
  Person(const Person&);
  
  Person& operator=(const Person&) = default;

  Person(Person&&);
  
  ~Person();

  const char* name() const;

  int age() const;
};

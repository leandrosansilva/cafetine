#pragma once

#include "pimpme.h"

struct Person
{
  struct PersonImpl;

  using Impl = PimpMe<PersonImpl, 16, alignof(std::uint64_t)>;

  Impl impl;

  Person();

  Person(Person&&);

  const char* name() const;

  int age() const;
};

#pragma once

#include "pimpme.h"
#include <string>

struct Person
{
  struct PersonImpl;
  using Impl = PimpMe<PersonImpl, 16, alignof(std::uint32_t)>;

  Impl pimp;
  Impl::ptr impl;

  Person();

  Person(Person&&);

  const char* name() const;

  int age() const;
};

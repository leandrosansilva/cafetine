#pragma once

#include <type_traits>
#include <utility>
#include <cstdint>

template<typename T, std::size_t Size, std::size_t Align>
struct PimpMe
{
  constexpr static std::size_t size = Size;
  constexpr static std::size_t align = Align;

  using type = T;
  using ptr = typename std::add_pointer<T>::type;
  using storage_type = typename std::aligned_storage<size, align>::type;
  using storage_type_ptr = typename std::add_pointer<storage_type>::type;
  using destructor_type = typename std::add_pointer<void(storage_type_ptr&&)>::type;

  destructor_type destructor;
  storage_type storage;

  PimpMe(destructor_type&& destructor): destructor(std::move(destructor))
  {
  }

  ~PimpMe()
  {
    destructor(&storage);
  }
};

template<typename Impl>
struct PimpMeAlloc
{
  using type = typename Impl::type;
  using ptr = typename Impl::ptr;
  using storage_type_ptr = typename Impl::storage_type_ptr;

  static_assert(sizeof(type) <= Impl::size, "Too small!!!");

  constexpr static Impl pimp()
  {
    return Impl([](storage_type_ptr&& storage){ reinterpret_cast<ptr>(storage)->~type(); });
  }

  template<typename... Args>
  constexpr static ptr alloc(Impl& impl, Args&&... args)
  {
    return new(&(impl.storage)) typename Impl::type(std::forward<Args>(args)...);
  }
};

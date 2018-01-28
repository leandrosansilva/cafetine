#pragma once

#include <type_traits>
#include <utility>
#include <cstdint>

/**
 * Cafetine is a small utility library that aims to make the implementation
 * of the "PIMPL" more effficient by reducing the memory overhead of the dynamic
 * memory allocation and allocating the implementation in the same space as the
 * interface object.
 * 
 * Often one needs to hide only the implementation details (methods, members, etc.),
 * and exposing the storage memory for it is not a big issue. In those cases,
 * if the implementation object grows in size, the interface object will need to grow
 * as well, to bear the new size. All the classes that depend on the interface object
 * will need recompilation. 
 * 
 * If this is not a problem for you, maybe cafetine might suit you :-)
 * 
 * It also makes the PIMPL implementation easier, taking care of the object lifetime,
 * so you don't need to delete it manually (ok, this is not a problem if you use a "smart pointer",
 * but anyways...).
 * 
 * This is an example about how to use it:
 * 
 * // example.h
 * class Example
 * {
 *   class PrivateImpl;
 * 
 *   // 56 is how much space you allocate for the implementation 
 *   // and 4 is the alignment (you can use alignof(type) as well)
 *   // You really don's need to define this type, but it makes your 
 *   // life easier in the implementation file.
 *   // This class will not compile if the space is not large enough 
 *   // for the pimpl'd object. 
 *   using Impl = cafetine::Pimpl<PrivateImpl, 56, 4>;
 * 
 *   // allocate the implementation object.
 *   // it has a pointer-like interface
 *   Impl impl;
 * 
 * public:
 *   // Some constructor
 *   Example(int value);
 * 
 *   // If the object is moveable, you must implement it manually
 *   Example(Example&&);
 * 
 *   // Maybe default the other special functions...?
 * 
 *   int someMethod() const;
 * };
 * 
 * 
 * // example.cpp
 * class Example::PrivateImpl { ... };
 * 
 * Example::Example(int value)
 * {
 *   // first parameter is the impl member, and the other parameters are forwarded
 *   // to PrivateImpl constructor
 *   // it behaves like `impl = new PrivateImpl(value)`
 *   cefetine::Alloc<Impl>::alloc(impl, value);
 * }
 * 
 * Example::Example(Example&& other):
 *   impl(cafetine::Alloc<Impl>::move(other.impl))
 * {
 * }
 * 
 * int Example::someMethod() const
 * {
 *   // Use impl as a pointer
 *   return impl->someMethod();
 * }
*/

namespace cafetine {

template<typename T, std::size_t Size, std::size_t Align>
struct Pimpl
{
  constexpr static std::size_t size = Size;
  constexpr static std::size_t align = Align;

  using type = typename std::decay<T>::type;
  using ptr = typename std::add_pointer<T>::type;
  using storage_type = typename std::aligned_storage<size, align>::type;
  using storage_type_ptr = typename std::add_pointer<storage_type>::type;
  using destructor_type = typename std::add_pointer<void(storage_type_ptr&&)>::type;

  static destructor_type default_destructor;

  storage_type storage;
  destructor_type destructor;

  Pimpl(const Pimpl&) = delete;
  Pimpl& operator=(const Pimpl&) = delete;

  Pimpl(Pimpl&& other) = default;

  explicit Pimpl():
    destructor(nullptr)
  {
  }
  
  explicit Pimpl(const storage_type& s, destructor_type d):
    storage(s),
    destructor(d)
  {
  }

  ~Pimpl()
  {
    if (destructor) {
      destructor(&storage);
    }
  }

  ptr get() const noexcept
  {
    const auto storage_ptr = storage_type_ptr(&storage);
    return reinterpret_cast<ptr>(storage_ptr);
  }

  ptr operator->() const noexcept
  {
    return get();
  }

  operator bool() const noexcept
  {
    return destructor != nullptr;
  }

  typename std::add_lvalue_reference<T>::type operator*() const noexcept
  {
    return *get();
  }
};

template<typename T, std::size_t Size, std::size_t Align>
typename Pimpl<T, Size, Align>::destructor_type Pimpl<T, Size, Align>::default_destructor
  = [](storage_type_ptr&& storage){ reinterpret_cast<ptr>(storage)->~type(); };

template<typename Impl>
struct Alloc
{
  using type = typename Impl::type;
  using ptr = typename Impl::ptr;
  using storage_type = typename Impl::storage_type;
  using storage_type_ptr = typename Impl::storage_type_ptr;

  static_assert(sizeof(type) <= Impl::size, "Storage space it too small for implementation");

  template<typename... Args>
  static ptr alloc(Impl& impl, Args&&... args)
  {
    impl.destructor = Impl::default_destructor;
    return new(&impl.storage) typename Impl::type(std::forward<Args>(args)...);
  }

  // TODO: somehow hook it into std::move so that the user class
  // can just use the default move constructor
  static Impl move(Impl& impl) noexcept
  {
    auto storage = storage_type{};

    new(&storage) typename Impl::type(std::move(*impl));

    impl.destructor = nullptr;

    return Impl{storage, Impl::default_destructor};
  }
};

}

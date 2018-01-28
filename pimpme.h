#pragma once

#include <type_traits>
#include <utility>
#include <cstdint>

template<typename T, std::size_t Size, std::size_t Align>
struct PimpMe
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

  PimpMe(const PimpMe&) = delete;
  PimpMe& operator=(const PimpMe&) = delete;

  PimpMe(PimpMe&& other) = default;

  explicit PimpMe():
    destructor(nullptr)
  {
  }
  
  explicit PimpMe(const storage_type& s, destructor_type d):
    storage(s),
    destructor(d)
  {
  }

  ~PimpMe()
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
typename PimpMe<T, Size, Align>::destructor_type PimpMe<T, Size, Align>::default_destructor
  = [](storage_type_ptr&& storage){ reinterpret_cast<ptr>(storage)->~type(); };

template<typename Impl>
struct PimpMeAlloc
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
    return new(&(impl.storage)) typename Impl::type(std::forward<Args>(args)...);
  }

  static Impl move(Impl& impl) noexcept
  {
    auto storage = storage_type{};

    new(&(storage)) typename Impl::type(std::move(*impl.get()));

    impl.destructor = nullptr;

    return Impl{storage, Impl::default_destructor};
  }
};

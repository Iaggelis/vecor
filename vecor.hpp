// Copyright  2021 Ioannis Angelis <john_agelis@hotmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef VECOR_HPP
#define VECOR_HPP

#include <algorithm>
#include <cmath>
#include <concepts>
#include <numeric>
#include <ostream>
#include <vector>

template <typename T>
concept Number = std::is_arithmetic_v<T>;

namespace Vecor {

template <typename T>
struct Vec {
  using Impl_t                 = typename std::vector<T>;
  using value_type             = typename Impl_t::value_type;
  using size_type              = typename Impl_t::size_type;
  using difference_type        = typename Impl_t::difference_type;
  using reference              = typename Impl_t::reference;
  using const_reference        = typename Impl_t::const_reference;
  using pointer                = typename Impl_t::pointer;
  using const_pointer          = typename Impl_t::const_pointer;
  using iterator               = typename Impl_t::iterator;
  using const_iterator         = typename Impl_t::const_iterator;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // constructors
  constexpr Vec() noexcept {}

  explicit Vec(size_type vec_size) : data_vec(vec_size) {}

  constexpr Vec(size_type vec_size, const T& value)
      : data_vec(vec_size, value) {}

  constexpr Vec(const Vec<T>& v) : data_vec(v.data_vec) {}

  constexpr Vec(Vec<T>&& v) noexcept : data_vec(std::move(v.data_vec)) {}
  constexpr Vec(Impl_t&& v) noexcept : data_vec(std::move(v)) {}

  constexpr Vec(const Impl_t& v) : data_vec(std::cbegin(v), std::cend(v)) {}

  template <class InputIt>
  requires std::input_iterator<InputIt> constexpr Vec(InputIt first,
                                                      InputIt last)
      : data_vec(first, last) {}

  constexpr Vec(std::initializer_list<T> init) : data_vec(init) {}

  // assignment
  constexpr Vec<T>& operator=(const Vec<T>& v) {
    data_vec = v.data_vec;
    return *this;
  }

  constexpr Vec<T>& operator=(Vec<T>&& v) noexcept {
    std::swap(data_vec, v.data_vec);
    return *this;
  }

  constexpr Vec<T>& operator=(std::initializer_list<T> ilist) {
    data_vec = ilist;
    return *this;
  }

  // conversion
  template <typename U>
  requires std::convertible_to<U, T> constexpr operator Vec<U>() const {
    Vec<U> ret(size());
    std::copy(begin(), end(), std::begin(ret));
    return ret;
  }

  // accessors
  constexpr auto at(size_type pos) -> reference { return data_vec.at(pos); }
  constexpr auto at(size_type pos) const -> const_reference {
    return data_vec.at(pos);
  }
  constexpr auto at(size_type pos, value_type fallback) -> value_type {
    return pos < data_vec.size() ? data_vec[pos] : fallback;
  }
  constexpr auto at(size_type pos, value_type fallback) const -> value_type {
    return pos < data_vec.size() ? data_vec[pos] : fallback;
  }
  constexpr reference operator[](size_type pos) { return data_vec[pos]; }
  constexpr const_reference operator[](size_type pos) const {
    return data_vec[pos];
  }

  template <typename V>
  requires std::convertible_to<V, T> constexpr Vec
  operator[](const Vec<V>& conds) const {
    const size_type n = conds.size();

    // if (n != size())
    // {
    //     panic()
    // }

    Vec<T> ret;
    ret.reserve(n);
    for (size_type i = 0; i < n; ++i) {
      if (conds[i]) {
        ret.emplace_back(data_vec[i]);
      }
    }
    return ret;
  }

  constexpr auto front() -> reference { return data_vec.front(); }
  constexpr auto front() const -> const_reference { return data_vec.front(); }
  constexpr auto back() -> reference { return data_vec.back(); }
  constexpr auto back() const -> const_reference { return data_vec.back(); }
  constexpr auto data() noexcept -> pointer { return data_vec.data(); }
  constexpr auto data() const noexcept -> const_pointer {
    return data_vec.data();
  }
  // iterators
  constexpr auto begin() noexcept -> iterator { return data_vec.begin(); }
  constexpr auto begin() const noexcept -> const_iterator {
    return data_vec.begin();
  }
  constexpr auto cbegin() const noexcept -> const_iterator {
    return data_vec.cbegin();
  }
  constexpr auto end() noexcept -> iterator { return data_vec.end(); }
  constexpr auto end() const noexcept -> const_iterator {
    return data_vec.end();
  }
  constexpr auto cend() const noexcept -> const_iterator {
    return data_vec.cend();
  }
  constexpr auto rbegin() noexcept -> reverse_iterator {
    return data_vec.rbegin();
  }
  constexpr auto rbegin() const noexcept -> const_reverse_iterator {
    return data_vec.rbegin();
  }
  constexpr auto crbegin() const noexcept -> const_reverse_iterator {
    return data_vec.crbegin();
  }
  constexpr auto rend() noexcept -> reverse_iterator { return data_vec.rend(); }
  constexpr auto rend() const noexcept -> const_reverse_iterator {
    return data_vec.rend();
  }
  constexpr auto crend() const noexcept -> const_reverse_iterator {
    return data_vec.crend();
  }
  // capacity
  constexpr bool empty() const noexcept { return data_vec.empty(); }
  constexpr auto size() const noexcept -> size_type { return data_vec.size(); }
  constexpr auto max_size() const noexcept -> size_type {
    return data_vec.size();
  }
  constexpr void reserve(size_type new_cap) { data_vec.reserve(new_cap); }
  constexpr auto capacity() const noexcept -> size_type {
    return data_vec.capacity();
  }
  constexpr void shrink_to_fit() { data_vec.shrink_to_fit(); };
  // modifiers
  constexpr void clear() noexcept { data_vec.clear(); }
  constexpr auto erase(iterator pos) -> iterator { return data_vec.erase(pos); }
  constexpr auto erase(iterator first, iterator last) -> iterator {
    return data_vec.erase(first, last);
  }
  constexpr void push_back(T&& value) {
    data_vec.push_back(std::forward<T>(value));
  }
  constexpr void push_back(const value_type& value) {
    data_vec.push_back(value);
  };
  template <class... Args>
  constexpr auto emplace_back(Args&&... args) -> reference {
    return data_vec.emplace_back(std::forward<Args>(args)...);
  }

  template <typename U>
  requires Number<U> constexpr auto emplace(const_iterator pos, U value)
      -> iterator {
    return data_vec.emplace(pos, value);
  }
  constexpr void pop_back() { data_vec.pop_back(); }
  constexpr void resize(size_type count) { data_vec.resize(count); }
  constexpr void resize(size_type count, const value_type& value) {
    data_vec.resize(count, value);
  }
  constexpr void swap(Vec<T>& other) { std::swap(data_vec, other.data_vec); }

  template <typename F, typename R = decltype(
                            std::declval<F>()(std::declval<value_type>()))>
  requires std::invocable<F, T> constexpr auto map(F&& fn) const -> Vec<R> {
    Vec<R> ret;
    ret.reserve(size());
    std::transform(cbegin(), cend(), std::back_inserter(ret),
                   std::forward<F>(fn));
    return ret;
  }

  template <typename F>
  requires std::predicate<F, T> constexpr auto filter(F&& f) const -> Vec<T> {
    Vec<T> res;
    res.reserve(size());
    for (const auto& val : data_vec) {
      if (f(val)) {
        res.emplace_back(val);
      }
    }
    return res;
  }

  constexpr auto take(const Vec<size_type>& ids) const -> Vec<T> {
    const size_type out_size = size();
    Vec<T> res;
    res.reserve(out_size);
    for (size_type k = 0; k < out_size; k++) {
      res.push_back(data_vec[ids[k]]);
    }
    return res;
  }

  constexpr auto take(size_type n) const -> Vec<T> {
    Vec<T> res;
    res.reserve(n);
    std::copy(cbegin(), cbegin() + n, std::back_inserter(res));
    return res;
  }

  constexpr auto Min() const -> value_type {
    return *std::min_element(cbegin(), cend());
  }

  constexpr auto IdAndMin() const -> std::pair<difference_type, value_type> {
    constexpr auto val_it{std::min_element(cbegin(), cend())};
    constexpr auto idx{std::distance(cbegin(), val_it)};
    return std::make_pair(idx, *val_it);
  }

  constexpr auto Max() const -> value_type {
    return *std::max_element(cbegin(), cend());
  }

  constexpr auto IdAndMax() const -> std::pair<difference_type, value_type> {
    constexpr auto val_it{std::max_element(cbegin(), cend())};
    constexpr auto idx{std::distance(cbegin(), val_it)};
    return std::make_pair(idx, *val_it);
  }

  constexpr auto Sort() const -> Vec<T> {
    Vec<T> res(data_vec);
    std::sort(std::begin(res), std::end(res));
    return res;
  }

  template <typename F>
  requires std::predicate<F, T> constexpr auto Sort(F&& c) const -> Vec<T> {
    Vec<T> res(data_vec);
    std::sort(std::begin(res), std::end(res), std::forward<F>(c));
    return res;
  }

private:
  Impl_t data_vec;
};

template <class T>
auto operator<<(std::ostream& os, const Vec<T>& v) -> std::ostream& {
  os << "{ ";
  const auto size = v.size();
  if (size) {
    for (std::size_t i = 0; i < size - 1; ++i) {
      os << v[i] << ", ";
    }
    os << v[size - 1];
  }
  os << " }";
  return os;
}

} // namespace Vecor

#endif // VECOR_HPP

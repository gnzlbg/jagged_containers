// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Implements a jagged vector

#if !defined(JAGGED_DETAIL_VECTOR_HPP)
#define JAGGED_DETAIL_VECTOR_HPP

#include <vector>
#include <iterator>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace jagged {

struct reserve_t {};
const constexpr reserve_t reserve{};

template <class T>
using default_jagged_vector_container
    = boost::container::vector<T, std::allocator<T>>;

/// \brief vector
template <class ValueType,
          class ValueContainer = default_jagged_vector_container<ValueType>,
          class IndexContainer
          = std::vector<typename ValueContainer::size_type>>
class vector {
 public:
  using value_type = ValueType;
  using container_type = ValueContainer;
  using allocator_type = typename container_type::allocator_type;
  using size_type = typename container_type::size_type;
  using difference_type = typename container_type::difference_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer
      = typename std::allocator_traits<allocator_type>::const_pointer;

  using container_iterator = typename container_type::iterator;
  using container_const_iterator = typename container_type::const_iterator;
  using range_type = boost::iterator_range<container_iterator>;
  using const_range_type = boost::iterator_range<container_const_iterator>;

  // static assert container iterator == random access iterator
  // static assert range_type == random access range

 private:
  using index_container = IndexContainer;
  using index_container_iterator = typename index_container::iterator;
  using index_container_const_iterator
      = typename index_container::const_iterator;

  class iterator_impl
      : public boost::iterator_facade<iterator_impl, range_type,
                                      std::random_access_iterator_tag,
                                      range_type, difference_type> {
   public:
    iterator_impl() = default;
    iterator_impl(iterator_impl const&) = default;
    iterator_impl(iterator_impl&&) = default;
    iterator_impl& operator=(iterator_impl const&) = default;
    iterator_impl& operator=(iterator_impl&&) = default;
    explicit iterator_impl(index_container_iterator iIt, container_iterator cIt)
      : index_iterator_(iIt), container_iterator_(cIt) {}

   private:
    friend class boost::iterator_core_access;

    void increment() { ++index_iterator_; }
    void decrement() { --index_iterator_; }

    void advance(difference_type n) { index_iterator_ += n; }
    difference_type distance_to(iterator_impl const& other) {
      return std::distance(this->index_iterator_, other.index_iterator_);
    }

    bool equal(iterator_impl const& other) const {
      return this->index_iterator_ == other.index_iterator_
             && this->container_iterator_ == other.container_iterator_;
    }

    range_type dereference() const {
      return boost::make_iterator_range(
          std::next(container_iterator_, *index_iterator_),
          std::next(container_iterator_, *std::next(index_iterator_, 1)));
    }

    index_container_iterator index_iterator_;
    container_iterator container_iterator_;
  };

 public:
  using iterator = iterator_impl;

  ///
  // using reference = ; // value_type&
  // using const_reference; // value_type const&
  // using iterator = ; // RandomAccessIterator
  // using const_iterator = ; //	 Constant random access iterator
  // using reverse_iterator = std::reverse_iterator<iterator>;
  // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /// Constructors
  ///@{
  explicit vector(reserve_t, size_type noElements, size_type noVectors) {
    indices_.reserve(noVectors + 1);
    indices_.push_back(0);
    values_.reserve(noElements);
  }
  // explicit vector(std::initializer_list<
  //                                std::initializer_list<value_type>>&& values)
  //                                {

  // }
  ///@}

  /// \name vector iterators
  ///@{
  iterator begin() noexcept {
    return iterator{indices_.begin(), values_.begin()};
  }
  iterator end() noexcept {
    return iterator{--indices_.end(), values_.begin()};
  }
  // const_iterator cbegin() const noexcept { }
  // const_iterator cend() const noexcept {  }
  // const_iterator begin() const noexcept { return cbegin(); }
  // const_iterator end() const noexcept { return cend(); }

  // reverse_iterator rbegin() noexcept { return end(); }
  // reverse_iterator rend() noexcept { return begin(); }
  // const_reverse_iterator rbegin() const noexcept { return cend(); }
  // const_reverse_iterator rend() const noexcept { return cbegin(); }
  ///@}

  /// Capacity
  ///@{
  // inline bool empty() const {}
  // size_type capacity() const {}
  // size_type size() const {}
  // void reserve(std::size_t n) {}
  // void shrink_to_fit() {}
  // void max_size() {}
  ///@}

  /// Modifiers
  ///@{
  // void clear() {}
  // iterator insert(const_iterator pos, const T& value) {}
  // // iterator insert( const_iterator pos, size_type count, const T& value );
  // template <class InputIt>
  // iterator insert(const_iterator pos, InputIt first, InputIt last) { }
  // iterator erase(const_iterator pos) {}
  // iterator erase(const_iterator first, const_iterator last) {}

  template <class ForwardRange> void push_back(ForwardRange const& range) {
    const auto first_index = indices_.back();
    const auto no_elements = boost::distance(range);
    const auto last_index = first_index + no_elements;
    indices_.push_back(last_index);
    boost::push_back(values_, range);
  }
  // void push_back(T&& value) {};
  // void push_back(const value_type& value) { };
  // void push_back(value_type&& value) {};
  // void pop_back() {}
  // void resize(std::size_t n) {}
  ///@}

 private:
  index_container indices_;
  container_type values_;
};

}  // namespace jagged

#endif  // JAGGED_DETAIL_VECTOR_HPP

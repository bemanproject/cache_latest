#ifndef BEMAN_CACHE_LATEST_H
#define BEMAN_CACHE_LATEST_H

// clang-format off
#if __cplusplus < 202302L
  #error "C++23 or later is required"
#endif
// clang-format on

#include <ranges>
#include <optional>
#include <type_traits>
#include <functional>

namespace beman {
template <typename T>
class non_propagating_cache {
  private:
    std::optional<T> t_;

  public:
    non_propagating_cache() = default;
    non_propagating_cache(const non_propagating_cache& other) {}
    non_propagating_cache(non_propagating_cache&& other) { other.t_.reset(); }

    non_propagating_cache& operator=(const non_propagating_cache& other) {
        t_.reset();
        return *this;
    }

    non_propagating_cache& operator=(non_propagating_cache&& other) {
        other.t_.reset();
        t_.reset();
        return *this;
    }

    T* operator*() { return *t_; }

    bool has_value() { return t_.has_value(); }

    T& value() { return t_.value(); }

    template <class... Args>
    T& emplace(Args&&... args) {
        return t_.emplace(std::forward<Args>(args)...);
    }

    void reset() { t_.reset(); }
};

template <std::ranges::input_range V>
    requires std::ranges::view<V>
class cache_latest_view : public std::ranges::view_interface<cache_latest_view<V>> {
    V base_       = V();                                                                       // exposition only
    using cache_t = std::conditional_t<std::is_reference_v<std::ranges::range_reference_t<V>>, // exposition only
                                       std::add_pointer_t<std::ranges::range_reference_t<V>>,
                                       std::ranges::range_reference_t<V>>;

    non_propagating_cache<cache_t> cache_; // exposition only

    class iterator; // exposition only
    class sentinel; // exposition only

  public:
    cache_latest_view()
        requires std::default_initializable<V>
    = default;
    constexpr explicit cache_latest_view(V base) : base_(std::move(base)) {}

    constexpr V base() const&
        requires std::copy_constructible<V>
    {
        return base_;
    }
    constexpr V base() && { return std::move(base_); }

    constexpr auto begin() { return iterator(*this); }
    constexpr auto end() { return sentinel(*this); }

    constexpr auto size()
        requires std::ranges::sized_range<V>
    {
        return std::ranges::size(base_);
    }
    constexpr auto size() const
        requires std::ranges::sized_range<const V>
    {
        return std::ranges::size(base_);
    }
};

template <class R>
cache_latest_view(R&&) -> cache_latest_view<std::views::all_t<R>>;

template <std::ranges::input_range V>
    requires std::ranges::view<V>
class cache_latest_view<V>::iterator {
    cache_latest_view*         parent_;  // exposition only
    std::ranges::iterator_t<V> current_; // exposition only

    constexpr explicit iterator(cache_latest_view& parent)
        : current_(std::ranges::begin(parent.base_)), parent_(std::addressof(parent)) {}

    friend class cache_latest_view<V>;

  public:
    using difference_type  = std::ranges::range_difference_t<V>;
    using value_type       = std::ranges::range_value_t<V>;
    using iterator_concept = std::input_iterator_tag;

    iterator(iterator&&)            = default;
    iterator& operator=(iterator&&) = default;

    constexpr std::ranges::iterator_t<V>        base() && { return std::move(current_); }
    constexpr const std::ranges::iterator_t<V>& base() const& noexcept { return current_; }

    constexpr std::ranges::range_reference_t<V>& operator*() const {
        if constexpr (std::is_reference_v<std::ranges::range_reference_t<V>>) {
            if (!parent_->cache_.has_value()) {
                parent_->cache_ = std::addressof(*current_);
            }
            return **parent_->cache_;
        } else {
            if (!parent_->cache_.has_value()) {
                return parent_->cache_.emplace(*current_);
            }
            return parent_->cache_.value();
        }
    }

    constexpr iterator& operator++() {
        ++current_;
        parent_->cache_.reset();
        return *this;
    }

    constexpr void operator++(int) { ++*this; }

    friend constexpr std::ranges::range_rvalue_reference_t<V>
    iter_move(const iterator& i) noexcept(noexcept(std::ranges::iter_move(i.current_))) {
        return std::ranges::iter_move(i.current_);
    }

    friend constexpr void
    iter_swap(const iterator& x, const iterator& y) noexcept(noexcept(std::ranges::iter_swap(x.current_, y.current_)))
        requires std::indirectly_swappable<std::ranges::iterator_t<V>>
    {
        std::ranges::iter_swap(x.current_, y.current_);
    }
};

template <std::ranges::input_range V>
    requires std::ranges::view<V>
class cache_latest_view<V>::sentinel {
    std::ranges::sentinel_t<V> end_ = std::ranges::sentinel_t<V>(); // exposition only

    constexpr explicit sentinel(cache_latest_view& parent) : end_(std::ranges::end(parent.base_)) {}

    friend cache_latest_view<V>;
    friend cache_latest_view<V>::iterator;

  public:
    sentinel() = default;

    constexpr std::ranges::sentinel_t<V> base() const { return end_; }

    friend constexpr bool operator==(const iterator& x, const sentinel& y) { return x.current_ == y.end_; }
};

namespace __cache_latest {
template <class _Fn>
struct __pipeable : _Fn, std::ranges::range_adaptor_closure<__pipeable<_Fn>> {
    constexpr explicit __pipeable(_Fn&& __f) : _Fn(std::move(__f)) {}
};

struct __fn {
    template <std::ranges::input_range _View>
        requires std::ranges::view<_View>
    [[nodiscard]] constexpr auto operator()(_View&& _view) {
        return cache_latest_view(std::forward<_View>(_view));
    }

    [[nodiscard]] constexpr auto operator()() {
        return __pipeable([*this](auto&& r) -> decltype(auto) { return (*this)(std::forward<decltype(r)>(r)); });
    }
};

struct pipeable_cache_latest {
    template <std::ranges::viewable_range R>
    friend constexpr auto operator|(R&& r, pipeable_cache_latest) {
        return __fn{}(std::forward<R>(r));
    }
};
} // namespace __cache_latest

namespace views {
inline constexpr auto cache_latest = __cache_latest::pipeable_cache_latest{};
}
} // namespace beman

#endif

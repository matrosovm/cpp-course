#pragma once

#include <tuple>
#include <algorithm>
#include <type_traits>


namespace cls01 {
    template <class... Container>
    class zip {
    public:
        explicit zip(Container&... container) :
            data_(std::reference_wrapper<Container>(container)...) { }

        template <class... Iterator>
        class ZipTuple : public std::tuple<Iterator...> {
        public:
            template <size_t I>
            auto& get() {
                return *std::get<I>(*this);
            }

            template <size_t I>
            const auto& get() const {
                return *std::get<I>(*this);
            }

            ZipTuple* operator->() {
                return this;
            }

            const ZipTuple* operator->() const {
                return this;
            }
        };

        class iterator : public std::iterator<std::bidirectional_iterator_tag,
                ZipTuple<typename Container::iterator...>> {
        public:
            explicit iterator(const std::tuple<typename Container::iterator...>& rhs) :
                data_(rhs) { }

            iterator& operator=(const iterator& rhs) = default;
            iterator& operator=(iterator&& rhs) = default;

            auto operator*() {
                return ZipTuple(data_);
            }

            auto operator->() {
                return ZipTuple(data_);
            }

            const iterator& operator++() {
                std::for_each(data_, [](auto&... it) {
                    (++it, ...);
                });
                return *this;
            }

            iterator operator++(int) {
                iterator res(data_);
                std::for_each(data_, [](auto&... it) {
                    (++it, ...);
                });
                return res;
            }

            const iterator& operator--() {
                std::for_each(data_, [](auto&... it) {
                    (--it, ...);
                });
                return *this;
            }

            iterator operator--(int) {
                iterator res(data_);
                std::for_each(data_, [](auto&... it) {
                    (--it, ...);
                });
                return res;
            }

            bool operator==(iterator& rhs) {
                return equals(std::make_index_sequence<std::tuple_size_v<std::tuple<
                        typename Container::iterator...>>>(), rhs);
            }

        private:
            std::tuple<typename Container::iterator...> data_;

            template<std::size_t... I>
            bool is_eq(std::index_sequence<I...>,
                    const iterator& other) const {
                return (... || (std::get<I>(data_) == std::get<I>(other.data_)));
            }
        };

        class const_iterator {

        };

        auto begin() {
            return get_iterators(std::make_index_sequence<std::tuple_size_v<std::tuple<
                    typename Container::iterator...>>>(), [](auto& el) {
                    return std::begin(el);
            });
        }

        auto end() {
            if constexpr (is_const) {
                return cend();
            } else {
                return get_iterators(std::make_index_sequence<size>(), [](auto &el) {
                    return std::end(el);
                });
            }
        }

    private:
        std::tuple<std::reference_wrapper<Container>...> data_;

        template<std::size_t... I, class Functor>
        auto make_iterator(std::index_sequence<I...>, Functor get_iters) const {
            return iterator(std::make_tuple((get_iters(std::get<I>(data_).get()))...));
        }
    };

    template <class... Container>
    zip<Container...> make_zip(Container&... container) {
        return zip<Container...>(container...);
    }

}

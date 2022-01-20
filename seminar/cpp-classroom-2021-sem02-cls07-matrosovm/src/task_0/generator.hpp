#pragma once

#include <experimental/coroutine>
#include <optional>

namespace cls_07 {
    namespace cor = std::experimental;

    template<class T>
    class generator {
    public:
        struct promise_type {
            auto get_return_object() {
                return generator{handle::from_promise(*this)};
            }

            cor::suspend_always initial_suspend() noexcept {
                return {};
            }

            cor::suspend_always final_suspend() noexcept {
                return {};
            }

            cor::suspend_always yield_value(T value) {
                current_value = std::move(value);
                return {};
            }

            void unhandled_exception() {
                throw;
            }

            auto return_void() {
                return cor::suspend_never{};
            }

            T current_value{};
        };

        struct iterator {
            generator& owner;
            bool done;

            iterator(generator& o, bool d) : owner(o), done(d) { }

            void advance() {
                owner.coro_.resume();
                auto still_going = not owner.coro_.done();
                done = not still_going;
            }

            bool operator!=(const iterator& r) const {
                return done != r.done;
            }

            iterator& operator++() {
                advance();
                return *this;
            }

            auto operator*() const {
                return owner.coro_.promise().current_value;
            }
        };

        using handle = cor::coroutine_handle<promise_type>;

        generator(handle h) : coro_(h) {}

        generator(generator&& gen) noexcept : coro_(std::move(gen.coro_)) {
            gen.coro_ = nullptr;
        }

        auto current_value() {
            return coro_.promise().current_value;
        }

        bool move_next() {
            coro_.resume();
            return !coro_.done();
        }

        auto begin() {
            return iterator{*this, false};
        }

        auto end() {
            return iterator{*this, true};
        }

        ~generator() {
            if (coro_) {
                coro_.destroy();
            }
        }

        void swap(generator& gen) {
            std::swap(coro_, gen.coro_);
        }

    private:
        handle coro_;
    };

    template<class T>
    class generate {
        using type_elem = decltype(T{}());

    public:
        generate(T func) : callable_([](T func_in) -> generator<type_elem> {
            while (true) {
                co_yield func_in();
            }
        } (func)) {
            move_next();
        }

        generate(const generate&) = delete;
        generate(generate&& other) noexcept : callable_(std::move(other.callable_)) {}

        generate& operator=(const generate&) = delete;

        generate& operator=(generate&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            auto tmp = std::move(other);
            callable_.swap(tmp.callable_);
            return *this;
        }

        auto current_value() {
            return callable_.current_value();
        }

        bool move_next() {
            return callable_.move_next();
        }

        auto begin() {
            return callable_.begin();
        }

        auto end() {
            return callable_.end();
        }

    private:
        generator<type_elem> callable_;
    };

}
#pragma once

#include <type_traits>
#include <algorithm>
#include <tuple>

namespace util {
    class bad_get : public std::exception {
        const char* what () const noexcept override {
            return "unable to get this type";
        }
    };
    const size_t variant_npos = -1;

    template <class... Types>
    class variant;

    template <class T, class... Types>
    const T& get(const variant<Types...>& var);

    template <class T, class... Types>
    T& get(variant<Types...>& var);

    template <class T, class... Types>
    T&& get(variant<Types...>&& var);

    template <class T, class... Types>
    const T* get(const variant<Types...>* var);

    template <class T, class... Types>
    T* get(variant<Types...>* var);

    namespace detail {
        template <class First, class... Types>
        struct storage_base {
            static void copy(void* storage_ptr, const void* other_storage_ptr, size_t type_hash) {
                if (type_hash == typeid(First).hash_code()) {
                    new(storage_ptr) First(*static_cast<const First*>(other_storage_ptr));
                } else {
                    storage_base<Types...>::copy(storage_ptr, other_storage_ptr, type_hash);
                }
            }

            static void move(void* storage_ptr, void* other_storage_ptr, size_t type_hash) {
                if (type_hash == typeid(First).hash_code()) {
                    new(storage_ptr) First(std::move(*static_cast<First*>(other_storage_ptr)));
                } else {
                    storage_base<Types...>::move(storage_ptr, other_storage_ptr, type_hash);
                }
            }

            template <class Type>
            static bool set(void* storage_ptr, Type&& object) {
                if (typeid(Type).hash_code() == typeid(First).hash_code()) {
                    new(storage_ptr) Type(std::forward<Type>(object));
                    return true;
                }
                return storage_base<Types...>::set(storage_ptr, std::forward<Type>(object));
            }

            template <class Type, typename std::enable_if_t<std::is_convertible_v<Type, First>, bool> = true>
            static void convert(void* storage_ptr, Type object, size_t& type_hash) {
                new(storage_ptr) First(std::move(object));
                type_hash = typeid(First).hash_code();
            }

            template <class Type, typename std::enable_if_t<!std::is_convertible_v<Type, First>, bool> = true>
            static void convert(void* storage_ptr, Type object, size_t& type_hash) {
                storage_base<Types...>::convert(storage_ptr, std::move(object), type_hash);
            }

            template <class Type>
            static Type& get(void* storage_ptr) {
                if (typeid(Type).hash_code() == typeid(First).hash_code()) {
                    return *static_cast<Type*>(storage_ptr);
                }
                return storage_base<Types...>::template get<Type>(storage_ptr);
            }

            static size_t index(size_t type_hash, size_t i) {
                if (type_hash == typeid(First).hash_code()) {
                    return i;
                }
                return storage_base<Types...>::index(type_hash, i + 1);
            }

            template <class Type, class... Args>
            static void apply_visitor(Type&& object, const variant<Args...>& var) {
                if (var.type_hash_ == typeid(First).hash_code()) {
                    object(util::get<First>(var));
                }
                storage_base<Types...>::template apply_visitor(std::forward<Type>(object), var);
            }

            template <class Type, class... Args>
            static void apply_visitor(Type&& object, variant<Args...>& var) {
                if (var.type_hash_ == typeid(First).hash_code()) {
                    object(util::get<First>(var));
                }
                storage_base<Types...>::template apply_visitor(std::forward<Type>(object), var);
            }

            template <class Type, class... Args>
            static void apply_visitor(Type&& object, variant<Args...>&& var) {
                if (var.type_hash_ == typeid(First).hash_code()) {
                    object(util::get<First>(std::move(var)));
                }
                storage_base<Types...>::template apply_visitor(std::forward<Type>(object), std::move(var));
            }

            static void deleter(void* storage_ptr, size_t type_hash) {
                if (type_hash == typeid(First).hash_code()) {
                    static_cast<First*>(storage_ptr)->~First();
                } else {
                    storage_base<Types...>::deleter(storage_ptr, type_hash);
                }
            }
        };

        template <class First>
        struct storage_base<First> {
            static void copy(void* storage_ptr, const void* other_storage_ptr, size_t type_hash) {
                if (type_hash == typeid(First).hash_code()) {
                    new(storage_ptr) First(*static_cast<const First*>(other_storage_ptr));
                }
            }

            static void move(void* storage_ptr, void* other_storage_ptr, size_t type_hash) {
                if (type_hash == typeid(First).hash_code()) {
                    new(storage_ptr) First(std::move(*static_cast<First*>(other_storage_ptr)));
                }
            }

            template <class Type>
            static bool set(void* storage_ptr, Type&& object) {
                if (typeid(Type).hash_code() == typeid(First).hash_code()) {
                    new(storage_ptr) Type(std::forward<Type>(object));
                    return true;
                }
                return false;
            }

            template <class Type, typename std::enable_if_t<std::is_convertible_v<Type, First>, bool> = true>
            static void convert(void* storage_ptr, Type object, size_t& type_hash) {
                new(storage_ptr) First(std::move(object));
                type_hash = typeid(First).hash_code();
            }

            template <class Type, typename std::enable_if_t<!std::is_convertible_v<Type, First>, bool> = true>
            static void convert(void* storage_ptr, Type object, size_t& type_hash) = delete;

            template <class Type>
            static Type& get(void* storage_ptr) {
                if (typeid(Type).hash_code() == typeid(First).hash_code()) {
                    return *static_cast<Type*>(storage_ptr);
                }
                throw bad_get();
            }

            static size_t index(size_t type_hash, size_t i) {
                if (type_hash == typeid(First).hash_code()) {
                    return i;
                }
                return variant_npos;
            }

            template <class Type, class... Args>
            static void apply_visitor(Type&& object, const variant<Args...>& var) {
                if (var.type_hash_ == typeid(First).hash_code()) {
                    object(util::get<First>(var));
                }
             }

            template <class Type, class... Args>
            static void apply_visitor(Type&& object, variant<Args...>& var) {
                if (var.type_hash_ == typeid(First).hash_code()) {
                    object(util::get<First>(var));
                }
            }

            template <class Type, class... Args>
            static void apply_visitor(Type&& object, variant<Args...>&& var) {
                if (var.type_hash_ == typeid(First).hash_code()) {
                    object(util::get<First>(std::move(var)));
                }
            }

            static void deleter(void* storage_ptr, size_t type_hash) {
                if (type_hash == typeid(First).hash_code()) {
                    static_cast<First*>(storage_ptr)->~First();
                }
            }
        };
    }

    template <class... Types>
    class variant {
    public:
        variant() : type_hash_(typeid(void).hash_code()) { }

        variant(const variant& other) noexcept((... && (std::is_nothrow_default_constructible_v<Types>))) :
            type_hash_(other.type_hash_) {
            detail::storage_base<Types...>::copy(static_cast<void*>(&storage_),
                    static_cast<const void*>(&other.storage_), other.type_hash_);
        }

        variant(variant&& other) noexcept((... && (std::is_nothrow_move_constructible_v<Types>))) :
            type_hash_(other.type_hash_) {
            detail::storage_base<Types...>::move(static_cast<void*>(&storage_), static_cast<void*>(&other.storage_),
                    other.type_hash_);
        }

        template <class Type>
        variant(Type object) noexcept(std::is_nothrow_default_constructible_v<Type>) :
                type_hash_(typeid(Type).hash_code()) {
            if (!detail::storage_base<Types...>::set(static_cast<void*>(&storage_), std::move(object))) {
                detail::storage_base<Types...>::convert(static_cast<void*>(&storage_),
                std::move(object), type_hash_);
            }
        }

        variant& operator=(const variant<Types...>& other) noexcept((... && (
                std::is_nothrow_default_constructible_v<Types>))) {
            if (this == &other) {
                return *this;
            }
            variant copy(*this);
            try {
                detail::storage_base<Types...>::deleter(static_cast<void*>(&storage_), type_hash_);
                type_hash_ = other.type_hash_;
                detail::storage_base<Types...>::copy(static_cast<void*>(&storage_),
                    static_cast<const void*>(&other.storage_), other.type_hash_);
            } catch (...) {
                type_hash_ = copy.type_hash_;
                detail::storage_base<Types...>::copy(static_cast<void*>(&storage_),
                    static_cast<const void*>(&copy.storage_), copy.type_hash_);
                throw ;
            }
            return *this;
        }

        variant& operator=(variant<Types...>&& other) noexcept((... && (
                std::is_nothrow_move_constructible_v<Types>))) {
            if (this == &other) {
                return *this;
            }
            variant move(std::move(*this));
            try {
                detail::storage_base<Types...>::deleter(static_cast<void*>(&storage_), type_hash_);
                type_hash_ = other.type_hash_;
                detail::storage_base<Types...>::move(static_cast<void*>(&storage_), static_cast<void*>(&other.storage_),
                                                     other.type_hash_);
            } catch (...) {
                type_hash_ = move.type_hash_;
                detail::storage_base<Types...>::move(static_cast<void*>(&storage_),
                    static_cast<void*>(&move.storage_), move.type_hash_);
                throw ;
            }
            return *this;
        }

        template <class Type>
        variant& operator=(Type object) noexcept(std::is_nothrow_default_constructible_v<Type>) {
            if (static_cast<void*>(&storage_) == static_cast<void*>(&object)) {
                return *this;
            }
            variant copy(*this);
            try {
                detail::storage_base<Types...>::deleter(static_cast<void*>(&storage_), type_hash_);
                type_hash_ = typeid(object).hash_code();
                if (!detail::storage_base<Types...>::set(static_cast<void*>(&storage_), std::move(object))) {
                    detail::storage_base<Types...>::convert(static_cast<void*>(&storage_),
                        std::move(object), type_hash_);
                }
            } catch (...) {
                type_hash_ = copy.type_hash_;
                detail::storage_base<Types...>::copy(static_cast<void*>(&storage_),
                    static_cast<const void*>(&copy.storage_), copy.type_hash_);
                throw ;
            }
            return *this;
        }

        bool empty() const {
            return type_hash_ == typeid(void).hash_code();
        }

        void swap(variant<Types...>& var) {
            auto tmp = *this;
            *this = var;
            var = tmp;
        }

        size_t index() const {
            if (empty()) {
                return variant_npos;
            }
            return detail::storage_base<Types...>::index(type_hash_, 0);
        }

        void clear() {
            if (empty()) {
                return;
            }
            detail::storage_base<Types...>::deleter(static_cast<void*>(&storage_), type_hash_);
            type_hash_ = typeid(void).hash_code();
        }

        using type = std::aligned_storage_t<std::max({sizeof(Types)...}), std::max({alignof(Types)...})>;

        template <class T, class... Args>
        friend const T& get(const variant<Args...>& var);

        template <class T, class... Args>
        friend T& get(variant<Args...>& var);

        template <class T, class... Args>
        friend T&& get(variant<Args...>&& var);

        template <class T, class... Args>
        friend const T* get(const variant<Args...>* var);

        template <class T, class... Args>
        friend T* get(variant<Args...>* var);

        template <size_t Ind, class... Args>
        friend const typename std::tuple_element<Ind, std::tuple<Args...>>::type& get(const variant<Args...>& var);

        template <size_t Ind, class... Args>
        friend typename std::tuple_element<Ind, std::tuple<Args...>>::type& get(variant<Args...>& var);

        template <size_t Ind, class... Args>
        friend typename std::tuple_element<Ind, std::tuple<Args...>>::type&& get(variant<Args...>&& var);

        template <size_t Ind, class... Args>
        friend const typename std::tuple_element<Ind, std::tuple<Args...>>::type* get(const variant<Args...>* var);

        template <size_t Ind, class... Args>
        friend typename std::tuple_element<Ind, std::tuple<Args...>>::type* get(variant<Args...>* var);

        template <class T, class... Args>
        friend void apply_visitor(T&& object, const variant<Args...>& var);

        template <class T, class... Args>
        friend void apply_visitor(T&& object, variant<Args...>& var);

        template <class T, class... Args>
        friend void apply_visitor(T&& object, variant<Args...>&& var);

        template <class First, class... Args>
        friend struct detail::storage_base;

        ~variant() {
            if (empty()) {
                return;
            }
            detail::storage_base<Types...>::deleter(static_cast<void*>(&storage_), type_hash_);
        }

    private:
        type storage_;
        size_t type_hash_;

    };

    template <class T, class... Types>
    const T& get(const variant<Types...>& var) {
        if (var.type_hash_ != typeid(T).hash_code()) {
            throw bad_get();
        }
        return detail::storage_base<Types...>::template get<T>(static_cast<void*>(
                &(const_cast<variant<Types...>*>(&var))->storage_));
    }

    template <class T, class... Types>
    T& get(variant<Types...>& var) {
        if (var.type_hash_ != typeid(T).hash_code()) {
            throw bad_get();
        }
        return detail::storage_base<Types...>::template get<T>(static_cast<void*>(&var.storage_));
    }

    template <class T, class... Types>
    T&& get(variant<Types...>&& var) {
        if (var.type_hash_ != typeid(T).hash_code()) {
            throw bad_get();
        }
        return std::move(detail::storage_base<Types...>::template get<T>(static_cast<void*>(&var.storage_)));
    }

    template <class T, class... Types>
    const T* get(const variant<Types...>* var) {
        try {
            if (var == nullptr || var->type_hash_ != typeid(T).hash_code()) {
                throw bad_get();
            }
            return &detail::storage_base<Types...>::template get<T>(static_cast<void*>(
                    &(const_cast<variant<Types...>*>(var))->storage_));
        } catch (bad_get&) {
            return nullptr;
        }
    }

    template <class T, class... Types>
    T* get(variant<Types...>* var) {
        try {
            if (var == nullptr || var->type_hash_ != typeid(T).hash_code()) {
                throw bad_get();
            }
            return &detail::storage_base<Types...>::template get<T>(static_cast<void*>(&var->storage_));
        } catch (bad_get&) {
            return nullptr;
        }
    }

    template <size_t Ind, class... Types>
    const typename std::tuple_element<Ind, std::tuple<Types...>>::type& get(const variant<Types...>& var) {
        using type = typename std::tuple_element<Ind, std::tuple<Types...>>::type;
        if (var.type_hash_ != typeid(type).hash_code()) {
            throw bad_get();
        }
        return detail::storage_base<Types...>::template get<type>(
                static_cast<void*>(&(const_cast<variant<Types...>*>(&var))->storage_));
    }

    template <size_t Ind, class... Types>
    typename std::tuple_element<Ind, std::tuple<Types...>>::type& get(variant<Types...>& var) {
        using type = typename std::tuple_element<Ind, std::tuple<Types...>>::type;
        if (var.type_hash_ != typeid(type).hash_code()) {
            throw bad_get();
        }
        return detail::storage_base<Types...>::template get<type>(static_cast<void*>(&var.storage_));
    }

    template <size_t Ind, class... Types>
    typename std::tuple_element<Ind, std::tuple<Types...>>::type&& get(variant<Types...>&& var) {
        using type = typename std::tuple_element<Ind, std::tuple<Types...>>::type;
        if (var.type_hash_ != typeid(type).hash_code()) {
            throw bad_get();
        }
        return std::move(detail::storage_base<Types...>::template get<type>(static_cast<void*>(&var.storage_)));
    }

    template <size_t Ind, class... Types>
    const typename std::tuple_element<Ind, std::tuple<Types...>>::type* get(const variant<Types...>* var) {
        using type = typename std::tuple_element<Ind, std::tuple<Types...>>::type;
        try {
            if (var == nullptr || var->type_hash_ != typeid(type).hash_code()) {
                throw bad_get();
            }
            return &detail::storage_base<Types...>::template get<type>(static_cast<void*>(
                    &(const_cast<variant<Types...>*>(var))->storage_));
        } catch (bad_get&) {
            return nullptr;
        }
    }

    template <size_t Ind, class... Types>
    typename std::tuple_element<Ind, std::tuple<Types...>>::type* get(variant<Types...>* var) {
        using type = typename std::tuple_element<Ind, std::tuple<Types...>>::type;
        try {
            if (var == nullptr || var->type_hash_ != typeid(type).hash_code()) {
                throw bad_get();
            }
            return &detail::storage_base<Types...>::template get<type>(static_cast<void*>(&var->storage_));
        } catch (bad_get&) {
            return nullptr;
        }
    }

    template <class... Types>
    void swap(variant<Types...>& l_var, variant<Types...>& r_var) {
        l_var.swap(r_var);
    }

    template <class T, class... Types>
    void apply_visitor(T&& object, const variant<Types...>& var) {
        if (var.type_hash_ == typeid(void).hash_code()) {
            object();
            return;
        }
        detail::storage_base<Types...>::template apply_visitor<T, Types...>(std::forward<T>(object), var);
    }

    template <class T, class... Types>
    void apply_visitor(T&& object, variant<Types...>& var) {
        if (var.type_hash_ == typeid(void).hash_code()) {
            object();
            return;
        }
        detail::storage_base<Types...>::template apply_visitor<T, Types...>(std::forward<T>(object), var);
    }

    template <class T, class... Types>
    void apply_visitor(T&& object, variant<Types...>&& var) {
        if (var.type_hash_ == typeid(void).hash_code()) {
            object();
            return;
        }
        detail::storage_base<Types...>::template apply_visitor<T, Types...>(std::forward<T>(object), std::move(var));
    }
}
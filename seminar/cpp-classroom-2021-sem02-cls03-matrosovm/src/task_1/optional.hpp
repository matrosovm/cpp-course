#pragma once

#include <memory>
#include <type_traits>
#include <iostream>

namespace cls_04 {

    template <class T, class = void>
    class optional;


    template <class T>
    class optional<T, std::enable_if_t<!std::is_literal_type_v<T>>> {
    public:
        optional() : empty_(true) { }

        optional(const T& object) : object_(), empty_(false) {
            new(&object_) T(object);
        }

        optional(T&& object) : object_(), empty_(false) {
            new(&object_) T(std::move(object));
        }

        optional(const optional& other) : object_(), empty_(other.empty_) {
            if (!other.empty_) {
                new(&object_) T(*reinterpret_cast<const T*>(&other.object_));
            }
        }

        optional(optional&& other) noexcept : object_(), empty_(other.empty_) {
            if (!other.empty_) {
                new(&object_) T(std::move(*reinterpret_cast<T*>(&other.object_)));
            }
        }

        optional& operator=(const T& object) {
            if (static_cast<const void*>(&object_) == static_cast<const void*>(&object)) {
                return *this;
            }
            reset();
            new(&object_) T(object);
            empty_ = false;
            return *this;
        }

        optional& operator=(T&& object) {
            if (static_cast<const void*>(&object_) == static_cast<const void*>(&object)) {
                return *this;
            }
            reset();
            new(&object_) T(std::move(object));
            empty_ = false;
            return *this;
        }

        optional& operator=(const optional& other) {
            if (static_cast<const void*>(&object_) == static_cast<const void*>(&other.object_)) {
                return *this;
            }
            reset();
            if (other.empty_) {
                return *this;
            }
            new(&object_) T(*reinterpret_cast<const T*>(&other.object_));
            empty_ = false;
            return *this;
        }

        optional& operator=(optional&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            reset();
            if (other.empty_) {
                return *this;
            }
            new(&object_) T(std::move(*reinterpret_cast<T*>(&other.object_)));
            empty_ = false;
            return *this;
        }

        friend bool operator==(const optional& opt, const T& obj) {
            if (opt.empty_) {
                return false;
            }
            return *reinterpret_cast<const T*>(&opt.object_) == *reinterpret_cast<const T*>(&obj);
        }

        friend bool operator==(const T& obj, const optional& opt) {
            return opt == obj;
        }

        friend bool operator==(const optional& l, const optional& r) {
            if (l.empty_ && r.empty_) {
                return true;
            } else if (l.empty_ || r.empty_) {
                return false;
            }
            return *reinterpret_cast<const T*>(&l.object_) == *reinterpret_cast<const T*>(&r.object_);
        }

        explicit operator bool() const {
            return !empty_;
        }

        const T& operator*() const {
            return *reinterpret_cast<const T*>(&object_);
        }

        T& operator*() {
            return *reinterpret_cast<T*>(&object_);
        }

        void reset() {
            if (empty_) {
                return;
            }
            empty_ = true;
            reinterpret_cast<T*>(&object_)->~T();
        }

        const T* operator->() const {
            return reinterpret_cast<const T*>(&object_);
        }

        T* operator->() {
            return reinterpret_cast<T*>(&object_);
        }

        const T& value() const {
            return operator*();
        }

        T& value() {
            return operator*();
        }

        T value_or(const T& val) const {
            return empty_ ? val : value();
        }

        ~optional() {
            if (!empty_) {
                reinterpret_cast<const T*>(&object_)->~T();
            }
        }

    private:
        std::aligned_storage_t<sizeof(T), alignof(T)> object_;
        bool empty_;
    };

    template <class T>
    class optional<T, std::enable_if_t<std::is_literal_type_v<T>>> {
    public:
        constexpr optional() : another_(), empty_(true) { }

        constexpr optional(const T& object) : object_(object), empty_(false) { }
        constexpr optional(T&& object) : object_(std::move(object)), empty_(false) { }
        constexpr optional(const optional& other) : object_(other.object_), empty_(other.empty_) { }
        constexpr optional(optional&& other) noexcept : object_(std::move(other.object_)), empty_(other.empty_) { }

        constexpr optional& operator=(const T& object) {
            if (object_ == &object) {
                return *this;
            }
            empty_ = false;
            object_ = object;
            return *this;
        }

        constexpr optional& operator=(T&& object) {
            if (&object_ == &object) {
                return *this;
            }
            empty_ = false;
            object_ = std::move(object);
            return *this;
        }

        constexpr optional& operator=(const optional& other) {
            if (this == &other) {
                return *this;
            }
            empty_ = other.empty_;
            object_ = other.object_;
            return *this;
        }

        constexpr optional& operator=(optional&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            empty_ = std::move(other.empty_);
            object_ = std::move(other.object_);
            return *this;
        }

        constexpr friend bool operator==(const optional& opt, const T& obj) {
            return opt.object_ == obj;
        }

        constexpr friend bool operator==(const T& obj, const optional& opt) {
            return opt == obj;
        }

        constexpr friend bool operator==(const optional& l, const optional& r) {
            return l.object_ == r.object_;
        }

        explicit constexpr operator bool() const {
            return !empty_;
        }

        constexpr const T& operator*() const {
            return object_;
        }

        constexpr T& operator*() {
            return object_;
        }

        constexpr void reset() {
            if (empty_) {
                return;
            }
            empty_ = true;
            reinterpret_cast<T*>(&object_)->~T();
        }

        constexpr const T* operator->() const {
            return &object_;
        }

        constexpr T* operator->() {
            return &object_;
        }

        constexpr const T& value() const {
            return operator*();
        }

        constexpr T& value() {
            return operator*();
        }

        constexpr T value_or(const T& val) const {
            return empty_ ? val : object_;
        }

    private:
        union {
            T object_;
            char another_;
        };
        bool empty_;
    };
}
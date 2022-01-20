#pragma once

namespace control_03 {

    template <class T>
    class maybe {
    public:
        maybe() = default;
        maybe(const maybe<T>& mb);
        maybe(maybe<T>&& mb) noexcept;
        explicit maybe(const T& t);
        explicit maybe(T&& t);

        maybe& operator=(const maybe<T>& mb);
        maybe& operator=(maybe<T>&& mb) noexcept;
        maybe& operator=(const T& t);
        maybe& operator=(T&& t);
        operator bool() noexcept;

        void reset();
        void reset(const T& t);
        template <class... Args>
        void emplace(Args&&... args);

        const T& value() const;
        T& value();

        bool has_value() const;

        ~maybe();

    private:
        alignas(T) char buffer_[sizeof(T)] = {};
        bool has_value_ = false;
    };

    template <class T>
    class maybe<T*> {
    public:
        maybe() = default;
        explicit maybe(T* t);
        maybe& operator=(T* t);
        operator bool() noexcept;
        void reset(T* t = nullptr);

        const T* value() const;
        T* value();
        bool has_value() const;

        ~maybe() = default;

    private:
        T* buffer_ = nullptr;
    };
}

namespace control_03 {

    template <class T>
    inline maybe<T>::maybe(const maybe<T>& mb) {
        if (mb.has_value_) {
            new(buffer_) T(mb.value());
            has_value_ = true;
        }
    }

    template <class T>
    inline maybe<T>::maybe(maybe<T>&& mb) noexcept {
        if (mb.has_value_) {
            new(buffer_) T(mb.value());
            has_value_ = true;
        }
    }

    template <class T>
    inline maybe<T>::maybe(const T& t) {
        new(buffer_) T(t);
        has_value_ = true;
    }

    template <class T>
    inline maybe<T>::maybe(T&& t) {
        new(buffer_) T(std::move(t));
        has_value_ = true;
    }

    template <class T>
    inline maybe<T>& maybe<T>::operator=(const maybe<T>& mb) {
        if (this == &mb) {
            return *this;
        }
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
        if (mb.has_value_) {
            new(buffer_) T(mb.value());
        }
        has_value_ = mb.has_value_;
        return *this;
    }

    template <class T>
    inline maybe<T>& maybe<T>::operator=(maybe<T>&& mb) noexcept {
        if (this == &mb) {
            return *this;
        }
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
            if (mb.has_value_)
                value() = std::move(mb.value());
            else
                new(buffer_) T(std::move(mb.value()));
        }
        else if (mb.has_value_) {
            new(buffer_) T(std::move(mb.value()));
        }
        has_value_ = mb.has_value_;
        return *this;
    }

    template <class T>
    inline maybe<T>& maybe<T>::operator=(const T& t) {
        if (static_cast<const void*>(buffer_) == static_cast<const void*>(&t)) {
            return *this;
        }
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
        new(buffer_) T(t);
        has_value_ = true;
        return *this;
    }

    template <class T>
    inline maybe<T>& maybe<T>::operator=(T&& t) {
        if (static_cast<const void*>(buffer_) == static_cast<const void*>(&t)) {
            return *this;
        }
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
        new(buffer_) T(std::move(t));
        has_value_ = true;
        return *this;
    }

    template <class T>
    inline maybe<T>::operator bool() noexcept {
        return has_value_;
    }

    template <class T>
    inline void maybe<T>::reset() {
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
        has_value_ = false;
    }

    template <class T>
    inline void maybe<T>::reset(const T& t) {
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
        new(buffer_) T(t);
        has_value_ = true;
    }

    template <class T>
    inline const T& maybe<T>::value() const {
        return *reinterpret_cast<const T*>(buffer_);
    }

    template <class T>
    inline T& maybe<T>::value() {
        return *reinterpret_cast<T*>(buffer_);
    }

    template <class T>
    inline bool maybe<T>::has_value() const {
        return has_value_;
    }

    template <class T>
    inline maybe<T>::~maybe() {
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
    }

    template <class T>
    inline maybe<T*>::maybe(T* t) : buffer_(&t) { }

    template <class T>
    inline maybe<T*>& maybe<T*>::operator=(T* t) {
        buffer_ = t;
        return *this;
    }

    template <class T>
    inline maybe<T*>::operator bool() noexcept {
        return buffer_ != nullptr;
    }

    template <class T>
    inline void maybe<T*>::reset(T* t) {
        buffer_ = t;
    }

    template <class T>
    inline const T* maybe<T*>::value() const {
        return buffer_;
    }

    template <class T>
    inline T* maybe<T*>::value() {
        return buffer_;
    }

    template <class T>
    inline bool maybe<T*>::has_value() const {
        return buffer_ != nullptr;
    }

    template <class T>
    template <class... Args>
    inline void maybe<T>::emplace(Args&&... args) {
        if (has_value_) {
            reinterpret_cast<T*>(buffer_)->~T();
        }
        new(buffer_) T(std::forward<Args>(args)...);
        has_value_ = true;
    }

}
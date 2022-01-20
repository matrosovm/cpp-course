namespace control_03 {

    template <class T>
    class maybe {
    public:
        maybe();
        maybe(const maybe& mb);
        maybe(maybe&& mb);
        maybe(const T& t);
        maybe(T&& t);

        maybe& operator=(const maybe& mb);
        maybe& operator=(maybe&& mb);
        maybe& operator=(const T& t);
        maybe& operator=(T&& t);

        operator bool() const noexcept;

        void reset();
        void reset(const T& t);

        T& value();
        const T& value() const;
        bool has_value() const ;

        ~maybe();
    private:
        bool has_value_;
        alignas(sizeof(T)) void* data_;
    };

}

namespace control_03 {
    template <class T>
    maybe<T>::maybe() : has_value_(false), data_() {
        data_ = &data_;
    }

    template <class T>
    maybe<T>::maybe(const maybe& mb) : has_value_(false), data_() {
        data_ = &data_;
        if (mb.has_value_) {
            has_value_ = true;
            data_ = new(data_) T(*static_cast<T*>(mb.data_));
        }
    }

    template <class T>
    maybe<T>::maybe(maybe&& mb) : has_value_(false), data_() {
        data_ = &data_;
        if (mb.has_value_) {
            has_value_ = true;
            data_ = new(data_) T(*std::move(static_cast<T*>(mb.data_)));
        }
    }
    template <class T>
    maybe<T>::maybe(const T& t) : has_value_(true), data_() {
        data_ = &data_;
        data_ = new(data_) T(t);
    }

    template <class T>
    maybe<T>::maybe(T&& t) : has_value_(true), data_() {
        data_ = &data_;
        data_ = new(data_) T(std::move(t));
    }

    template <class T>
    void maybe<T>::reset() {
        if (!has_value_) {
            return;
        }
        static_cast<T*>(data_)->~T();
        has_value_ = false;
    }

    template <class T>
    void maybe<T>::reset(const T& t) {
        reset();
        data_ = new (data_) T(t);
        has_value_ = true;
    }

    template <class T>
    T& maybe<T>::value() {
        return *static_cast<T*>(data_);
    }

    template <class T>
    const T& maybe<T>::value() const {
        return *static_cast<T*>(data_);
    }

    template <class T>
    bool maybe<T>::has_value() const {
        return has_value_;
    }

    template <class T>
    maybe<T>& maybe<T>::operator=(const maybe& mb) {
        if (this == &mb) {
            return *this;
        }
        maybe<T> copy(mb);
        std::swap(data_, copy.data_);
        std::swap(has_value_, copy.has_value_);
        return *this;
    }

    template <class T>
    maybe<T>& maybe<T>::operator=(maybe&& mb) {
        if (this == &mb) {
            return *this;
        }
        reset();
        std::swap(*static_cast<T*>(data_), *static_cast<T*>(mb.data_));
        std::swap(has_value_, mb.has_value_);
        return *this;
    }

    template <class T>
    maybe<T>& maybe<T>::operator=(const T& t) {
        if (data_ == &t) {
            return *this;
        }
        reset();
        new T (&data_, t);
        has_value_ = true;
    }

    template <class T>
    maybe<T>& maybe<T>::operator=(T&& t) {
        if (data_ == &t) {
            return *this;
        }
        reset();
        std::swap(*static_cast<T*>(data_), t);
        has_value_ = true;
        return *this;
    }

    template <class T>
    maybe<T>::operator bool() const noexcept {
        return has_value_;
    }

    template <class T>
    maybe<T>::~maybe() {
        if (has_value_) {
            static_cast<T*>(data_)->~T();
        }
    }
}
#include <utility>

namespace cls09 {

    template <class T>
    class unique_ptr {
    public:
        unique_ptr() noexcept;
        unique_ptr(T* t) noexcept;
        unique_ptr(const unique_ptr&) = delete;
        unique_ptr(unique_ptr&& t) noexcept;

        unique_ptr& operator=(const unique_ptr&) = delete;
        unique_ptr& operator=(unique_ptr&& t) noexcept;

        T* operator->() noexcept;
        T operator*() noexcept;
        T* operator->() const noexcept;
        T operator*() const noexcept;
        operator bool() const noexcept;

        T* get() noexcept;
        T* get() const noexcept;

        T* release() noexcept;
        void reset(T* t) noexcept;
        void reset() noexcept;

        ~unique_ptr() noexcept;

    private:
        T* data_;
    };

}

namespace cls09 {

    template<class T>
    unique_ptr<T>::unique_ptr() noexcept : data_(nullptr) { }

    template<class T>
    inline unique_ptr<T>::unique_ptr(T* t) noexcept : data_(t) {}

    template<class T>
    inline unique_ptr<T>::unique_ptr(unique_ptr&& t) noexcept : data_(nullptr) {
        std::swap(data_, t.data_);
    }

    template<class T>
    inline unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T>&& t) noexcept {
        if (this == &t) {
            return *this;
        }
        reset();
        std::swap(data_, t.data_);
        return *this;
    }

    template<class T>
    T* unique_ptr<T>::operator->() noexcept {
        return data_;
    }

    template<class T>
    T unique_ptr<T>::operator*() noexcept {
        return *data_;
    }

    template<class T>
    T* unique_ptr<T>::operator->() const noexcept {
        return data_;
    }

    template<class T>
    T unique_ptr<T>::operator*() const noexcept {
        return *data_;
    }

    template<class T>
    T* unique_ptr<T>::get() noexcept {
        return data_;
    }

    template<class T>
    T* unique_ptr<T>::get() const noexcept {
        return data_;
    }

    template<class T>
    inline T* unique_ptr<T>::release() noexcept {
        T* tmp = data_;
        data_ = nullptr;
        return tmp;
    }

    template<class T>
    inline void unique_ptr<T>::reset() noexcept {
        delete data_;
        data_ = nullptr;
    }

    template<class T>
    inline void unique_ptr<T>::reset(T* t) noexcept {
        if (data_ == t) {
            return;
        }
        reset();
        data_ = t;
    }

    template <class T>
    inline unique_ptr<T>::operator bool() const noexcept {
        return data_ != nullptr;
    }

    template<class T, class... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template <class T>
    inline unique_ptr<T>::~unique_ptr() noexcept {
        delete data_;
    }

}
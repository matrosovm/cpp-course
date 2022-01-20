#pragma once

#include <type_traits>
#include <memory>
#include <exception>

namespace utils {

    class bad_whatever_cast: public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class whatever {
    public:
        whatever();

        template <class T>
        explicit whatever(const T& t);

        template<class T, class =
                std::enable_if_t<!std::is_same<whatever, std::decay_t<T>>::value>>
        explicit whatever(T&& value);

        whatever(const whatever& we);

        template <class T>
        whatever& operator=(const T& t);
        template<class T, class =
                std::enable_if_t<!std::is_same<whatever, std::decay_t<T>>::value>>
        whatever& operator=(T&& t);

        whatever& operator=(const whatever& we);

        bool empty() const;
        void clear();
        void swap(whatever& we);

        template<class T>
        friend T* whatever_cast(whatever* we);

        ~whatever() = default;

    private:
        struct base {
        public:
            virtual const std::type_info& type_id() const;
            virtual base* clone() const = 0;

            virtual ~base() = default;
        };

        template <class T>
        class base_wrapper : public base {
        public:
            base_wrapper(const base_wrapper& bw);
            explicit base_wrapper(const T& t);

            T& get();
            const std::type_info& type_id() const final;
            base_wrapper* clone() const final;

            ~base_wrapper() override = default;

        private:
            std::decay_t<T> object_;
        };

        std::unique_ptr<base> object_;
    };

    template<class T>
    const T* whatever_cast(const whatever* we);
    template<class T>
    T& whatever_cast(whatever& we);
    template<class T>
    T whatever_cast(const whatever& we);

    void swap(whatever& we1, whatever& we2);
}

namespace utils {

    inline const std::type_info& whatever::base::type_id() const {
        return typeid(nullptr);
    }

    template <class T>
    inline whatever::base_wrapper<T>::base_wrapper(const base_wrapper& bw) : object_(bw.object_) { }

    template <class T>
    inline whatever::base_wrapper<T>::base_wrapper(const T& t) : object_(t) { }

    template<class T, class>
    inline whatever::whatever(T&& value) : object_(
            new base_wrapper<std::remove_cv_t<std::decay_t<const T>>>(std::forward<T>(value))) { }


    template <class T>
    inline const std::type_info& whatever::base_wrapper<T>::type_id() const {
        return typeid(object_);
    }

    template <class T>
    inline whatever::base_wrapper<T>* whatever::base_wrapper<T>::clone() const {
        return new base_wrapper(object_);
    }

    template <class T>
    inline T& whatever::base_wrapper<T>::get() {
        return object_;
    }

    inline whatever::whatever() : object_(nullptr) { }

    template <class T>
    inline whatever::whatever(const T& t) : object_(
            new base_wrapper<std::remove_cv_t<std::decay_t<const T>>>(t)) { }

    inline whatever::whatever(const whatever& we) : object_(we.object_ == nullptr ?
            nullptr : we.object_->clone()) { }

    template <class T>
    inline whatever& whatever::operator=(const T& t) {
        whatever tmp(t);
        swap(tmp);
        return *this;
    }

    template<class T, class>
    inline whatever& whatever::operator=(T&& t) {
        whatever tmp(std::forward<T>(t));
        swap(tmp);
        return *this;
    }

    inline whatever& whatever::operator=(const whatever& we) {
        if (this == &we) {
            return *this;
        }
        whatever tmp(we);
        swap(tmp);
        return *this;
    }

    inline bool whatever::empty() const {
        return object_ == nullptr;
    }

    inline void whatever::clear() {
        object_.reset(nullptr);
    }

    template<typename T>
    inline T* whatever_cast(whatever* we) {
        if (!we || we->empty() || typeid(T) != we->object_->type_id()) {
            return nullptr;
        }
        return &static_cast<whatever::base_wrapper<T>*>(&*we->object_)->get();
    }

    template<class T>
    inline const T* whatever_cast(const whatever* we) {
        return whatever_cast<T>(const_cast<whatever*>(we));
    }

    template<class T>
    inline T& whatever_cast(whatever& we) {
        using no_ref = typename std::remove_reference<T>::type;
        no_ref* tmp = whatever_cast<no_ref>(&we);
        if (tmp == nullptr) {
            throw bad_whatever_cast("bad whatever cast");
        }
        return static_cast<no_ref&>(*tmp);
    }

    template<class T>
    inline T whatever_cast(const whatever& we) {
        using no_ref = typename std::remove_reference<T>::type;
        return whatever_cast<const no_ref&>(const_cast<whatever&>(we));
    }

    inline void whatever::swap(whatever& we) {
        std::swap(object_, we.object_);
    }

    inline void swap(whatever& we1, whatever& we2) {
        we1.swap(we2);
    }

}
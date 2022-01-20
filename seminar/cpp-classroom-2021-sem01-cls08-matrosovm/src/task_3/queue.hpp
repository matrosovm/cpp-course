#pragma once

#include <utility>
#include <cstddef>

namespace cls_08 {
    template<class T>
    struct node {
        explicit node(const T& value);

        T value;
        node *next;
    };

    template<class T>
    class queue;

    template<class T>
    class list {
    public:
        list();
        explicit list(const T& value);
        list(const list& l);

        list& operator=(const list& l);

        friend class queue<T>;

        ~list();

    private:
        node<T>* root_;
        node<T>* last_;
        size_t size_;
    };

    template<class T>
    class queue {
    public:
        queue() = default;
        queue(const queue<T>& q);

        queue& operator=(const queue<T>& q);

        void push(const T& value);
        void pop();
        T& front();
        const T& front() const;
        size_t size() const;

        ~queue() = default;

    private:
        list<T> data_;
    };
}

namespace cls_08 {
    template <class T>
    inline node<T>::node(const T& value) : value(value), next(nullptr) { }

    template <class T>
    inline list<T>::list() : root_(nullptr), size_(0) {
        last_ = root_;
    }

    template <class T>
    inline list<T>::list(const T& value) : root_(new node(value)), size_(1) {
        last_ = root_;
    }

    template<class T>
    inline list<T>::list(const list<T>& l) : root_(nullptr), last_(nullptr), size_(0) {
        if (!l.root_)
            return;
        root_ = new node<T>(l.root_->value);
        size_ = l.size_;

        node<T>* it_r = root_;
        node<T>* it_l = l.root_->next;
        while (it_l) {
            it_r->next = new node<T>(it_l->value);
            it_r = it_r->next;
            it_l = it_l->next;
        }
        last_ = it_r;
    }

    template <class T>
    inline list<T>& list<T>::operator=(const list<T>& l) {
        if (this == &l) {
            return *this;
        }
        list<T> tmp(l);
        std::swap(root_, tmp.root_);
        std::swap(last_, tmp.last_);
        std::swap(size_, tmp.size_);
        return *this;
    }

    template <class T>
    inline list<T>::~list<T>() {
        if (size_ == 0)
            return;
        node<T>* next = root_->next;
        while (root_) {
            next = root_->next;
            delete root_;
            root_ = next;
        }
    }

    template<class T>
    inline queue<T>::queue(const queue<T>& q) : data_(q.data_) { }

    template<class T>
    inline queue<T>& queue<T>::operator=(const queue<T>& q) {
        if (this == &q)
            return *this;
        list<T> tmp = q.data_;
        std::swap(data_, tmp);

        return *this;
    }

    template<class T>
    inline void queue<T>::push(const T& value) {
        if (data_.size_ == 0) {
            data_.root_ = new node<T>(value);
            data_.last_ = data_.root_;
        } else {
            data_.last_->next = new node<T>(value);
            data_.last_ = data_.last_->next;
        }
        ++data_.size_;
    }

    template<class T>
    inline void queue<T>::pop() {
        if (data_.size_ == 0)
            return;
        node<T>* prev = data_.root_->next;
        delete data_.root_;
        data_.root_ = prev;
        --data_.size_;
    }

    template<class T>
    inline T& queue<T>::front() {
        return data_.root_->value;
    }

    template<class T>
    inline const T& queue<T>::front() const {
        return data_.root_->value;
    }

    template<class T>
    inline size_t queue<T>::size() const {
        return data_.size_;
    }
}
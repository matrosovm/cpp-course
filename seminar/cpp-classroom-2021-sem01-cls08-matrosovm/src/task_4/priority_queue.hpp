#pragma once

#include <algorithm>
#include <vector>

namespace cls_08 {
    template <class T>
    class priority_queue {
    public:
        priority_queue() = default;
        priority_queue(const priority_queue<T>& pq);

        priority_queue& operator=(const priority_queue<T>& pq);

        void push(const T& value);
        void pop();
        const T& front() const;
        size_t size() const;

        ~priority_queue() = default;
    private:
        std::vector<T> data_;
    };

    template <class T>
    inline priority_queue<T>::priority_queue(const priority_queue& pq) : data_(std::make_heap(
            pq.data_.begin(), pq.data_.end())) { }

    template <class T>
    inline priority_queue<T>& priority_queue<T>::operator=(const priority_queue<T>& pq) {
        if (this == &pq)
            return *this;
        std::vector<T> tmp = pq.data_;
        data_.swap(tmp);
        return *this;
    }

    template <class T>
    inline void priority_queue<T>::push(const T& value) {
        data_.push_back(value);
        std::push_heap(data_.begin(), data_.end());
    }

    template <class T>
    inline void priority_queue<T>::pop() {
        std::pop_heap(data_.begin(), data_.end());
        data_.pop_back();
    }

    template <class T>
    inline const T& priority_queue<T>::front() const {
        return data_.front();
    }

    template <class T>
    inline size_t priority_queue<T>::size() const {
        return data_.size();
    }
}
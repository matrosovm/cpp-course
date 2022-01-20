//#pragma once
//
//#include <memory>
//
//namespace cls10 {
//
//    class base_deque {
//    public:
//        void** data_ = nullptr;
//        size_t size_ = 0;
//        size_t left_offset_ = 0;
//        static const size_t block_capacity_ = 8;
//
//        base_deque() = default;
//        base_deque(size_t count, size_t object_size, std::align_val_t align);
//
//        ~base_deque();
//
//        static void* allocate_storage(size_t count, std::align_val_t align);
//        static void** allocate(size_t count, std::align_val_t align);
//        static void deallocate(void** data, size_t count);
//    };
//
//    inline void* base_deque::allocate_storage(size_t count, std::align_val_t align) {
//        void* storage = ::operator new(count, align);
//        return storage;
//    }
//
//    inline void** base_deque::allocate(size_t count, std::align_val_t align) {
//        size_t storage_size = count / block_capacity_ + 1;
//        void** storage = static_cast<void**>(allocate_storage(storage_size, std::align_val_t(alignof(void*))));
//        for (size_t i = 0; i < storage_size; ++i) {
//            storage[i] = ::operator new(block_capacity_, align);
//        }
//        return storage;
//    }
//
//    inline void base_deque::deallocate(void** data, size_t count) {
//        size_t buffer_size = count / block_capacity_ + 1;
//        for (size_t i = 0; i < buffer_size; ++i) {
//            ::operator delete(data[i]);
//        }
//        ::operator delete(data);
//    }
//
//    base_deque::base_deque(size_t count, size_t object_size, std::align_val_t align) :
//            data_(allocate(count, align)),
//            size_(count),
//            left_offset_(0) { }
//
//    base_deque::~base_deque() {
//        deallocate(data_, size_ + left_offset_);
//    }
//
//    template <class ValueType>
//    class deque : private base_deque {
//    public:
//        deque() = default;
//        deque(const deque& dq);
//        deque(deque&& dq) noexcept;
//
//        deque& operator=(const deque& dq);
//        deque& operator=(deque&& dq) noexcept;
//
//        const deque& operator[](size_t ind) const;
//        deque& operator[](size_t ind);
//
//        void push_back(ValueType&& value);
//        void push_front(ValueType&& value);
//        void pop_back();
//        void pop_front();
//
//        const ValueType& front() const;
//        ValueType& front();
//        const ValueType& back() const;
//        ValueType& back();
//
//        size_t size() const noexcept;
//        bool empty() const noexcept;
//        void clear();
//
//        ~deque() noexcept;
//
//    private:
//        void swap(deque& dq);
//        ValueType* storage(size_t ind) { return static_cast<ValueType*>(data_[ind]); };
//    };
//
//    template <class ValueType>
//    deque<ValueType>::deque(const deque& dq) :
//            base_deque(dq.size_ + dq.left_offset, std::align_val_t(alignof(ValueType))) {
//        size_t storage_size = (size_ + left_offset_) / block_capacity_ + 1;
//        size_t dq_i = 0;
//        size_t dq_j = left_offset_;
//        size_t current_size = 0;
//        for (size_t i = 0; i < storage_size; ++i) {
//            for (size_t j = 0; j < block_capacity_ && current_size < size_; ++i, ++current_size) {
//                if (dq_j == block_capacity_) {
//                    ++dq_i;
//                    dq_j = 0;
//                }
//                std::uninitialized_copy(dq.storage(i, dq.storage(i, j) + 1, storage(i, j));
//            }
//        }
//    }
//
//    template <class ValueType>
//    deque<ValueType>::deque(deque&& dq) noexcept {
//        data_ = std::move(dq.data_);
//        size_ = std::move(dq.size_);
//        left_offset_ = std::move(dq.left_offset);
//    }
//
//    template <class ValueType>
//    void deque<ValueType>::swap(deque<ValueType> &dq) {
//        std::swap(data_, dq.data_);
//        std::swap(size_, dq.size_);
//        std::swap(left_offset_, dq.left_offset);
//    }
//
//    template <class ValueType>
//    deque<ValueType>& deque<ValueType>::operator=(const deque& dq) {
//        if (this == &dq) {
//            return *this;
//        }
//        deque tmp(dq);
//        swap(tmp);
//        return *this;
//    }
//
//    template <class ValueType>
//    deque<ValueType>& deque<ValueType>::operator=(deque&& dq) noexcept {
//        if (this == &dq) {
//            return *this;
//        }
//        deque tmp(std::forward<deque<ValueType>>(dq));
//        swap(tmp);
//        return *this;
//    }
//
//    template <class ValueType>
//    const deque<ValueType>& deque<ValueType>::operator[](size_t ind) const {
//        return data_[(size_ + left_offset_) / block_capacity_][(size_ + left_offset_) % block_capacity_];
//    }
//
//    template <class ValueType>
//    deque<ValueType>& deque<ValueType>::operator[](size_t ind) {
//        return data_[(size_ + left_offset_) / block_capacity_][(size_ + left_offset_) % block_capacity_];
//    }
//
//    template <class ValueType>
//    void deque<ValueType>::push_back(ValueType&& value) {
//        if (data_ == nullptr) {
//            allocate(1, std::align_val_t(alignof(ValueType)));
//            new(data_[0][0]) ValueType(std::forward<ValueType>(value));
//            ++size_;
//            return;
//        }
//        if ((size_ + left_offset_) % block_capacity_ + 1 != block_capacity_) {
//            new(data_[(size_ + left_offset_) / block_capacity_][(size_ + left_offset_) % block_capacity_])
//                    ValueType(std::forward<ValueType>(value));
//            ++size_;
//            return;
//        }
//        size_t storage_size = (size_ + left_offset_) / block_capacity_ + 1;
//        void** new_storage = static_cast<void**>(allocate_storage(storage_size + 1,
//                    std::align_val_t(alignof(void*))));
//        for (size_t i = 0; i < storage_size; ++i) {
//            new_storage[i] = data_[i];
//        }
//        new_storage[storage_size] = ::operator new(block_capacity_, std::align_val_t(alignof(ValueType)));
//        swap(data_, new_storage);
//        new(data_[storage_size][0]) ValueType(std::forward<ValueType>(value));
//        ++size_;
//    }
//
//    template <class ValueType>
//    void deque<ValueType>::push_front(ValueType&& value) {
//        if (data_ == nullptr) {
//            allocate(1, std::align_val_t(alignof(ValueType)));
//            new(data_[0][0]) ValueType(std::forward<ValueType>(value));
//            ++size_;
//            return;
//        }
//        if (left_offset_ > 0) {
//            --left_offset_;
//            new(data_[0][left_offset_]) ValueType(std::forward<ValueType>(value));
//            ++size_;
//            return;
//        }
//        size_t storage_size = (size_ + left_offset_) / block_capacity_ + 1;
//        void** new_storage = static_cast<void**>(allocate_storage(storage_size + 1,
//                                                                  std::align_val_t(alignof(void*))));
//        for (size_t i = 1; i <= storage_size; ++i) {
//            new_storage[i] = data_[i - 1];
//        }
//        new_storage[0] = ::operator new(block_capacity_, std::align_val_t(alignof(ValueType)));
//        swap(data_, new_storage);
//        left_offset_ = block_capacity_ - 1;
//        new(data_[0][left_offset_]) ValueType(std::forward<ValueType>(value));
//        ++size_;
//    }
//
//    template <class ValueType>
//    void deque<ValueType>::pop_back() {
//        if (data_ == nullptr) {
//            throw std::out_of_range("deque is empty");
//        }
//        if ((left_offset_ + size_) % block_capacity_ == 0) {
//            deallocate(data_[(left_offset_ + size_) / block_capacity_], 1);
//            --size_;
//            return;
//        }
//        data_[(left_offset_ + size_) / block_capacity_][(left_offset_ + size_) % block_capacity_]->~ValueType();
//        --size_;
//    }
//
//    template <class ValueType>
//    void deque<ValueType>::pop_front() {
//        if (data_ == nullptr) {
//            throw std::out_of_range("deque is empty");
//        }
//        if (left_offset_ + 1 == block_capacity_) {
//            data_[0][left_offset_]->~ValueType();
//            deallocate(data_[0], 1);
//            left_offset_ = 0;
//            --size_;
//            return;
//        }
//        data_[0][left_offset_]->~ValueType();
//        ++left_offset_;
//        --size_;
//    }
//
//    template <class ValueType>
//    const ValueType& deque<ValueType>::front() const {
//        if (data_ == nullptr) {
//            throw std::out_of_range("deque is empty");
//        }
//        return data_[0][left_offset_];
//    }
//
//    template <class ValueType>
//    ValueType& deque<ValueType>::front() {
//        if (data_ == nullptr) {
//            throw std::out_of_range("deque is empty");
//        }
//        return data_[0][left_offset_];
//    }
//
//    template <class ValueType>
//    const ValueType& deque<ValueType>::back() const {
//        if (data_ == nullptr) {
//            throw std::out_of_range("deque is empty");
//        }
//        return data_[size_ / block_capacity_][size_ % block_capacity_];
//    }
//
//    template <class ValueType>
//    ValueType& deque<ValueType>::back() {
//        if (data_ == nullptr) {
//            throw std::out_of_range("deque is empty");
//        }
//        return data_[size_ / block_capacity_][size_ % block_capacity_];
//    }
//
//    template <class ValueType>
//    inline size_t deque<ValueType>::size() const noexcept {
//        return size_;
//    }
//
//    template <class ValueType>
//    inline bool deque<ValueType>::empty() const noexcept {
//        return size_ == 0;
//    }
//
//    template <class ValueType>
//    inline void deque<ValueType>::clear() {
//        deque tmp;
//        swap(tmp);
//    }
//
//    template <class ValueType>
//    deque<ValueType>::~deque() noexcept {
//        size_t buffer_size = (size_ + left_offset_) / block_capacity_ + 1;
//        for (size_t i = 0; i < buffer_size; ++i) {
//            for (size_t j = (i == 0 ? left_offset_ : 0);
//                    j < (i == buffer_size - 1 ? buffer_size % block_capacity_ : block_capacity_); ++i) {
//                data_[i][j].~T();
//            }
//        }
//    }
//
//}
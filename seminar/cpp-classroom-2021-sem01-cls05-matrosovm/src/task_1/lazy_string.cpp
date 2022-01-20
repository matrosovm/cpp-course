#include <cstring>

#include "lazy_string.hpp"

LazyString::LazyString() : shared_buffer_(1) {
    strcpy(shared_buffer_.getData(), "\0");
}

LazyString::LazyString(const char* c_str) : shared_buffer_(strlen(c_str) + 1){
    strcpy(shared_buffer_.getData(), c_str);
}

LazyString& LazyString::operator=(const LazyString& lazy_string) {
    if (this == &lazy_string) {
        return *this;
    }
    shared_buffer_ = lazy_string.shared_buffer_;
    return *this;
}

size_t LazyString::getSize() const {
    return shared_buffer_.getSize() - 1;
}

const char* LazyString::cStr() const {
    return shared_buffer_.getData();
}

size_t LazyString::useCount() const {
    return shared_buffer_.useCount();
}

char LazyString::at(size_t ind) const {
    return shared_buffer_.getData()[ind];
}

bool LazyString::operator==(const LazyString& lazy_string) const {
    return strcmp(cStr(), lazy_string.cStr()) == 0;
}

LazyString cStringConcatenation(const char* c_str1, const char* c_str2) {
    char* concat = new char[strlen(c_str1) + strlen(c_str2) + 1]();
    strcat(concat, c_str1);
    strcat(concat, c_str2);

    LazyString tmp(concat);

    delete[] concat;
    return tmp;
}

LazyString operator+(const LazyString& lazy_string, const char* c_str) {
    return LazyString(cStringConcatenation(lazy_string.cStr(), c_str));
}

LazyString& operator+=(LazyString& lazy_string, const char* c_str) {
    LazyString tmp(cStringConcatenation(lazy_string.cStr(), c_str));
    swap(lazy_string.shared_buffer_, tmp.shared_buffer_);
    return lazy_string;
}

CharWrapper LazyString::operator[](size_t ind) {
    return CharWrapper(ind, this);
}

CharWrapper::CharWrapper(size_t element_ind, LazyString* parent_str) : element_ind_(element_ind),
    parent_str_(parent_str) { }

CharWrapper& CharWrapper::operator=(const CharWrapper& char_wrapper) {
    if (this == &char_wrapper) {
        return *this;
    }
    element_ind_ = char_wrapper.element_ind_;
    parent_str_= char_wrapper.parent_str_;
    return *this;
}

CharWrapper& CharWrapper::operator=(char c) {
    if (this->parent_str_ == nullptr) {
        return *this;
    }
    if (parent_str_->useCount() > 1) {
        char* copy_string = new char[parent_str_->getSize() + 1]();
        strcat(copy_string, parent_str_->cStr());
        copy_string[element_ind_] = c;

        LazyString tmp(copy_string);
        swap(parent_str_->shared_buffer_, tmp.shared_buffer_);

        delete[] copy_string;
        return *this;
    }
    parent_str_->shared_buffer_.getData()[element_ind_] = c;
    return *this;
}

CharWrapper& CharWrapper::operator++() {
    *this = static_cast<char>(parent_str_->at(element_ind_) + static_cast<char>(1));
    return *this;

}

CharWrapper CharWrapper::operator++(int) {
    CharWrapper tmp = *this;
    ++(*this);
    return tmp;
}

CharWrapper& CharWrapper::operator--() {
    *this = static_cast<char>(parent_str_->at(element_ind_) - static_cast<char>(1));
    return *this;
}

CharWrapper CharWrapper::operator--(int) {
    CharWrapper tmp = *this;
    --(*this);
    return tmp;
}

CharWrapper::operator char() const {
    return parent_str_->cStr()[element_ind_];
}
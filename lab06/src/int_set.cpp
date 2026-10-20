// CSCI 41 -- Lab 06: Abstract Data Types
//
// Implement every function below. include/int_set.h says what each one must
// do; read it first.
//
// Rules:
//   * Store the values in an array you allocate with new[] and free with
//     delete[]. No std::vector, no std::set, no other container.
//   * Do not change include/int_set.h.
//   * The class owns memory, so you must write all three of the destructor,
//     the copy constructor and the copy assignment operator.
//
// A stub below throws so that an unimplemented function fails its own tests
// instead of silently returning something wrong. Delete the throw when you
// implement the function.

#include "int_set.h"

#include <cassert>
#include <memory_resource>
#include <stdexcept>

namespace {
const int kDefaultCapacity = 4;
}

IntSet::IntSet() : data_(nullptr), size_(0), capacity_(0) {
    capacity_ = kDefaultCapacity;
    data_ = new int[capacity_];
}

IntSet::IntSet(int initialCapacity) : data_(nullptr), size_(0), capacity_(0) {
    assert(initialCapacity > 0 && "IntSet requires a positive initial capacity");

    capacity_ = initialCapacity;
    data_ = new int[capacity_];
}

IntSet::IntSet(const IntSet& other) : data_(nullptr), size_(0), capacity_(0) {

    size_ = other.size_;
    capacity_ = other.capacity_;

    data_ = new int[capacity_];

    for (int i=0; i<size_; i++) {
        data_[i] = other.data_[i];
    }
}

IntSet& IntSet::operator=(const IntSet& other) {

    if (this == &other) {
        return *this;
    }

    int* newData = new int[other.capacity_];
    for (int i=0; i<other.size_; i++) {
        newData[i] = other.data_[i];
    }

    delete [] data_;

    data_ = newData;
    size_ = other.size_;
    capacity_ = other.capacity_;

    return *this;
}

IntSet::~IntSet() {
    delete [] data_;
}

bool IntSet::insert(int value) {
     if (contains(value)) {
         return false;
     }

    growIfNeeded();

    data_[size_] = value;
    size_++;

    return true;
}

bool IntSet::remove(int value) {

    int index = indexOf(value);

    if (index == -1) {
        return false;
    }

    data_[index] = data_[size_ -1];
    size_--;

    return true;
}

bool IntSet::contains(int value) const {
    return indexOf(value) != -1;
}

int IntSet::size() const {
    return size_;
}

bool IntSet::isEmpty() const {
    return size_ == 0;
}

void IntSet::clear() {
    size_ = 0;
}

int IntSet::capacity() const {
    return capacity_;
}

int IntSet::indexOf(int value) const {

    for (int i=0; i<size_; i++) {
        if (data_[i] == value) {
            return i;
        }
    }

    return -1;
}

void IntSet::growIfNeeded() {

    if (size_ < capacity_) {
        return;
    }

    int newCapacity = capacity_ * 2;
    int* newData = new int[newCapacity];

    for (int i=0; i<size_; i++) {
        newData[i] = data_[i];
    }
    delete [] data_;

    data_ = newData;
    capacity_ = newCapacity;
}

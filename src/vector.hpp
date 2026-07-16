#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
    T *data_;
    size_t size_;
    size_t capacity_;
    
    // Helper methods using placement new
    void destruct_all() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
    }
    
    void reallocate(size_t new_capacity) {
        T *new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(data_[i]);
        }
        destruct_all();
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
    
    void ensure_capacity(size_t needed) {
        if (needed > capacity_) {
            size_t new_cap = capacity_ == 0 ? 1 : capacity_;
            while (new_cap < needed) {
                new_cap *= 2;
            }
            reallocate(new_cap);
        }
    }
    
public:
    /**
     * TODO
     * a type for actions of the elements of a vector, and you should write
     *   a class named const_iterator with same interfaces.
     */
    /**
     * you can see RandomAccessIterator at CppReference for help.
     */
    class const_iterator;
    class iterator
    {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the following
    // typedef) to work properly. In particular, without the following code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
    // About iterator_category: https://en.cppreference.com/w/cpp/iterator
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        T *ptr_;
        const vector *vec_;
    public:
        iterator() : ptr_(nullptr), vec_(nullptr) {}
        iterator(T *p, const vector *v) : ptr_(p), vec_(v) {}
        iterator(const iterator &other) : ptr_(other.ptr_), vec_(other.vec_) {}
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        iterator operator+(const int &n) const
        {
            return iterator(ptr_ + n, vec_);
        }
        iterator operator-(const int &n) const
        {
            return iterator(ptr_ - n, vec_);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const
        {
            if (vec_ != rhs.vec_) {
                throw invalid_iterator();
            }
            return static_cast<int>(ptr_ - rhs.ptr_);
        }
        iterator& operator+=(const int &n)
        {
            ptr_ += n;
            return *this;
        }
        iterator& operator-=(const int &n)
        {
            ptr_ -= n;
            return *this;
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ptr_++;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        iterator& operator++() {
            ptr_++;
            return *this;
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            ptr_--;
            return tmp;
        }
        /**
         * TODO --iter
         */
        iterator& operator--() {
            ptr_--;
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const {
            return *ptr_;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator==(const const_iterator &rhs) const;
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr_ != rhs.ptr_;
        }
        bool operator!=(const const_iterator &rhs) const;
        
        // Additional operators for random access iterator
        T* operator->() const {
            return ptr_;
        }
        
        T& operator[](int n) const {
            return *(ptr_ + n);
        }
        
        bool operator<(const iterator &rhs) const {
            return ptr_ < rhs.ptr_;
        }
        bool operator<=(const iterator &rhs) const {
            return ptr_ <= rhs.ptr_;
        }
        bool operator>(const iterator &rhs) const {
            return ptr_ > rhs.ptr_;
        }
        bool operator>=(const iterator &rhs) const {
            return ptr_ >= rhs.ptr_;
        }
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     */
    class const_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

    private:
        const T *ptr_;
        const vector *vec_;
        
    public:
        const_iterator() : ptr_(nullptr), vec_(nullptr) {}
        const_iterator(const T *p, const vector *v) : ptr_(p), vec_(v) {}
        const_iterator(const const_iterator &other) : ptr_(other.ptr_), vec_(other.vec_) {}
        const_iterator(const iterator &other) : ptr_(other.ptr_), vec_(other.vec_) {}
        
        const_iterator operator+(const int &n) const {
            return const_iterator(ptr_ + n, vec_);
        }
        const_iterator operator-(const int &n) const {
            return const_iterator(ptr_ - n, vec_);
        }
        
        int operator-(const const_iterator &rhs) const {
            if (vec_ != rhs.vec_) {
                throw invalid_iterator();
            }
            return static_cast<int>(ptr_ - rhs.ptr_);
        }
        
        const_iterator& operator+=(const int &n) {
            ptr_ += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            ptr_ -= n;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ptr_++;
            return tmp;
        }
        const_iterator& operator++() {
            ptr_++;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            ptr_--;
            return tmp;
        }
        const_iterator& operator--() {
            ptr_--;
            return *this;
        }
        
        const T& operator*() const {
            return *ptr_;
        }
        
        const T* operator->() const {
            return ptr_;
        }
        
        const T& operator[](int n) const {
            return *(ptr_ + n);
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator==(const iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr_ != rhs.ptr_;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr_ != rhs.ptr_;
        }
        
        bool operator<(const const_iterator &rhs) const {
            return ptr_ < rhs.ptr_;
        }
        bool operator<=(const const_iterator &rhs) const {
            return ptr_ <= rhs.ptr_;
        }
        bool operator>(const const_iterator &rhs) const {
            return ptr_ > rhs.ptr_;
        }
        bool operator>=(const const_iterator &rhs) const {
            return ptr_ >= rhs.ptr_;
        }
    };
    
    // Friend declarations for iterator/const_iterator comparisons
    friend class const_iterator;
    
    /**
     * TODO Constructs
     * At least two: default constructor, copy constructor
     */
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    vector(const vector &other) : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ > 0) {
            data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
            capacity_ = other.capacity_;
            for (size_t i = 0; i < other.size_; ++i) {
                new (data_ + size_) T(other.data_[i]);
                ++size_;
            }
        }
    }
    
    /**
     * TODO Destructor
     */
    ~vector() {
        clear();
        ::operator delete(data_);
    }
    
    /**
     * TODO Assignment operator
     */
    vector &operator=(const vector &other) {
        if (this == &other) {
            return *this;
        }
        // Clear current contents
        destruct_all();
        size_ = 0;
        
        // Reallocate if necessary
        if (capacity_ < other.size_) {
            ::operator delete(data_);
            data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
            capacity_ = other.capacity_;
        }
        
        // Copy elements
        for (size_t i = 0; i < other.size_; ++i) {
            new (data_ + size_) T(other.data_[i]);
            ++size_;
        }
        
        return *this;
    }
    
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
    T & at(const size_t &pos) {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    
    const T & at(const size_t &pos) const {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary but I want you to do.
     */
    T & operator[](const size_t &pos) {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    
    const T & operator[](const size_t &pos) const {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    
    /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
    const T & front() const {
        if (size_ == 0) {
            throw container_is_empty();
        }
        return data_[0];
    }
    
    /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
    const T & back() const {
        if (size_ == 0) {
            throw container_is_empty();
        }
        return data_[size_ - 1];
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(data_, this);
    }
    
    const_iterator begin() const {
        return const_iterator(data_, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(data_, this);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(data_ + size_, this);
    }
    
    const_iterator end() const {
        return const_iterator(data_ + size_, this);
    }
    
    const_iterator cend() const {
        return const_iterator(data_ + size_, this);
    }
    
    /**
     * checks whether the container is empty
     */
    bool empty() const {
        return size_ == 0;
    }
    
    /**
     * returns the number of elements
     */
    size_t size() const {
        return size_;
    }
    
    /**
     * clears the contents
     */
    void clear() {
        destruct_all();
        size_ = 0;
    }
    
    /**
     * inserts value before pos
     * returns an iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) {
        size_t index = pos - begin();
        return insert(index, value);
    }
    
    /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
     */
    iterator insert(const size_t &ind, const T &value) {
        if (ind > size_) {
            throw index_out_of_bound();
        }
        
        ensure_capacity(size_ + 1);
        
        // Move elements to make room
        for (size_t i = size_; i > ind; --i) {
            new (data_ + i) T(data_[i - 1]);
            data_[i - 1].~T();
        }
        
        new (data_ + ind) T(value);
        ++size_;
        
        return iterator(data_ + ind, this);
    }
    
    /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is returned.
     */
    iterator erase(iterator pos) {
        size_t index = pos - begin();
        return erase(index);
    }
    
    /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
    iterator erase(const size_t &ind) {
        if (ind >= size_) {
            throw index_out_of_bound();
        }
        
        data_[ind].~T();
        
        // Move elements forward
        for (size_t i = ind; i < size_ - 1; ++i) {
            new (data_ + i) T(data_[i + 1]);
            data_[i + 1].~T();
        }
        
        --size_;
        return iterator(data_ + ind, this);
    }
    
    /**
     * adds an element to the end.
     */
    void push_back(const T &value) {
        ensure_capacity(size_ + 1);
        new (data_ + size_) T(value);
        ++size_;
    }
    
    /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
    void pop_back() {
        if (size_ == 0) {
            throw container_is_empty();
        }
        --size_;
        data_[size_].~T();
    }
};

// Out-of-class definitions for cross comparisons
template<typename T>
bool vector<T>::iterator::operator==(const const_iterator &rhs) const {
    return ptr_ == rhs.ptr_;
}

template<typename T>
bool vector<T>::iterator::operator!=(const const_iterator &rhs) const {
    return ptr_ != rhs.ptr_;
}

}

#endif

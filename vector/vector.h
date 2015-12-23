#include <stdexcept>
#include <memory>


template <typename T>
class Vector {
public:
    typedef T                                       value_type;
    typedef value_type*                             pointer;    
    typedef value_type&                             reference;
    typedef const pointer                           const_pointer;
    typedef const value_type&                       const_reference;
    typedef value_type*                             iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef const iterator                          const_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
    
    Vector() :
        _data(nullptr),
        _size(0),
        _capacity(0)
    { }

    Vector(size_t n, const value_type& value) :
        Vector()
    {
        _realloc(n);

        for (size_t i = 0; i < n; i++) {
            push_back(value);
        }
    }

    Vector(const Vector& that) :
        Vector()
    {
        *this = that;
    }

   ~Vector()
    {
        _free(_data, _size);
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    bool empty() const
    {
        return _size == 0;
    }

    void clear() 
    {
        _realloc(0);
    }

    void reserve(size_t n)
    {
        if (n > _size) {
            _realloc(n);
        }
    }

    void resize(size_t n)
    {
        _realloc(n);

        for (size_t i = _size; i < n; i++) {
            push_back(value_type());
        }
    }

    reference back()
    {
        return _data[_size - 1];
    }

    reference front()
    {
        return _data[0];
    }

    const_reference back() const
    {
        return _data[_size - 1];
    }

    const_reference front() const
    {
        return _data[0];
    }

    void push_back(const value_type& value)
    {
        insert(_size, value);
    }

    void pop_back()
    {
        erase(_size - 1);
    }

    void insert(size_t pos, const value_type& value)
    {
        if (_size == _capacity) {
            _realloc_ahead(_size + 1);
        }
        if (_size != 0) {
            new(&_data[_size]) value_type(_data[_size - 1]);
            for (size_t i = _size - 1; i > pos; i--) {
                _data[i] = _data[i - 1];
            }
            _data[pos] = value;
        }
        else {
            new(&_data[_size]) value_type(value);
        }

        _size++;
    }

    void erase(size_t n)
    {
        for (size_t i = n; i < _size - 1; i++) {
            _data[i] = _data[i + 1];
        }

        _data[_size - 1].~value_type();
        _size--;

        if (_size < DECREASE_FACTOR * _capacity) {
            _realloc_ahead(_size);
        }
    }

    Vector<value_type>& operator= (const Vector<T>& that)
    {
        pointer tmp = _alloc_and_copy(that.capacity(), that.data(), that.size());
        
        std::swap(_data, tmp);
        _free(tmp, _size);

        _size = that.size();
        _capacity = that.capacity();

        return *this;
    }

    pointer data()
    {
        return _data;
    }

    const_pointer data() const
    {
        return _data;
    }

    reference at(size_t pos)
    {
        _range_check(pos);
        return _data[pos];
    }

    const_reference at(size_t pos) const
    {
        _range_check(pos);
        return _data[pos];
    }

    reference operator[] (size_t pos)
    {
        return _data[pos];
    }

    const_reference operator[] (size_t pos) const
    {
        return _data[pos];
    }

    iterator begin()
    {
        return &_data[0];
    }

    iterator end()
    {
        return &_data[_size];
    }

    reverse_iterator rbegin()
    {
        return std::reverse_iterator<iterator>(end());
    }

    reverse_iterator rend()
    {
        return std::reverse_iterator<iterator>(begin());
    }

    const_iterator cbegin() const
    {
        return &_data[0];
    }

    const_iterator cend() const
    {
        return &_data[_size];
    }

    const_reverse_iterator crbegin() const
    {
        return std::reverse_iterator<const_iterator>(cend());
    }

    const_reverse_iterator crend() const
    {
        return std::reverse_iterator<const_iterator>(cbegin());;
    }

private:
    const float INCREASE_FACTOR = 1.5;
    const float DECREASE_FACTOR = 0.5;

    size_t _size;
    size_t _capacity;
    pointer _data;

    void _range_check(size_t n) const
    {
        if (n >= _size) {
            throw std::out_of_range();
        }
    }

    pointer _alloc(size_t n)
    {
        auto tmp = std::get_temporary_buffer<value_type> (n);
        if (tmp.second != n) {
            _free(tmp.first);
            throw std::bad_alloc();
        }

        return tmp.first;
    }

    void _free(pointer ptr, size_t n = 0)
    {
        for (size_t i = 0; i < n; i++) {
            ptr[i].~value_type();
        }

        std::return_temporary_buffer(ptr);
    }

    pointer _alloc_and_copy(size_t size, pointer from, size_t n)
    {
        pointer to = _alloc(size);
        size_t copied = 0;

        try {
            for (size_t i = 0; i < n; i++, copied++) {
                new(to + i) value_type(from[i]);
            }
        }
        catch(...) {
            _free(to, copied);
            throw;
        }

        return to;
    }

    void _realloc(size_t n)
    {
        size_t new_size = std::min(_size, n);

        pointer tmp = _alloc_and_copy(n, _data, new_size);
        std::swap(_data, tmp);

        _free(tmp, _size);
        _size = new_size;
        _capacity = n;
    }

    void _realloc_ahead(size_t n)
    {
        _realloc(INCREASE_FACTOR * n);
    }
};
#include <algorithm>
#include <iterator>

template <typename C>
class DequeIterator : public std::iterator<std::random_access_iterator_tag, 
                                           typename C::value_type> {
public:
    typedef typename C::value_type      value_type;
    typedef typename C::pointer         pointer;
    typedef typename C::reference       reference;
    typedef std::ptrdiff_t              difference_type;
    typedef C                           container;


    DequeIterator(C& container, size_t pos) :
        _container(container),
        _pos(pos)
    {
    }

    bool operator== (const DequeIterator& that) const
    {
        return _container == that._container && 
               _pos       == that._pos;
    }

    bool operator!= (const DequeIterator& that) const
    {
        return !(*this == that);
    }

    DequeIterator& operator++ ()
    {
        _pos = _container._next(_pos);
        return *this;
    }

    DequeIterator operator++ (int)
    {
        DequeIterator tmp = *this;
        _pos = _container._next(_pos);
        return tmp;
    }

    DequeIterator& operator-- ()
    {
        _pos = _container._prev(_pos);
        return *this;
    }

    DequeIterator operator-- (int)
    {
        DequeIterator tmp = *this;
        _pos = _container._prev(_pos);
        return tmp;
    }

    difference_type operator- (const DequeIterator& that) const
    {
        return _container._number(_pos) - 
          that._container._number(that._pos);
    }

    reference operator* ()
    {
        return _container._data[_pos];
    }

private:
    C& _container;
    size_t _pos;
};


template <typename T>
class Deque {
public:
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
    typedef DequeIterator<Deque<T>>             iterator;
    typedef std::reverse_iterator<iterator>     reverse_iterator;

    friend iterator;

    Deque() :
        _data(nullptr),
        _capacity(0),
        _size(0),
        _begin(0)
    {
        _alloc(0);
    }

    Deque(const Deque<T>& that) :
        Deque()
    {
        *this = that;
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    size_t empty() const
    {
        return _size == 0;
    }

    T& front()
    {
        return _data[_begin];
    }

    T& back()
    {
        return _data[_prev(_end())];
    }

    const T& front() const
    {
        return _data[_begin];
    }

    const T& back() const
    {
        return _data[_prev(_end())];
    }

    void push_front(const T& value)
    {
        if (_size == _capacity) {
            _alloc_ahead(_size+1);
        }
        
        _data[_prev(_begin)] = value;

        _size++;
        _begin = _prev(_begin);
    }

    void push_back(const T& value)
    {
        if (_size == _capacity) {
            _alloc_ahead(_size+1);
        }

        _data[_end()] = value;

        _size++;
    }

    void pop_front()
    {
        _size--;
        _begin = _next(_begin);

        if (_size < _capacity * DECREASE_FACTOR) {
            _alloc_ahead(_size+1);
        }
    }

    void pop_back()
    {
        _size--;

        if (_size < _capacity * DECREASE_FACTOR) {
            _alloc_ahead(_size+1);
        }
    }

    T& at(size_t n)
    {
        if (n >= _size) {
            throw std::out_of_range();
        }

        return _data[_index(n)]; 
    }

    bool operator== (const Deque<T>& that) const
    {
        return _data == that._data;
    }

    void reserve(size_t n)
    {
        _alloc(n);
    }

    void clear()
    {
        _alloc(0);
    }

    T& operator[] (size_t n)
    {
        return _data[_index(n)];
    }

    Deque<T>& operator= (const Deque<T>& that)
    {        
        _alloc(that.size());
        for (size_t i = that._begin; i != that._end(); i = that._next(i)) {
            push_back(that._data[i]);
        }

        return *this;
    }

    iterator begin()
    {
        return iterator(*this, _begin);
    }

    iterator end()
    {
        return iterator(*this, _end());
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

private:
    const float INCREASE_FACTOR = 1.5;
    const float DECREASE_FACTOR = 0.5;

    T* _data;
    size_t _size;
    size_t _capacity;
    size_t _begin;


    size_t _end() const
    {
        return (_begin + _size) % (_capacity + 1);
    }

    size_t _next(size_t n, size_t offset = 1) const
    {
        return (n + offset) % (_capacity + 1);
    }

    size_t _prev(size_t n, size_t offset = 1) const
    {
        return (offset > n) ? (_capacity + 1) - (offset - n) : n - offset;        
    }

    size_t _index(size_t n) const
    {
        return _next(_begin + n, 0);
    }

    size_t _number(size_t n) const
    {
        return _prev(n, _begin);
    }

    void _alloc(size_t n)
    {
        T* tmp;

        try {
            size_t new_size = std::min(_size, n);       

            // allocating n + 1 cells to save one for the end index since deque is implemented like a cyclic buffer
            tmp = new T [n + 1];

            for (size_t i = _begin, j = 0; i != _end(); i = _next(i), j++) {
                tmp[j] = _data[i];
            }
            std::swap(tmp, _data);
            _size = new_size;
            _capacity = n;
            _begin = 0;
        }
        // finally  emulation
        catch(...) {
            delete[] tmp;
            throw;
        }

        delete[] tmp;
    }

    void _alloc_ahead(size_t n)
    {
        _alloc(n * INCREASE_FACTOR);
    }
};
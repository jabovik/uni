#pragma once

template <typename T>
class MyVector
{
    std::size_t _size = 0;
    std::size_t _reserved = 1;
    T *arr = new T[_reserved];
    public:
    void _copy_and_reserve()
    {
        _reserved = _reserved*2;
        T *newarr = new T[_reserved];
        for (std::size_t i = 0; i < _size; ++i)
        {
            newarr[i] = arr[i];
        }
        delete[] arr;
        arr = newarr;
    }
    void push_back(T elem)
    {
        if(_size>=_reserved)
            _copy_and_reserve();
        arr[_size] = elem;
        ++_size;
    }
    size_t size()
    {
        return _size;
    }
    T& operator[](std::size_t idx)
    {
        if (idx>_size-1)
            throw std::invalid_argument("Index out of range");
        else
            return arr[idx];
    }

    ~MyVector()
    {
        delete[] arr;
    }
};
#pragma once
#include <stdexcept>
#include <initializer_list>
template <typename T>
class MyVector
{
    std::size_t _size;
    std::size_t _reserved;
    T *_data;
    void expand()
    {
        _reserved *= 2;
        T *new_data = new T[_reserved];
        for (std::size_t i = 0; i < _size; i++)
        {
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
    }

public:
    MyVector() : _size(0), _reserved(1), _data(new T[_reserved])
    {
    }
    MyVector(std::initializer_list<T> init_list) : MyVector()
    {
        for (const auto &i : init_list)
        {
            push_back(i);
        }
    }
    MyVector(const MyVector &other) : _size(other._size), _reserved(other._reserved), _data(new T[other._reserved]) // копирующий конструктор
    {
        for (std::size_t i = 0; i < _size; ++i)
        {
            _data[i] = other._data[i];
        }
    }
    MyVector& operator=(const MyVector &other) // копирующее присваивание
    {
        if (this != &other)
        {
            delete[] _data;
            _size = other._size;
            _reserved = other._reserved;
            _data = new T[_reserved];
            for (std::size_t i = 0; i < _size; ++i)
            {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }
    std::size_t reserved()
    {
        return _reserved;
    }
    std::size_t size()
    {
        return _size;
    }
    std::size_t capacity()
    {
        return _reserved - _size;
    }
    void zero_init() // инициализация нулевым значением типа T
    {
        for(std::size_t i = _size; i < _reserved; i++)
        {
            _data[i] = T(); // инициализация нулевым значением типа T
        }
    }
    void reserve(std::size_t new_reserved) // резервирование памяти
    {
        if (new_reserved > _reserved)
        {
            _reserved = new_reserved;
            T *new_data = new T[_reserved];
            for (std::size_t i = 0; i < _size; i++)
            {
                new_data[i] = _data[i];
            }
            delete[] _data;
            _data = new_data;
        }
    }
    void push_back(const T &value)
    {
        if (_size >= _reserved)
        {
            expand();
        }
        _data[_size] = value;
        _size++;
    }
    T &operator[](std::size_t idx)
    {
        if (idx > _size - 1)
            throw std::out_of_range("Index out of range");
        else
            return _data[idx];
    }

    ~MyVector()
    {
        delete[] _data;
    }
};
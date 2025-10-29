#include <iostream>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
#include <chrono>
using namespace std;

/// @brief сортировка выбором
/// @param vec сортируемый вектор
void selection_sort(vector<int> &vec)
{
    size_t size = vec.size();
    for (size_t i = 0; i < size - 1; ++i)
    {
        size_t min_idx = i;
        for (size_t j = i + 1; j < size; ++j)
        {
            if (vec[min_idx] > vec[j])
            {
                min_idx = j;
            }
        }
        swap(vec[i], vec[min_idx]);
    }
}
/// @brief функция слияния двух отсортированных частей
/// @param vec сортируемый вектор
/// @param left начальный индекс
/// @param right конечный индекс
/// @param mid срединный индекс (конец левой части включительно)
void merge(vector<int> &vec, size_t left, size_t right, size_t mid)
{
    vector<int> left_vec(mid - left + 1);
    vector<int> right_vec(right - mid);
    for (size_t i = 0; i < left_vec.size(); ++i)
        left_vec[i] = vec[left + i];
    for (size_t i = 0; i < right_vec.size(); ++i)
        right_vec[i] = vec[mid + 1 + i];
    
    size_t i = 0, j = 0, k = left;
    while (i < left_vec.size() && j < right_vec.size()) // пока есть элементы в обеих частях
    {
        if (left_vec[i] <= right_vec[j]) // сравниваем элементы из обеих частей
        {
            vec[k++] = left_vec[i++]; // добавляем меньший элемент в результирующий массив
        }
        else
        {
            vec[k++] = right_vec[j++];
        }
    } // добавляем оставшиеся элементы из левой или правой части
    while (i < left_vec.size())
    {
        vec[k++] = left_vec[i++];
    }
    while (j < right_vec.size())
    {
        vec[k++] = right_vec[j++];
    }
}
/// @brief рекурсивная функция сортировки слиянием
/// @param vec сортируемый вектор
/// @param left начальный индекс
/// @param right конечный индекс
void merge_sort_recursive(vector<int> &vec, size_t left, size_t right)
{
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    merge_sort_recursive(vec, left, mid);
    merge_sort_recursive(vec, mid + 1, right);
    merge(vec, left, right, mid);
}
/// @brief сортировка слиянием
/// @param vec сортируемый вектор
void merge_sort(vector<int> &vec)
{
    size_t size = vec.size();
    merge_sort_recursive(vec, 0, size - 1);
}
template <typename T>
T safe_input(const string prompt, const string error_message, function<bool(T)> validator = nullptr)
{
    T input;
    cout << prompt;
    while (true)
    {
        if (!(cin >> input) || (cin.peek() != '\n') || (validator && !validator(input)))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << error_message << endl;
            cout << prompt;
        }
        else
        {
            return input;
        }
    }
}
/// @brief генерация случайного вектора
/// @param size размер
/// @param UPPER_RAND наибольшее возможное число
/// @return случайный вектор
vector<int> generate_random_vector(size_t size, int UPPER_RAND = 100)
{
    vector<int> vec(size);
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = rand() % UPPER_RAND;
    }
    return vec;
}
/// @brief генерация почти отсортированного вектора
/// @param size размер
/// @param UPPER_RAND наибольшее возможное число
/// @return почти отсортированный вектор
vector<int> generate_almost_sorted_vector(size_t size, int UPPER_RAND = 100)
{
    vector<int> vec = generate_random_vector(size, UPPER_RAND);
    sort(vec.begin(), vec.end());
    int entropies = size / 10; // количество случайных перестановок
    for (int i = 0; i < entropies; i++)
    {
        size_t idx1 = rand() % size;
        size_t idx2 = rand() % size;
        swap(vec[idx1], vec[idx2]);
    }
    return vec;
}
void print_vector(const vector<int> &vec, size_t REASANOBLE_SIZE = 100)
{
    size_t i = 0;
    for (const auto &val : vec)
    {
        cout << val << " ";
        ++i;
        if (i >= REASANOBLE_SIZE)
        {
            cout << endl;
            return;
        }
    }
    cout << endl;
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    bool run = true;
    while (run)
    {
        int size = safe_input<int>("Input vector size:", "Invalid input", [](int s)
                                   { return s >= 1; });
        vector<int> vec = generate_random_vector(size);
        vector<int> vec2 = vec;
        cout << "Generated vector:\n";
        print_vector(vec);
        selection_sort(vec);
        cout << "Selection sort:\n";
        print_vector(vec);
        merge_sort(vec2);
        cout << "Merge sort:\n";
        print_vector(vec2);
        char choice = safe_input<char>("Do you want to continue? (y/n):", "(y/n)", [](char c)
                                       { return string("YyNn").find(c) != string::npos; });
        run = choice == 'Y' || choice == 'y';
    }
    return 0;
}
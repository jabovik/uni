#include <iostream>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
#include <chrono>
#include <iomanip>
using namespace std;

/// @brief сортировка вставками
/// @param vec 
void insertion_sort(vector<int> &vec)
{
    size_t size = vec.size();
    for (size_t i = 1; i < size; ++i) // проходим по всем элементам, начиная со второго
    {
        int key = vec[i]; // текущий элемент для вставки
        size_t j = i;
        while (j > 0 && vec[j - 1] > key) //
        {
            vec[j] = vec[j - 1]; // сдвигаем элемент вправо
            --j;
        }
        vec[j] = key; // вставляем текущий элемент на его место
    }
}
/// @brief функция слияния двух отсортированных частей
/// @param vec сортируемый вектор
/// @param left начальный индекс
/// @param right конечный индекс
/// @param mid срединный индекс (конец левой части включительно)
void _merge(vector<int> &vec, size_t left, size_t right, size_t mid)
{
    vector<int> left_vec(mid - left + 1); // левая часть
    vector<int> right_vec(right - mid); // правая часть
    for (size_t i = 0; i < left_vec.size(); ++i)
        left_vec[i] = vec[left + i]; // копируем элементы в левую часть
    for (size_t i = 0; i < right_vec.size(); ++i)
        right_vec[i] = vec[mid + 1 + i]; // копируем элементы в правую часть

    size_t i = 0, j = 0, k = left;
    while (i < left_vec.size() && j < right_vec.size()) // пока есть элементы в обеих частях
    {
        if (left_vec[i] <= right_vec[j]) // сравниваем элементы из обеих частей
        {
            vec[k++] = left_vec[i++]; // добавляем меньший элемент в результирующий вектор
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
/// @brief рекурсивная вспомогательная функция сортировки слиянием
/// @param vec сортируемый вектор
/// @param left начальный индекс
/// @param right конечный индекс
void _merge_sort_recursive(vector<int> &vec, size_t left, size_t right)
{
    if (left >= right) // базовый случай рекурсии (один элемент)
        return;
    size_t mid = left + (right - left) / 2; // разделение массива на две части
    _merge_sort_recursive(vec, left, mid); // рекурсивная сортировка левой части
    _merge_sort_recursive(vec, mid + 1, right); // рекурсивная сортировка правой части
    _merge(vec, left, right, mid); // слияние двух отсортированных частей
}
/// @brief сортировка слиянием
/// @param vec сортируемый вектор
void merge_sort(vector<int> &vec)
{
    size_t size = vec.size();
    _merge_sort_recursive(vec, 0, size - 1); // запуск алгоритма
}
/// @brief шаблонная функция безопасного ввода
/// @tparam T
/// @param prompt пояснение пользователю
/// @param error_message
/// @param validator функция проверки корректности ввода
/// @return обработанный и безопасный ввод
template <typename T>
T safe_input(const string &prompt, const string &error_message, function<bool(T)> validator = nullptr)
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
void print_vector(const vector<int> &vec, size_t REASONABLE_SIZE = 100)
{
    size_t i = 0;
    for (const auto &val : vec)
    {
        cout << val << " ";
        ++i;
        if (i >= REASONABLE_SIZE)
        {
            cout << endl;
            return;
        }
    }
    cout << endl;
}
/// @brief функция вывода длительности с правильным форматированием.
/// @param duration
void print_duration(uint64_t duration)
{
    if (duration > 1'000'000)
    {
        cout << "Duration: " << duration / 1'000'000 << "ms\n";
    }
    else if (duration > 1'000)
    {
        cout << "Duration: " << duration / 1'000 << "us\n";
    }
    else
    {
        cout << "Duration: " << duration << "ns\n";
    }
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    bool run = true;
    while (run)
    {
        char choice;
        cout << "Insertion and Merge sort\n";
        int size = safe_input<int>("Input vector size:", "Invalid input", [](int s)
                                   { return s >= 1; });
        vector<int> vec(size);
        choice = safe_input<char>("Do you want to input vector manually? (y/n):", "(y/n)", [](char c)
                                  { return string("YyNn").find(c) != string::npos; });
        if (choice == 'Y' || choice == 'y')
        {
            cout << "Manual mode\n";
            for (size_t i = 0; i < size; ++i)
            {
                int input = safe_input<int>("[" + to_string(i) + "]=", "Invalid input");
                vec[i] = input;
            }
        }
        else
        {
            cout << "Generation mode\n";
            char choice2 = safe_input<char>("Do you want vector to be random(y) or almost sorted?(n):", "(y/n)", [](char c)
                                            { return string("YyNn").find(c) != string::npos; });
            if (choice2 == 'Y' || choice2 == 'y')
            {
                cout << "Random vector chosen\n";
                vec = generate_random_vector(size);
            }
            else
            {
                cout << "Almost sorted vector chosen\n";
                vec = generate_almost_sorted_vector(size);
            }
        }
        vector<int> vec_insertion = vec;
        vector<int> vec_merge = vec;
        vector<int> vec_stdsort = vec;
        cout << "Original vector:\n";
        print_vector(vec);

        auto t1_s = chrono::high_resolution_clock::now();
        insertion_sort(vec_insertion);
        auto t1_e = chrono::high_resolution_clock::now();
        auto t1_d = chrono::duration_cast<chrono::nanoseconds>(t1_e - t1_s).count();

        cout << "Insertion sort:\n";
        print_vector(vec_insertion);
        print_duration(t1_d);

        auto t2_s = chrono::high_resolution_clock::now();
        merge_sort(vec_merge);
        auto t2_e = chrono::high_resolution_clock::now();
        auto t2_d = chrono::duration_cast<chrono::nanoseconds>(t2_e - t2_s).count();

        cout << "Merge sort:\n";
        print_vector(vec_merge);
        print_duration(t2_d);

        auto t3_s = chrono::high_resolution_clock::now();
        sort(vec_stdsort.begin(), vec_stdsort.end());
        auto t3_e = chrono::high_resolution_clock::now();
        auto t3_d = chrono::duration_cast<chrono::nanoseconds>(t3_e - t3_s).count();

        cout << "std::sort():\n";
        print_vector(vec_stdsort);
        print_duration(t3_d);

        long double stdsort_insertion_ratio = static_cast<long double>(t1_d) / static_cast<long double>(t3_d);
        string stdsort_insertion_relation = "faster";
        if (stdsort_insertion_ratio < 1)
        {
            stdsort_insertion_ratio = 1 / stdsort_insertion_ratio;
            stdsort_insertion_relation = "slower";
        }
        long double stdsort_merge_ratio = static_cast<long double>(t2_d) / static_cast<long double>(t3_d);
        string stdsort_merge_relation = "faster";
        if (stdsort_merge_ratio < 1)
        {
            stdsort_merge_ratio = 1 / stdsort_merge_ratio;
            stdsort_merge_relation = "slower";
        }
        cout << "std::sort() is " << setprecision(3) << stdsort_insertion_ratio << " times " << stdsort_insertion_relation << " than insertion sort\n";
        cout << "std::sort() is " << setprecision(3) << stdsort_merge_ratio << " times " << stdsort_merge_relation << " than merge sort\n";
        choice = safe_input<char>("Do you want to continue? (y/n):", "(y/n)", [](char c)
                                  { return string("YyNn").find(c) != string::npos; });
        run = choice == 'Y' || choice == 'y';
    }
    return 0;
}
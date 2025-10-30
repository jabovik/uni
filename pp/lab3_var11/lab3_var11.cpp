#include <iostream>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
#include <chrono>
#include <iomanip>
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
    if (left >= right)
        return;
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

void print_duration(uint64_t duration)
{
    if (duration > 1'000'000)
    {
        cout << "Duration: " << duration/1'000'000 << "ms\n";
    }
    else if (duration > 1'000)
    {
        cout << "Duration: " << duration/1'000 << "us\n";
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
        cout << "Bubble and Merge sort\n";
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
        vector<int> vec_selection = vec;
        vector<int> vec_merge = vec;
        vector<int> vec_stdsort = vec;
        cout << "Original vector:\n";
        print_vector(vec);

        auto t1_s = chrono::high_resolution_clock::now();
        selection_sort(vec_selection);
        auto t1_e = chrono::high_resolution_clock::now();
        auto t1_d = chrono::duration_cast<chrono::nanoseconds>(t1_e - t1_s).count();

        cout << "Selection sort:\n";
        print_vector(vec_selection);
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

        long double stdsort_selection_ratio = static_cast<long double>(t1_d) / static_cast<long double>(t3_d);
        string stdsort_selection_relation = "faster";
        if(stdsort_selection_ratio<1)
        {
            stdsort_selection_ratio = 1/stdsort_selection_ratio;
            stdsort_selection_relation = "slower";
        }
        long double stdsort_merge_ratio = static_cast<long double>(t2_d) / static_cast<long double>(t3_d);
        string stdsort_merge_relation = "faster";
        if(stdsort_merge_ratio<1)
        {
            stdsort_merge_ratio = 1/stdsort_merge_ratio;
            stdsort_merge_relation = "slower";
        }
        cout << "std::sort() is " << setprecision(3) << stdsort_selection_ratio << " times " << stdsort_selection_relation << " than selection sort\n";
        cout << "std::sort() is " << setprecision(3) << stdsort_merge_ratio << " times " << stdsort_merge_relation << " than merge sort\n";
        choice = safe_input<char>("Do you want to continue? (y/n):", "(y/n)", [](char c)
                                  { return string("YyNn").find(c) != string::npos; });
        run = choice == 'Y' || choice == 'y';
    }
    return 0;
}
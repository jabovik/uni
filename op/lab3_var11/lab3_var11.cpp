#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <limits>
using namespace std;

/// @brief Bubble sort (changes vector)
/// @param vec vector to sort
void bubble_sort(int *vec, size_t size)
{
    bool swapped = true;
    for (size_t i = 0; (i < size - 1) && swapped; i++)
    {
        for (size_t j = 0; j < size - i - 1; j++)
        {
            swapped =false;
            if (vec[j] > vec[j + 1])
            {
                swap(vec[j], vec[j + 1]);
                swapped =true;
            }
        }
        /*if(!swapped)
        {
            break;
        }
        */
    }
}
/// @brief generic safe input
/// @tparam T type to input
/// @param prompt tells user what he has to input
/// @param error_message
/// @param validator optional function for input validation
/// @return processed and safe to use user input
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

int* generate_random_vector(size_t size, int UPPER_RAND)
{
    int* vec = new int[size];
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = rand() % UPPER_RAND;
    }
    return vec;
}

void print_vector(const int* vec, size_t size)
{
    const size_t REASANOBLE_SIZE = 100;
    for (size_t i = 0; i<size && i< REASANOBLE_SIZE;i++)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
}
int main(int argc, char const *argv[])
{
    srand(time(0));
    bool run = true;
    while (run)
    {
        cout << "Bubble sort\n";
        int size = safe_input<int>("Input vector size:", "Invalid input", [](int s)
                                         { return s >= 1; });
        int* vec = generate_random_vector(size, 100);
        cout << "Generated vector:\n";
        print_vector(vec, size);
        bubble_sort(vec, size);
        cout << "Sorted vector:\n";
        print_vector(vec, size);
        delete vec;
        char choice = safe_input<char>("Do you want to continue? (y/n):", "(y/n)", [](char c)
                                       { return string("YyNn").find(c) != string::npos; });
        run = choice == 'Y' || choice == 'y';
    }
    return 0;
}

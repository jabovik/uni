/*
Числа Армстронга - натуральные числа, равные сумме своих цифр,
возведённых в степеть количества цифр в их записи
например 153 = 1^3+ 5^3+ 3^3    - это число Армстронга.
Требуется найти все числа Армстронга в заданных границах.
*/
#include <iostream>
#include <limits>
#include <vector>
#include <cmath>

using namespace std;
int get_num_10(long long number)
{
    int count = 1;
    while (number / 10 != 0)
    {
        number /= 10;
        ++count;
    }
    return count;
}
/*
bool check(long long number)
{
    long long origin = number;
    int power = get_num_10(number);
    long long sum = 0;
    while (number / 10 != 0)
    {
        sum += pow(number % 10, power);
        number /= 10;
    }
    sum += pow(number % 10, power);
    return sum == origin;
}
*/

bool check(long long number)
{
    vector<int> digits;
    int power = 1;
    long long sum = 0;
    long long origin = number;
    while (number / 10 != 0)
    {
        digits.push_back(number%10);
        number /= 10;
        ++power;
    }
    digits.push_back(number%10);
    for(auto digit : digits)
    {
        sum+=pow(digit, power);
    }
    return sum == origin;
}
const string INVALID_INPUT = "ERROR: INVALID INPUT. TRY AGAIN\n";
long long safe_input(string prompt)
{
    long long input;
    cout << prompt;
    while (!(cin >> input) || (cin.peek() != '\n') || input < 1)
    {
        cin.clear();                                         // сброс состояния ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока ввода
        cout << INVALID_INPUT;
    }
    return input;
}

int main(int argc, char const *argv[])
{
    while (true)
    {
        long long a, b;
        cout << "Поиск чисел Армстронга на отрезке от a до b\n";
        bool input_fl = true;
        while (input_fl)
        {
            a = safe_input("Введите положительное целое число a: ");
            b = safe_input("Введите положительное целое число b: ");
            if (a > b)
            {
                cout << "a должно быть <=b\n";
            }
            else
            {
                input_fl = false;
            }
        }
        for (long long i = a; i <= b; i++)
        {
            if (check(i))
            {
                cout << i << endl;
            }
        }
        cout << "Хотите повторить программу?(y/any key): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice!='y')
        {
            return 0;
        }
    }
}

#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
using namespace std;
/// @brief функция расчёта члена ряда в контексте суммы ряда. т.е можно использовать только при подсчёте с n=0
/// @param x 
/// @param n 
/// @return член ряда a_n
long double f(double x, double n)
{
    static long long int n_fact; // n!
    static long double x_to_n;   // pow(x,n)
    static long double two_pow; // 2^(n/2)
    if (n == 0)
    {
        n_fact = 1;
        x_to_n = 1;
        two_pow = 1;
    }
    else
    {
        n_fact *= n;
        x_to_n *= x;
        two_pow *= M_SQRT2;
    }
    return (two_pow * sin(M_PI * n / 4) * x_to_n) / n_fact;
}
const int WIDTH = 20;
void iter_info(int n, long double a_n, long double s_n, long double alpha_n)
{
    cout << n << setw(WIDTH) << a_n << setw(WIDTH) << s_n << setw(WIDTH) << alpha_n << '\n';
}

double safe_input()
{
    double input;
    while (!(cin >> input) || (cin.peek() != '\n'))
    {
        cin.clear();                                         // сброс состояния ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока ввода
        cout << "Неверный ввод. Повторите попытку: ";
    }
    return input;
}

int main(int argc, char const *argv[])
{
    char choice = 'y';
    while (choice == 'y')
    {
        double alpha;
        double x;
        cout << "Подсчёт частичного числового ряда вида:\n2^(n/2)*sin(pi*n/4)*x^n/n! \n";
        cout << "Введите x: ";
        x = safe_input();
        do // цикл ввода для alpha
        {
        cout << "Введите положительное число alpha: ";
        alpha = safe_input();
        } while (alpha<0);
        long double a_n = f(x,0);
        long double s_n = 0;
        long double a_n_next = 0;
        long double alpha_n = numeric_limits<long double>::max();
        cout << "n" << setw(WIDTH) << "a_n" << setw(WIDTH) << "s_n" << setw(WIDTH) << "alpha_n\n";
        if (trunc(alpha) == alpha) // проверка alhpha - целое или нет.
        {
            for (int n = 0; n < alpha; n++) // для целого
            {
                a_n_next = f(x,n+1);
                s_n += a_n;
                alpha_n = abs(a_n_next/s_n);
                iter_info(n, a_n, s_n, alpha_n);
                a_n = a_n_next;
            }
        }
        else
        {
            for (int n = 0; alpha_n >= alpha; n++) // для дробного
            {
                a_n_next = f(x,n+1);
                s_n += a_n;
                alpha_n = abs(a_n_next/s_n);
                iter_info(n, a_n, s_n, alpha_n);
                a_n = a_n_next;
            }
        }
    cout << "\nПовторить? (y/any key)\n";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}
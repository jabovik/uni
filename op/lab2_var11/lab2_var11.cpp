#include <iostream>
#include <math.h>
#include <iomanip>
#include <limits>
using namespace std;
long double f(double x, double n)
{
    static long long int n_fact; // n!
    static long double x_to_n;   // pow(x,n)
    static long double two_pow;
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

void iter_info(int n, long double a_n, long double s_n, long double alpha_n)
{
    cout << n << setw(15) << a_n << setw(15) << s_n << setw(15) << alpha_n << '\n';
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
        cout << "Введите alpha: ";
        alpha = safe_input();
        long double a_n = f(x,0);
        long double s_n = 0;
        long double s_prev = 0;
        long double a_n_next = 0;
        long double alpha_n = numeric_limits<long double>::max();
        cout << "n" << setw(15) << "a_n" << setw(15) << "s_n" << setw(15) << "alpha_n\n";
        if (trunc(alpha) == alpha)
        {
            for (int n = 0; n < alpha; n++)
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
            for (int n = 0; alpha_n >= alpha; n++)
            {
                a_n_next = f(x,n+1);
                s_n += a_n;
                alpha_n = abs(a_n_next/s_n);
                iter_info(n, a_n, s_n, alpha_n);
                a_n = a_n_next;
            }
        }
    cout << "\n\nПовторить? (y/any key)\n";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}
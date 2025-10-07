#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <string>
#include <cctype>
using namespace std;
/// @brief функция расчёта члена ряда в контексте суммы ряда.
/// @param x
/// @param n
/// @return член ряда a_n
long double f(double x, double n, long double a_n_prev)
{
    return (a_n_prev*M_SQRT2*x/n)*sin(M_PI*n/4)/sin(M_PI*(n-1)/4);
}

const int WIDTH = 20;
void iter_info(int n, long double a_n, long double s_n, long double alpha_n)
{
    cout << n << setw(WIDTH) << a_n << setw(WIDTH) << s_n << setw(WIDTH) << alpha_n << '\n';
}

template <typename T>
T safe_input()
{
    T input;
    while (!(cin >> input) || (cin.peek() != '\n'))
    {
        cin.clear();                                         // сброс состояния ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока ввода
        cout << "Неверный ввод. Повторите попытку: ";
    }
    return input;
}

bool validate_alpha_string(string alpha_str)
{
    int dot_count;
    for (auto &i : alpha_str)
    {
        if (!isdigit(i) && i != '.')
        {
            return false;
        }
        else if (i == '.')
        {
            ++dot_count;
        }
        if(dot_count>1)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    char choice = 'y';
    while (choice == 'y')
    {
        string alpha_str;
        double alpha;
        double x;
        cout << "Подсчёт частичного числового ряда вида:\n2^(n/2)*sin(pi*n/4)*x^n/n! \n";
        cout << "Введите x: ";
        x = safe_input<double>();

        do // цикл ввода для alpha
        {
            cout << "Введите положительное число alpha: ";
            alpha_str = safe_input<string>();
            try
            {
                if (!validate_alpha_string(alpha_str))
                {
                    throw invalid_argument("invalid string");
                }

                alpha = stod(alpha_str);
            }
            catch (const invalid_argument &e)
            {
                alpha = -1;
            }

        } while (alpha <= 0);

        bool alpha_is_int = false;
        if (alpha_str == to_string(stoi(alpha_str))) // если дробная часть не была отброшена, значит её и не было.
        {
            alpha_is_int = true; // тогда число целое
        }
        cout << "Выбран режим: ";
        if (alpha_is_int)
        {
            cout << "по количеству членов\n";
        }
        else
        {
            cout << "по точности\n";
        }

        long double a_n = 0;
        long double s_n = 0;
        long double a_n_next = 0;
        long double alpha_n = numeric_limits<long double>::max();

        cout << "n" << setw(WIDTH) << "a_n" << setw(WIDTH) << "s_n" << setw(WIDTH) << "alpha_n\n";
        //cout << "0" << setw(WIDTH) << "0" << setw(WIDTH) << "0" << setw(WIDTH) << "-\n";
        if (alpha_is_int) // проверка alhpha - целое или нет.
        {
            for (int n = 0; n < alpha; n++) // для целого
            {
                a_n_next = f(x, n, a_n);
                s_n += a_n;
                alpha_n = abs(a_n_next / s_n);
                iter_info(n, a_n, s_n, alpha_n);
                a_n = a_n_next;
            }
        }
        else
        {
            for (int n = 0; alpha_n >= alpha; n++) // для дробного
            {
                a_n_next = f(x, n, a_n);
                s_n += a_n;
                alpha_n = abs(a_n_next / s_n);
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
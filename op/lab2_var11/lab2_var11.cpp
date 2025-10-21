#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <string>
#include <functional>
using namespace std;
// b_n+1 = b_n(sqrt(2)x/n+1)
//  1.2e+3 + балл
/// @brief рекуррентная формула без sin.
/// @param x
/// @param n
/// @param b_n_prev предыдущий член
/// @return
long double calc_b_n(double x, double n, long double b_n_prev)
{
    return b_n_prev * M_SQRT2 * x / n;
}
/// @brief расчёт члена ряда из b_n умножением на синус
/// @param n
/// @param b_n
/// @return
long double calc_a_n(double n, long double b_n)
{
    return b_n * sin(M_PI * n / 4);
}

const int WIDTH = 20;
void iter_info(int n, long double b_n, long double a_n, long double s_b_n, long double s_a_n, long double alpha_n)
{
    cout << n << setw(WIDTH) << b_n << setw(WIDTH) << a_n << setw(WIDTH) << s_b_n << setw(WIDTH) << s_a_n << setw(WIDTH) << alpha_n << endl;
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
/// @brief валидация строки с double при помощи regex
/// @param str строка double
/// @return соответствует ли шаблону
bool validate_double_string(string str)
{
    bool match = true;
    bool has_exp = false;
    bool has_digits = false;
    bool has_dot = false;
    int i = 0;
    if (str[i] == '+' || str[i] == '-')
    {
        i++;
    }
    for (; i < str.size(); i++)
    {
        if (isdigit(str[i]))
        {
            has_digits = true;
        }
        else if (str[i] == '.')
        {
            if (has_dot || has_exp)
            {
                match = false;
                return match;
            }
            has_dot = true;
        }
        else if (str[i] == 'e' || str[i] == 'E')
        {
            if (has_exp || !has_digits)
            {
                match = false;
                return match;
            }
            has_exp = true;
            if (i + 1 == str.size())
            {
                match = false;
                return match;
            }
            if (str[i + 1] == '+' || str[i + 1] == '-')
            {
                ++i;
                if (i + 1 == str.size())
                {
                    match = false;
                    return match;
                }
            }
        }
        else
        {
            match = false;
            return match;
        }
    }
    return match;
    //  ^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$
}

int main(int argc, char const *argv[])
{
    char choice = 'y';
    while (choice == 'y')
    {
        string alpha_str;
        string x_str;
        double alpha;
        double x;
        cout << "Подсчёт частичного числового ряда вида:\n2^(n/2)*sin(pi*n/4)*x^n/n! \n";
        x_str = safe_input<string>("Введите x:", "Неверный ввод", validate_double_string);
        x = stod(x_str);
        alpha_str = safe_input<string>("Введите alpha:", "Неверный ввод(alpha должна быть>0)", [](string s)
                                       {
            if(validate_double_string(s) && stod(s)>0)
            {
                return true;
            }
            return false; });
        alpha = stod(alpha_str);
        bool alpha_is_int = true;
        if (alpha_str.find('.') != string::npos)
        {
            alpha_is_int = false;
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
        long double b_n = 1;
        long double a_n = 0;
        long double s_a_n = 0;
        long double s_b_n = b_n;
        long double alpha_n = abs(calc_b_n(x, 1, b_n) / s_b_n);

        cout << "n" << setw(WIDTH) << "b_n" << setw(WIDTH) << "a_n" << setw(WIDTH) << "s_b_n" << setw(WIDTH) << "s_a_n" << setw(WIDTH) << "alpha_bn\n";
        iter_info(0, b_n, a_n, s_b_n, s_a_n, alpha_n);
        if (alpha_is_int) // проверка alhpha - целое или нет.
        {
            for (int n = 1; n < alpha; n++) // для целого
            {
                b_n = calc_b_n(x, n, b_n);
                s_b_n += b_n;
                a_n = calc_a_n(n, b_n);
                s_a_n += a_n;
                alpha_n = abs(calc_b_n(x, n + 1, b_n) / s_b_n);
                iter_info(n, b_n, a_n, s_b_n, s_a_n, alpha_n);
            }
        }
        else
        {
            for (int n = 1; alpha_n >= alpha; n++) // для дробного
            {
                b_n = calc_b_n(x, n, b_n);
                s_b_n += b_n;
                a_n = calc_a_n(n, b_n);
                s_a_n += a_n;
                alpha_n = abs(calc_b_n(x, n + 1, b_n) / s_b_n);
                iter_info(n, b_n, a_n, s_b_n, s_a_n, alpha_n);
            }
        }
        cout << "\nПовторить? (y/any key)\n";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}
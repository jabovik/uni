#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
using namespace std;

/// @brief рекуррентная формула без sin.
long double calc_b_n(double x, double n, long double b_n_prev)
{
    return b_n_prev * M_SQRT2 * x / n;
}

/// @brief расчёт члена ряда из b_n умножением на синус
long double calc_a_n(double n, long double b_n)
{
    return b_n * sin(M_PI * n / 4);
}

const int WIDTH = 20;
void iter_info(Data data)
{
    cout << data.n << setw(WIDTH) << data.b_n << setw(WIDTH) << data.a_n << setw(WIDTH) << data.s_b_n << setw(WIDTH) << data.s_a_n << setw(WIDTH) << data.alpha_n << endl;
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
        i++; // если есть знак в начале - пропустить.
    }
    for (; i < str.size(); i++) // проверяем остальную строку
    {
        if (isdigit(str[i]))
        {
            has_digits = true;
        }
        else if (str[i] == '.')
        {
            if (has_dot || has_exp) // не может быть 2 точки или точка после E
            {
                match = false;
                return match;
            }
            has_dot = true;
        }
        else if (str[i] == 'e' || str[i] == 'E')
        {
            has_exp = true;
            if (has_exp || !has_digits) // не может быть 2 E или отсутствовать цифры
            {
                match = false;
                return match;
            }
            else if (i + 1 == str.size()) // если после e ничего больше нет - некорректно
            {
                match = false;
                return match;
            }
            else if (str[i + 1] == '+' || str[i + 1] == '-') // если есть знак - пропустить
            {
                ++i;
                if (i + 1 == str.size()) //  если после знака ничего - некорректно
                {
                    match = false;
                    return match;
                }
            }
        }
        else // если токен не подходит ни одному из условий - некорректно.
        {
            match = false;
            return match;
        }
    }
    return match;
    //  ^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$
}

struct Data
{
    int n;
    long double b_n;
    long double a_n;
    long double s_b_n;
    long double s_a_n;
    long double alpha_n;
};


int main(int argc, char const *argv[])
{
    char choice = 'y';
    vector<Data> data_prev;
    double x_prev = std::numeric_limits<double>::quiet_NaN();
    while (choice == 'y')
    {
        string alpha_str;
        string x_str;
        double alpha;
        double x;
        long double b_n = 1;
        long double a_n = 0;
        long double s_a_n = 0;
        long double s_b_n = b_n;
        long double alpha_n = numeric_limits<long double>::max();
        cout << "Подсчёт частичного числового ряда вида:\n2^(n/2)*sin(pi*n/4)*x^n/n! \n";
        bool x_valid = true;
        do // цикл ввода для x
        {
            cout << "Введите x: ";
            x_str = safe_input<string>();
            if (validate_double_string(x_str))
                x = stod(x_str);
            else
                x_valid = false;

        } while (!x_valid);
        do // цикл ввода для alpha
        {
            cout << "Введите положительное число alpha: ";
            alpha_str = safe_input<string>();
            if (validate_double_string(alpha_str))
                alpha = stod(alpha_str);
            else
                alpha = -1;

        } while (alpha <= 0);
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
        cout << "n" << setw(WIDTH) << "b_n" << setw(WIDTH) << "a_n" << setw(WIDTH) << "s_b_n" << setw(WIDTH) << "s_a_n" << setw(WIDTH) << "alpha_bn\n";
        cout << 0 << setw(WIDTH) << b_n << setw(WIDTH) << a_n << setw(WIDTH) << s_b_n << setw(WIDTH) << s_a_n << setw(WIDTH) << "-\n";
        int n = 1;
        if (x == x_prev)
        {
            int last_n = data_prev.back().n;
            if (alpha_is_int)
            {
                while (n < alpha && n < last_n)
                {
                    Data data = data_prev[n];
                    iter_info(data);
                    ++n;
                }
            }
            else
            {
                while (data_prev[n].alpha_n >= alpha && n < last_n)
                {
                    Data data = data_prev[n];
                    iter_info(data);
                    ++n;
                }
            }
        }
        else
        {
            data_prev.clear();
            data_prev.push_back({0, b_n, a_n, s_b_n, s_a_n, alpha_n});
        }
        if (alpha_is_int) // проверка alhpha - целое или нет.
        {
            for (; n < alpha; n++) // для целого
            {
                b_n = calc_b_n(x, n, b_n);
                s_b_n += b_n;
                a_n = calc_a_n(n, b_n);
                s_a_n += a_n;
                alpha_n = abs(calc_b_n(x, n + 1, b_n) / s_b_n);
                Data data = {n, b_n, a_n, s_b_n, s_a_n, alpha_n};
                iter_info(data);
                data_prev.push_back({n, b_n, a_n, s_b_n, s_a_n, alpha_n});
            }
        }
        else
        {
            for (; alpha_n >= alpha; n++) // для дробного
            {
                b_n = calc_b_n(x, n, b_n);
                s_b_n += b_n;
                a_n = calc_a_n(n, b_n);
                s_a_n += a_n;
                alpha_n = abs(calc_b_n(x, n + 1, b_n) / s_b_n);
                Data data = {n, b_n, a_n, s_b_n, s_a_n, alpha_n};
                iter_info(data);
                data_prev.push_back(data);
            }
        }
        x_prev = x;
        cout << "\nПовторить? (y/any key)\n";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}
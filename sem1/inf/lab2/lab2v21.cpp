/*
Вариант 21
Концентрация раствора

Входные данные:
Текущий объём раствора
Текущая концентрация
Требуемая концентрация

Выходные данные:
Объем воды или вещества для достижения требуемой концентрации


Концентрация считается по формуле:
V_вещества/(V_вещества + V_воды)
*/

#include <iostream>
#include <limits>
#include <functional>
using namespace std;

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
int main(int argc, char const *argv[])
{
    const string INVALID_INPUT = "INVALID INPUT, TRY AGAIN\n";
    bool run = true;
    while (run)
    {
        double init_conc, init_volume, target_conc;
        init_volume = safe_input<double>("Enter initial volume: ", INVALID_INPUT, [](double d)
                                         { return d > 0; });
        init_conc = safe_input<double>("Enter initial concentration: ", INVALID_INPUT, [](double d)
                                       { return d <= 1 && d >= 0; });
        target_conc = safe_input<double>("Enter target concentration: ", INVALID_INPUT, [](double d)
                                         { return d <= 1 && d >= 0; });
        double init_sub_volume = init_volume * init_conc;
        double added_volume;
        bool is_sub_added;
        if (init_conc < target_conc)
        {
            // добавим вещества
            is_sub_added = true;
            added_volume = (target_conc * init_volume - init_sub_volume) / (1 - target_conc);
        }
        else if (init_conc > target_conc)
        {
            // добавим воды
            is_sub_added = false;
            added_volume = init_sub_volume / target_conc - init_volume;
        }
        
        if (init_conc == target_conc)
        {
            cout << "Do nothing\n";
        }
        else if (target_conc == 0 && init_conc != 0)
        {
            cout << "Water volume to add tends to infinity\n";
        }
        else if (target_conc == 1 && init_conc != 1)
        {
            cout << "Substance volume to add tends to infinity\n";
        }
        else
        {
            cout << "Add " << added_volume;
            is_sub_added ? cout << " of substance\n" : cout << " of water\n";
        }
        cout << "Repeat? ";
        char choice = safe_input<char>("(y/n):", "(y/n)", [](char c)
                                       { return string("YyNn").find(c) != string::npos; });
        run = choice == 'Y' || choice == 'y';
    }
    return 0;
}
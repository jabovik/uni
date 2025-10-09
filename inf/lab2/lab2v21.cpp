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
using namespace std;

const string INVALID_INPUT = "ERROR: INVALID INPUT. TRY AGAIN\n";
template <typename T>
T safe_input(string prompt)
{
    T input;
    cout << prompt;
    while (!(cin >> input) || (cin.peek() != '\n'))
    {
        cin.clear();                                         // сброс состояния ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока ввода
        cout << INVALID_INPUT;
    }
    return input;
}
int main(int argc, char const *argv[])
{
    char choice = 'y';
    while (choice == 'y')
    {
        double init_conc, init_volume, target_conc;
        do
        {
            init_volume = safe_input<double>("Введите начальный объём: ");
            if (init_volume < 0)
            {
                cout << INVALID_INPUT;
            }
        } while (init_volume < 0);

        do
        {
            init_conc = safe_input<double>("Введите начальную концентрацию: ");
            if (init_conc > 1 || init_conc < 0)
            {
                cout << INVALID_INPUT;
            }
        } while (init_conc >= 1 || init_conc < 0);

        do
        {
            target_conc = safe_input<double>("Введите целевую концентрацию: ");
            if (target_conc >= 1 || target_conc < 0)
            {
                cout << INVALID_INPUT;
            }
        } while (target_conc > 1 || target_conc < 0);

        double init_sub_volume = init_volume * init_conc;
        double added_volume;
        bool added_sub;
        if (init_conc < target_conc)
        {
            // добавим вещества
            added_sub = true;
            added_volume = (target_conc * init_volume - init_sub_volume) / (1 - target_conc);
        }
        else if (init_conc > target_conc)
        {
            // добавим воды
            added_sub = false;
            added_volume = init_sub_volume / target_conc - init_volume;
        }
        else
        {
            cout << "init_conc = target_conc";
        }
        if (init_conc != target_conc)
        {
            cout << "Добавьте " << added_volume;
            added_sub ? cout << " вещества\n" : cout << " воды\n";
        }
        else
        {
            cout << "Ничего добавлять не нужно\n";
        }
        cout << "Повторить программу? ";
        do
        {
            choice = safe_input<char>("(y/n)\n");
        } while (choice != 'y' && choice != 'n');
    }
    return 0;
}

/*
Вариант 29. Игра со спичками
Игра со спичками заключается в следующем. На столе лежат n
спичек, играют двое, ходят по очереди. За свой ход игрок может взять
от a до b спичек включительно. Проигрывает тот, кто не может
сделать ход. Требуется написать программу, играющую с
пользователем в эту игру. Перед началом игры пользователь задаёт
числа a, b и n, после чего пользователь и программа по очереди
делают ходы. В конце игры программа определяет и объявляет
победителя.

Входные данные. Параметры игры a, b и n (a ≤ b), а также ходы игрока.
Выходные данные. Результат каждого хода и общий результат игры.

0<a≤b


Скажем, можно брать от 1 до 3
а количество спичек = 10
тогда по каждой позиции
0 - П
1 - В
2 - В
3 - В
4 - П
5 - В
6 - В
7 - В
8 - П
9 - В
10 -В

от 2 до 3
0 - П
1 - П
2 - В
3 - В
4 - В
5 - П
6 - П
7 - В
8 - В
9 - В
10 - П

от 3 до 3
0 - П
1 - П
2 - П
3 - В
4 - В
5 - В
6 - П
7 - П
8 - П
9 - В
10 - В


i%T >=a
или же
n%t <a для проигрышной

Заметим, периоды = a+b, в которых состояния повторяются
также количество проигрышных состояний в каждом периоде = b, выигрышных = a
чтобы понять, является ли позиция выигрышной, выведем условие:
i%T >=a
для проигрышной:
i%T < a
,где i - номер позиции(кол-во гвоздей)
T - период = a+b

следовательно бот должен стремиться вогнать игрока в проигрышное состояние и не выпускать.
*/

#include <iostream>
#include <limits>
using namespace std;
const string INVALID_INPUT = "ERROR: INVALID INPUT. TRY AGAIN\n";
int safe_input(string prompt)
{
    int input;
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
    char choice = 'y';
    while (choice == 'y')
    {
        cout << "Game with matches\n";
        int n = safe_input("Input the total number of matches: ");
        int a = safe_input("Input the minimal number of taken matches: ");
        int b = safe_input("Input the maximal number of taken matches: ");
        cout << "Do you want to go first? (y/any key)";
        char go_first;
        cin >> go_first;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        bool is_player_turn = false;
        if (go_first == 'y')
            is_player_turn = true;
        bool game = n >= a;
        while (game)
        {
            cout << "Current number of matches: " << n << endl;
            if (is_player_turn)
            {
                bool fl = true;
                int turn;
                while (fl)
                {
                    turn = safe_input("Your turn: ");
                    fl = turn < a || turn > b;
                    if (fl)
                    {
                        cout << "INVALID INPUT: MUST BE A NUMBER FROM " << a << " TO " << b << ". TRY AGAIN\n";
                    }
                }
                if (turn > n)
                {
                    cout << "You entered " << turn << " matches, but only " << n << " is present. Taking " << n << " matches.\n";
                    turn = n;
                }
                n -= turn;
            }
            else
            {
                int turn = b;
                if (n >= a && n <= b)
                {
                    turn = n;
                }
                else
                {

                    for (int i = n - b; i <= n - a; i++) // поиск нужного состояния
                    {
                        if (i % (a + b) < a) // если найдено
                        {
                            turn = n - i;
                            break;
                        }
                    }
                }
                n -= turn;
                cout << "Bot takes " << turn << " matches\n";
            }
            is_player_turn = !is_player_turn;
            if (n < a)
            {
                game = false;
            }
        }
        cout << "Game is over. ";
        if (is_player_turn)
        {
            cout << "You lose!\n";
        }
        else
        {
            cout << "You won!\n";
        }
        cout << "Do you want to play again? (y/any key)";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}

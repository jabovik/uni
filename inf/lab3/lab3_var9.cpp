/* Вариант 9 Задача о вагонах.
Среда: n соединённых вагонов, причём первый и последний тоже соеденены(кольцо).
В каждом вагоне есть лампочка, которая может быть либо включена либо выключена.
среда принимает ответ - кол-во вагонов
--------------------------------------------------
Агент может: перейти в соседний вагон,
получить состояние лампочки,
изменить состояние лампочки,
дать ответ среде.

Цель агента - угадать кол-во вагонов.
сделать ручной и автоматический режимы.


1)
Запоминаем состояние лампочки в начальном вагоне
идём в любую сторону и считаем кол-во переходов до тех пор, пока не найдём вагон с таким же состоянием.
меняем состояние лампочки
идём обратно к начальному вагону. Если состояние изменилось - значит кол-во проходов есть кол-во вагонов
Иначе повторяем
*/
#include <iostream>
#include <vector>
#include <functional>
#include <limits>
using namespace std;

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
class Env
{
private:
    int idx;
    vector<bool> wagons;

public:
    Env(int idx, vector<bool> wagons) : idx(idx), wagons(wagons)
    {
    }
    bool get_wagon_state()
    {
        return wagons[idx];
    }
    /*void change_wagon_state(bool state)
    {
        wagons[idx] = state;
    }*/
    void change_wagon_state()
    {
        wagons[idx] = !wagons[idx];
    }
    bool check_answer(int answer)
    {
        return answer == wagons.size();
    }
    void agent_move_next()
    {
        idx = (idx + 1) % wagons.size();
    }
    void agent_move_prev()
    {
        idx = (idx + wagons.size() - 1) % wagons.size();
    }
};

class Agent
{
private:
    Env &env;

protected:
    void move_next() { env.agent_move_next(); }
    void move_prev() { env.agent_move_prev(); }
    bool get_wagon_state() { return env.get_wagon_state(); }
    bool check_answer(int answer) { return env.check_answer(answer); }
    void change_wagon_state() { env.change_wagon_state(); }
    void print_info()
    {
        cout << "The light is ";
        get_wagon_state() ? cout << "on" : cout << "off";
        cout << "\n";
    }

public:
    Agent(Env &env) : env(env)
    {
    }
    virtual void run() = 0;
};
class AutoAgent : public Agent
{
private:
    enum AgentStatus
    {
        INIT,
        SEARCHING,
        RETURNING,
    };
    void print_info()
    {
        Agent::print_info();
        cout << "Input an action (next/previous/switch/answer)>";
    }
    void move_next()
    {
        cout << "next\n";
        Agent::move_next();
    }
    void change_wagon_state()
    {
        cout << "switch\n";
        Agent::change_wagon_state();
    }
    void move_prev()
    {
        cout << "previous\n";
        Agent::move_prev();
    }
    bool check_answer(int answer)
    {
        cout << "answer\n";
        cout << answer << '\n';
        return Agent::check_answer(answer);
    }
public:
    using Agent::Agent;
    void run() override
    {
        AgentStatus agent_status = INIT;
        bool first_state = get_wagon_state();
        int counter = 0;
        int temp;
        bool is_answer_correct = false;
        cout << "Auto Mode \n";
        while (!is_answer_correct)
        {
            print_info();
            switch (agent_status)
            {
            case INIT:
                first_state = get_wagon_state();
                move_next();
                ++counter;
                agent_status = SEARCHING;
                break;
            case SEARCHING:
                if (get_wagon_state() == first_state)
                {
                    change_wagon_state();
                    temp = counter;
                    agent_status = RETURNING;
                }
                else
                {
                    move_next();
                    ++counter;
                }
                break;
            case RETURNING:
                if (counter)
                {
                    move_prev();
                    --counter;
                }
                else
                {
                    if (get_wagon_state() != first_state)
                    {
                        is_answer_correct = check_answer(temp);
                        if (!is_answer_correct)
                        {
                            cout << "The answer is incorrect\n";
                            throw runtime_error("Error in RS algorithm");
                        }
                        else
                            cout << "The answer is correct\n";
                    }
                    else
                    {
                        cout << "next\n";
                        move_next();
                        ++counter;
                        agent_status = SEARCHING;
                    }
                }
                break;
            default:
                break;
            }
        }
    };
};

class ManualAgent : public Agent
{
public:
    using Agent::Agent;
    void run() override
    {
        cout << "Manual Mode \n";
        bool is_answer_correct = false;
        while (!is_answer_correct)
        {
            print_info();
            string choice = safe_input<string>("Input an action (next/previous/switch/answer)>", "invalid input", [](string s)
                                               { return s == "answer" || s == "previous" || s == "switch" || s == "next"; });
            if (choice == "answer")
            {
                int answer = safe_input<int>("Input an answer>", "invalid input", [](int i)
                                             { return i > 0; });
                is_answer_correct = check_answer(answer);
                if (is_answer_correct)
                {
                    cout << "The answer is correct\n";
                }
                else
                {
                    cout << "The answer is incorrect\n";
                }
            }
            else if (choice == "previous")
            {
                move_prev();
            }
            else if (choice == "next")
            {
                move_next();
            }
            else if (choice == "switch")
            {
                change_wagon_state();
            }
        }
    }
};

int main(int argc, char const *argv[])
{
    cout << "Ring of wagons\n";

    vector<bool> wagons;
    int start_idx;

    char custom = safe_input<char>("Do you want to set custom wagons? (y/n)>", "invalid input", [](char c)
                                   { return c == 'y' || c == 'n'; });
    if (custom == 'y')
    {
        int n = safe_input<int>("Input number of wagons>", "invalid input", [](int i)
                                { return i > 0; });
        for (int i = 0; i < n; ++i)
        {
            string prompt = "Input state of wagon " + to_string(i) + " (o/f)>";
            char state = safe_input<char>(prompt, "invalid input", [](char c)
                                          { return c == 'o' || c == 'f'; });
            if (state == 'o')
                wagons.push_back(true);
            else
                wagons.push_back(false);
        }
        start_idx = safe_input<int>("Input starting wagon index>", "invalid input", [n](int i)
                                    { return i >= 0 && i < n; });
    }
    else
    {
        wagons = {false, true, false, false, true, true, false};
        start_idx = 0; // temporary
        cout << "Using default wagons\n";
    }
    char mode = safe_input<char>("Manual or auto mode? (m/a)>", "invalid input", [](char c)
                                 { return c == 'm' || c == 'a'; });
    Env env(start_idx, wagons);
    Agent *agent;
    if (mode == 'a')
    {
        agent = new AutoAgent(env);
    }
    else if (mode == 'm')
    {
        agent = new ManualAgent(env);
    }
    agent->run();
    delete agent;
    return 0;
}
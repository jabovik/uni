#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Time
{
    double hours;
    double minutes;
    double seconds;
    string num_alphabet = "0123456789";
    void validate_string(const string &str) // проверка форматирования строки
    {
        int count = 0;
        int numbers = 0;
        for (auto c : str) // подсчёт количества разделителей и чисел
        {
            if (num_alphabet.find(c) != string::npos)
            {
                ++numbers;
            }
            else if (c == ':')
            {
                ++count;
            }
        }
        if (count != 2 || numbers != 6 || str.length()!=8)
        {
            throw invalid_argument("Invalid time format");
        }
    }
    void validate_attributes() // проверка корректности времени
    {
        if ((hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60 || seconds < 0 || seconds >= 60) ||
            (hours != static_cast<int>(hours) || minutes != static_cast<int>(minutes) || seconds != static_cast<int>(seconds)))
        {
            throw invalid_argument("Invalid time numbers");
        }
    }

public:
    Time(const string &str)
    {
        validate_string(str);
        stringstream ss(str); // создание потока
        string token;         // строка для хранения отделённого токена
        getline(ss, token, ':');
        hours = stod(token);
        getline(ss, token, ':');
        minutes = stod(token);
        getline(ss, token, ':');
        seconds = stod(token);
        validate_attributes();
    }
    int to_seconds() // метод перевода в секунды
    {
        return hours * 3600 + minutes * 60 + seconds;
    }

    double to_minutes() // метод перевода в минуты
    {
        return hours * 60 + minutes + seconds / 60.0;
    }
};

Time input_time() // функция безопасного ввода
{
    string str;
    while (true)
    {
        try
        {
            getline(cin, str);
            return Time(str); // завершает выполнение функции
        }
        catch (const invalid_argument &e)
        {
            cout << "Error: " << e.what() << endl;
            cout << "Enter the time again (HH:MM:SS): ";
        }
    }
}

int main(int argc, const char **argv)
{
    char answer = 'y';
    while (answer == 'y') // главный цикл программы
    {
        std::cout << "Enter start time (HH:MM:SS): ";
        Time start_time = input_time();
        std::cout << "Enter end time (HH:MM:SS): ";
        Time end_time = input_time();

        cout << "Duration in seconds: " << end_time.to_seconds() - start_time.to_seconds() << " seconds" << endl;
        cout << "Duration in minutes: " << end_time.to_minutes() - start_time.to_minutes() << " minutes" << endl;
        cout << "Do you want to try again? (y/anything): ";
        cin >> answer;
    }
    return 0;
}
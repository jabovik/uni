#include <iostream>
#include <functional>
#include <vector>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;


/// @brief шаблонная функция безопасного ввода
/// @tparam T
/// @param prompt пояснение пользователю
/// @param error_message
/// @param validator функция проверки корректности ввода
/// @return обработанный и безопасный ввод
template <typename T>
T safe_stream_input(const string &prompt, const string &error_message, istream &is, function<bool(T)> validator = nullptr)
{
    T input;
    cout << prompt;
    while (!(is >> input) || (is.peek() != '\n') || (validator && !validator(input)))
    {
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << error_message << endl;
        cout << prompt;
    }
    is.get(); // newline removal
    return input;
}

/// @brief класс содержащий данные о разговорах: количество и длительность в секундах
class ConversationData
{
    int _size;
    int *_durations;

public:
    ConversationData() : _size(0), _durations(nullptr) {};
    ConversationData(int size, int *durations) : _size(size)
    {
        _durations = new int[_size];
        for (int i = 0; i < _size; i++)
        {
            _durations[i] = durations[i];
        }
    }
    ConversationData(const ConversationData &other) : _size(other._size) // копирующий конструктор
    {
        _durations = new int[_size];
        for (int i = 0; i < _size; i++)
        {
            _durations[i] = other._durations[i];
        }
    }
    ConversationData &operator=(const ConversationData &other)
    {
        if (this != &other)
        {
            if (_durations != nullptr)
            {
                delete[] _durations;
            }
            _size = other._size;
            _durations = new int[_size];
            for (int i = 0; i < _size; i++)
            {
                _durations[i] = other._durations[i];
            }
        }
        return *this;
    }
    vector<int> process()
    {
        vector<int> result;
        for (int i = 0; i < _size; i++)
        {
            result.push_back(_durations[i] / 60);
        }
        return result;
    }
    friend ofstream &operator<<(ofstream &os, const ConversationData &cd)
    {
        os << cd._size << ' ';
        for (size_t i = 0; i < cd._size; i++)
        {
            os << cd._durations[i] << ' ';
        }
        return os;
    }
    friend ostream &operator<<(ostream &os, const ConversationData &cd)
    {
        os << cd._size << ' ';
        for (size_t i = 0; i < cd._size; i++)
        {
            os << cd._durations[i] << ' ';
        }
        return os;
    }
    /*
    friend istream &operator>>(istream &is, ConversationData &cd)
    {
        bool run = true;
        while (run)
        {
            run = false;
            string input;
            getline(is >> ws, input); // ws - манипулятор, пропускающий 'пустые' символы: пробелы и перенос строки.
            istringstream iss(input);
            if (!(iss >> cd._size) || cd._size < 0)
            {
                cerr << "Input error: invalid size\n";
                run = true;
            }
            else
            {
                if (cd._durations != nullptr) // можно оптимизировать
                {
                    delete[] cd._durations;
                }
                cd._durations = new int[cd._size];
                for (size_t i = 0; i < cd._size; i++)
                {
                    if ((!(iss >> cd._durations[i]) || cd._durations[i] < 0))
                    {
                        cerr << "Input error: invalid duration\n";
                        run = true;
                        i = cd._size;
                    }
                }
                if (iss >> ws && !iss.eof() && run == false)
                {
                    cerr << "Input error: too much data for duration\n";
                    run = true;
                }
            }
        }
        return is;
    }
        */
    friend istream &operator>>(istream &is, ConversationData &cd)
    {
        cd._size = safe_stream_input<int>("Input size:", "Invalid size", is, [](int s)
                                          { return s >= 0; });
        if (cd._durations != nullptr) // можно оптимизировать
        {
            delete[] cd._durations;
        }
        cd._durations = new int[cd._size];
        for (size_t i = 0; i < cd._size; i++)
        {
            cd._durations[i] = safe_stream_input<int>("Input duration[" + to_string(i) + "]: ", "Invalid duration", is, [](int dur)
                                                      { return dur > 0; });
        }
        return is;
    }
    friend ifstream &operator>>(ifstream &is, ConversationData &cd)
    {
        string input;
        getline(is >> ws, input); // ws - манипулятор, пропускающий 'пустые' символы: пробелы и перенос строки.
        istringstream iss(input);
        if (!(iss >> cd._size) || cd._size < 0)
        {
            is.setstate(ios::failbit);
            return is;
        }
        if (cd._durations != nullptr) // можно оптимизировать
        {
            delete[] cd._durations;
        }
        cd._durations = new int[cd._size];
        for (size_t i = 0; i < cd._size; i++)
        {
            if (!(iss >> cd._durations[i]) || cd._durations[i] < 0)
            {
                is.setstate(ios::failbit);
                return is;
            }
        }
        if (iss >> ws && !iss.eof())
        {
            is.setstate(ios::failbit);
        }
        return is;
    }
    int size() const
    {
        return _size;
    }
    const int *durations() const
    {
        return _durations;
    }
    void add_conversation(int conv)
    {
        if (conv < 0)
        {
            throw invalid_argument("Conversation length must not be negative");
        }
        int *new_durations = new int[_size + 1];
        for (int i = 0; i < _size - 1; i++)
        {
            new_durations[i] = _durations[i];
        }
        new_durations[_size] = conv;
        if (_durations != nullptr)
        {
            delete[] _durations;
        }
        ++_size;
        _durations = new_durations;
    }
    ~ConversationData()
    {
        if (_durations != nullptr)
        {
            delete[] _durations;
        }
    }
};

/// @brief класс, содержащий информацию об абоненте: имя, тариф, данные о разговорах
class Subscriber
{
    string name;
    double tariff;
    ConversationData cd;

public:
    Subscriber() = default;
    Subscriber(const string &name, double tariff, const ConversationData &cd) : name(name), tariff(tariff), cd(cd) {}
    Subscriber(const Subscriber &other) : name(other.name), tariff(other.tariff), cd(other.cd) {} // копирующий конструктор
    Subscriber &operator=(const Subscriber &other)
    {
        if (this != &other)
        {
            name = other.name;
            tariff = other.tariff;
            cd = other.cd;
        }
        return *this;
    }
    double process() // поминутный
    {
        double total_minutes = 0;
        for (auto duration : cd.process())
        {
            total_minutes += duration;
        }
        return total_minutes * tariff;
    }
    double process_seconds() // посекундный
    {
        double total_seconds = 0;
        for (int i = 0; i < cd.size(); i++)
        {
            total_seconds += cd.durations()[i];
        }
        return total_seconds * tariff;
    }
    friend ostream &operator<<(ostream &os, const Subscriber &sub)
    {
        os << sub.name << '\n'
           << sub.tariff << '\n';
        os << sub.cd;
        return os;
    }
    friend ifstream &operator>>(ifstream &is, Subscriber &sub)
    {
        getline(is >> ws, sub.name);
        if (!(is >> sub.tariff) || sub.tariff < 0)
        {
            is.setstate(ios::failbit);
            return is;
        }
        is >> sub.cd;
        return is;
    }
    friend istream &operator>>(istream &is, Subscriber &sub)
    {
        getline(is >> ws, sub.name);
        sub.tariff = safe_stream_input<double>("Input tariff:", "Invalid tariff", is, [](double tar){return tar>=0;});
        is >> sub.cd;
        return is;
    }
    string get_name() const
    {
        return name;
    }
    double get_tariff() const
    {
        return tariff;
    }
    const ConversationData &get_conversation_data() const
    {
        return cd;
    }
    void set_name(const string &new_name)
    {
        name = new_name;
    }
    void set_tariff(double new_tariff)
    {
        if (new_tariff < 0)
        {
            throw invalid_argument("Tariff cannot be negative");
        }
        tariff = new_tariff;
    }
    void set_conv_data(const ConversationData &new_cd)
    {
        cd = new_cd;
    }
};
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
    while (!(cin >> input) || (cin.peek() != '\n') || (validator && !validator(input)))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << error_message << endl;
        cout << prompt;
    }
    cin.get(); // newline removal
    return input;
}

void execute()
{
    cout << "Subscribers\n";
    vector<Subscriber> sub_vec;       // вектор для объектов класса
    vector<double> sub_processed_vec; // вектор результатов обработки
    int amount = 0;
    char choice = safe_input<char>("Console or file input? (c/f):", "(c/f)", [](char c)
                                   { return string("CcFf").find(c) != string::npos; });
    if (choice == 'C' || choice == 'c')
    {
        cout << "Console input mode\n";
        amount = safe_input<int>("Enter amount:", "Invalid input", [](int i)
                                 { return i > 0; });
        for (size_t i = 0; i < amount; i++)
        {
            Subscriber sub;
            bool run = true;
            cout << "Enter subscriber data [" << i << "] name, tariff, conversation data(size followed by durations in seconds):\n";
            while (run)
            {
                cin >> sub;
                if (cin.fail())
                {
                    cin.clear();
                    cout << "Invalid input, try again.\n";
                }
                else
                {
                    run = false;
                }
            }
            sub_vec.push_back(sub);
            sub_processed_vec.push_back(sub.process());
        }
    }
    else
    {
        // const Subscriber corrupt_subscriber("CORRUPT_SUBSCRIBER", 0.0, ConversationData(1, new int[1]{0}));
        cout << "File input mode\n";
        bool path_input_cycle = true;
        string path;
        ifstream input;
        while (path_input_cycle)
        {
            cout << "Input path: ";
            getline(cin, path);
            input.open(path);
            path_input_cycle = !input;
            if (path_input_cycle)
                cerr << "Error opening file\n";
        }
        while (input >> ws && !input.eof())
        {
            Subscriber sub;
            input >> sub;
            if (input.fail())
            {
                cerr << "FILE INPUT ERROR. CORRUPTED ENTRY DISCARDED\n";
                input.clear();
            }
            else
            {
                sub_vec.push_back(sub);
                sub_processed_vec.push_back(sub.process());
            }
        }
        input.close();
    }
    choice = safe_input<char>("Console or file output? (c/f):", "(c/f)", [](char c)
                              { return string("CcFf").find(c) != string::npos; });
    // ostream *out_stream;
    ostream *output;
    ofstream fout;
    if (choice == 'C' || choice == 'c')
    {
        cout << "Console output mode\n";
        output = &cout;
    }
    else
    {
        cout << "File output mode\n";
        bool path_input_cycle = true;
        string path;
        while (path_input_cycle)
        {
            cout << "Input path: ";
            getline(cin >> ws, path);
            fout.open(path);
            path_input_cycle = !fout;
            output = &fout;
        }
    }
    for (size_t i = 0; i < sub_vec.size(); i++)
    {
        *output << "vec element [" << i << "]\n"
                << sub_vec[i] << '\n';
        *output << "Evaluation: " << sub_processed_vec[i] << " money units\n\n";
    }
}
int main(int argc, char const *argv[])
{
    bool run = true;
    while (run)
    {
        execute();
        char choice = safe_input<char>("Run again? (y/n):", "(y/n)", [](char c)
                                       { return string("YyNn").find(c) != string::npos; });
        if (choice == 'N' || choice == 'n')
        {
            run = false;
        }
    }
    return 0;
}
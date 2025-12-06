#include <iostream>
#include <functional>
#include <vector>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

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
    friend ostream &operator<<(ostream &os, const ConversationData &cd)
    {

        for (size_t i = 0; i < cd._size; i++)
        {
            os << '[' << i << "]= " << cd._durations[i] << " seconds\n";
        }
        return os;
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
    friend istream &operator>>(istream &is, ConversationData &cd)
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
        os << "Subscriber: " << sub.name << "\nTariff: " << sub.tariff << " money units\n";
        os << "calls:\n";
        os << sub.cd;
        return os;
    }
    friend ofstream &operator<<(ofstream &os, const Subscriber &sub)
    {
        os << sub.name << '\n'
           << sub.tariff << '\n';
        os << sub.cd << '\n';
        return os;
    }
    friend istream &operator>>(istream &is, Subscriber &sub)
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

bool file_output(string path, vector<Subscriber> &sub_vec, vector<double> sub_processed_vec)
{
    ofstream fout(path);
    if (!fout)
    {
        cerr << "Fail opening file\n";
        return false;
    }
    for (auto element : sub_vec)
    {
        fout << element << "\n";
    }
    for (auto element : sub_processed_vec)
    {
        fout << "Evaluation: " << element << " money units\n";
    }
    return true;
}
bool console_output(const vector<Subscriber> &sub_vec, const vector<double> sub_processed_vec)
{
    for (auto element : sub_vec)
    {
        cout << element << "\n";
    }
    for (auto element : sub_processed_vec)
    {
        cout << "Evaluation: " << element << " money units\n";
    }
    return true;
}

void execute()
{
    cout << "Subscribers\n";
    vector<Subscriber> sub_vec;
    vector<double> sub_processed_vec;
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
    ofstream output;
    if (choice == 'C' || choice == 'c')
    {
        cout << "Console output mode\n";
        console_output(sub_vec, sub_processed_vec);
    }
    else
    {
        cout << "File output mode\n";
        bool path_input_cycle = true;
        string path;
        while (path_input_cycle)
        {
            cout << "Input path: ";
            getline(cin, path);
            path_input_cycle = !file_output(path, sub_vec, sub_processed_vec);
        }
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
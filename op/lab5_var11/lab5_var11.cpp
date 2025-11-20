#include <iostream>
#include <functional>
#include <vector>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
class ConversationData
{
    int size = 0;
    vector<int> durations;

public:
    vector<int> process()
    {
        vector<int> result;
        for (auto d : durations)
        {
            result.push_back(d / 60);
        }
        return result;
    }
    friend ostream &operator<<(ostream &os, const ConversationData &cd)
    {

        for (size_t i = 0; i < cd.size; i++)
        {
            os << '[' << i << "]= " << cd.durations[i] << " seconds\n";
        }
        return os;
    }
    friend istream &operator>>(istream &is, ConversationData &cd)
    {
        string input;
        getline(is >> ws, input);
        istringstream iss(input);
        cd.durations.clear();
        if (!(iss >> cd.size) || cd.size < 0)
        {
            is.setstate(ios::failbit);
            return is;
        }
        cd.durations.resize(cd.size);
        for (size_t i = 0; i < cd.size; i++)
        {
            if (!(iss >> cd.durations[i]) || cd.durations[i] < 0)
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
            cin.get(); // newline removal
            return input;
        }
    }
}

void execute()
{
    cout << "Conversation Data\n";
    vector<ConversationData> cd_vec;
    vector<vector<int>> cd_processed_vec;
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
            ConversationData cd;
            bool run = true;
            cout << "Enter conversation data [" << i << "] (size followed by durations in seconds):\n";
            while (run)
            {
                cin >> cd;
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
            cd_vec.push_back(cd);
            cd_processed_vec.push_back(cd.process());
        }
    }
    else
    {
        cout << "File input mode\n";
        ifstream input("fin");
        if (!input)
        {
            cerr << "Error opening fin\n";
        }
        else
        {
            if (!(input >> amount) || amount < 0)
            {
                cerr << "Error reading amount from file\n";
                input.close();
                return;
            }
            for (size_t i = 0; i < amount; i++)
            {
                ConversationData cd;
                input >> cd;
                if (input.fail())
                {
                    cerr << "File input error\n";
                    input.close();
                    return;
                }
                cd_vec.push_back(cd);
                cd_processed_vec.push_back(cd.process());
            }
            input.close();
        }
    }
    choice = safe_input<char>("Console or file output? (c/f):", "(c/f)", [](char c)
                              { return string("CcFf").find(c) != string::npos; });
    ostream *out_stream;
    ofstream output("fout");
    if (choice == 'C' || choice == 'c')
    {
        cout << "Console output mode\n";
        out_stream = &cout;
    }
    else
    {
        cout << "File output mode\n";
        if (!output)
        {
            cerr << "Error opening fout\n";
            return;
        }
        out_stream = &output;
    }
    for (size_t i = 0; i < cd_vec.size(); i++)
    {
        *out_stream << "vec element [" << i << "]\n";
        *out_stream << cd_vec[i];
        *out_stream << "Processed durations (in minutes):\n";
        for (size_t j = 0; j < cd_processed_vec[i].size(); j++)
        {
            *out_stream << '[' << j << "]= " << cd_processed_vec[i][j] << " minutes\n";
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

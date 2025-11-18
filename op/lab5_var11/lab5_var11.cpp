#include <iostream>
#include <functional>
#include <vector>
#include <fstream>
#include <limits>
#include <regex>
using namespace std;
class ConversationData
{
    size_t size = 0;
    vector<int> durations;

public:
    friend ostream &operator<<(ostream &os, const ConversationData &cd)
    {

        for (size_t i = 0; i < cd.size; i++)
        {
            os << '[' << i << "]= " << cd.durations[i] / 60 << " minutes\n";
        }
        return os;
    }
    friend istream &operator>>(istream &is, ConversationData &cd)
    {
        is >> cd.size;
        cd.durations.resize(cd.size);
        for (size_t i = 0; i < cd.size; i++)
        {
            is >> cd.durations[i];
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

int main(int argc, char const *argv[])
{
    bool run = true;
    while (run)
    {
        cout << "Conversation Data\n";
        vector<ConversationData> cd_vec;
        char choice = safe_input<char>("Console or file input? (c/f):", "(c/f)", [](char c)
                                       { return string("CcFf").find(c) != string::npos; });
        if (choice == 'C' || choice == 'c')
        {
            cout << "Console input mode\n";
            string input_line;
            int amount = safe_input<int>("Enter amount:", "Invalid input", [](int i)
                                         { return i > 0; });
            for (size_t i = 0; i < amount; i++)
            {
                bool flag = true;
                while (flag)
                {
                    cout << "Enter number of conversations followed by their durations in seconds:\n";
                    getline(cin, input_line);
                    stringstream ss_check(input_line);
                    int n;
                    if (!(ss_check >> n) || n < 0)
                    {
                        cout << "Invalid input.\n";
                    }
                    else
                    {
                        regex pattern("^\\s*\\d+(\\s+\\d+){" + to_string(n) + "}\\s*$");
                        if (regex_match(input_line, pattern))
                        {
                            flag = false;
                        }
                        else
                        {
                            cout << "Invalid input.\n";
                        }
                    }
                }
                stringstream ss(input_line);
                ConversationData cd;
                ss >> cd;
                cd_vec.push_back(cd);
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
                int amount;
                input >> amount;
                for (size_t i = 0; i < amount; i++)
                {
                    ConversationData cd;
                    input >> cd;
                    cd_vec.push_back(cd);
                }
                
                input.close();
            }
        }
        choice = safe_input<char>("Console or file output? (c/f):", "(c/f)", [](char c)
                                  { return string("CcFf").find(c) != string::npos; });
        if (choice == 'C' || choice == 'c')
        {
            cout << "Console output mode\n";
            for(size_t i=0; i<cd_vec.size(); i++)
            {
                cout << "vec element [" << i << "]\n";
                cout << cd_vec[i];
            }
        }
        else
        {
            cout << "File output mode\n";
            ofstream output("fout");
            for(size_t i=0; i<cd_vec.size(); ++i)
            {
                output << "vec element [" << i << "]\n";
                output << cd_vec[i];
            }
            output.close();
        }
        choice = safe_input<char>("Run again? (y/n):", "(y/n)", [](char c)
                                  { return string("YyNn").find(c) != string::npos; });
        if (choice == 'N' || choice == 'n')
        {
            run = false;
        }
    }
    return 0;
}

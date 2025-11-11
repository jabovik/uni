#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
using namespace std;

vector<size_t> find_words_w_less_len(const string &str, size_t length)
{
    vector<size_t> indices;
    size_t count = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isspace(str[i]) && !ispunct(str[i]))
            ++count;
        else
        {
            if (count < length)
            {
                indices.push_back(i - count);
            }
            count = 0;
        }
    }
    if (count < length)
    {
        indices.push_back(str.size() - count);
    }
    return indices;
}

void replace_letters(string &str, size_t length, char replacement)
{
    for (auto idx : find_words_w_less_len(str, length))
    {
        str[idx] = replacement;
    }
}

int main(int argc, char const *argv[])
{
    ifstream input("fin");
    ofstream output("fout");
    string str;
    size_t length;
    getline(input, str);
    input >> length;
    replace_letters(str, length, '*');
    output << str;
    input.close();
    output.close();
    return 0;
}
#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
using namespace std;

/// @brief функция поиска индексов начала слов, длина которых меньше заданной
/// @param str строка
/// @param length длина слова
/// @return вектор индексов начала слов
vector<size_t> find_words_w_less_len(const string &str, size_t length)
{
    vector<size_t> indices;
    size_t count = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (isalnum(str[i]))
            ++count;
        else
        {
            if (count < length && count != 0)
            {
                indices.push_back(i - count);
            }
            count = 0;
        }
    }
    if (count < length && count != 0)
    {
        indices.push_back(str.size() - count);
    }
    return indices;
}

void replace_letters(string &str, const vector<size_t> &indices, char replacement)
{
    for (auto idx : indices)
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
    vector<size_t> indices = find_words_w_less_len(str, length);
    replace_letters(str,indices, '*');
    output << str;
    return 0;
}
#include <fstream>
#include <iostream>
#include <vector>

const int MAX_TEXT_LENGTH = 1000;
const int MAX_WORD_LENGTH = 100;

bool is_palindrome(const char *str, int len)
{
    for (int i = 0; i < len / 2; ++i)
    {
        if (str[i] != str[len - 1 - i])
        {
            return false;
        }
    }
    return true;
}
/// @brief checks if char is cyrillic in windows-1251
/// @param c char
/// @return true if cyrillic
bool is_cyrillic(unsigned char c)
{
    return (c >= 192 && c <= 255);
}

bool is_numeric(char c)
{
    return (c >= '0' && c <= '9');
}

bool validate_word(char *word, int len)
{
    if (len > 6)
        return false;
    if (!is_palindrome(word, len))
        return false;
    int i = 0;
    while (word[i])
    {
        if (!is_cyrillic(word[i]))
            return false;
        ++i;
    }
    return true;
}

std::vector<char *> process_text(const char *text)
{
    int i = 0;
    std::vector<char *> list;
    char *word = new char[MAX_WORD_LENGTH];
    int cur = 0;
    while (text[i])
    {
        if (text[i] == ' ' || text[i] == '\t' || text[i] == '\n')
        {
            word[cur] = 0;
            if (validate_word(word, cur))
                list.push_back(word);
            else
                delete[] word;
            word = new char[MAX_WORD_LENGTH];
            cur = 0;
        }
        else
        {
            word[cur] = text[i];
            ++cur;
        }
        ++i;
    }
    // last word processing
    word[cur] = 0;
    if (validate_word(word, cur))
        list.push_back(word);
    else
        delete[] word;
    return list;
}

void delete_list(std::vector<char *> &list)
{
    for (auto &a : list)
    {
        delete[] a;
    }
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    char text[MAX_TEXT_LENGTH];
    input.getline(text, 1000, '\0');
    std::vector<char *> list = process_text(text);
    for (auto &i : list)
    {
        std::cout << i << ' ';
        output << i << ' ';
    }
    delete_list(list);
    return 0;
}

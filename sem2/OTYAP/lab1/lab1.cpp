#include <fstream>
#include <iostream>
#include <vector>

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
bool is_cyrillic(unsigned char c)
{
    return ((c >= 192 && c <= 255) || c == 168 || c == 184); // 192 - 255 ru letters w/o ё Ё
}

bool validate_word(char *word, int len)
{
    if (len > 6)
        return false;
    if (!is_palindrome(word, len))
        return false;
    for(int i = 0;i<len;++i)
    {
        if (!is_cyrillic(word[i]))
            return false;
    }
    return true;
}

std::vector<char *> process_text(const char *text, int size)
{
    int i = 0;
    std::vector<char *> list;
    char *word = new char[size];
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
            word = new char[size];
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
    for (size_t i = 0;i<list.size();++i)
    {
        delete[] list[i];
    }
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    int size = 0;
    char temp;
    while(input.get(temp)) // count size
    {
        ++size;
    }
    ++size;
    input.clear();
    input.seekg(0);
    char* text = new char[size];
    input.getline(text, size, '\0');
    std::vector<char *> list = process_text(text, size);
    for (size_t i=0;i<list.size();++i)
    {
        std::cout << list[i] << ' ';
        output << list[i] << ' ';
    }
    delete[] text;
    delete_list(list);
    return 0;
}
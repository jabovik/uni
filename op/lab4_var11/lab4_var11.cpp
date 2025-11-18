#include <iostream>
#include <limits>
#include <cctype>
#include <fstream>
#include <cstring>
using namespace std;

int find_word_w_len(char *str, int size, int length)
{
    int count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (!isspace(str[i]) && !ispunct(str[i]))
            ++count;
        else
        {
            if (count == length)
            {
                return i - count;
            }
            else
                count = 0;
        }
    }
    if (count == length)
    {
        return size - count;
    }
    return -1;
}

char* delete_word_w_len(char *str, int size, int idx, int length)
{
    if(idx<0)
    {
        throw invalid_argument("idx<0");
    }
    char *cut_str = new char[size - length];
    for (size_t i = 0; i < idx; i++)
    {
        cut_str[i] = str[i];
    }
    for (size_t i = idx + length; i < size; i++)
    {
        cut_str[i - length] = str[i];
    }
    return cut_str;
}

int main(int argc, char const *argv[])
{
    const size_t MAX_LENGTH = 1'000'000;
    ifstream input("fin");
    ofstream output("fout");
    int size, length;
    input >> length;
    input.get();
    char *str = new char[MAX_LENGTH];
    while (input.getline(str, MAX_LENGTH))
    {
        size = strlen(str);
        char *cut_str = new char[size-length];
        int idx = find_word_w_len(str, size, length);
        if (idx >= 0)
        {
            cut_str = delete_word_w_len(str, size, idx, length);
            size = size - (length);
        }
        for (size_t i = 0; i < size; i++)
        {
            output << cut_str[i];
        }
        output << '\n';
        delete[] cut_str;
    }
    delete[] str;
    return 0;
}
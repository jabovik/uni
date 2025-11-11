#include <iostream>
#include <limits>
#include <cctype>
#include <fstream>
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

int delete_word_w_len(char *&str, int size, int length)
{
    int idx = find_word_w_len(str, size, length);
    if (idx == -1)
    {
        return idx;
    }
    char *cut_str = new char[size - length];
    for (size_t i = 0; i < idx; i++)
    {
        cut_str[i] = str[i];
    }
    for (size_t i = idx + length; i < size; i++)
    {
        cut_str[i - length] = str[i+1];
    }
    str = cut_str;
    delete[] cut_str;
    return idx;
}

int main(int argc, char const *argv[])
{
    ifstream input("fin");
    ofstream output("fout");
    int size, length;
    input >> size >> length;
    char *str = new char[size];
    input.get(); // to consume the newline character after length input
    input.getline(str, size + 1);
    delete_word_w_len(str, size, length);
    for (size_t i = 0; i < size; i++)
    {
        output << str[i];
    }
    delete[] str;
    return 0;
}
#include <iostream>
#include <fstream>
#include <cctype>
#include <safe_input.cpp>
using namespace std;

class TextStatistics
{
    size_t char_count;
    size_t whitespace_count;
    size_t word_count;
public:
    TextStatistics() : char_count(0), whitespace_count(0), word_count(0) {}
    TextStatistics(istream &fin) : TextStatistics()
    {
        process(fin);
    }
    void process(istream &fin)
    {
        char ch;
        bool in_word = false;
        while (fin.get(ch))
        {
            char_count++;
            if (isspace(ch))
            {
                whitespace_count++;
                in_word = false;
            }
            else
            {
                if (!in_word)
                {
                    word_count++;
                    in_word = true;
                }
            }
        }
    }
    void print_statistics() 
    {
        cout << "Character count(with whitespaces): " << char_count << endl;
        cout << "Character count(without whitespaces): " << char_count - whitespace_count << endl;
        cout << "Word count: " << word_count << endl;
    }
};
void execute()
{
    cout << "Text statistics\n";
    const size_t MAX_LENGTH = 256;
    char filename[MAX_LENGTH];
    cout << "Input file name: ";
    cin.getline(filename, MAX_LENGTH);
    ifstream fin(filename);
    if (!fin)
    {
        cerr << "Error opening file\n";
        return;
    }
    TextStatistics stats(fin);
    stats.print_statistics();
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
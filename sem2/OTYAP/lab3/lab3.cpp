#include <fstream>
#include <cctype>

char *read_file(std::ifstream &input, size_t &size)
{
    input.seekg(0, std::ios::end);
    size = input.tellg();
    input.seekg(0, std::ios::beg);
    char *buffer = new char[size + 1];
    input.read(buffer, size);
    buffer[size] = '\0';
    return buffer;
}

enum EState
{
    S_INIT, // начальное состояние
    S_ID, // id или keyword
    S_CONST, // константа
    S_OPER, // арифметический оператор
    S_COMP // 
};
enum ESignal
{
    DIGIT,
    ALPHABETIC,
    DELIM,
    UNIDENTIFIED,
};

enum ELexType
{
    ID,
    KW,
    CO,
    EQ,
    AO,
    CONST,
    INVALID,
};

struct Lex
{
    char* str;
    ELexType type;
    Lex* next;
    Lex* prev;
};

ESignal det_signal(char c)
{
    if(isalpha(c)) return ALPHABETIC;
    if(isdigit(c)) return DIGIT;
    if(c == ' ' || c == '\n' || c == '\t') return DELIM;
    return UNIDENTIFIED;
}
/*
Lex* push(Lex* node, char* str, ELexType type)
{
    Lex* new_node = new Lex;
    new_node->str = str;
    new_node->type = type;
    new_node->prev = node;
    new_node->next = node->next;
    new_node->next->prev = new_node->prev;
    node->next = new_node;
}
*/

const int STATES_NUM = 5;
const int SIGNALS_NUM = 4;
const int MAX_LEN = 100;
void create_table(EState (*table)[STATES_NUM])
{
    table[ALPHABETIC][S_INIT] = S_ID;
    table[DIGIT][S_INIT] = S_CONST;
    table[DELIM][S_INIT] = S_INIT;
}

void lexAnalysis(Lex* head, char* text, size_t size)
{
    EState state = S_INIT;
    EState table[SIGNALS_NUM][STATES_NUM];
    ESignal signal;
    create_table(table);
    char lexem[MAX_LEN] = "\0";
    char* cur_lexem = lexem;
    while(*text)
    {
        signal = det_signal(*text);
    }
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    if (!input)
        return -1;
    size_t size = 0;
    char *text = read_file(input, size);
    input.close();
    return 0;
}

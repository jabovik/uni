#include <fstream>
#include <cctype>
#include <cstring>

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

const int STATES_NUM = 8;
const int SIGNALS_NUM = 6;
const int MAX_LEN = 100;

enum EState
{
    S_INIT,   // начальное состояние
    S_ID,     // id или keyword
    S_CONST,  // константа
    S_OPER,   // простые операторы
    S_OPER2,  // сложные операторы
    S_EQ,     // оператор присваивания и сравнения
    S_ERROR,  // состояние ошибки
    S_FINISH, // конечное состояние
};
enum ESignal
{
    DIGIT,
    ALPHABETIC,
    DELIM,
    SIGN,
    SIGN2,
    UNIDENTIFIED,
};

enum ELexType
{
    ID,
    KW,
    CO,
    EQ,
    AO,
    VL,
    INVALID,
};

struct Lex
{
    char *str;
    ELexType type;
    Lex *next;
    Lex *prev;
};

ESignal det_signal(char c)
{
    if (isalpha(c))
        return ALPHABETIC;
    if (isdigit(c))
        return DIGIT;
    if (c == ' ' || c == '\n' || c == '\t' || c == '\0')
        return DELIM;
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return SIGN;
    if (c == '=' || c == '<' || c == '>')
        return SIGN2;
    return UNIDENTIFIED;
}

ELexType state_to_type(EState state)
{
    switch (state)
    {
    case S_ID:
        return ID; // идентификатор может быть ключевым словом, но это можно проверить уже после определения типа лексемы

    case S_CONST:
        return VL;

    case S_OPER:
        return AO;

    case S_OPER2:
        return CO;

    case S_EQ:
        return EQ;

    default:
        return INVALID;
    }
}

const char* lex_type_to_str(ELexType type)
{
    switch (type)
    {
    case ID:
        return "id";
    case KW:
        return "kw";
    case CO:
        return "co";
    case EQ:
        return "eq";
    case AO:
        return "ao";
    case VL:
        return "vl";
    default:
        return "INVALID";
    }
}

// функция push для двусвязного нециклического списка лексем
Lex *push(Lex *node, Lex *new_node)
{
    new_node->next = nullptr;
    new_node->prev = nullptr;
    if (node)
    {
        if (node->next)
        {
            node->next->prev = new_node;
            new_node->next = node->next;
        }
        node->next = new_node;
        new_node->prev = node;
    }
    return new_node;
}

Lex *push(Lex *node, char *str, ELexType type)
{
    Lex *new_node = new Lex;
    new_node->str = new char[strlen(str) + 1];
    strcpy(new_node->str, str);
    new_node->type = type;
    return push(node, new_node);
}

bool is_keyword(const char *str)
{
    const char *keywords[] = {"if", "else", "while", "for", "return", "select", "end"};
    for (const char *kw : keywords)
    {
        if (strcmp(str, kw) == 0)
            return true;
    }
    return false;
}

void delete_list(Lex *head)
{
    while (head)
    {
        Lex *temp = head;
        head = head->next;
        delete[] temp->str; // удаляем строку, выделенную для лексемы
        delete temp;       // удаляем узел списка
    }
}

void print_list(Lex *head, std::ostream &output)
{
    while (head)
    {
        output << head->str << '[' << lex_type_to_str(head->type) << ']' << ' ';
        head = head->next;
    }
}
void create_table(EState (*table)[STATES_NUM])
{
    table[ALPHABETIC][S_INIT] = S_ID;
    table[DIGIT][S_INIT] = S_CONST;
    table[DELIM][S_INIT] = S_INIT;
    table[SIGN][S_INIT] = S_OPER;
    table[SIGN2][S_INIT] = S_OPER2;

    table[ALPHABETIC][S_ID] = S_ID;
    table[DIGIT][S_ID] = S_ID;
    table[DELIM][S_ID] = S_FINISH;
    table[SIGN][S_ID] = S_FINISH;
    table[SIGN2][S_ID] = S_FINISH;

    table[ALPHABETIC][S_CONST] = S_ERROR;
    table[DIGIT][S_CONST] = S_CONST;
    table[DELIM][S_CONST] = S_FINISH;
    table[SIGN][S_CONST] = S_FINISH;
    table[SIGN2][S_CONST] = S_FINISH;

    table[ALPHABETIC][S_OPER] = S_FINISH;
    table[DIGIT][S_OPER] = S_FINISH;
    table[DELIM][S_OPER] = S_FINISH;
    table[SIGN][S_OPER] = S_ERROR;
    table[SIGN2][S_OPER] = S_ERROR; // мб

    table[ALPHABETIC][S_OPER2] = S_FINISH;
    table[DIGIT][S_OPER2] = S_FINISH;
    table[DELIM][S_OPER2] = S_FINISH;
    table[SIGN][S_OPER2] = S_ERROR;
    table[SIGN2][S_OPER2] = S_ERROR;

    table[ALPHABETIC][S_ERROR] = S_ERROR;
    table[DIGIT][S_ERROR] = S_ERROR;
    table[DELIM][S_ERROR] = S_INIT; // ошибка сбрасывается при встрече разделителя
    table[SIGN][S_ERROR] = S_ERROR;
    table[SIGN2][S_ERROR] = S_ERROR;
}
Lex *lexAnalysis(char *text, size_t size)
{
    EState state = S_INIT;
    EState table[SIGNALS_NUM][STATES_NUM];
    ESignal signal;
    ELexType lex_type;
    create_table(table);
    char lexem[MAX_LEN] = "\0";
    char *cur_lexem = lexem;
    Lex *tail = nullptr;
    Lex *head = nullptr;
    bool first = true;
    for(size_t i = 0; i < size+1; ++i)
    {
        signal = det_signal(*text);
        lex_type = state_to_type(state);
        state = table[signal][state];
        if (state == S_FINISH || state == S_ERROR)
        {
            *cur_lexem = '\0'; // завершаем строку
            if (lex_type == ID && is_keyword(lexem))
                lex_type = KW; // если идентификатор совпал с ключевым словом, то меняем его тип на KW
            if (first)
            {
                head = push(head, lexem, lex_type); // добавляем лексему в список
                tail = head;
                first = false;
            }
            else
                tail = push(tail, lexem, lex_type); // добавляем лексему в список
            lexem[0] = '\0'; // очищаем строку для новой лексемы
            cur_lexem = lexem; // начинаем новую лексему
            state = table[signal][S_INIT]; // начинаем работу из начального состояния
        }
        if(state != S_INIT)
            *cur_lexem++ = *text;
        text++;
    }
    return head;
}

        int main(int argc, char const *argv[])
        {
            std::ifstream input("input.txt");
            if (!input)
                return -1;
            size_t size = 0;
            char *text = read_file(input, size);
            input.close();
            Lex *lex_list = lexAnalysis(text, size);
            if(lex_list)
            {
                std::ofstream output("output.txt");
                print_list(lex_list, output);
                output.close();
            }
            delete[] text;
            delete_list(lex_list);
            return 0;
        }

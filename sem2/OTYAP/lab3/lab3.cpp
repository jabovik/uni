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

const int STATES_NUM = 11;
const int MAX_LEN = 100;

enum EState
{
    // состояния:
    S_INIT,   // начальное состояние
    S_OPER2,  // сложные операторы (<=, >=, <>, ==)
    S_ERROR,  // состояние ошибки
    S_FINISH, // конечное состояние

    // состояния-сигналы:
    S_ID,           // id или keyword
    S_CONST,        // константа
    S_OPER,         // простые операторы
    S_OPER_LESS,    // <
    S_OPER_GREATER, // >
    S_EQ,           // =
    S_SC            // ;
};

enum ELexType
{
    ID,
    KW,
    CO,
    EQ,
    AO,
    VL,
    WL,
    SC,
};

struct Lex
{
    char *str;
    ELexType type;
    Lex *next;
    Lex *prev;
};

EState det_signal(char c)
{
    if (isalpha(c))
        return S_ID;
    if (isdigit(c))
        return S_CONST;
    if (c == ' ' || c == '\n' || c == '\t' || c == '\0')
        return S_FINISH;
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return S_OPER;
    if (c == '<')
        return S_OPER_LESS;
    if (c == '>')
        return S_OPER_GREATER;
    if (c == '=')
        return S_EQ;
    if (c == ';')
        return S_SC;
    return S_ERROR;
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
    case S_OPER_GREATER:
    case S_OPER_LESS:
    case S_OPER2:
        return CO;
    case S_EQ:
        return EQ;
    case S_SC:
        return SC;
    default:
        return WL;
    }
}

const char *lex_type_to_str(ELexType type)
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
    case SC:
        return "sc";
    default:
        return "wl";
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
    const char *keywords[] = {"do", "loop", "until", "end", "not", "and", "or"};
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
        delete temp;        // удаляем узел списка
    }
}

void print_list(Lex *head, std::ostream &output)
{
    bool first = true;
    while (head)
    {
        if (!first)
            output << ' ';
        output << head->str << '[' << lex_type_to_str(head->type) << ']';
        first = false;
        head = head->next;
    }
}

void print_list(Lex *head, std::ostream &output, ELexType type)
{
    bool first = true;
    while (head)
    {
        if (head->type == type)
        {
            if (!first)
                output << ' ';
            output << head->str;
            first = false;
        }
        head = head->next;
    }
}
    void create_table(EState(*table)[STATES_NUM])
    // сигнал(состояние), текущее состояние -> следующее состояние
    {
        table[S_ID][S_INIT] = S_ID;
        table[S_CONST][S_INIT] = S_CONST;
        table[S_FINISH][S_INIT] = S_INIT;
        table[S_OPER][S_INIT] = S_OPER;
        table[S_OPER_LESS][S_INIT] = S_OPER_LESS;
        table[S_OPER_GREATER][S_INIT] = S_OPER_GREATER;
        table[S_EQ][S_INIT] = S_EQ;
        table[S_SC][S_INIT] = S_SC;
        table[S_ERROR][S_INIT] = S_ERROR;

        table[S_ID][S_ID] = S_ID;
        table[S_CONST][S_ID] = S_ID;
        table[S_FINISH][S_ID] = S_FINISH;
        table[S_OPER][S_ID] = S_FINISH;
        table[S_OPER_LESS][S_ID] = S_FINISH;
        table[S_OPER_GREATER][S_ID] = S_FINISH;
        table[S_EQ][S_ID] = S_FINISH;
        table[S_SC][S_ID] = S_FINISH;
        table[S_ERROR][S_ID] = S_ERROR;

        table[S_ID][S_CONST] = S_ERROR;
        table[S_CONST][S_CONST] = S_CONST;
        table[S_FINISH][S_CONST] = S_FINISH;
        table[S_OPER][S_CONST] = S_FINISH;
        table[S_OPER_LESS][S_CONST] = S_FINISH;
        table[S_OPER_GREATER][S_CONST] = S_FINISH;
        table[S_EQ][S_CONST] = S_FINISH;
        table[S_SC][S_CONST] = S_FINISH;
        table[S_ERROR][S_CONST] = S_ERROR;

        table[S_ID][S_OPER] = S_FINISH;
        table[S_CONST][S_OPER] = S_FINISH;
        table[S_FINISH][S_OPER] = S_FINISH;
        table[S_OPER][S_OPER] = S_FINISH; // т.к принято, что операторы - разделители, то может стоять сколько угодно операторов подряд.
        table[S_OPER_LESS][S_OPER] = S_FINISH;
        table[S_OPER_GREATER][S_OPER] = S_FINISH;
        table[S_EQ][S_OPER] = S_FINISH;
        table[S_SC][S_OPER] = S_FINISH;
        table[S_ERROR][S_OPER] = S_FINISH;

        table[S_ID][S_OPER_LESS] = S_FINISH;
        table[S_CONST][S_OPER_LESS] = S_FINISH;
        table[S_FINISH][S_OPER_LESS] = S_FINISH;
        table[S_OPER][S_OPER_LESS] = S_FINISH;
        table[S_OPER_LESS][S_OPER_LESS] = S_FINISH;
        table[S_OPER_GREATER][S_OPER_LESS] = S_OPER2; // <> - валидный оператор
        table[S_EQ][S_OPER_LESS] = S_FINISH;
        table[S_SC][S_OPER_LESS] = S_FINISH;
        table[S_ERROR][S_OPER_LESS] = S_FINISH;

        table[S_ID][S_OPER_GREATER] = S_FINISH;
        table[S_CONST][S_OPER_GREATER] = S_FINISH;
        table[S_FINISH][S_OPER_GREATER] = S_FINISH;
        table[S_OPER][S_OPER_GREATER] = S_FINISH;
        table[S_OPER_LESS][S_OPER_GREATER] = S_FINISH;
        table[S_OPER_GREATER][S_OPER_GREATER] = S_FINISH;
        table[S_EQ][S_OPER_GREATER] = S_FINISH;
        table[S_SC][S_OPER_GREATER] = S_FINISH;
        table[S_ERROR][S_OPER_GREATER] = S_FINISH;

        table[S_ID][S_EQ] = S_FINISH;
        table[S_CONST][S_EQ] = S_FINISH;
        table[S_FINISH][S_EQ] = S_FINISH;
        table[S_OPER][S_EQ] = S_FINISH;
        table[S_OPER_LESS][S_EQ] = S_FINISH;
        table[S_OPER_GREATER][S_EQ] = S_FINISH;
        table[S_EQ][S_EQ] = S_FINISH;
        table[S_SC][S_EQ] = S_FINISH;
        table[S_ERROR][S_EQ] = S_FINISH;

        table[S_ID][S_ERROR] = S_ERROR;
        table[S_CONST][S_ERROR] = S_ERROR;
        table[S_FINISH][S_ERROR] = S_FINISH; // разделитель сбрасывает ошибку
        table[S_OPER][S_ERROR] = S_FINISH;
        table[S_OPER_LESS][S_ERROR] = S_FINISH;
        table[S_OPER_GREATER][S_ERROR] = S_FINISH;
        table[S_EQ][S_ERROR] = S_FINISH;
        table[S_SC][S_ERROR] = S_FINISH;
        table[S_ERROR][S_ERROR] = S_ERROR;

        table[S_ID][S_OPER2] = S_FINISH;
        table[S_CONST][S_OPER2] = S_FINISH;
        table[S_FINISH][S_OPER2] = S_FINISH;
        table[S_OPER][S_OPER2] = S_FINISH;
        table[S_OPER_LESS][S_OPER2] = S_FINISH;
        table[S_OPER_GREATER][S_OPER2] = S_FINISH;
        table[S_EQ][S_OPER2] = S_FINISH;
        table[S_SC][S_OPER2] = S_FINISH;
        table[S_ERROR][S_OPER2] = S_FINISH;

        table[S_ID][S_SC] = S_FINISH;
        table[S_CONST][S_SC] = S_FINISH;
        table[S_FINISH][S_SC] = S_FINISH;
        table[S_OPER][S_SC] = S_FINISH;
        table[S_OPER_LESS][S_SC] = S_FINISH;
        table[S_OPER_GREATER][S_SC] = S_FINISH;
        table[S_EQ][S_SC] = S_FINISH;
        table[S_SC][S_SC] = S_FINISH;
        table[S_ERROR][S_SC] = S_FINISH;
    }
    Lex *lexAnalysis(char *text, size_t size)
    {
        EState state = S_INIT;
        EState signal;
        EState table[STATES_NUM][STATES_NUM];
        ELexType lex_type;
        create_table(table);
        char lexem[MAX_LEN] = "\0";
        char *cur_lexem = lexem;
        Lex *tail = nullptr;
        Lex *head = nullptr;
        bool first = true;
        for (size_t i = 0; i < size + 1; ++i)
        {
            signal = det_signal(*text);
            lex_type = state_to_type(state);
            state = table[signal][state];
            if (state == S_FINISH)
            {
                *cur_lexem = '\0'; // завершаем строку
                if (lex_type == ID){
                    if(is_keyword(lexem))
                        lex_type = KW;       // если идентификатор совпал с ключевым словом, то меняем его тип на KW
                    else if(strlen(lexem) > 5) // проверка на валидность идентификатора (не более 5 символов)
                        lex_type = WL;
                }
                else if (lex_type == VL) // проверка на валидность константы (от -32 768 до 32 767)
                {
                    if (atoi(lexem) > 32768) // более точная проверка - прерогатива синтаксического анализатора
                        lex_type = WL;
                }
                if (first)
                {
                    head = push(head, lexem, lex_type); // добавляем лексему в список
                    tail = head;
                    first = false;
                }
                else
                    tail = push(tail, lexem, lex_type); // добавляем лексему в список
                lexem[0] = '\0';                        // очищаем строку для новой лексемы
                cur_lexem = lexem;                      // начинаем новую лексему
                state = table[signal][S_INIT];          // начинаем работу из начального состояния
            }
            if (state != S_INIT)
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
        if (lex_list)
        {
            std::ofstream output("output.txt");
            print_list(lex_list, output);
            output << std::endl;
            print_list(lex_list, output, ID);
            output << std::endl;
            print_list(lex_list, output, VL);
            output.close();
        }
        delete[] text;
        delete_list(lex_list);
        return 0;
    }

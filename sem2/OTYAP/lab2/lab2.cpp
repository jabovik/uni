/*
Вариант 25
Словом текста является последовательность букв русского алфавита длиной не более 6 символов.
Найти те слова, в которых буквы алфавита образуют симметричную последовательность букв (палиндром).

Сначала рассмотрим грамматику ровно из 6 символов.
Автоматная грамматика для 6-и символьных палиндромов должна описывать
все возможные палиндромы из русских букв (строчных и заглавных). Алфавит 66 букв.
Посчитаем количество состояний в таком автомате.
1 начальное состояние.
1 ошибочное состояние.
Первый символ может быть любым из 66 символов. Это 66 состояний.
Второй символ может быть любым из 66 символов. В сумме это 66^2 состояний
Третий символ может быть любым из 66 символов. В сумме это 66^3 состояний
Четвёртый допустимый символ только 1, остальные 65 - ошибочное состояние. В сумме это 66^2 состояний
Пятый допустимый символ только 1, остальные 65 - ошибочное состояние. В сумме это 66 состояний
Шестой допустимый символ только 1, остальные 65 - ошибочное состояние. В сумме это 1 состояние, которое является завершающим.
Итого: 1+1+66+66^2+66^3+66^2+66+1=296343 состояний.
Для грамматики от 1 до 6 символов, количество состояний будет примерно в 2 раза больше
Намного эффективнее будет использовать МП-автомат
*/

#include <fstream>
#include <vector>

char *read_file(std::ifstream &input, int &size)
{
    input.seekg(0, std::ios::end);
    size = input.tellg();
    input.seekg(0, std::ios::beg);
    char *buffer = new char[size + 1];
    input.read(buffer, size);
    buffer[size] = '\0';
    return buffer;
}

void print_vector(std::vector<char *> &vec, std::ofstream &output, int size)
{
    for (int i = 0; i < size; ++i)
    {
        output << vec[i];
        if (i != size - 1)
            output << ' ';
    }
}

bool is_cyrillic(unsigned char c)
{
    return ((c >= 192 && c <= 255) || c == 168 || c == 184); // 192 - 255 ru letters w/o ё Ё
}

bool is_delim(unsigned char c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\0';
}
const int STACK_SIZE = 6;
const int SIGNALS_NUM = 3;
const int STATES_NUM = 10; // временно
const int STACK_STATES_NUM = 3;
const int WORD_MAX_SIZE = 6;

enum Signals
{
    ALPHABETIC,
    DELIM,
    UNIDENTIFIED
};
enum States
{
    S_BEGIN,
    S_OOR,
    S_POTENTIAL_FAIL,
    S_FAIL,
    S_PUSH,
    S_POP,
    S_GOOD,
    S_REPUSH,
    S_DOUBLE_PUSH,
    S_DOUBLE_PUSH2
};
// Заключительные состояния:
// S_OOR - слово больше чем автомат должен обрабатывать
// S_FAIL - слово не палиндром
enum StackStates
{
    MATCH,
    DIFF,
    EMPTY
};
enum Result
{
    GOOD,
    FAIL,
    CONTINUE
};

StackStates compare(char c, char stack_top)
{
    if (!stack_top)
        return EMPTY;
    if (stack_top == c)
        return MATCH;
    return DIFF;
}

Signals det_signal(char c)
{
    if (is_cyrillic(c))
        return ALPHABETIC;
    if (is_delim(c))
        return DELIM;
    return DELIM; // для всех остальных символов считаем, что это разделитель
}

void table_unidentified(States (*table)[STATES_NUM][STACK_STATES_NUM])
{
    for (int i = 0; i < STATES_NUM; ++i)
    {
        for (int j = 0; j < STACK_STATES_NUM; ++j)
        {
            table[UNIDENTIFIED][i][j] = S_FAIL;
        }
    }
}
void create_table(States (*table)[STATES_NUM][STACK_STATES_NUM], int size)
{
    switch (size)
    {
    case 2:
        table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH;                   // начало, слова, пишем в стек
        table[ALPHABETIC][S_PUSH][MATCH] = S_POP;                     // совпадение с 2 символом
        table[DELIM][S_PUSH][DIFF] = S_GOOD;                          // если после 1 символа стоит разделитель, то слово подходит
        table[ALPHABETIC][S_PUSH][DIFF] = S_POTENTIAL_FAIL;           // несовпадение с 2 символом
        table[DELIM][S_POP][EMPTY] = S_GOOD;                          // если после 2 символа стоит разделитель, то слово подходит
        table[ALPHABETIC][S_POP][EMPTY] = S_OOR;                      // если после 2 символа стоит буква, то слово не подходит
        table[ALPHABETIC][S_POTENTIAL_FAIL][DIFF] = S_POTENTIAL_FAIL; // пропускаем до конца
        table[ALPHABETIC][S_POTENTIAL_FAIL][MATCH] = S_POTENTIAL_FAIL;
        table[DELIM][S_POTENTIAL_FAIL][EMPTY] = S_FAIL;     // слово нужного размера, но не палиндром
        table[ALPHABETIC][S_POTENTIAL_FAIL][EMPTY] = S_OOR; // слово больше нужного размера
        break;
    case 3:
        table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH;  // начало, слова, пишем в стек
        table[ALPHABETIC][S_PUSH][MATCH] = S_REPUSH; // середина не важна
        table[ALPHABETIC][S_PUSH][DIFF] = S_REPUSH;
        table[ALPHABETIC][S_REPUSH][MATCH] = S_POP;                   // совпадение с 3 символом.
        table[ALPHABETIC][S_REPUSH][DIFF] = S_POTENTIAL_FAIL;         // несовпадение с 3 символом
        table[DELIM][S_POP][EMPTY] = S_GOOD;                          // если после 3 символа стоит разделитель, то слово подходит
        table[ALPHABETIC][S_POP][EMPTY] = S_OOR;                      // если после 3 символа стоит буква, то слово не подходит
        table[ALPHABETIC][S_POTENTIAL_FAIL][DIFF] = S_POTENTIAL_FAIL; // пропускаем до конца
        table[ALPHABETIC][S_POTENTIAL_FAIL][MATCH] = S_POTENTIAL_FAIL;
        table[DELIM][S_POTENTIAL_FAIL][EMPTY] = S_FAIL;     // слово нужного размера, но не палиндром
        table[ALPHABETIC][S_POTENTIAL_FAIL][EMPTY] = S_OOR; // слово больше нужного размера
        break;
    case 4:
        table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH;       // начало, слова, пишем в стек
        table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH; // 2 символ
        table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH;
        table[ALPHABETIC][S_DOUBLE_PUSH][MATCH] = S_POP;              // совпадение с 3 символом.
        table[ALPHABETIC][S_DOUBLE_PUSH][DIFF] = S_POTENTIAL_FAIL;    // несовпадение с 3 символом
        table[ALPHABETIC][S_POP][MATCH] = S_POP;                      // совпадение с 4 символом.
        table[ALPHABETIC][S_POP][DIFF] = S_POTENTIAL_FAIL;            // несовпадение с 4 символом
        table[DELIM][S_POP][EMPTY] = S_GOOD;                          // если после 4 символа стоит разделитель, то слово подходит
        table[ALPHABETIC][S_POP][EMPTY] = S_OOR;                      // если после 4 символа стоит буква, то слово не подходит
        table[ALPHABETIC][S_POTENTIAL_FAIL][DIFF] = S_POTENTIAL_FAIL; // пропускаем до конца
        table[ALPHABETIC][S_POTENTIAL_FAIL][MATCH] = S_POTENTIAL_FAIL;
        table[DELIM][S_POTENTIAL_FAIL][EMPTY] = S_FAIL;     // слово нужного размера, но не палиндром
        table[ALPHABETIC][S_POTENTIAL_FAIL][EMPTY] = S_OOR; // слово больше нужного размера
        break;
    case 5:
        table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH;       // начало, слова, пишем в стек
        table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH; // 2 символ
        table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH;
        table[ALPHABETIC][S_DOUBLE_PUSH][MATCH] = S_REPUSH; // середина не важна
        table[ALPHABETIC][S_DOUBLE_PUSH][DIFF] = S_REPUSH;
        table[ALPHABETIC][S_REPUSH][MATCH] = S_POP;                   // совпадение с 4 символом.
        table[ALPHABETIC][S_REPUSH][DIFF] = S_POTENTIAL_FAIL;         // несовпадение с 4 символом
        table[ALPHABETIC][S_POP][MATCH] = S_POP;                      // совпадение с 5 символом.
        table[ALPHABETIC][S_POP][DIFF] = S_POTENTIAL_FAIL;            // несовпадение с 5 символом
        table[DELIM][S_POP][EMPTY] = S_GOOD;                          // если после 5 символа стоит разделитель, то слово подходит
        table[ALPHABETIC][S_POP][EMPTY] = S_OOR;                      // если после 5 символа стоит буква, то слово не подходит
        table[ALPHABETIC][S_POTENTIAL_FAIL][DIFF] = S_POTENTIAL_FAIL; // пропускаем до конца
        table[ALPHABETIC][S_POTENTIAL_FAIL][MATCH] = S_POTENTIAL_FAIL;
        table[DELIM][S_POTENTIAL_FAIL][EMPTY] = S_FAIL;     // слово нужного размера, но не палиндром
        table[ALPHABETIC][S_POTENTIAL_FAIL][EMPTY] = S_OOR; // слово больше нужного размера
        break;
    case 6:
        table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH;       // начало слова, пишем в стек
        table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH; // 2 символ
        table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH;
        table[ALPHABETIC][S_DOUBLE_PUSH][MATCH] = S_DOUBLE_PUSH2; // 3 символ
        table[ALPHABETIC][S_DOUBLE_PUSH][DIFF] = S_DOUBLE_PUSH2;
        table[ALPHABETIC][S_DOUBLE_PUSH2][DIFF] = S_POTENTIAL_FAIL;   // несовпадение с 4 символом
        table[ALPHABETIC][S_DOUBLE_PUSH2][MATCH] = S_POP;             // совпадение с 4 символом.
        table[ALPHABETIC][S_POP][MATCH] = S_POP;                      // совпадение с 5-6 символом.
        table[ALPHABETIC][S_POP][DIFF] = S_POTENTIAL_FAIL;            // несовпадение с 5-6 символом
        table[DELIM][S_POP][EMPTY] = S_GOOD;                          // если после 6 символа стоит разделитель, то слово подходит
        table[ALPHABETIC][S_POP][EMPTY] = S_OOR;                      // если после 6 символа стоит буква, то слово не подходит
        table[ALPHABETIC][S_POTENTIAL_FAIL][DIFF] = S_POTENTIAL_FAIL; // пропускаем до конца
        table[ALPHABETIC][S_POTENTIAL_FAIL][MATCH] = S_POTENTIAL_FAIL;
        table[DELIM][S_POTENTIAL_FAIL][EMPTY] = S_FAIL;     // слово нужного размера, но не палиндром
        table[ALPHABETIC][S_POTENTIAL_FAIL][EMPTY] = S_OOR; // слово больше нужного размера
        break;
    default:
        throw("unexpected size");
        break;
    }
    table_unidentified(table);
}
Result PDA_process_word(States (*table)[STATES_NUM][STACK_STATES_NUM], char *&word)
{
    States state = S_BEGIN;
    Signals signal;
    StackStates stack_state;
    int stack[STACK_SIZE + 1];
    int stack_top = 0;
    stack[stack_top] = 0;   // перед стеком всегда 0, чтобы не проверять на выход за границы
    stack[++stack_top] = 0; // начальное состояние стека
    while (state != S_FAIL && state != S_GOOD && state != S_OOR)
    {
        signal = det_signal(*word);
        stack_state = compare(*word, stack[stack_top]);
        state = table[signal][state][stack_state];
        switch (state)
        {
        case S_PUSH:
            stack[stack_top] = *word;
            break;
        case S_REPUSH:
            break;
        case S_DOUBLE_PUSH:
        case S_DOUBLE_PUSH2:
            stack[++stack_top] = *word;
            break;
        case S_POP:
            stack[stack_top--] = 0;
            break;
        default:
            stack[stack_top--] = 0;
        }
        ++word;
    }
    --word; // возвращаем указатель на последний символ слова
    switch (state)
    {
    case S_GOOD:
        return GOOD;
    case S_FAIL:
        return FAIL;
    case S_OOR:
        return CONTINUE;
    default:
        throw("unexpected state");
    }
}

char *copy_word(char *begin, char *end)
{
    int len = end - begin;
    char *word = new char[len + 1];
    for (int i = 0; i < len; ++i)
    {
        word[i] = begin[i];
    }
    word[len] = '\0';
    return word;
}

std::vector<char *> compile(char *text)
{
    std::vector<char *> vec;
    States table[SIGNALS_NUM][STATES_NUM][STACK_STATES_NUM];
    char *word;
    short table_select;
    Result res;
    while (*text)
    {
        while (is_delim(*text) && *text != '\0')
            ++text;
        if (!*text)
            return vec;
        table_select = 1;
        do
        {
            word = text;
            ++table_select;
            create_table(table, table_select);
            res = PDA_process_word(table, word);
        } while (res == CONTINUE && table_select < WORD_MAX_SIZE - 2);
        if (res == GOOD)
        {
            vec.push_back(copy_word(text, word));
        }
        text = word;
        while (!is_delim(*text) && *text != '\0')
            ++text;
    }
    return vec;
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    if (!input)
        return -1;
    int size = 0;
    char *text = read_file(input, size);
    input.close();
    std::vector<char *> vec = compile(text);
    delete[] text;
    if (vec.empty())
        return 0;
    std::ofstream output("output.txt");
    if (output)
    {
        print_vector(vec, output, vec.size());
        for (int i = 0; i < vec.size(); ++i)
            delete[] vec[i];
    }
    else
    {
        for (int i = 0; i < vec.size(); ++i)
            delete[] vec[i];
        return -1;
    }
    return 0;
}
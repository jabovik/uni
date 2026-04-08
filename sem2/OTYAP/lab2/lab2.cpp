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

char* read_file(std::ifstream &input, int &size) {
    input.seekg(0, std::ios::end);
    size = input.tellg();
    input.seekg(0, std::ios::beg);
    char* buffer = new char[size + 1];
    input.read(buffer, size);
    buffer[size] = '\0';
    return buffer;
}

bool is_cyrillic(unsigned char c)
{
    return ((c >= 192 && c <= 255) || c == 168 || c == 184); // 192 - 255 ru letters w/o ё Ё
}

bool is_delim(unsigned char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\0';
}
const int STACK_SIZE = 6;
const int SIGNALS_NUM = 2;
const int STATES_NUM = 9; // временно
const int STACK_STATES_NUM = 3;
const int WORD_MAX_SIZE = 6;

enum Signals{ALPHABETIC, DELIM};
enum States {S_BEGIN, S_END, S_OOR, S_FAIL, S_PUSH, S_POP, S_GOOD, S_REPUSH, S_DOUBLE_PUSH, S_DOUBLE_PUSH2};
enum StackStates {MATCH, DIFF, EMPTY};
enum Result {GOOD, FAIL, CONTINUE};

StackStates compare(char c, char stack_top)
{
    if (stack_top == c) return MATCH;
    if (!stack_top) return EMPTY;
    return DIFF;
}

Signals det_signal(char c)
{
    if (is_cyrillic(c)) return ALPHABETIC;
    if (is_delim(c)) return DELIM;
    return DELIM; // для всех остальных символов считаем, что это разделитель
}
void create_table2(States (*table)[STATES_NUM][STACK_STATES_NUM]) // 1 или 2 символа
{
    table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH; // начало, слова, пишем в стек
    table[ALPHABETIC][S_PUSH][MATCH] = S_POP; // совпадение с 1 символом
    table[DELIM][S_PUSH][DIFF] = S_GOOD; // если после 1 символа стоит разделитель, то слово подходит
    table[ALPHABETIC][S_PUSH][DIFF] = S_FAIL; // несовпадение с 1 символом
    table[DELIM][S_POP][EMPTY] = S_GOOD; // если после 2 символа стоит разделитель, то слово подходит
    table[ALPHABETIC][S_POP][EMPTY] = S_FAIL; // если после 2 символа стоит буква, то слово не подходит
}
void create_table3(States (*table)[STATES_NUM][STACK_STATES_NUM])
{
    table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH; // начало, слова, пишем в стек
    table[ALPHABETIC][S_PUSH][MATCH] = S_REPUSH; // середина не важна
    table[ALPHABETIC][S_PUSH][DIFF] = S_REPUSH;
    table[ALPHABETIC][S_REPUSH][MATCH] = S_POP; // совпадение с 3 символом.
    table[ALPHABETIC][S_REPUSH][DIFF] = S_FAIL; // несовпадение с 3 символом
    table[DELIM][S_POP][EMPTY] = S_GOOD; // если после 3 символа стоит разделитель, то слово подходит
    table[ALPHABETIC][S_POP][EMPTY] = S_FAIL; // если после 3 символа стоит буква, то слово не подходит
}
void create_table4(States (*table)[STATES_NUM][STACK_STATES_NUM])
{
    table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH; // начало, слова, пишем в стек
    table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH; // 2 символ
    table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH;
    table[ALPHABETIC][S_DOUBLE_PUSH][MATCH] = S_POP; // совпадение с 3 символом.
    table[ALPHABETIC][S_DOUBLE_PUSH][DIFF] = S_FAIL; // несовпадение с 3 символом
    table[ALPHABETIC][S_POP][MATCH] = S_POP; // совпадение с 4 символом.
    table[ALPHABETIC][S_POP][DIFF] = S_FAIL; // несовпадение с 4 символом
    table[DELIM][S_POP][EMPTY] = S_GOOD; // если после 4 символа стоит разделитель, то слово подходит
    table[ALPHABETIC][S_POP][EMPTY] = S_FAIL; // если после 4 символа стоит буква, то слово не подходит
}
void create_table5(States (*table)[STATES_NUM][STACK_STATES_NUM])
{
    table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH; // начало, слова, пишем в стек
    table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH; // 2 символ
    table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH;
    table[ALPHABETIC][S_DOUBLE_PUSH][MATCH] = S_REPUSH; // середина не важна
    table[ALPHABETIC][S_DOUBLE_PUSH][DIFF] = S_REPUSH;
    table[ALPHABETIC][S_REPUSH][MATCH] = S_POP; // совпадение с 4 символом.
    table[ALPHABETIC][S_REPUSH][DIFF] = S_FAIL; // несовпадение с 4 символом
    table[ALPHABETIC][S_POP][MATCH] = S_POP; // совпадение с 5 символом.
    table[ALPHABETIC][S_POP][DIFF] = S_POP; // несовпадение с 5 символом
    table[DELIM][S_POP][EMPTY] = S_GOOD; // если после 5 символа стоит разделитель, то слово подходит
    table[ALPHABETIC][S_POP][EMPTY] = S_FAIL; // если после 5 символа стоит буква, то слово не подходит
}
void create_table6(States (*table)[STATES_NUM][STACK_STATES_NUM])
{
    table[ALPHABETIC][S_BEGIN][EMPTY] = S_PUSH; // начало, слова, пишем в стек
    table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH; // 2 символ
    table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH;
    table[ALPHABETIC][S_PUSH][MATCH] = S_DOUBLE_PUSH2; // 3 символ
    table[ALPHABETIC][S_PUSH][DIFF] = S_DOUBLE_PUSH2;
    table[ALPHABETIC][S_DOUBLE_PUSH2][DIFF] = S_FAIL;
    table[ALPHABETIC][S_DOUBLE_PUSH2][MATCH] = S_POP; // совпадение с 4 символом.
    table[ALPHABETIC][S_POP][MATCH] = S_POP; // совпадение с 5 символом.
    table[ALPHABETIC][S_POP][DIFF] = S_FAIL; // несовпадение с 5 символом
    table[DELIM][S_POP][EMPTY] = S_GOOD; // если после 6 символа стоит разделитель, то слово подходит
    table[ALPHABETIC][S_POP][EMPTY] = S_FAIL; // если после 6 символа стоит буква, то слово не подходит
}
Result PDA_process_word(States (*table)[STATES_NUM][STACK_STATES_NUM], char* word)
{
    States state = S_BEGIN;
    Signals signal;
    StackStates stack_state;
    int stack[STACK_SIZE];
    int stack_top = 0;
    stack[stack_top] = 0; // начальное состояние стека
    while(state != S_FAIL && state != S_GOOD)
    {
        signal = det_signal(*word);
        stack_state = compare(*word, stack[stack_top]);
        state = table[signal][state][stack_state];
        switch(state)
        {
            case S_PUSH: 
                stack[stack_top++] = *word;
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
    switch(state)
    {
        case S_GOOD: return GOOD;
        case S_FAIL: return FAIL;
        case S_END: return CONTINUE;
        default: throw("unexpected state");
    }
}

char* copy_word(char* begin, char* end)
{
    int len = end - begin;
    char* word = new char[len + 1];
    for(int i = 0;i<len;++i)
    {
        word[i] = begin[i];
    }
    word[len] = '\0';
    return word;
}

std::vector<char*> compile(char* text)
{
    std::vector<char*> vec;
    States table[SIGNALS_NUM][STATES_NUM][STACK_STATES_NUM];
    create_table2(table);
    char* word_begin;
    while(*text)
    {
        while(is_delim(*text)) ++text;
        word_begin = text;
        
    }
    return vec;
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    if(!input) return -1;
    int size = 0;
    char* text = read_file(input, size);
    input.close();
    return 0;
}

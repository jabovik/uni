// Односвязный циклический список.
// Быстрая сортировка и сортировка пузырьком.
#include <fstream>

struct List
{
    int val;    // значение
    List *next; // указатель на следующий элемент
};

List *push(List *p, int val)
{
    List *new_node = new List;
    new_node->val = val;
    if (p == nullptr)
    {                              // если список пустой
        new_node->next = new_node; // новый элемент указывает на себя
        return new_node;           // возвращаем новый элемент как начало списка
    }
    new_node->next = p->next;
    p->next = new_node;
    return p;
}

void delete_list(List *p)
{
    if (p == nullptr)
        return; // если список пустой, ничего не удаляем
    List *current = p->next;
    while (current != p)
    {
        List *temp = current;
        current = current->next;
        delete temp;
    }
    delete p;
}

void print_list(List *p, std::ostream &output)
{
    if (p == nullptr)
        return; // если список пустой, ничего не выводим
    List *current = p;
    do
    {
        output << current->val << " ";
        current = current->next;
    } while (current != p);
}

List *read_SLCL(std::istream &input, int &size)
{
    List *list = nullptr;
    int val;
    size = 0;
    while (input >> val)
    {
        list = push(list, val);
        list = list->next;
        size++;
    }
    return list; // возвращаем указатель на последний добавленный элемент, который является конечным элементом списка
}

void swap_nodes(List *prev1, List *prev2)
{
    if (prev1 == prev2)
        return;

    List *a = prev1->next; // первый узел
    List *b = prev2->next; // второй узел

    if (a == b)
        return; // один и тот же узел

    // СЛУЧАЙ 1: a перед b (соседи)
    if (a->next == b)
    {
        prev1->next = b;
        a->next = b->next;
        b->next = a;
        return;
    }

    // СЛУЧАЙ 2: b перед a (соседи)
    if (b->next == a)
    {
        prev2->next = a;
        b->next = a->next;
        a->next = b;
        return;
    }

    // СЛУЧАЙ 3: не соседи
    List *temp = a->next;
    a->next = b->next;
    b->next = temp;

    prev1->next = b;
    prev2->next = a;
}
/*
List *list_quicksort(List *list);

List *list_bubblesort(List *begin, List *end)
{
    List* new_head; // новый указатель будет указывать на минимальный элемент списка (начальный)
    bool swapped;
    List *prev = end;
    List *current = begin;
    new_head = begin;
}
    */
int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    int n = 0;
    input >> n; // ключ для сортировки (0 - быстрая сортировка, 1 - сортировка пузырьком)
    int size = 0;
    List *end = read_SLCL(input, size);
    List *begin = end->next;
    input.close();
    if (begin)
    {
        std::ofstream output("output.txt");
        print_list(begin, output);
        output << std::endl;
        swap_nodes(end, end->next);
        print_list(begin, output);
        output.close();
    }
    // delete_list(end);
    return 0;
}

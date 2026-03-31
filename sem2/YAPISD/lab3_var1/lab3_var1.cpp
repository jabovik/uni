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
    {
        new_node->next = new_node;
        return new_node;
    }
    new_node->next = p->next;
    p->next = new_node;
    return p;
}

void delete_list(List *head)
{
    if (head == nullptr)
        return; // если список пустой, ничего не удаляем
    List *current = head->next->next;
    while (current != head->next)
    {
        List *temp = current;
        current = current->next;
        delete temp;
    }
    delete current;
    delete head;
}

void print_list(List *head, std::ostream &output)
{
    if (head == nullptr)
        return; // если список пустой, ничего не выводим
    List *current = head->next;
    do
    {
        output << current->val << " ";
        current = current->next;
    } while (current != head->next);
}
/// @brief Чтение и создание односвязного циклического списка из входного потока.
/// Возвращает голову списка, на которую ничего не указывает
/// @param input
/// @param size
/// @return
List *read_SLCL(std::istream &input, int &size)
{
    List *head = new List; // создаем фиктивный узел, который будет служить началом списка
    head->val = 0;         // пустая голова
    List *list = nullptr;
    int val;
    size = 0;
    while (input >> val)
    {
        list = push(list, val);
        list = list->next;
        size++;
    }
    head->next = list->next;
    return head;
}

List *get_end(List *head)
{
    List *current = head->next;
    while (current->next != head->next) // пока не достигнем последнего элемента (который указывает на head)
    {
        current = current->next;
    }
    return current; // возвращаем указатель на последний элемент
}

void swap_nodes(List *prev1, List *prev2, List *head)
{
    if (prev1 == prev2)
        return;

    List *a = prev1->next; // первый узел
    List *b = prev2->next; // второй узел

    if (a == b)
        return; // один и тот же узел

    if (head->next == a)
        head->next = b;
    else if (head->next == b)
        head->next = a;

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
    List *head = read_SLCL(input, size);
    List *end = get_end(head);
    input.close();
    if (head->next)
    {
        std::ofstream output("output.txt");
        print_list(head, output);
        output << std::endl;
        swap_nodes(end, end->next, head);
        print_list(head, output);
        output.close();
    }
    delete_list(head);
    return 0;
}

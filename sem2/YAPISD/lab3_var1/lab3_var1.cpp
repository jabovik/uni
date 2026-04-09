// Односвязный циклический список.
// Быстрая сортировка и сортировка пузырьком.
#include <fstream>
#include <iostream>

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

List *push(List *p, List *node)
{
    if (p == nullptr)
    {
        node->next = node;
        return node;
    }
    node->next = p->next;
    p->next = node;
    return p;
}

void delete_list(List *head)
{
    if (head == nullptr)
        return; // если список пустой, ничего не удаляем
    List *current = head->next;
    while (current != head)
    {
        List *temp = current;
        current = current->next;
        delete temp;
    }
    delete current;
}

void print_list(List *head, std::ostream &output)
{
    if (head == nullptr)
        return; // если список пустой, ничего не выводим
    List *current = head;
    do
    {
        output << current->val << " ";
        current = current->next;
    } while (current != head);
}
/// @brief чтение и создание односвязного циклического списка из входного потока.
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
    List *head = list->next;
    return head;
}

List *get_end(List *head)
{
    List *current = head;
    while (current->next != head) // пока не достигнем последнего элемента (который указывает на head)
    {
        current = current->next;
    }
    return current; // возвращаем указатель на последний элемент
}

void swap_nodes(List *prev1, List *prev2, List *&head)
{
    if (prev1 == prev2)
        return;

    List *a = prev1->next; // первый узел
    List *b = prev2->next; // второй узел

    if (a == b)
        return; // один и тот же узел

    if (head == a)
        head = b;
    else if (head == b)
        head = a;
    // необходимо учесть все случаи, т.к если все узлы будем
    // рассматривать как случай 3, то список может сломатся, один из узлов может указывать на себя
    // СЛУЧАИ 2 И 3 НЕ ПРИМЕНЯЮТСЯ В ПРОГРАММЕ
    // случай 1: a перед b (соседи)
    if (a->next == b)
    {
        prev1->next = b;
        a->next = b->next;
        b->next = a;
        return;
    }

    // случай 2: b перед a (соседи)
    if (b->next == a)
    {
        prev2->next = a;
        b->next = a->next;
        a->next = b;
        return;
    }

    // случай 3: не соседи
    List *temp = a->next;
    a->next = b->next;
    b->next = temp;

    prev1->next = b;
    prev2->next = a;
}

void list_quicksort(List *&head)
{
    if (head == nullptr || head->next == head)
        return;         // если список пустой или содержит один элемент, сортировка не требуется
    List *pivot = head; // выбираем первый элемент в качестве опорного
    List *less_tail = nullptr;
    List *greater_tail = nullptr;
    List *current = head->next;
    while (current != head)
    {
        List *next = current->next;
        if (current->val < pivot->val)
        {
            push(less_tail, current);
            less_tail = current;
        }
        else
        {
            push(greater_tail, current);
            greater_tail = current;
        }
        current = next;
    }
    List *less_head = less_tail ? less_tail->next : nullptr;
    List *greater_head = greater_tail ? greater_tail->next : nullptr;
    list_quicksort(less_head);
    list_quicksort(greater_head);
    // объединяем отсортированные части
    less_tail = less_head ? get_end(less_head) : nullptr;
    greater_tail = greater_head ? get_end(greater_head) : nullptr;
    if (less_tail) // если есть элементы меньше опорного, то они должны идти перед ним
    {
        less_tail->next = pivot; // опорный элемент идет после всех меньших
        pivot->next = less_head; // замыкаем список меньших элементов
        head = less_head;        // новый head - это голова списка меньших элементов. При этом указатель pivot остаётся там же.
    }
    if (greater_tail) // если есть элементы больше или равные опорному, то они должны идти после него
    {
        greater_tail->next = head; // замыкаем список больших элементов
        pivot->next = greater_head; // опорный элемент идет перед всеми большими
    }
}

void list_bubble_sort(List *&head, int size)
{
    // сортируем относительно головы
    List *end = get_end(head);
    if (head == end)
        return; // если список пустой или содержит один элемент, сортировка не требуется
    bool swapped = true;
    bool once = false;
    List *prev = end;
    for (int i = 0; (i < size - 1) && swapped; ++i) // классический вид для bubblesort
    {
        swapped = false;
        for (int j = 0; j < size - i - 1; ++j)
        {
            if (prev->next->val > prev->next->next->val)
            {
                swap_nodes(prev, prev->next, head);
                swapped = true;
            }
            prev = prev->next;
        }
        if (!once) // для перестановок первого элемента нужен конец списка, который после первого прохода мог измениться.
        {
            once = true; // конец может изменться только после первого прохода, так что нужно поменять всего один раз
            end = get_end(head);
        }
        prev = end;
    }
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    int n = 0;
    input >> n; // ключ для сортировки (0 - быстрая сортировка, 1 - сортировка пузырьком)
    int size = 0;
    List *head = read_SLCL(input, size);
    input.close();
    if (head)
    {
        std::ofstream output("output.txt");
        if(n)
            list_bubble_sort(head, size);
        else
            list_quicksort(head);
        output << size << ' ';
        print_list(head, output);
        output.close();
    }
    delete_list(head);
    return 0;
}
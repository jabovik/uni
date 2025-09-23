#include <iostream>

using namespace std;
/*
void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

Оказывается, в std уже есть swap.
*/

void draw(int **mat, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (mat[i][j] != 0)
            {
                cout << mat[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void transpose(int **mat, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            swap(mat[i][j], mat[j][i]);
        }
    }
}

int main(int argc, char const *argv[])
{
    int rows, columns;
    // cin >> rows >> columns;
    rows = 2;
    columns = 2;
    int size = max(rows, columns);
    int **mat = new int *[size] {};
    for (int i = 0; i < size; ++i) // создаём пустые подмассивы
    {
        mat[i] = new int[size]{}; // матрица всегда квадратная
    }
    for (int i = 0; i < rows; ++i) // заполнение числами
    {
        for (int j = 0; j < columns; ++j)
        {
            cin >> mat[i][j];
        }
    } // получилась матрица с паддингом нулями до квадратной
    draw(mat, size);
    cout << endl;
    transpose(mat, size);
    draw(mat, size);
    return 0;
}

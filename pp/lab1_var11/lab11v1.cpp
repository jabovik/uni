#include <iostream>
#include <limits>
using namespace std;

void my_swap(char& a, char& b)
{
    char temp = a;
    a = b;
    b = temp;
}

void delete_matrix(char **mat, int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] mat[i];
    }
    delete[] mat;
}
/// @brief функция отрисовки матрицы. игнорирует нуль-терминатор в элементах строки 
/// @param mat указатель на кв. матрицу
/// @param size размер матрицы
void draw(char **mat, int rows, int columns)
{
    for (int i = 0; i < rows; ++i) // заполнение пользователя
        {
            for (int j = 0; j < columns; ++j)
            {
                cout << mat[i][j] << ' ';
            }
            cout << endl;
        }
}
/// @brief функция транспонирования матрицы. Принимает квадратную матрицу, проходит только
/// по элементам выше главной диагонали и меняет местами с соответствующими элементами ниже.
/// @param mat указатель на матрицу
/// @param size размер матрицы
void transpose(char **mat, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            my_swap(mat[i][j], mat[j][i]);
        }
    }
}

int main(int argc, char const *argv[])
{
    int rows, columns;
    char choice = 'y';
    do{ // цикл для повтора программы
        cout << "Введите размеры матрицы:\n";
        do
        { // цикл для ввода размеров
            cin >> rows;
            if(rows<1)
            {
                cout << "Неверный ввод размеров. Повторите попытку:\n";
            }
        } while (rows<1);
        do
        { // цикл для ввода размеров
            cin >> columns;
            if(columns<1)
            {
                cout << "Неверный ввод размеров. Повторите попытку:\n";
            }
        } while (columns<1);
        int size = max(rows, columns);
        char **mat = new char *[size];
        for (int i = 0; i < size; ++i) // создаём пустые подмассивы
        {
            mat[i] = new char[size]; // матрица всегда квадратная
            for (int j = 0; j < size; ++j)
            {
                mat[i][j] = '\n'; // получилась матрица с паддингом нуль-терминатором.
            }
        }
        cout << "Введите элементы матрицы:\n";
        for (int i = 0; i < rows; ++i) // заполнение пользователя
        {
            for (int j = 0; j < columns; ++j)
            {
                cin >> mat[i][j];
            }
        } // получилась матрица с паддингом нулями до квадратной
        cout << "Ваша матрица:\n";
        draw(mat, rows, columns);
        cout << "Транспонированная матрица:\n";
        transpose(mat, size);
        draw(mat, columns, rows);
        delete_matrix(mat, columns);
        cout << "Вы хотите использовать программу заново?(y/any key)\n";
        cin >> choice;
    }while(choice == 'y');
    return 0;
}

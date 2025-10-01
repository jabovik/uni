/*
Практикум по программированию
Лабораторная работа №1
Вариант 11**
----------------------------------------------------------
Транспонирование матрицы без дополнительных массивов.
Входные данные: размеры матрицы и матрица.
Выходные данные: транспонированная матрица.
*/
#include <iostream>
#include <limits>
using namespace std;
// функция вывода матрицы
void draw(double **mat, int m, int n)
{
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}
// функция освобождения памяти
void delete_matrix(double **mat, int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] mat[i];
    }
    delete[] mat;
}
// шаблонная функция безопасного ввода
template <typename T>
T safe_input()
{
    T input;
    if (!(cin >> input) || (cin.peek() != '\n' && cin.peek() != ' '))
    {
        cin.clear();                                         // сброс состояния ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока ввода
        throw invalid_argument("Invalid input");
    }
    return input;
}
// функция проверки на наличие лишних аргументов в потоке
bool no_more_args()
{
    char x;
    while (cin.peek() == ' ') cin.get(); // пропускаем пробелы
    if (cin.peek() != '\n')
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Too many args" << endl;
        cout << "Enter the numbers again: " << endl;
        return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    char choice;
    do // цикл для повторного выполнения программы
    {
        int m, n; // m - количество строк, n - количество столбцов в исходной матрице
        cout << "Enter numbers of rows and columns: " << endl;
        bool entry_error;
        do
        {
            entry_error = false;
            try
            {
                m = safe_input<int>();
                n = safe_input<int>();
            }
            catch (const invalid_argument &e)
            {
                entry_error = true;
                cout << "Error: " << e.what() << endl;
                cout << "Enter the numbers again: " << endl;
            }
            entry_error = entry_error || !no_more_args();
            if (entry_error == false &&(m < 1 || n < 1 || m * n < 2))
            {
                cout << "Invalid input, try again:" << endl;
                entry_error = true;
            }
        } while (entry_error); // повторяется пока не завершится без ошибок
        double **mat = new double *[n] {}; // создаём пустую транспонированную матрицу с числом строк n
        cout << "Enter matrix elements: " << endl;
        for (int i = 0; i < n; ++i)
        {
            mat[i] = new double[m]{}; // число столбцов в новой матрице - m
        }
        bool fill_error;
        do
        {
            fill_error = false;
            for (int i = 0; i < m; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    try
                    {
                        mat[j][i] = safe_input<double>(); // матрица заполняется транспонированной
                    }
                    catch (const invalid_argument &e)
                    {
                        fill_error = true;
                        cout << "Error " << e.what() << endl;
                        cout << "Enter the elements again:" << endl;
                        j = m;
                        i = n;
                    }
                }
            }
            fill_error = fill_error || !no_more_args();
        } while (fill_error); // повторяется пока не завершится без ошибок
        cout << "Transposed matrix: " << endl;
        draw(mat, n, m); // отрисовка матрицы
        delete_matrix(mat, n); // очистка памяти
        cout << "Continue? (y/any key):";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y');
    return 0;
}
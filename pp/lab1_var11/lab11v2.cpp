#include <iostream>
#include <limits>

using namespace std;
void draw(double **mat, int m, int n) // функция вывода матрицы
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

void delete_matrix(double **mat, int rows) // функция освобождения памяти
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] mat[i];
    }
    delete[] mat;
}

template <typename T>
T safe_input(bool positive_only) // шаблонная функция безопасного ввода
{
    T input;
    while (!(cin >> input) || (positive_only && input <= 0))
    {
        cin.clear();                                         // сброс состояния ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока ввода
        cout << "Invalid input, try again" << endl;
    }
    return input;
}

int main(int argc, char const *argv[])
{
    char choice = 'y';
    while (choice == 'y') // цикл для повторного ввода матрицы
    {
        int m, n; // m - количество строк, n - количество столбцов в исходной матрице
        cout << "Enter number of rows and columns: " << endl;
        m = safe_input<int>(true);
        n = safe_input<int>(true);
        double **mat = new double *[n] {}; // создаём пустую транспонированную матрицу с числом строк = n
        cout << "Enter matrix elements: " << endl;
        for (int i = 0; i < n; ++i)
        {
            mat[i] = new double[m]{}; // число столбцов в новой матрице = m
        }

        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                mat[j][i] = safe_input<double>(false); // матрица заполняется и сразу транспонируется
            }
        }
        cout << "Transposed matrix: " << endl;
        draw(mat, n, m);
        delete_matrix(mat, n);
        cout << "Continue? (y/n)" << endl;
        cin >> choice;
        while (choice != 'y' && choice != 'n')
        {
            cout << "y/n" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> choice;
        }
    }
    return 0;
}
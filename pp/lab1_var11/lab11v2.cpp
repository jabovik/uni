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

int main(int argc, char const *argv[])
{
    char choice;
    do // цикл для повторного ввода матрицы
    {
        int m, n; // m - количество строк, n - количество столбцов в исходной матрице
        cout << "Enter number of rows and columns: " << endl;
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
            if ((m < 1 || n < 1 || m * n < 2) && entry_error == false)
                cout << "Invalid input, try again" << endl;
        } while ((m < 1 || n < 1 || m * n < 2) || entry_error == true);
        double **mat = new double *[n] {}; // создаём пустую транспонированную матрицу с числом строк n
        cout << "Enter matrix elements: " << endl;
        for (int i = 0; i < n; ++i)
        {
            mat[i] = new double[m]{}; // число столбцов в новой матрице - m
        }
        bool fill_error;
        do{
            fill_error = false;
            for (int i = 0; i < m; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    try{
                        mat[j][i] = safe_input<double>(); // матрица заполняется транспонированной
                    }
                    catch(const invalid_argument &e)
                    {
                        fill_error = true;
                        cout << "Error " << e.what() << endl;
                        cout << "Enter the elements again" << endl;
                        j=m;
                        i=n;
                    }
                }
            }
        } while(fill_error);
        cout << "Transposed matrix: " << endl;
        draw(mat, n, m);
        delete_matrix(mat, n);
        cout << "Continue? (y/any other key to exit):";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y');
    return 0;
}
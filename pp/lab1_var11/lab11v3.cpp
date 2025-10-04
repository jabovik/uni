#include <iostream>
using namespace std;
/// @brief вычисление индекса элемента в векторизованной матрице
/// @param i номер строки
/// @param j номер столбца
/// @param columns количество столбцов в матрице
/// @return индекс элемента в векторизованной матрице
int vectorised_index(int i, int j, int columns)
{
    return i * columns + j;
}

int index_transposed(int idx, int m, int n)
{
    return idx%m * n + idx/m;
}

void cycle_swap(char *mat, int p, int m, int n){
    int start = p;
    int next = index_transposed(p,m,n);
    if (next == start){
        return;
    } // цикл длины 1
    do
    {
        swap(mat[p], mat[next]);
        p = next;
        next = index_transposed(p,m,n);
    } while (start != next);
    
}
bool is_idx_min_in_cycle(char *mat, int p, int m, int n)
{
    int start = p;
    int next = index_transposed(p,m,n);
    if (next == start) return false; // цикл длины 1
    do
    {
        if(start > next) return false;
        p = next;
        next = index_transposed(p,m,n);
    } while (start != next);
    return true;
}
void transpose(char *mat, int m, int n)
{
    for (int i = 0; i < m*n; i++)
    {
        if(is_idx_min_in_cycle(mat, i, m, n))
        {
            cycle_swap(mat, i, m, n);
        }
    }
}

void draw(char *mat, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << mat[vectorised_index(i,j,n)] << ' ';
        }
        cout << endl;
    }   
}

int main(int argc, char const *argv[])
{
    int m, n;
    cin >> m >> n;
    int size = m * n;
    char *mat = new char[size];
    for (int i = 0; i < size; i++)
    {
        cin >> mat[i];
    }
    draw(mat, m, n);
    transpose(mat, m, n);
    cout << "after transposition" << endl;
    draw(mat, n, m);
    delete[] mat;
    
    return 0;
}
#include <fstream>

double &mat_iloc(double *mat, int columns, int column, int row)
{
    int idx = column + row * columns;
    return mat[idx];
}

void hor_mirror_mat(double *mat, int rows, int columns)
{
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < columns / 2; ++i)
        {
            double temp = mat_iloc(mat, columns, i, j);
            mat_iloc(mat, columns, i, j) = mat_iloc(mat, columns, columns - 1 - i, j);
            mat_iloc(mat, columns, columns - 1 - i, j) = temp;
        }
    }
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    int rows, columns;
    input >> rows >> columns;
    int size = rows * columns;
    double *mat = new double[size];
    for (int i = 0; i < size; ++i)
    {
        input >> mat[i];
    }
    input.close();
    if (!size)
        return 0;
    hor_mirror_mat(mat, rows, columns);
    std::ofstream output("output.txt");
    for (int i = 0; i < size; ++i)
    {
        output << mat[i] << ' ';
    }
    output.close();
    delete[] mat;
    return 0;
}

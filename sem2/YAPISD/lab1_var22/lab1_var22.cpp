#include <iomanip>
#include <fstream>
#include <cmath>
double foo(double* arr, int size, double d)
{
    double cumsum = 0;
    for(int i = 0;i<size;++i)
    {
        double el = arr[i];
        if(el<0 && el<=d)
        {
            cumsum += el*el;
        }
    }
    return cumsum;
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    double d;
    input >> d;
    double el;
    int size = 0;
    /*
    double arr[1000];
    while(input >> el)
    {
        arr[size] = el;
        ++size;
    }
    */
    while(input >> el) // подсчет кол-ва элементов
    {
        ++size;
    }
    input.clear();
    input.seekg(0); // возврат к началу файла
    input >> el; // пропуск первой строки
    double *arr = new double[size];
    int i = 0;
    while(input >> el)
    {
        arr[i] = el;
        ++i;
    }
    double cumsum = foo(arr, size, d);
    output << std::setprecision(15) << cumsum; // форматирование под условия тестов
    delete[] arr;
    return 0;
}
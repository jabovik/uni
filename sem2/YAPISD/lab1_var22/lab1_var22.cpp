
#include <iomanip>
#include <fstream>
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
    double arr[1000];
    double el;
    int size = 0;
    while(input >> el)
    {
        arr[size] = el;
        ++size;
    }
    double cumsum = foo(arr, size, d);
    output << std::setprecision(15) << cumsum;
    return 0;
}
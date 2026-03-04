// lab0.cpp : A + B
#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
    std::fstream input("input.txt");
    if (!input)
    {
        std::cerr << "input file error\n";
        return 1;
    }
    double a, b;
    if (!(input >> a >> b))
    {
        std::cerr << "input data error\n";
        return 2;
    }
    std::ofstream output("output.txt");
    if(!output)
    {
        std::cerr << "output file error\n";
        return 1;
    }
    output << a + b;
    return 0;
}
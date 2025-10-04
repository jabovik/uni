#include <iostream>
#include <math.h>
using namespace std;
double f(double x, double n)
{
    static int n_fact = n;
    n_fact *= n;
    return (pow(2,n/2)* sin(M_PI*n/4)*pow(x,n))/n_fact;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

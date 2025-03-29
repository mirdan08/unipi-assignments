#include "utilities.hpp"

using ull = unsigned long long;

ull collatz_steps(ull n){
    ull steps=0;
    while (n!=1){
        n= (n%2==0) ? n/2 : 3*n +1;
        ++steps;
    }
    return steps;
}


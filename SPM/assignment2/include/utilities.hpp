#include<utility>
#include<string>
using ull = unsigned long long;


std::pair<int,int> parseRange(const std::string& strRange);

ull collatz_steps(ull n);
ull partialMaxCollatzStep(int start,int end);

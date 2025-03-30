#include<utility>
#include<string>
using ull = unsigned long long;


std::pair<int,int> parseRange(const std::string& strRange);
template <typename Func, typename... Args> auto make_task(Func&& func, Args&&... args);


ull collatz_steps(ull n);
ull partialMaxCollatzStep(int start,int end);

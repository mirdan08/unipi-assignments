#include<utility>
#include<string>
#include <future>
#include <type_traits>
#include <functional>

#include "utilities.hpp"

std::pair<int,int> parseRange(const std::string& strRange){
    int dashIdx= strRange.find('-');

    std::string startStr= strRange.substr(0,dashIdx);
    std::string endStr= strRange.substr(dashIdx+1);

    int start= std::stoi(startStr);
    int end= std::stoi(endStr);

    return {start,end};

}

/* 
template <typename Func, typename... Args>
auto make_task(Func&& func, Args&&... args) {
    using Rtrn = std::invoke_result_t<Func, Args...>;

    // Lambda that captures everything and calls the function with args
    auto bound = [f = std::forward<Func>(func), ...a = std::forward<Args>(args)]() mutable {
        return std::invoke(f, std::move(a)...);
    };

    // Wrap in packaged_task
    return std::packaged_task<Rtrn()>(std::move(bound));
} */


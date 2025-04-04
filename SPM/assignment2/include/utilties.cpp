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


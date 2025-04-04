#if !defined(TASKFACTORY_)
#define TASKFACTORY_

#include <functional> // std::bind
#include<iostream>
#include<utility>
#include<vector>
#include<deque>
#include<string>
#include<algorithm>
#include<cmath>
#include<future>
#include<mutex>
#include<ctime>
#include<chrono>
template<
	typename Func,    // <-- type of the func
	typename ... Args,// <-- arguments type arg0, arg1, ...
	typename Rtrn=typename std::result_of<Func(Args...)>::type
	>                 // ^-- type of the return value func(args)     
auto make_task(
	  Func && func,
	  Args && ...args) -> std::packaged_task<Rtrn(void)> {

	// basically it builds a callable object aux
	// (an auxilliary function aux(void)) without arguments
	// and returning func(arg0,arg1,...)
	auto aux = std::bind(std::forward<Func>(func),
						 std::forward<Args>(args)...);

	// create a task wrapping the auxilliary function:
	// task() executes aux(void) := func(arg0,arg1,...)
	auto task = std::packaged_task<Rtrn(void)>(aux);
	
	// the return value of aux(void) is assigned to a
	// future object accessible via task.get_future()
	return task;
}

#endif 
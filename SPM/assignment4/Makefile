CXX		= g++ -std=c++20
INCLUDES	= -I 
CXXFLAGS  	+= -Wall 

LDFLAGS 	= -pthread -fopenmp -Wall -Wextra
OPTFLAGS	= -O3 -ffast-math -DNDEBUG

TARGETS		= mergeSortSeq

.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)

clean		: 
	rm -f $(TARGETS) 
cleanall	: clean
	\rm -f *.o *~
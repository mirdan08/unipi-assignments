CXX		= g++ -std=c++20
INCLUDES	= 
CXXFLAGS  	+= -Wall 

LDFLAGS 	= -pthread -fopenmp -Wall -Wextra
OPTFLAGS	= -O3 -ffast-math -DNDEBUG

TARGETS		= mergeSortSeq

SRCS= mergesort.cpp mergeSortSeq.cpp

OBJS      = $(SRCS:.cpp=.o)

.PHONY: all clean cleanall
.SUFFIXES: .cpp 

$(TARGETS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -c $< -o $@

all	: $(TARGETS)

clean : 
	rm -f $(TARGETS) 
cleanall	: clean
	\rm -f *.o *~
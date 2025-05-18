CXX        = g++ -std=c++20
INCLUDES   = -Ifastflow/
CXXFLAGS  += -Wall 
LDFLAGS    = -pthread -fopenmp -Wall -Wextra
OPTFLAGS   = -O3 -ffast-math -DNDEBUG

# Targets
TARGETS    = mergeSortPar mergeSortSeq

# Sources
COMMON_SRC = mergesort.cpp
SEQ_SRC    = mergeSortSeq.cpp
PAR_SRC    = mergeSortPar.cpp

# Object files
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)
SEQ_OBJ    = $(SEQ_SRC:.cpp=.o)
PAR_OBJ    = $(PAR_SRC:.cpp=.o)

.PHONY: all clean cleanall
.SUFFIXES: .cpp

# Rules
all: $(TARGETS)

mergeSortSeq: $(SEQ_OBJ) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

mergeSortPar: $(PAR_OBJ) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS)

cleanall: clean
	rm -f *.o *~
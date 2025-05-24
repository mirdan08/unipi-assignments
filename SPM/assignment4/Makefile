CXX        = g++ -std=c++20
MPICXX     = mpicxx -std=c++20

INCLUDES   = -Ifastflow/
CXXFLAGS  += -Wall 
LDFLAGS    = -pthread -fopenmp -Wall -Wextra
OPTFLAGS   = -O3 -ffast-math -DNDEBUG

# Targets
TARGETS    = mergeSortPar mergeSortSeq mergeSortDist sortSeq

# Sources
COMMON_SRC = mergesort.cpp
SORT_SRC   = sortSeq.cpp
SEQ_SRC    = mergeSortSeq.cpp
PAR_SRC    = mergeSortPar.cpp
MPI_SRC    = mergeSortDist.cpp

# Object files
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)
SEQ_OBJ    = $(SEQ_SRC:.cpp=.o)
PAR_OBJ    = $(PAR_SRC:.cpp=.o)
SORT_OBJ   = $(SORT_SRC:.cpp=.o)
MPI_OBJ   = $(MPI_SRC:.cpp=.o)

.PHONY: all clean cleanall
.SUFFIXES: .cpp

# Rules
all: $(TARGETS)

sortSeq: $(SORT_OBJ) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

mergeSortSeq: $(SEQ_OBJ) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

mergeSortPar: $(PAR_OBJ) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

mergeSortDist: $(MPI_OBJ) $(COMMON_OBJ)
	$(MPICXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS) -lmpi

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS)

cleanall: clean
	rm -f *.o *~
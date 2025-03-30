CXX                = g++ -std=c++20
OPTFLAGS           = -O3
CXXFLAGS          += -Wall 
INCLUDES           = -I. -I./include

LIBS               = -pthread # -fopenmp
COMMON_SOURCES     = $(wildcard ./include/*.cpp)  # Common source files
COMMON_OBJECTS     = $(COMMON_SOURCES:.cpp=.o)    # Common object files
MAIN_SOURCES       = $(wildcard *.cpp)           # Main source files with `main`
MAIN_OBJECTS       = $(MAIN_SOURCES:.cpp=.o)     # Main object files
TARGETS            = $(MAIN_SOURCES:.cpp=)       # Executables

.PHONY: all clean cleanall 
all: $(TARGETS)

# Rule to build each program
$(TARGETS): %: %.o $(COMMON_OBJECTS)
	$(CXX) $(OPTFLAGS) $^ -o $@ $(LIBS)

# Rule to compile main source files
%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

# Rule to compile common source files
include/%.o: include/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean: 
	-rm -f *.o *~ include/*.o

cleanall: clean
	-rm -f $(TARGETS)

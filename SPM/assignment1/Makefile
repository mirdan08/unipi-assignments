CXX                = g++ -std=c++17
OPTFLAGS	   = -O3
AUTOFLAGS          = -ftree-vectorize -fopt-info-vec-missed -fopt-info-vec -march=native -funroll-loops -ffast-math
AVXFLAGS           = -mavx -march=native -ffast-math
CXXFLAGS          += -Wall 
INCLUDES	   = -I. -I./include
LIBS               = #-pthread -fopenmp
SOURCES            = $(wildcard *.cpp)
TARGET             = $(SOURCES:.cpp=)

.PHONY: all clean cleanall 

%: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o $@ $< $(LIBS)

# For files with _avx in their name, append flags to CXXFLAGS
%_avx: CXXFLAGS += ${AVXFLAGS}

# For files with _auto in their name, append flags to CXXFLAGS
%_auto: CXXFLAGS += ${AUTOFLAGS}

all: $(TARGET)

clean: 
	-rm -fr *.o *~
cleanall: clean
	-rm -fr $(TARGET)
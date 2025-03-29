CXX                = g++ -std=c++17
OPTFLAGS	   = -O3
CXXFLAGS          += -Wall 
INCLUDES	   = -I. -I./include

LIBS               = #-pthread -fopenmp
SOURCES            = $(wildcard *.cpp ./include/*.cpp)

OBJECTS            = $(SOURCES:.cpp=.o)  # Convert .cpp to .o

TARGET             = $(SOURCES:.cpp=)

.PHONY: all clean cleanall 
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(OPTFLAGS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean: 
	-rm -fr *.o *~ include/*.o

cleanall: clean
	-rm -fr $(TARGET)
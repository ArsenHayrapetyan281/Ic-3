CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGETS = first second
OBJECTS = first.o second.o
LIBS = -lrt -pthread

all: $(TARGETS)

first: first.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

second: second.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

first.o: first.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c first.cpp

second.o: second.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c second.cpp

clean:
	rm -f $(TARGETS) $(OBJECTS)


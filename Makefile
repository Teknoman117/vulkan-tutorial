CFLAGS=-Werror -ggdb
CXXFLAGS=$(CFLAGS) -std=c++14
LDFLAGS=`pkg-config --static --libs glfw3` -lvulkan

CC=gcc
CXX=g++

all: 00_enumerate 01_drawtriangle

00_enumerate: 00_enumerate.cpp
	$(CXX) $(CXXFLAGS) -o 00_enumerate 00_enumerate.cpp $(LDFLAGS)

01_drawtriangle: 01_drawtriangle.cpp
	$(CXX) $(CXXFLAGS) -o 01_drawtriangle 01_drawtriangle.cpp $(LDFLAGS)

.PHONY: test_00_enumerate test_01_drawtriangle clean

test_00_enumerate: 00_enumerate
	./00_enumerate

test_01_drawtriangle: 01_drawtriangle
	./01_drawtriangle

clean:
	rm -f 00_enumerate 01_drawtriangle
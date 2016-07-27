CFLAGS=-std=c++11
LDFLAGS=`pkg-config --static --libs glfw3` -lvulkan

00_enumerate: 00_enumerate.cpp
	$(CXX) $(CXXFLAGS) -o 00_enumerate 00_enumerate.cpp $(LDFLAGS)

.PHONY: test_00_enumerate clean

test_00_enumerate: 00_enumerate
	./00_enumerate

clean:
	rm -f 00_enumerate
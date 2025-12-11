CXX = clang++

CXXFLAGS = -std=c++2b -Wall -Wextra -Wpedantic

TARGET = main

all: $(TARGET)

$(TARGET): main.o Rational.o
	$(CXX) $(CXXFLAGS) main.o Rational.o -o $(TARGET)

main.o: main.cpp Rational.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Rational.o: Rational.cpp Rational.hpp
	$(CXX) $(CXXFLAGS) -c Rational.cpp

clean:
	rm -f *.o $(TARGET)
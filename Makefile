CXXFLAGS := -g -Wall -Wextra -I./include/
CXX := g++
TARGET := vector
SRCS = vector.cpp main.cpp
OBJS := $(SRCS:.cpp=.0)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
vector.o: vector.cpp
	$(CXX) $(CXXFLAGS) -c vector.cpp
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
clean:
	$(RM) -f $(TARGET) *.o

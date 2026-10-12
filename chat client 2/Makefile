CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = ChatClient
SRCS = ChatClient.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) -lncurses -pthread

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
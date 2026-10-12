
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.
TARGET = ChatServer
SRCS = main.cpp ChatServer.cpp
HEADERS = ChatServer.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) -pthread

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
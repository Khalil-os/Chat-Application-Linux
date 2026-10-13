CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.

TARGET = ChatClient
SRCS = main.cpp ChatClient.cpp ChatUI.cpp
HEADERS = ChatClient.h ChatUI.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) -lncurses -pthread

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

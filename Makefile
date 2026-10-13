CXX := g++
CXXFLAGS := -std=c++11 -O2 -Wall -Wextra
SOURCES := $(wildcard *.c)

TARGET := student-grade-management-system.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(wildcard *.h)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

clean:
	$(RM) $(TARGET)

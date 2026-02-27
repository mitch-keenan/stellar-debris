CXX ?= g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I../src -O2

SOURCES = test_main.cpp test_ar_envelope.cpp test_adsr_envelope.cpp test_oscillator.cpp test_polivoks_filter.cpp
TARGET = run_tests

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean

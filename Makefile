CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = lexer
SOURCES = main.cpp lexer.cpp
HEADERS = lexer.h

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) tokens_output.txt

test: $(TARGET)
	@echo "Testing small file:"
	@echo "test_small.txt" | ./$(TARGET)
	@echo "\nTesting medium file:"
	@echo "test_medium.txt" | ./$(TARGET)
	@echo "\nTesting large file:"
	@echo "test_large.txt" | ./$(TARGET)

.PHONY: clean test
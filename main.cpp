#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;
    bool comment = false;

    std::ifstream f("input1.txt");

    std::string line;
    while (std::getline(f, line)) {
        std::string curr = "";

        // Skip if the line is empty
        if (line.empty()) {
            continue;
        }

        // Only add the character to curr line if it's not a comment
        // and not enclosed by a comment
        for (char c : line) {
            if (c == '\"' && !comment) {
                comment = true;
            } else if (c == '\"' && comment) {
                comment = false;
            } else if (!comment) {
                curr += c;
            }
        }
        
        // If the line is all comments, skip it
        if (curr.empty()) {
            continue;
        }
        
        lines.push_back(curr);
    }

    // Prints out every line we parsed (not needed for functionality)
    std::cout << "[";
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "'" << lines[i] << "'";
        if (i < lines.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    f.close();
}
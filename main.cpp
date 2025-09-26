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

        if (line.empty()) {
            continue;
        }

        for (char c : line) {
            if (c == '\"' && !comment) {
                comment = true;
            } else if (c == '\"' && comment) {
                comment = false;
            } else if (!comment) {
                curr += c;
            }
        }

        if (curr.empty()) {
            continue;
        }
        
        lines.push_back(curr);
    }

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
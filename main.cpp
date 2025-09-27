#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;
    std::vector<std::vector<std::string> > tokens;
    bool comment = false;

    std::ifstream f("test_input.txt");

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

    // Break down the lines into individual tokens
    for (std::string& current_line : lines) {
        std::vector<std::string> line_tokens;
        std::string word = "";
        for (char c: current_line){
            if (isspace(c)) {
                if (!word.empty()) {
                    line_tokens.push_back(word);
                    word = ""; // Reset the word
                }
            } else {
                word += c; // Append character to the current word
            }
        }
        if(!word.empty()) {
            line_tokens.push_back(word);
        }
        tokens.push_back(line_tokens);
    }

    // Prints out every token we parsed (not needed for functionality)
    for(std::vector<std::string> line: tokens){
        for(std::string token : line) {
            std::cout << token << " $$$ ";
        }
        std::cout << std::endl;
    }

    f.close();
}
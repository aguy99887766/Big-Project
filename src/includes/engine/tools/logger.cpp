#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <limits>
#include <vector>
#include <map>

void output(std::string message) {
    std::cout << message << std::endl;
}

void information(std::string message) {
    output("[+] "+message);
}

void warning(std::string message) {
    output("[~] "+message);
}

void error(std::string message) {
    output("[ CONSOLE:ERROR ] "+message);
}

template <typename T, size_t N>
void outputArray(const T (&neededArray)[N]) {
    for (int i = 0; i < N; i++) {
        std::stringstream ss;
        ss << (i + 1) << ": " << neededArray[i];
        output(ss.str());
    }
}

template <typename F, typename S>
void outputMap(const std::map<F, S>& neededMap) {
    for (const auto& i : neededMap) {
        std::stringstream ss;
        ss << i.first << ": " << i.second;
        output(ss.str());
    }

}

template <typename T>
void outputVector(const std::vector<T>& neededVector) {
    for (size_t i = 0; i<neededVector.size(); i++) {
        std::stringstream ss;
        ss << i+1 << ": " << neededVector[i];
        output(ss.str());
    }
}

template <typename T>
void input(const std::string& question, T& value) {
    /*
    
    user input, has validation check
    
    */
    while (true) {
        std::cout << question << ": ";
        std::cin >> value; 

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Validation failed. Try again." << std::endl;
            
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        

    }
}

void outputHalt(const std::string& question) {
    std::string dummy;
    std::cout << question << ": ";
    std::getline(std::cin, dummy);
    if (dummy.empty()) {
        return;
    }
}


#include <iostream>
#include <vector>
#include <map>

// define terminal escape codes for color
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

class ParkingLot {
private:
    std::map<char, std::vector<char>> spaces;

public:
    ParkingLot() {
        spaces = {
            {'C', std::vector<char>(10, 'F')},
            {'T', std::vector<char>(10, 'F')},
            {'M', std::vector<char>(10, 'F')}
        };
    }

    bool park(char vehicleType, int spaceIndex) {
        if (spaces[vehicleType][spaceIndex] == 'F') {
            spaces[vehicleType][spaceIndex] = 'O';
            return true;
        } else {
            return false;
        }
    }

    void leave(char vehicleType, int spaceIndex) {
        spaces[vehicleType][spaceIndex] = 'F';
    }

    void print() {
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                  << "+                                 " << BLUE << "E" << RESET << "                                         +\n"
                  << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    
        for (char vehicleType : {'C', 'T', 'M'}) {
            std::cout << "+   ";
            for (char space : spaces[vehicleType]) {
                std::string color = (vehicleType == 'C') ? RED : (vehicleType == 'T' ? YELLOW : GREEN);
                std::cout << "|" << color << space << RESET << "|   ";
            }
            std::cout << "+\n+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n";
        }
    
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                  << "+                                 " << BLUE << "E" << RESET << "                                         +\n"
                  << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    }
};

int main() {
    ParkingLot p;
    p.print();
    p.park('C', 0); // Park a car in the first space
    p.print();
    p.leave('C', 0); // Car leaves the first space
    p.print();
    return 0;
}

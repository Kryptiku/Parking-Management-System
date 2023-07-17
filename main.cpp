#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <limits>
#include <cmath>
#include <windows.h>

using namespace std;



class Node {
public:
    string plateNumber;
    string vehicleType;
    double feePerHour;
    time_t entryTime;
    Node* next;

    Node(string plateNumber, string vehicleType, double feePerHour)
        : plateNumber(plateNumber), vehicleType(vehicleType), feePerHour(feePerHour), next(nullptr) {
        this->entryTime = time(0);
    }
};

class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    void insertAtTail(Node* newNode) {
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    Node* searchByPlateNumber(string plateNumber) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->plateNumber == plateNumber) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void removeByPlateNumber(string plateNumber) {
        if (head == nullptr) {
            return;
        }

        if (head->plateNumber == plateNumber) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* temp = head;
        Node* toDelete = nullptr;
        while (temp->next != nullptr) {
            if (temp->next->plateNumber == plateNumber) {
                toDelete = temp->next;
                temp->next = temp->next->next;
                delete toDelete;
                return;
            }
            temp = temp->next;
        }
    }

};

class ParkingLot {
private:
    LinkedList parkingLot;
    map<string, int> spaces = { {"car", 50}, {"truck", 30}, {"motor", 20} };
    map<string, double> fees = { {"car", 50.0}, {"truck", 100.0}, {"motor", 30.0} };
    map<string, int> totalSpace = { {"car", 50}, {"truck", 30}, {"motor", 20} };
    string csvFileName;

    void readDataFromCSV() {
        ifstream inputFile(csvFileName);

        if(!inputFile.is_open()) {
            cerr << "[ERROR] Failed to open the CSV file.\n";
            return;
        }

        string line;
        getline(inputFile, line);
        while(getline(inputFile, line)) {
            istringstream iss(line);
            string plateNumber, vehicleType;
            double feePerHour;
            if(getline(iss, plateNumber, ',') && getline(iss, vehicleType, ',') && iss >> feePerHour) {
                parkingLot.insertAtTail(new Node(plateNumber, vehicleType, feePerHour));
            }
        }

        inputFile.close();
    }

    void readSpacesFromCSV() {
        ifstream inputFile("spaces.csv");

        if(!inputFile.is_open()) {
            cerr << "[ERROR] Failed to open the spaces CSV file.\n";
            return;
        }

        string line;
        getline(inputFile, line);
        while(getline(inputFile, line)) {
            istringstream iss(line);
            string vehicleType;
            int space;
            if(getline(iss, vehicleType, ',') && iss >> space) {
                spaces[vehicleType] = space;
            }
        }

        inputFile.close();
    }

    void writeSpacesToCSV() {
        ofstream outputFile("spaces.csv");

        if (!outputFile.is_open()) {
            cerr << "[ERROR] Failed to open the spaces CSV file for writing.\n";
            return;
        }

        outputFile << "VehicleType,AvailableSpaces\n";
        for (auto const& space : spaces)
            outputFile << space.first << "," << space.second << "\n";

        outputFile.close();
    }

public:
    ParkingLot(string fileName) : csvFileName(fileName) {
        readDataFromCSV();
        readSpacesFromCSV();
    }


    void displayHeader() {
        // Text Color
        HANDLE console_color;
        console_color = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console_color, 3);

 cout << R"(
______          _             _ _ _
| ___ \        | |           | (_) |
| |_/ /_ _ _ __| | ____ _  __| |_| |_ ___
|  __/ _` | '__| |/ / _` |/ _` | | __/ _ \
| | | (_| | |  |   < (_| | (_| | | || (_) |
\_|  \__,_|_|  |_|\_\__,_|\__,_|_|\__\___/

                                           )" << endl;
    }

    void addVehicle() {
        string plateNumber;
        string vehicleType;
        double feePerHour;
        // Text Color
        HANDLE console_color;
        console_color = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console_color, 7);

        cout << "Available parking spaces:\n";
        for (auto const& space : spaces)
            cout << space.first << ": " << space.second << "\n";

        while (true) {
            SetConsoleTextAttribute(console_color, 7);
            cout << endl <<"[Motor = 30php/hr ]\n[Car = 50php/hr]\n[Truck = 100php/hr]\n" << endl << "Enter vehicle type: ";
            getline(cin, vehicleType);

            transform(vehicleType.begin(), vehicleType.end(), vehicleType.begin(), ::tolower);

            if (spaces.find(vehicleType) == spaces.end()) {
                SetConsoleTextAttribute(console_color, 4);
                cout << "\n[ERROR] Invalid vehicle type. Please try again.\n";
                continue;
            }

            if (spaces[vehicleType] <= 0) {
                SetConsoleTextAttribute(console_color, 4);
                cout << "\n[FAILED] No available space for " << vehicleType << ". Please choose another vehicle type.\n";
                continue;
            }
            break;
        }

        feePerHour = fees[vehicleType];

        cout << "Enter plate number: ";
        getline(cin, plateNumber);

        Node* vehicleNode = parkingLot.searchByPlateNumber(plateNumber);
        if (vehicleNode != nullptr) {
            HANDLE console_color;
            console_color = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(console_color, 4);
            cout << "[ERROR] Vehicle already exist.\n";
            return;
        }

        parkingLot.insertAtTail(new Node(plateNumber, vehicleType, feePerHour));
        spaces[vehicleType]--;

        writeSpacesToCSV();

        SetConsoleTextAttribute(console_color, 2);
        cout << "\n[SUCCESS] " << vehicleType << " " << plateNumber << " added successfully.\n";
        SetConsoleTextAttribute(console_color, 6);
        cout << "\n[INFO] The fee per hour is PHP" << feePerHour << "\n";
        SetConsoleTextAttribute(console_color, 7);

        ofstream outputFile(csvFileName, ios::app);
        if(outputFile.is_open()) {
            outputFile << plateNumber << "," << vehicleType << "," << feePerHour << "\n";
            outputFile.close();
        }
        else {
            SetConsoleTextAttribute(console_color, 4);
            cerr << "[ERROR] Failed to open the CSV file for writing. \n";
            return;
        }
    }

    // Check details of a vehicle
    void checkVehicle() {
        string plateNumber;
        // Text Color
        HANDLE console_color;
        console_color = GetStdHandle(STD_OUTPUT_HANDLE);
        
        SetConsoleTextAttribute(console_color, 7);
        cout << "Enter the plate number of the vehicle: " << endl;
        getline(cin, plateNumber);
        Node* vehicleNode = parkingLot.searchByPlateNumber(plateNumber);
        if (vehicleNode == nullptr) {
            SetConsoleTextAttribute(console_color, 4);
            cout << "[FAILED] Vehicle not found in the parking lot.\n";
            return;
        }

        time_t currentTime = time(0);
        double hoursParked = ceil(difftime(currentTime, vehicleNode->entryTime) / 3600.0);

        double cost = hoursParked * vehicleNode->feePerHour;

        SetConsoleTextAttribute(console_color, 7);
        cout << "Parking time (hour): " << hoursParked << "\n";
        cout << "Total cost: PHP" << cost << "\n";
    }

    // Parking Lot UI
    void printParkingLot() {
        // Text Color
        HANDLE console_color;
        console_color = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console_color, 2);

        for(const auto& space : spaces) {
            string vehicleType = space.first;
            int availableSpaces = space.second;
            int occupiedSpaces = totalSpace[vehicleType] - availableSpaces;
            
            SetConsoleTextAttribute(console_color, 7);
            cout << endl << vehicleType << " Spaces:\n";
            for(int i = 0; i < occupiedSpaces; ++i) {
                SetConsoleTextAttribute(console_color, 2);
                cout << "[X]\t ";
            }

            for(int i = 0; i < availableSpaces; ++i) {
                SetConsoleTextAttribute(console_color, 3);
                cout << "[ ]\t ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

void removeVehicle() {
    // Color of the console
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);

    string plateNumber;
    SetConsoleTextAttribute(console_color, 7);
    cout << "Enter the plate number of the vehicle: " << endl;
    getline(cin, plateNumber);
    Node* vehicleNode = parkingLot.searchByPlateNumber(plateNumber);
    if (vehicleNode == nullptr) {
        SetConsoleTextAttribute(console_color, 4);
        cout << "[FAILED] Vehicle not found in the parking lot.\n";
        return;
    }

    time_t currentTime = time(0);
    double hoursParked = difftime(currentTime, vehicleNode->entryTime) / 3600.0;
    double cost = hoursParked * vehicleNode->feePerHour;

    string vehicleType = vehicleNode->vehicleType; // Save vehicleType before node is deleted

    parkingLot.removeByPlateNumber(plateNumber);
    spaces[vehicleType]++;

    writeSpacesToCSV();

    SetConsoleTextAttribute(console_color, 2);
    cout << "[SUCCESS] " << vehicleType << " " << plateNumber << " removed successfully.\n";
    SetConsoleTextAttribute(console_color, 6); 
    cout << "[INFO] Vehicle has been parked for " << hoursParked << " hours.\n";
    cout << "[INFO] Total cost is PHP " << cost << "\n";


    // delete vehicleNode; -> This line should be removed because node is already deleted in removeByPlateNumber

    string tempFileName = "temp.csv";
    ifstream inputFile(csvFileName);
    ofstream outputFile(tempFileName);

    if(!inputFile.is_open() || !outputFile.is_open()) {
        SetConsoleTextAttribute(console_color, 4);
        cerr << "[ERROR] Failed to open the CSV file.\n";
        return;
    }

    string line;
    while(getline(inputFile, line)) {
        if(line.find(plateNumber) == string::npos) {
            outputFile << line << "\n";
        }
    }

    inputFile.close();
    outputFile.close();

    // Handle possible file operation errors
    if(remove(csvFileName.c_str()) != 0) {
        SetConsoleTextAttribute(console_color, 4);
        perror("[ERROR] Error deleting original CSV file");
        return;
    }
    if(rename(tempFileName.c_str(), csvFileName.c_str()) != 0) {
        SetConsoleTextAttribute(console_color, 4);
        perror("[ERROR] Error renaming temporary CSV file");
    }
}

};

void pressAnyKeyToContinue() {
    // Color of the console
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console_color, 7);
    cout << "\nPress ENTER to continue...\n";
    cin.get();
    system("CLS");
}

int main() {
    system("cls");
    ParkingLot parkingLot("parkingLot.csv");

    int option;
    while (true) {
        // Text color
        HANDLE console_color;
        console_color = GetStdHandle(STD_OUTPUT_HANDLE);

        parkingLot.displayHeader();
        cout << SetConsoleTextAttribute(console_color, 7) << ". Add vehicle\n";
        cout << "2. Check vehicle\n";
        cout << "3. Remove vehicle\n";
        cout << "4. Exit\n\n";
        cout << "Please enter your option: ";

        if(!(cin >> option)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[ERROR] Invalid option. Please enter a number.\n";
            continue;
        } 

        cin.ignore();

        switch (option) {
            case 1:
                system("CLS");
                parkingLot.displayHeader();
                parkingLot.printParkingLot();
                parkingLot.addVehicle();
                pressAnyKeyToContinue();
                break;
            case 2:
                system("CLS");
                parkingLot.displayHeader();
                parkingLot.checkVehicle();
                pressAnyKeyToContinue();
                break;
            case 3:
                system("CLS");
                parkingLot.displayHeader();
                parkingLot.removeVehicle();
                pressAnyKeyToContinue();
                break;
            case 4:
                cout << "Thank You...\n";
                return 0;
            default:
                cout << "[ERROR] Invalid option. Please try again.\n";
        }
    }
    return 0;
}

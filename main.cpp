#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <ctime>

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
        while (temp->next != nullptr) {
            if (temp->next->plateNumber == plateNumber) {
                Node* toDelete = temp->next;
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
    map<string, double> fees = { {"car", 5.0}, {"truck", 10.0}, {"motor", 3.0} };
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

    void addVehicle() {
        string plateNumber;
        string vehicleType;
        double feePerHour;

        cout << "Available parking spaces:\n";
        for (auto const& space : spaces)
            cout << space.first << ": " << space.second << "\n";

        while (true) {
            cout << endl <<"[Car | Truck | Motor]" << endl << "Enter vehicle name: ";
            getline(cin, vehicleType);

            transform(vehicleType.begin(), vehicleType.end(), vehicleType.begin(), ::tolower);

            if (spaces.find(vehicleType) == spaces.end()) {
                cout << "\n[ERROR] Invalid vehicle type. Please try again.\n";
                continue;
            }

            if (spaces[vehicleType] <= 0) {
                cout << "\n[INFO] No available space for " << vehicleType << ". Please choose another vehicle type.\n";
                continue;
            }
            break;
        }

        feePerHour = fees[vehicleType];

        cout << "Enter plate number: ";
        getline(cin, plateNumber);

        parkingLot.insertAtTail(new Node(plateNumber, vehicleType, feePerHour));
        spaces[vehicleType]--;

        writeSpacesToCSV();

        cout << "\n[INFO] " << vehicleType << " " << plateNumber << " added successfully.\n";
        cout << "\n[INFO] The fee per hour is $" << feePerHour << "\n";

        ofstream outputFile(csvFileName, ios::app);
        if(outputFile.is_open()) {
            outputFile << plateNumber << "," << vehicleType << "," << feePerHour << "\n";
            outputFile.close();
        }
        else {
            cerr << "[ERROR] Failed to open the CSV file for writing. \n";
            return;
        }
    }

    void checkVehicle() {
        string plateNumber;
        cout << "Enter the plate number of the vehicle: ";
        getline(cin, plateNumber);
        Node* vehicleNode = parkingLot.searchByPlateNumber(plateNumber);
        if (vehicleNode == nullptr) {
            cout << "[INFO] Vehicle not found in the parking lot.\n";
            return;
        }

        time_t currentTime = time(0);
        double hoursParked = difftime(currentTime, vehicleNode->entryTime) / 3600.0;
        double cost = hoursParked * vehicleNode->feePerHour;

        cout << "[INFO] Vehicle has been parked for " << hoursParked << " hours.\n";
        cout << "[INFO] Total cost is $" << cost << "\n";
    }

    void removeVehicle() {
        string plateNumber;
        cout << "Enter the plate number of the vehicle: ";
        getline(cin, plateNumber);
        Node* vehicleNode = parkingLot.searchByPlateNumber(plateNumber);
        if (vehicleNode == nullptr) {
            cout << "[INFO] Vehicle not found in the parking lot.\n";
            return;
        }

        time_t currentTime = time(0);
        double hoursParked = difftime(currentTime, vehicleNode->entryTime) / 3600.0;
        double cost = hoursParked * vehicleNode->feePerHour;

        parkingLot.removeByPlateNumber(plateNumber);
        spaces[vehicleNode->vehicleType]++;

        writeSpacesToCSV();

        cout << "[INFO] " << vehicleNode->vehicleType << " " << plateNumber << " removed successfully.\n";
        cout << "[INFO] Vehicle has been parked for " << hoursParked << " hours.\n";
        cout << "[INFO] Total cost is $" << cost << "\n";

        delete vehicleNode;

        string tempFileName = "temp.csv";
        ifstream inputFile(csvFileName);
        ofstream outputFile(tempFileName);

        if(!inputFile.is_open() || !outputFile.is_open()) {
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
            perror("[ERROR] Error deleting original CSV file");
            return;
        }
        if(rename(tempFileName.c_str(), csvFileName.c_str()) != 0) {
            perror("[ERROR] Error renaming temporary CSV file");
        }
    }


};

int main() {
    ParkingLot parkingLot("parkingLot.csv");

    int option;
    while (true) {
        cout << "\n*** Parking Lot Management System ***\n";
        cout << "1. Add vehicle\n";
        cout << "2. Check vehicle\n";
        cout << "3. Remove vehicle\n";
        cout << "4. Exit\n";
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
                parkingLot.addVehicle();
                break;
            case 2:
                system("CLS");
                parkingLot.checkVehicle();
                break;
            case 3:
                system("CLS");
                parkingLot.removeVehicle();
                break;
            case 4:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "[ERROR] Invalid option. Please try again.\n";
        }
    }
    return 0;
}






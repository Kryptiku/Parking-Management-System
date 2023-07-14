#include <ctime>
#include <conio.h>
#include <iostream>
#include <map>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
class Vehicle {
private:
    string plateNumber;
    string vehicleType;
    time_t entryTime;
    double feePerHour;
public:
    Vehicle(string plateNumber, string vehicleType, double feePerHour)
        : plateNumber(plateNumber), vehicleType(vehicleType), feePerHour(feePerHour) {
        this->entryTime = time(0);
    }
    string getPlateNumber() { return this->plateNumber; }
    string getVehicleType() { return this->vehicleType; }
    time_t getEntryTime() { return this->entryTime; }
    double getFeePerHour() { return this->feePerHour; }
};

class ParkingLot {
private:
    map<string, Vehicle*> parking;
    map<string, int> spaces = { {"Car", 50}, {"Truck", 30}, {"Motor/Bike", 10} };
    map<string, double> fees = { {"Car", 5.0}, {"Truck", 7.0}, {"Motor/Bike", 3.0} };
    void readDataFromCSV();
public:
    string csvFileName;
    ParkingLot(string fileName) : csvFileName(fileName) {
        readDataFromCSV();
    }

    // Counter
    int countCars() {
        int carCount = 0;
    
        for(const auto& vehicle : parking) {
            if(vehicle.second->getVehicleType() == "Car") {
                carCount++;
            }
        }
        return carCount;
    }

    int countTrucks() {
        int truckCount = 0;

        for(const auto& vehicle : parking) {
            if(vehicle.second->getVehicleType() == "Truck") {
                truckCount++;
            }
        }
        return truckCount;
    }

    int countBikes() {
        int bikeCount = 0;

        for(const auto& vehicle : parking) {
            if(vehicle.second->getVehicleType() == "Motor/Bike") {
                bikeCount++;
            }
        }
        return bikeCount;
    }

    //Availability of Slots
    int countAvailableSlots(const string& vehicleType) {
        if(spaces.find(vehicleType) == spaces.end()) {
            return -1; //In case the vehicle type is not valid
        }

        int totalSlots = spaces[vehicleType];
        int occupiedSlots = 0;

        for(const auto& vehicle : parking) {
            if(vehicle.second->getVehicleType() == vehicleType) {
                occupiedSlots++;
            }
        }
        return totalSlots - occupiedSlots;
    }

    // Main Menu
    int menu() {
        int choice;
        cout << "\n[PARKING MANAGEMENT SYSTEM MENU]\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Check Vehicle\n";
        cout << "3. Remove Vehicle\n";
        cout << "4. Exit Program\n";
        cout << "Please enter your choice: ";
        cin >> choice;
        cin.ignore();
        return choice;
    };

    // Add Vehicle
    void addVehicle() {
        string plateNumber;
        string optionChoice;
        int loopInput;
        string vehicleType;
        double feePerHour;

        do {
            system("cls");
            loopInput = 0;
            cout << "Available parking spaces:\n";
            ParkingLot parkingLot("parking_lot_database.csv");

            int availableCarSlots = parkingLot.countAvailableSlots("Car");
            cout << "Car: " << availableCarSlots << endl;

            int availableTruckSlots = parkingLot.countAvailableSlots("Truck");
            cout << "Truck: " << availableTruckSlots << endl;
            
            int availableBikeSlots = parkingLot.countAvailableSlots("Motor/Bike");
            cout << "Motor/Bike: " << availableBikeSlots << endl;
            
            cout << "Enter vehicle type\n";
            cout << "[1] Car\n";
            cout << "[2] Truck\n";
            cout << "[3] Motor/Bike\n";
            cout << "Please enter your choice: ";
            cin >> optionChoice;
            cin.ignore();
            
            if(optionChoice == "1") {
                vehicleType = "Car";
                loopInput = 1;
            }

            else if(optionChoice == "2") {
                vehicleType = "Truck";
                loopInput = 1;
            }

            else if(optionChoice == "3") {
                vehicleType = "Motor/Bike";
                loopInput = 1; 
            }

            else {
                cout << "[ERROR] Invalid vehicle type.\n";
                cout << "Please try again.";
                cout << "Press any key to continue...";
                getch();
            }
        }
        while(loopInput == 0);

        if (spaces.find(vehicleType) == spaces.end()) {
            cout << "\n[ERROR] Invalid vehicle type.\n";
            return;
        }

        if (spaces[vehicleType] <= 0) {
            cout << "\n[INFO] No available space for " << vehicleType << ".\n";
            return;
        }
        
        feePerHour = fees[vehicleType];

        cout << "Enter plate number: ";
        getline(cin, plateNumber);

        parking.insert(pair<string, Vehicle*>(plateNumber, new Vehicle(plateNumber, vehicleType, feePerHour)));

        cout << "\n[INFO] " << vehicleType << " " << plateNumber << " added successfully.\n";

        ofstream outputFile(csvFileName, ios::app);
        if(outputFile.is_open()) {
            outputFile << "Plate Number" << "," << "Vehicle Type" << "," << "Fee Per Hour" << "\n";
            outputFile << plateNumber << "," << vehicleType << "," << feePerHour << "\n";
            outputFile.close();
        }
        else {
            cout << "[ERROR] Failed to open the CSV file for writing. \n";
            return;
        }
    }

    // Remove Vehicle
    void removeVehicle() {
        string plateNumber;
        cout << "Enter the plate number: ";
        getline(cin, plateNumber);
        auto vehicle = parking.find(plateNumber);

        if (vehicle != parking.end()) {
            time_t exitTime = time(0);
            time_t entryTime = vehicle->second->getEntryTime();
            double totalFee = difftime(exitTime, entryTime) / 3600 * vehicle->second->getFeePerHour();

            cout << "\n[RECEIPT]\n";
            cout << "Plate Number: " << vehicle->second->getPlateNumber() << "\n";
            cout << "Vehicle Type: " << vehicle->second->getVehicleType() << "\n";
            cout << "Time In: " << ctime(&entryTime);
            cout << "Time Out: " << ctime(&exitTime);
            cout << "Total Fee: $" << totalFee << "\n";

            delete vehicle->second;
            parking.erase(vehicle);
        } else {
            cout << "\n[ERROR] Vehicle " << plateNumber << " not found.\n";
        }

        ofstream outputFile(csvFileName);
        if(outputFile.is_open()) {
            for(const auto& vehicle:parking) {
                outputFile << vehicle.first << "," << vehicle.second->getVehicleType() << "," << vehicle.second->getFeePerHour() << "\n";
            }
            outputFile.close();
        }
        else {
            cout << "[ERROR] Failed to open the CSV file for writing.\n";
        }
    }

    // Check Vehicle
    void checkVehicle() {
        string plateNumber;
        cout << "Enter the plate number: ";
        getline(cin, plateNumber);
        auto vehicle = parking.find(plateNumber);
        if (vehicle != parking.end()) {
            cout << "\n[INFO] Vehicle " << vehicle->second->getPlateNumber() << " is present.\n";
        } else {
            cout << "\n[ERROR] Vehicle " << plateNumber << " not found.\n";
        }
    }

    // Print all Vehicles
    void printAllVehicles() {
        for (auto const& vehicle : parking)
            cout << "Plate Number: " << vehicle.first << ", Vehicle Type: " << vehicle.second->getVehicleType() << "\n";
    }


};
    //Readin Data in Database
    void ParkingLot::readDataFromCSV() {
        ifstream inputFile(csvFileName);

        if(!inputFile.is_open()) {
            cout << "[ERROR] Failed to open the CSV file.\n";
            return;
        }

        string line;
        while(getline(inputFile, line)) {
            istringstream iss(line);
            string plateNumber, vehicleType;
            double feePerHour;
            if(getline(iss, plateNumber, ',') && getline(iss, vehicleType, ',') && iss >> feePerHour) {
                parking[plateNumber] = new Vehicle(plateNumber, vehicleType, feePerHour);
            }
        }
        inputFile.close();
    }

// Driver Code
int main() {

    ParkingLot parkingLot("parking_lot_database.csv");
    parkingLot.csvFileName = "parking_lot_database.csv";

    while(true) {
        int choice = parkingLot.menu();
        switch (choice) {
            case 1: {
                system("cls");
                parkingLot.addVehicle();
                break;
            }
            case 2: {
                system("cls");
                parkingLot.printAllVehicles();
                parkingLot.checkVehicle();
                break;
            }
            case 3: {
                system("cls");
                parkingLot.removeVehicle();
                break;
            }
            case 4: {
                cout << "Exiting the program...\n";
                return 0;
            }
            default:
                cout << "[ERROR] Invalid option. Please enter a number from 1 to 4.\n";
                break;
        }
    }
}
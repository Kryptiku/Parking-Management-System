#include <ctime>
#include <iostream>
#include <map>
#include <limits>

using namespace std;

class Vehicle {
private:
    string plateNumber;
    string vehicleType;
    time_t entryTime;
    int duration;
    double feePerHour;
public:
    Vehicle(string plateNumber, string vehicleType, int duration, double feePerHour)
        : plateNumber(plateNumber), vehicleType(vehicleType), duration(duration), feePerHour(feePerHour) {
        this->entryTime = time(0);
    }
    string getPlateNumber() { return this->plateNumber; }
    string getVehicleType() { return this->vehicleType; }
    time_t getEntryTime() { return this->entryTime; }
    int getDuration() { return this->duration; }
    double getFeePerHour() { return this->feePerHour; }
};

class ParkingLot {
private:
    map<string, Vehicle*> parking;
public:
    ParkingLot() {}

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
    }

    // Add Vehicle
    void addVehicle() {
        string plateNumber;
        string vehicleType;
        int duration;
        double feePerHour = 5.0;

        cout << "\nEnter plate number: ";
        getline(cin, plateNumber);
        cout << "Enter vehicle type: ";
        getline(cin, vehicleType);
        cout << "Enter parking duration (in hours): ";
        cin >> duration;
        cin.ignore();

        cout << "\n[CONFIRMATION PAGE]\n";
        cout << "Plate Number: " << plateNumber << "\n";
        cout << "Vehicle Type: " << vehicleType << "\n";
        cout << "Duration: " << duration << " hour(s)\n";
        cout << "Fee Per Hour: $" << feePerHour << "\n";
        cout << "Total Estimated Fee: $" << feePerHour * duration << "\n";

        string confirmation;
        cout << "\nConfirm to add this vehicle? (y/n): ";
        getline(cin, confirmation);

        if (confirmation == "y" || confirmation == "Y") {
            parking.insert(pair<string, Vehicle*>(plateNumber, new Vehicle(plateNumber, vehicleType, duration, feePerHour)));
            cout << "\n[INFO] Vehicle " << plateNumber << " added successfully.\n";
        } else {
            cout << "\n[INFO] Vehicle addition cancelled.\n";
        }
    }

    // Remove Vehicle
    void removeVehicle(string plateNumber) {
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
            throw runtime_error("[ERROR] Vehicle " + plateNumber + " not found.\n");
        }
    }

    // Check Vehicle
    void checkVehicle(string plateNumber) {
        auto vehicle = parking.find(plateNumber);
        if (vehicle != parking.end()) {
            cout << "\n[INFO] Vehicle " << vehicle->second->getPlateNumber() << " is present.\n";
        } else {
            throw runtime_error("[ERROR] Vehicle " + plateNumber + " not found.\n");
        }
    }
};

// Driver Code
int main() {
    ParkingLot parkingLot;
    while (true) {
        try {
            int choice = parkingLot.menu();

            switch (choice) {
                case 1: {
                    system("CLS");
                    parkingLot.addVehicle();
                    break;
                }
                case 2: {
                    system("CLS");
                    string plateNumber;
                    cout << "Enter the plate number: ";
                    getline(cin, plateNumber);
                    parkingLot.checkVehicle(plateNumber);
                    break;
                }
                case 3: {
                    string plateNumber;
                    cout << "Enter the plate number: ";
                    getline(cin, plateNumber);
                    parkingLot.removeVehicle(plateNumber);
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
        } catch (runtime_error& e) {
            cout << e.what();
        }
    }
}


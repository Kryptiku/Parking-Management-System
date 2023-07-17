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
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


struct carDetails {
    string plateNo;

    //time is saved as 24 hour format like 1350 and such
    int entryTime;
    int exitTime;
    int duration;

    double parkingFee;
    double finalFee;

    bool isMember;
    string memberLevel;

    int parkingColumn;
    int parkingRow;

    void getData(ifstream &inCar) {
        getline(inCar >> ws, plateNo, ';');
        inCar >> entryTime;
        inCar.ignore();
        inCar >> parkingRow;
        inCar.ignore();
        inCar >> parkingColumn;
        inCar.ignore();
    }

    void displayData() {
        cout << plateNo << endl;
        cout << entryTime << endl;
    }
};

struct carMembershipDetails {
    string plateNo;
    string ownerName;
    string memberLevel;

    void getData(ifstream &inMembers) {
        getline(inMembers >> ws, plateNo, ';');
        getline(inMembers >> ws, ownerName, ';');
        getline(inMembers >> ws, memberLevel);
    }

    void displayData() {
        cout << "plateNo: " << plateNo << " ";
        cout << "ownerName: " << ownerName << " ";
        cout << "memberLevel " << memberLevel << " ";
    }
};

struct membershipDetails {
    string memberLevel;
    double disountRate;
};



void inputCars (carDetails[], ifstream &, int &);
void assignParkingLot(carDetails[], string[][20]);
void displayparkingLot(string [][20]);
void assignMembership(carMembershipDetails [], ifstream &, int &);
void displayCarMembershipDetails(carMembershipDetails [], int);
void addVehicle(carDetails [], string [][20], int &);
void assignActiveCarParkingLot(carDetails [], string [][20], int );
void removeVehicles(carDetails [], string [][20], int &);
void removeFromParkingLot(string, string [][20]);

int main() {
    ifstream inCar("active car.txt");
    ifstream inMembers("active membership.txt");
    if (!inCar) {
        cout << "Error reading \"active car.txt\"";
    }
    if (!inMembers) {
        cout << "Erro reading \"active membership.txt\"";
    }
    
    // inputting the active cars into array, and setting up the parking lot
    //all parking lot are empty, and will be filled with active cars after
    //parking lot will hold the plateNo of each cars
    string parkingLot[10][20];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            parkingLot[i][j] = "";
        }
    }

    //active cars inside the file already have their own respective parking lot assigned to them when they entered before.
    carDetails activeCars[200];
    int activeCarsCount = 0;
    inputCars(activeCars, inCar, activeCarsCount);
    //assign the active car into their own respective parkingLot
    assignActiveCarParkingLot(activeCars, parkingLot, activeCarsCount);

    //3 membership levels
    const membershipDetails membership[3] {
        {"GOLD", 0.05},
        {"PLANTIUM", 0.10},
        {"DIAMOND", 0.15}
    };

    carDetails exitedVehicles[200];
    int exitedVehicleCount;

    //assign all the current memberships inside the membership file into membershipArray.
    carMembershipDetails carMembershipDetails[200];
    int membershipCount = 0;
    assignMembership(carMembershipDetails, inMembers, membershipCount);

        while(true) {
            cout << "Vehicle Ticketing Menu, Please input the number as below\n";

            cout << "1. Vechicle Entry\n"
                << "2. Vehicle Exit\n"
                << "3. Membership Management\n"
                << "4. Search Vechicle\n"
                << "5. Current Statistics\n"
                << "6. End Day and Generate Report\n"
                << "7. Exit Program\n";

            int input;
            cout << "Enter thee input ";
            cin >> input;

            if (input == 7) {
                break;
            }

            switch (input) {
                case 1:
                    addVehicle(activeCars, parkingLot, activeCarsCount);
                    break;

                case 2:
                    removeVehicles(activeCars, parkingLot, activeCarsCount);
                    break;

                case 5:
                    displayparkingLot(parkingLot);
                    break;

                default:
                    cout << "Invalid option.\n";
                    continue;
            }
            
        
        }

}


void inputCars(carDetails activeCars[], ifstream &inCar, int &activeCarsCount) {
    //this while inputs the active car file into the active car array
    //and if input is successfull it returns true
    while (!inCar.eof()) {
        activeCars[activeCarsCount].getData(inCar);
        activeCarsCount++;
    }
}

void assignActiveCarParkingLot(carDetails activeCar[], string parkingLot[][20], int activeCarCount) {
    for (int i = 0; i < activeCarCount; i++) {
        parkingLot[activeCar[i].parkingRow][activeCar[i].parkingColumn] = activeCar[i].plateNo; 
    }
}

bool assignParkingLot(carDetails &newCar, string parkingLot[][20]) {
    //while assigning the parking lot of new car, its row and column is saved in the car struct/object
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (parkingLot[i][j] == "") {
                parkingLot[i][j] = newCar.plateNo;
                newCar.parkingColumn = j;
                newCar.parkingRow = i;
                return true;
            }
        }
    }

    return false;
}

void assignMembership(carMembershipDetails carMembershipDetails[], ifstream &inMembers, int &membershipCount) {
    while (!inMembers.eof()) {
        carMembershipDetails[membershipCount].getData(inMembers);
        membershipCount++;
    }
}

void displayparkingLot(string parkingLot[][20]) {    
    cout << "\n\n Parking Lot \n\n";
    for (int i = 0; i < 10; i++) {
        cout  << "Row " << i+1 << endl;
        for (int j = 0; j < 20; j++) {
            cout << parkingLot[i][j];
            cout << " || ";
        }
        cout << endl;
    }
}

void displayCarMembershipDetails(carMembershipDetails carMembershipDetails[], int membershipCount) {
    for (int i = 0; i < membershipCount; i++) {
        cout << "Member " << i + 1 << " ";
        carMembershipDetails[i].displayData();
        cout << endl;
    }
}

void addVehicle(carDetails activeCars[], string parkingLot[][20], int &activeCarsCount) {
    carDetails newCar;

    cout << "Enter Plate No: ";
    getline(cin >> ws, newCar.plateNo);

    cout << "Enter Entry Time: ";
    cin >> newCar.entryTime;

    bool isParked = assignParkingLot(newCar, parkingLot);

    if (!isParked) {
        cout << "Parking Full";
        return;
    }

    cout << "Found Parking at " 
        << newCar.parkingRow << "," << newCar.parkingColumn << endl;

    activeCars[activeCarsCount++] = newCar;
}

void removeFromParkingLot(string plateNo, string parkingLot[][20]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (parkingLot[i][j] == plateNo) {
                parkingLot[i][j] = "";
                return;
            }
        }
    }
}

void removeVehicles(carDetails activeCars[], string parkingLot[][20], int &activeCarsCount, carDetails exitedVehicles[], int &exitedVehicleCount) {
    string exitVehiclePlate;
    cout << "No Plate To Remove: ";
    getline(cin >> ws, exitVehiclePlate);

    int targetIndex = -1;
    bool found = false;

    //getting the target index;
    for (int i = 0; i < activeCarsCount; i++) {
        if (activeCars[i].plateNo == exitVehiclePlate) {
            targetIndex = i;
            found = true;
            cout << "Vehicle found!" << endl;
            break;
        }
    }

    if (found == false) {
        cout << "wrong plate no, please retry" << endl;
        return;
    }

   addExitedVehicles(exitedVehicles ,activeCars[targetIndex], exitedVehicleCount);

    removeFromParkingLot(exitVehiclePlate, parkingLot);

    // Shift elements to the left to overwrite the target
    for (int i = targetIndex; i < activeCarsCount - 1; i++) {
        activeCars[i] = activeCars[i + 1];
    }

    activeCarsCount--;
    cout << "Vehicle successfully exited" << endl;
}

void addExitedVehicles(carDetails exitedVehicles[], carDetails exitingCars, int exitedVehicleCount) {
    exitedVehicles[exitedVehicleCount].plateNo = exitingCars.plateNo;  
    exitedVehicles[exitedVehicleCount].entryTime = exitingCars.entryTime;  
    cout << "Enter Exit Time for current vehicle: ";
    cin >> exitedVehicles[exitedVehicleCount].exitTime; 
    exitedVehicles[exitedVehicleCount].duration = calculateDuration();  
}

int calculateDuration() {

}

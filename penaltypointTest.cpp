#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;


struct carDetails {
    string plateNo;

    //time is saved as 24 hour format like 1350 and such
    int entryTime;
    int exitTime;
    int duration;
    
    bool isMember;
    string memberLevel;
    
    double parkingFee;
    double finalFee;

    int parkingColumn;
    int parkingRow;

    int penaltyPoints = 0;

    void getData(ifstream &inCar) {
        getline(inCar >> ws, plateNo, ';');
        inCar >> entryTime;
        inCar.ignore();
        inCar >> parkingRow;
        inCar.ignore();
        inCar >> parkingColumn;
        inCar.ignore();
        inCar >> penaltyPoints;
        inCar.ignore();
    }

    void displayData() {
        cout << plateNo << endl;
        cout << entryTime << endl;
        cout << penaltyPoints << endl;
    }
};

struct activeMembership {
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
    double memberPrice;
    double disountRate;
};



void inputCars (carDetails[], ifstream &, int &);
void assignParkingLot(carDetails[], string[][20]);
void displayparkingLot(string [][20]);
void assignMembership(activeMembership [], ifstream &, int &);
void displayActiveMembership(activeMembership [], int);
void addVehicle(carDetails [], string [][20], int &);
void assignActiveCarParkingLot(carDetails [], string [][20], int );
void removeVehicles(carDetails [], string[][20], int &, carDetails [], int &);void removeFromParkingLot(string, string [][20]);
void addExitedVehicles(carDetails [], carDetails , int &);
int calculateDuration(int, int);
int getMembershipManageMentInput();
void processMembershipManagementInput(int, activeMembership [], membershipDetails [], int &);


//3 membership levels
const membershipDetails membership[3] {
        {"GOLD", 20, 0.05},
        {"PLANTIUM", 27, 0.10},
        {"DIAMOND", 35, 0.15}
    };


    int main() {
    ifstream inCar("active car.txt");
    ifstream inMembers("active membership.txt");
    if (!inCar) {
        cout << "Error reading \"active car.txt\"";
        return 1;
    }
    if (!inMembers) {
        cout << "Erro reading \"active membership.txt\"";
        return 1;
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

    carDetails exitedVehicles[200];
    int exitedVehicleCount = 0;

    //assign all the current memberships inside the membership file into membershipArray.
    activeMembership activeMembership[200];
    int activeMembershipCount = 0;
    assignMembership(activeMembership, inMembers, activeMembershipCount);

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
            if (!(cin >> input)) {
                cout << "Invalid input" << endl;
                cin.clear();
                cin.ignore();
                continue;
            }

            if (input == 7) {
                break;
            }

            switch (input) {
                case 1: {
                    addVehicle(activeCars, parkingLot, activeCarsCount);
                    break;
                }
                case 2: {
                    removeVehicles(activeCars, parkingLot, activeCarsCount, exitedVehicles, exitedVehicleCount);
                    break;
                }
                case 3: {
                    int memberInput = getMembershipManageMentInput();
                    cout << endl << memberInput;
                    processMembershipManagementInput(memberInput, activeMembership, activeMembershipCount);
                    break;
                }
                case 5: {
                    displayparkingLot(parkingLot);
                    break;
                }
                default: {
                    cout << "Invalid option.\n";
                    break;
                }
            }
            
        
        }


        /*
        cout << "\n\nCURRENT EXITED CAR: \n\n";
        cout << "number of exited vehicles: " << exitedVehicleCount << endl;
        for (int i = 0; i < exitedVehicleCount; i++)  {
            cout << exitedVehicles[i].plateNo << endl;
            cout << exitedVehicles[i].entryTime<< endl;
            cout << exitedVehicles[i].exitTime << endl;
            cout << exitedVehicles[i].duration << endl;
        }

        cout << "\n\nCURRENT ACTIVE CAR\n\n";
        for (int i = 0; i < activeCarsCount; i++) {
            activeCars[i].displayData();
        }*/
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

void assignMembership(activeMembership activeMembership[], ifstream &inMembers, int &activeMembershipCount) {
    while (!inMembers.eof()) {
        activeMembership[activeMembershipCount].getData(inMembers);
        activeMembershipCount++;
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

    removeFromParkingLot(exitVehiclePlate, parkingLot);

    addExitedVehicles(exitedVehicles, activeCars[targetIndex], exitedVehicleCount);

    // Shift elements to the left to overwrite the target
    for (int i = targetIndex; i < activeCarsCount - 1; i++) {
        activeCars[i] = activeCars[i + 1];
    }

    activeCarsCount--;
    cout << "Vehicle successfully exited" << endl;
}

void addExitedVehicles(carDetails exitedVehicles[], carDetails exitingCars, int &exitedVehicleCount) {
    int exitTime;
    cout << "Enter Exit Time for current vehicle: ";
    cin >> exitTime;
    if (exitTime < exitedVehicles[exitedVehicleCount].entryTime) {
        cout << "Invalid exit time. please retry" << endl;
        return;
    }

    exitedVehicles[exitedVehicleCount].plateNo = exitingCars.plateNo;  
    exitedVehicles[exitedVehicleCount].entryTime = exitingCars.entryTime;  
    exitedVehicles[exitedVehicleCount].exitTime = exitTime; 
    //exitedVehicles[exitedVehicleCount].duration = calculateDuration();  
    exitedVehicles[exitedVehicleCount].duration = calculateDuration(exitingCars.entryTime, exitTime);  
    
    exitedVehicleCount++;
}


int calculateDuration(int entryTime, int exitTime){
    int startMinutes = ((entryTime / 100) * 60) + (entryTime % 100);
    int endMinutes = ((exitTime / 100) * 60) + (exitTime % 100);

    return endMinutes - startMinutes;
}

int getMembershipManageMentInput() {
    cout << "\n============= MEMBERSHIP MANAGEMENT ===============\n\n";
    cout << "1. Add Member\n"
        << "2. Remove Member\n"
        << "3. View Membership Details\n"
        << "4. View Active Membership\n";
    
    int memberInput;
    cout << "\nPlease enter the number: ";
    cin >> memberInput;
    return memberInput;
}   


void addMember(activeMembership activeMembership[], int &activeMembershipCount) {
    cout << "Enter plateNo:";
    getline(cin >> ws, activeMembership[activeMembershipCount].plateNo);
    cout << "Enter ownerName: ";
    getline(cin >> ws, activeMembership[activeMembershipCount].ownerName);
    cout << "Enter membership level: ";
    getline(cin >> ws, activeMembership[activeMembershipCount].memberLevel);
}

void removeMember(activeMembership activeMembership[], int &activeMembershipCount) {
    
}

void displayMembershipDetails(){
    //membership global variable
}

void displayActiveMembership(activeMembership activeMembership[], int activeMembershipCount) {
    for (int i = 0; i < activeMembershipCount; i++) {
        cout << "Member " << i + 1 << " ";
        activeMembership[i].displayData();
        cout << endl;
    }
}
void processMembershipManagementInput(int memberInput, activeMembership activeMembership[], int &activeMembershipCount) {
    switch (memberInput) {
        case 1: {
            addMember(activeMembership, activeMembershipCount);
        }
        case 2: {
            removeMember(activeMembership, activeMembershipCount);
        } 
        case 3: {
            displayMembershipDetails();
        }
        case 4 :
            displayActiveMembership(activeMembership, activeMembershipCount);

    }
}
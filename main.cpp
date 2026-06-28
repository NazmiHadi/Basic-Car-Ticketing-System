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
    
    double parkingFee = 0;
    double finalFee = 0;

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

struct parkingSystem {
    string parkingLot[10][20];

    carDetails activeCars[200];
    int activeCarsCount = 0;

    carDetails exitedVehicles[200];
    int exitedVehicleCount = 0;

    activeMembership activeMembership[200];
    int activeMembershipCount = 0;
};


void inputCars(parkingSystem &system, ifstream &inCar);
void assignActiveCarParkingLot(parkingSystem &system);
bool assignParkingLot(carDetails &newCar, parkingSystem &system);
void assignMembership(parkingSystem &system, ifstream &inMembers);
void displayparkingLot(const parkingSystem &system);
void addVehicle(parkingSystem &system);
void removeFromParkingLot(string plateNo, parkingSystem &system);
void removeVehicles(parkingSystem &system);
void addExitedVehicles(parkingSystem &system, carDetails exitingCars);
int calculateDuration(int entryTime, int exitTime);


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
    
    parkingSystem system;

    // at program initialization, parking lot will be empty. then will be filled with active cars and new cars and so on
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            system.parkingLot[i][j] = "";
        }
    }

    //input the active car from file into active car array inside system
    inputCars(system, inCar);
    //fill the parking lot with active cars / cars that have been here before program initialization
    assignActiveCarParkingLot(system);
    //input the active membership from file into active membership array inside system
    assignMembership(system, inMembers);

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

        //edge case for string input
        if (!(cin >> input)) {
            cout << "\nInvalid input" << endl;
            cin.clear();
            cin.ignore();

            cout << "\n\n =================================== \n\n";
            continue;
        }

        if (input == 7) {
            break;
        }

        switch (input) {
            case 1: {
                addVehicle(system);
                break;
            }
            case 2: {
                removeVehicles(system);
                break;
            }
            /*case 3: {
                int memberInput = getMembershipManageMentInput();
                processMembershipManagementInput(memberInput, activeMembership, activeMembershipCount);
                break;
            }*/
            case 5: {
                displayparkingLot(system);
                break;
            }
            default: {
                cout << "Invalid option.\n";
                cout << "\n\n =================================== \n\n";
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


void inputCars(parkingSystem &system, ifstream &inCar) {
    //this while inputs the active car file into the active car array
    //and if input is successfull it returns true
    while (!inCar.eof()) {
        system.activeCars[system.activeCarsCount].getData(inCar);
        system.activeCarsCount++;
    }
}

void assignActiveCarParkingLot(parkingSystem &system) {
    for (int i = 0; i < system.activeCarsCount; i++) {
        system.parkingLot[system.activeCars[i].parkingRow][system.activeCars[i].parkingColumn] = system.activeCars[i].plateNo; 
    }
}

bool assignParkingLot(carDetails &newCar, parkingSystem &system) {
    //while adding a new car into active car, it is assigned a parking row and column if available
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (system.parkingLot[i][j] == "") {
                system.parkingLot[i][j] = newCar.plateNo;

                cout << "i: " << i << endl;
                cout << "j: " << j << endl;

                newCar.parkingColumn = j;
                newCar.parkingRow = i;
                
                cout << "i: " << i << endl;
                cout << "j: " << j << endl;

                return true;
            }
        }
    }

    return false;
}

void assignMembership(parkingSystem &system, ifstream &inMembers) {
    while (!inMembers.eof()) {
        system.activeMembership[system.activeMembershipCount].getData(inMembers);
        system.activeMembershipCount++;
    }
}


//system is a large struct, and passing by value means copying a very large value. this uses alot of recources
//instead, i use pass by reference and uses const so that this function cannot edit the original value and can only use them.
void displayparkingLot(const parkingSystem &system) {    
    cout << "\n\n =================== Parking Lot ========================== \n";
    for (int i = 0; i < 10; i++) {
        cout  << "Row " << i+1 << endl;
        for (int j = 0; j < 20; j++) {
            cout << system.parkingLot[i][j];
            cout << " || ";
        }
        cout << endl;
    }
    cout << "\n==============================================================\n\n";
}

void addVehicle(parkingSystem &system) {
    carDetails newCar;

    cout << "\n\n ============== CAR ENTRY ==================\n";

    cout << "Enter Plate No: ";
    getline(cin >> ws, newCar.plateNo);

    cout << "Enter Entry Time: ";
    cin >> newCar.entryTime;

    bool isParked = assignParkingLot(newCar, system);

    if (!isParked) {
        cout << "Parking Full";
        return;
    }

    cout << "Found Parking at " 
        << newCar.parkingRow << "," << newCar.parkingColumn << endl;

    system.activeCars[system.activeCarsCount++] = newCar;

    cout << "\n =============================================\n\n";
}

void removeFromParkingLot(string plateNo, parkingSystem &system) {
    //upon removal of vehicles, the parking lot will also be emptied
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (system.parkingLot[i][j] == plateNo) {
                system.parkingLot[i][j] = "";
                return;
            }
        }
    }
}

void removeVehicles(parkingSystem &system) {
    cout << "\n\n ============== CAR EXIT ==================\n";


    string exitVehiclePlate;
    cout << "No Plate To Remove: ";
    getline(cin >> ws, exitVehiclePlate);

    int targetIndex = -1;
    bool found = false;

    //getting the target index;
    for (int i = 0; i < system.activeCarsCount; i++) {
        if (system.activeCars[i].plateNo == exitVehiclePlate) {
            targetIndex = i;
            found = true;
            cout << "Vehicle found!" << endl;
            break;
        }
    }

    if (found == false) {
        cout << "wrong plate no, please retry" << endl;
        cout << "\n =============================================\n\n";

        return;
    }

    removeFromParkingLot(exitVehiclePlate, system);

    addExitedVehicles(system, system.activeCars[targetIndex]);

    // Shift elements to the left to overwrite the target
    for (int i = targetIndex; i < system.activeCarsCount - 1; i++) {
        system.activeCars[i] = system.activeCars[i + 1];
    }

    system.activeCarsCount--;
    cout << "Vehicle successfully exited" << endl;

    cout << "\n =============================================\n\n";

}


void addExitedVehicles(parkingSystem &system, carDetails exitingCars) {
    int exitTime;
    cout << "Enter Exit Time for current vehicle: ";
    cin >> exitTime;
    if (exitTime < system.exitedVehicles[system.exitedVehicleCount].entryTime) {
        cout << "Invalid exit time. please retry" << endl;
        return;
    }

    system.exitedVehicles[system.exitedVehicleCount].plateNo = exitingCars.plateNo;  
    system.exitedVehicles[system.exitedVehicleCount].entryTime = exitingCars.entryTime;  
    system.exitedVehicles[system.exitedVehicleCount].exitTime = exitTime; 
    system.exitedVehicles[system.exitedVehicleCount].duration = calculateDuration(exitingCars.entryTime, exitTime);  
    
    system.exitedVehicleCount++;
}


int calculateDuration(int entryTime, int exitTime){
    int startMinutes = ((entryTime / 100) * 60) + (entryTime % 100);
    int endMinutes = ((exitTime / 100) * 60) + (exitTime % 100);

    return endMinutes - startMinutes;
}

/*
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

void addMember(activeMembership activeMembership[], int &activeMembershipCount, carDetails &car) {
    string plateNo;
    string membershipLevel;
    string ownerName;

    //check noPlate is already registered and membership from the available membership
    cout << "Enter plateNo:";
    getline(cin >> ws, plateNo);
    if (checkMembershipPlateNo(plateNo, activeMembership, activeMembershipCount)) {
        cout << "Plate No already registered!" << endl;
        return;
    }

    //when adding a member, it means that a car is paying upfront, and therefore has be currently parked, meaning it is inside activeVehicle. cheking that
    carDetails searchCarByPlateNo(plateNo, activeCar, activeCarsCount) {


    cout << "Enter membership level: ";
    getline(cin >> ws, membershipLevel);
    if (!checkMembership(membershipLevel)) {
        cout << "Invalid Membershiplevel!" << endl;
        return;
    }
    cout << "Enter ownerName: ";
    getline(cin >> ws, ownerName);

    activeMembership[activeMembershipCount].plateNo = plateNo; 
    activeMembership[activeMembershipCount].memberLevel = membershipLevel; 
    activeMembership[activeMembershipCount].ownerName = ownerName;

    activeMembershipCount++;

    //when adding member that means that a car is a new member and have to pay upfront. price will be added to the noPlate's total Cost
    addMemberPriceToTotal(membershipLevel, car);

    
}

void removeMember(activeMembership activeMembership[], int &activeMembershipCount) {
    
}

void displayMembershipDetails(){
    for (int i = 0; i < 3; i++) {

    }
}

void displayActiveMembership(activeMembership activeMembership[], int activeMembershipCount) {
    cout << "\n\n========== ACTIVE MEMBERSHIP ================\n\n";
    for (int i = 0; i < activeMembershipCount; i++) {
        cout << "Member " << i + 1 << " ";
        activeMembership[i].displayData();
        cout << endl;
    }

    cout << "\n==================================\n\n";

}
void processMembershipManagementInput(int memberInput, activeMembership activeMembership[], int &activeMembershipCount) {
    switch (memberInput) {
        case 1: {
            addMember(activeMembership, activeMembershipCount,);
            break;
        }
        case 2: {
            removeMember(activeMembership, activeMembershipCount);
            break;
        } 
        case 3: {
            displayMembershipDetails();
            break;
        }
        case 4 :
            displayActiveMembership(activeMembership, activeMembershipCount);
            break;

    }
}

bool checkMembershipPlateNo(string plateNo, activeMembership activeMembership[], int activeMembershipCount) {
    for (int i = 0; i < activeMembershipCount; i++) {
        if (activeMembership[i].plateNo == plateNo) {
            return true;
        }
    }
    return false;
}

bool checkMembership(string membershipLevel) {
    //using i < 3 because membership is a constant and the client does not want to expand it anytime soon :)
    for (int i = 0; i < 3; i++) {
        if (membership[i].memberLevel == membershipLevel) {
            return true;
        }
    }
    return false;
}

void addMemberPriceToTotal(string memberLevel, carDetails &car) {
    int price;
    for (int i = 0; i < 3; i++) {
        if (membership[i].memberLevel = memberLevel) {
            price = membership[i].memberPrice;
            break;
        }
    }

    car.finalFee += price;
}

carDetails searchCarByPlateNo(string plateNo, carDetails activeCar[], int activeCarsCount) {
    for (int i = 0; i < activeCarsCount; i++) {
        if (activeCar[i].plateNo == plateNo) {
            return activeCar[i];
            break;
        }
    }
}
*/
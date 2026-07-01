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
};

struct member {
    string plateNo;
    string ownerName;
    string memberLevel;
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

    member activeMembership[200];
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
int getIndexFromPlateNo(const parkingSystem system, string plateNo);
void addExitedVehicles(parkingSystem &system, carDetails exitingCars);
void displayParkingLocation(const parkingSystem &system);
int calculateDuration(int entryTime, int exitTime);

int getMembershipmanagementInput();
void processMembershipManagementInput(parkingSystem &system, int memberInput);
void addMember(parkingSystem &system);
void removeMember(member activeMembership[], int &activeMembershipCount);
void displayMembershipDetails();
void displayActiveMembership(const parkingSystem &system);
bool checkMembershipPlateNo(const parkingSystem &system, string plateNo);
bool checkMembership(string membershipLevel);
void addMemberPriceToTotal(parkingSystem &system, string memberLevel, int targetIndex);
void removeMember(parkingSystem &system);
double calculateFinalFee(carDetails exitedCar);
int searchActiveMembershipWithPlateNo(const parkingSystem &system, string plateNo);

void displayCurrentStatistics(parkingSystem &system);
void displayActiveCars(carDetails activeCars[], int activeCarsCount);
void displayActiveMembership(member activeMembership[], int activeMembershipCount);
void displayExitedVehicles(carDetails exitedVehicles[], int exitedVehiclesCount);
void displayAndWriteEndDayReport(parkingSystem &system,ofstream &outAllStatistics, ofstream outActiveMembers, ofstream outActiveVehicles, ofstream);
double getDiscountRate(string memberLevel);


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
            << "3. View Parking Lot\n"
            << "4. Membership Management\n"
            << "5. Search Vechicle Location in Parking Lot\n"
            << "6. Current Statistics\n"
            << "7. End Day and Generate Report\n"
            << "8. Exit Program\n";

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
        cin.ignore();

        if (input == 8) {
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
            case 3: {
                displayparkingLot(system);
                break;
            }
            case 4: {
                int memberInput = getMembershipmanagementInput();
                cin.ignore();
                processMembershipManagementInput(system, memberInput);
                break;
            }
            case 5: {
                displayParkingLocation(system);
                break;
            }
            case 6: {
                displayCurrentStatistics(system);
                break;
            }
            case 7: {
                //End Day and Generate Report
                //reuse current statistics
                //display all current statistics, write the current statistics and exit to main
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
        getline(inCar, system.activeCars[system.activeCarsCount].plateNo, ';');
        inCar >> system.activeCars[system.activeCarsCount].entryTime;
        inCar.ignore();
        inCar >> system.activeCars[system.activeCarsCount].parkingRow;
        inCar.ignore();
        inCar >> system.activeCars[system.activeCarsCount].parkingColumn;
        inCar.ignore();
        inCar >> system.activeCars[system.activeCarsCount].penaltyPoints;
        inCar.ignore();
        system.activeCarsCount++;
    }    
}



void inputMembers(parkingSystem &system, ifstream &inmember){

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
                newCar.parkingColumn = j;
                newCar.parkingRow = i;
                return true;
            }
        }
    }

    return false;
}

void assignMembership(parkingSystem &system, ifstream &inMembers) {
    while (!inMembers.eof()) {
        getline(inMembers, system.activeMembership[system.activeMembershipCount].plateNo, ';');
        getline(inMembers, system.activeMembership[system.activeMembershipCount].ownerName, ';');
        getline(inMembers, system.activeMembership[system.activeMembershipCount].memberLevel);;
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
    getline(cin, newCar.plateNo);

    cout << "Enter Entry Time: ";
    cin >> newCar.entryTime;
    cin.ignore();

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

void displayActiveMember(member activeMembers) {
    cout << "plateNo: " << activeMembers.plateNo << " ";
    cout << "ownerName: " << activeMembers.ownerName << " ";
    cout << "memberLevel " << activeMembers.memberLevel << " ";
}

void removeVehicles(parkingSystem &system) {
    cout << "\n\n ============== CAR EXIT ==================\n";


    string exitVehiclePlate;
    cout << "No Plate To Remove: ";
    getline(cin, exitVehiclePlate);

    //getting the target index;
    int targetIndex = getIndexFromPlateNo(system, exitVehiclePlate);

    if (targetIndex == -1) {
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

int getIndexFromPlateNo(const parkingSystem system, string plateNo) {
     for (int i = 0; i < system.activeCarsCount; i++) {
        if (system.activeCars[i].plateNo == plateNo) {
            return i;
        }
    }
    return -1;
}


void addExitedVehicles(parkingSystem &system, carDetails exitingCars) {
    int exitTime;
    cout << "Enter Exit Time for current vehicle: ";
    cin >> exitTime;
    cin.ignore();
    if (exitTime < exitingCars.entryTime) {
        cout << "Invalid exit time. please retry" << endl;
        return;
    }

    system.exitedVehicles[system.exitedVehicleCount].plateNo = exitingCars.plateNo;  
    system.exitedVehicles[system.exitedVehicleCount].entryTime = exitingCars.entryTime;  
    system.exitedVehicles[system.exitedVehicleCount].exitTime = exitTime; 
    system.exitedVehicles[system.exitedVehicleCount].duration = calculateDuration(exitingCars.entryTime, exitTime);  
    system.activeCars[system.exitedVehicleCount].finalFee = calculateFinalFee(system.exitedVehicles[system.exitedVehicleCount]);

    system.exitedVehicleCount++;
}

void displayParkingLocation(const parkingSystem &system) {
    cout << "\n============= SEARCH PARKING ===============\n\n";

    string plateNo;
    cout << "Enter the plate no of the vehicle you are looking for: ";
    getline(cin, plateNo);

    int targetIndex = getIndexFromPlateNo(system, plateNo);

    if (targetIndex == -1) {
        cout << "Plate No not found\n\n\n";
        return;
    }

    cout << "Your car is at row " << system.activeCars[targetIndex].parkingRow << " And Column : " << system.activeCars[targetIndex].parkingColumn;
    cout << "\n =============================================\n\n";
}


int calculateDuration(int entryTime, int exitTime){
    int startMinutes = ((entryTime / 100) * 60) + (entryTime % 100);
    int endMinutes = ((exitTime / 100) * 60) + (exitTime % 100);

    return endMinutes - startMinutes;
}


double calculateFinalFee(carDetails exitedCar) {
    int totalDuration = (exitedCar.duration / 60) * 1; //duration in minutes
    double discountRate = getDiscountRate(exitedCar.memberLevel);
    int penaltyTotal = exitedCar.penaltyPoints * 20;

    return totalDuration + discountRate + penaltyTotal;
}

/// FLOWCHART ENDS HERRE ///
////////////////////////////
////////////////////////////
////////////////////////////


int getMembershipmanagementInput() {
    cout << "\n============= MEMBERSHIP MANAGEMENT ===============\n\n";
    cout << "1. Add Member\n"
    << "2. Remove Member\n"
    << "3. View Membership Details\n"
    << "4. View Active Membership\n"
    << "5. Back";
    
    int memberInput;
    cout << "\nPlease enter the number: ";
    cin >> memberInput;
    return memberInput;
}   

void processMembershipManagementInput(parkingSystem &system, int memberInput) {
    switch (memberInput) {
        case 1: {
            addMember(system);
            break;
        }
        case 2: {
            removeMember(system);
            break;
        } 
        case 3: {
            displayMembershipDetails();
            break;
        }
        case 4 :
            displayActiveMembership(system);
            break;
        case 5 :
            cout << "\n\n\n";
            return;
    }
}

void addMember(parkingSystem &system) {
    cout << "\n\n ============== MEMBERSHIP REGISTRATION ==================\n";


    string plateNo;
    string membershipLevel;
    string ownerName;
    
    //check noPlate is already registered and membership from the available membership
    cout << "Enter plateNo:";
    getline(cin, plateNo);
    int memberIndex = searchActiveMembershipWithPlateNo(system, plateNo) ;
    if (memberIndex != -1) {
        cout << "Plate No already registered!\n\n\n" << endl;
        return;
    }
    //since people that is registering have to be inside the parking lot to register, we search if their vehicle is in the parking lot
    int targetIndex = getIndexFromPlateNo(system, plateNo);
    if (targetIndex == -1) {
        cout << "Plate No isnt in the parking lot!\n\n\n" << endl;
        return;
    }
    
    cout << "Enter membershipLevel: ";
    getline(cin, membershipLevel);
    if(!checkMembership(membershipLevel)) {
        cout << "Membership doesnt exist!\n\n\n" << endl;
        return;
    }

    

    cout << "Enter Owner Name: ";
    getline(cin, ownerName);
    
    system.activeMembership[system.activeMembershipCount].plateNo = plateNo; 
    system.activeMembership[system.activeMembershipCount].memberLevel = membershipLevel; 
    system.activeMembership[system.activeMembershipCount].ownerName = ownerName; 
    system.activeMembershipCount++;

    //when adding member that means that a car is a new member and have to pay upfront. price will be added to the noPlate's total Cost
    addMemberPriceToTotal(system, membershipLevel, targetIndex);
    system.activeCars[targetIndex].memberLevel = membershipLevel;

    cout<< "\n VEHICLE ADDED!\n";
    cout << "\n==============================================================\n\n";
}


void displayMembershipDetails(){
    cout << "\n\n ============== MEMBERSHIP DETAILS ==================\n";
    cout<< setw(3) << "" << "________________________________________________" << endl;
    cout << left << setw(5) << "" << setw(20) << "MEMBERSHIP NAME" << setw(10) <<"PRICE" << setw(15) << "DISCOUNT RATE" << endl;
    cout<< setw(3) << "" << "________________________________________________" << endl;
    
    for (int i = 0; i < 3; i++) {
        cout << left << setprecision(2) << setw(10) << ""
            << setw(20) << membership[i].memberLevel 
            << setw(10) << membership[i].disountRate
            << setw(15) << membership[i].memberPrice
            << endl;
    }
    cout << "\n==============================================================\n\n";

}

void displayActiveMembership(const parkingSystem &system) {
    cout << "\n\n========== ACTIVE MEMBERSHIP ================\n\n";
    for (int i = 0; i < system.activeMembershipCount; i++) {
        cout << "Member " << i + 1 << " ";
        displayActiveMember(system.activeMembership[i]);
        cout << endl;
    }

    cout << "\n==================================\n\n";

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

void addMemberPriceToTotal(parkingSystem &system, string memberLevel, int targetIndex) {
    int price;
    for (int i = 0; i < 3; i++) {
        if (membership[i].memberLevel == memberLevel) {
            price = membership[i].memberPrice;
            break;
        }
    }
    system.activeCars[targetIndex].finalFee += price;
}
void removeMember(parkingSystem &system) {
    cout << "\n\n============= MEMBERSHIP REMOVAL ==================\n";

    string plateNo;
    cout << "Enter Plate No: ";
    getline(cin, plateNo);

    int targetIndex = searchActiveMembershipWithPlateNo(system, plateNo);

    if (targetIndex == -1) {
        cout << "Plate No doesn't exist!\n";
        return;
    }

    for (int i = targetIndex; i < system.activeMembershipCount - 1; i++) {
        system.activeMembership[i] = system.activeMembership[i + 1];
    }

    system.activeMembershipCount--;

    cout << "\nMember removed successfully!\n";
    cout << "\n==============================================================\n\n";
}

int searchActiveMembershipWithPlateNo(const parkingSystem &system, string plateNo) {
    for (int i = 0; i < system.activeMembershipCount; i++) {
        if (system.activeMembership[i].plateNo == plateNo) {
            return i;
        }
    }
    return -1;
}

void displayCurrentStatistics(parkingSystem &system) {
    displayparkingLot(system);
    displayActiveCars(system.activeCars, system.activeCarsCount);
    displayActiveMembership(system.activeMembership, system.activeMembershipCount);
    displayExitedVehicles(system.exitedVehicles, system.exitedVehicleCount);
}

void displayActiveCars(carDetails activeCars[], int activeCarsCount) {
    cout << "\n\n============= CURRENT ACTIVE CARS ==================\n";
    cout << left
         << setw(20) << "Plate Number"
         << setw(15) << "Entry Time"
         << endl;

    for (int i = 0; i < activeCarsCount; i++) {
        cout << left << setw(20) << activeCars[i].plateNo;
        cout << setw(2) << setfill('0') << activeCars[i].entryTime / 100
             << ":"
             << setw(2) << setfill('0') << activeCars[i].entryTime % 100;
        cout << endl;
    }

    cout << "\nTotal Active Cars: " << activeCarsCount << endl;
    cout << "\n==============================================================\n\n";
}

void displayActiveMembership(member activeMembership[], int activeMembershipCount) {
    cout << "\n\n============= CURRENT ACTIVE MEMBERSHIP ==================\n";
    cout << setw(15) <<  "Plate Number"
        << setw(15) << "Owner Name"
        << setw(20) << "Membership Level"
        << endl;

    for (int i = 0; i < activeMembershipCount; i++) {
        cout << setw(15) << activeMembership[i].plateNo;
        cout << setw(15) << activeMembership[i].ownerName;
        cout << setw(20) << activeMembership[i].memberLevel;
        cout << endl;
    }

    cout << "Total Active Membership: " << activeMembershipCount << "\n\n";
    cout << "\n==============================================================\n\n";
}

void displayExitedVehicles(carDetails exitedVehicles[], int exitedVehiclesCount) {
    cout << "\n\n============= ALL EXITED CARS ==================\n";
    cout << setprecision(2)
        << setw(15) <<  "Plate Number"
        << setw(15) << "Entry Time"
        << setw(15) << "Exited Time"
        << setw(15) << "Total Fee"
        << endl;

    for (int i = 0; i < exitedVehiclesCount; i++) {
        cout << setw(15) << exitedVehicles[i].plateNo
            << setw(15) << exitedVehicles[i].entryTime/100 << ":" << exitedVehicles[i].entryTime%100
            << setw(15) << exitedVehicles[i].exitTime/100 <<":" << exitedVehicles[i].exitTime%100
            << setw(15) << exitedVehicles[i].finalFee
            << endl;

    }

    cout << "Total Exited Cars Today: " << exitedVehiclesCount << "\n\n";
    cout << "\n==============================================================\n\n";
}

void displayAndWriteEndDayReport(parkingSystem &system,ofstream &outAllStatistics, ofstream outActiveMembers, ofstream outActiveVehicles, ofstream) {

}

double getDiscountRate(string memberLevel) {
    for (int i = 0; i < 3; i++) {
        if (memberLevel == membership[i].memberLevel) {
            return 1 + membership[i].disountRate;
        }
    }
    return 0;
}


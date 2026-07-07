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
    
    bool isMember = false;
    string memberLevel;
    double membershipCost = 0;
    
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
    double discountRate;
};


struct parkingSystem {
    string parkingLot[10][20];

    carDetails activeCars[200];
    int activeCarsCount = 0;

    carDetails exitedCars[200];
    int exitedCarCount = 0;

    member activeMembership[200];
    int activeMembershipCount = 0;
};


void inputCars(parkingSystem &system, ifstream &inCar);
void assignActiveCarParkingLot(parkingSystem &system);
bool assignParkingLot(carDetails &newCar, parkingSystem &system);
void assignMembership(parkingSystem &system, ifstream &inMembers);
void assignMembershipToActiveCars(parkingSystem &system);
void displayparkingLot(const parkingSystem &system);
void addCar(parkingSystem &system);
void removeFromParkingLot(string plateNo, parkingSystem &system);
void removeCars(parkingSystem &system);
int getIndexFromPlateNo(const parkingSystem &system, string plateNo);
void addExitedCars(parkingSystem &system, carDetails exitingCars, int exitTime);
void displayParkingLocation(const parkingSystem &system);
int calculateDuration(int entryTime, int exitTime, int daysParked);

void getMembershipmanagementInput(int *memberInput);
void processMembershipManagementInput(parkingSystem &system, int *memberInput);
void addMember(parkingSystem &system);
void displayMembershipDetails();
void displayActiveMembership(const parkingSystem &system);
bool checkMembership(string membershipLevel);
void addMemberPrice(parkingSystem &system, string memberLevel, int targetIndex);
void removeMember(parkingSystem &system);
double calculateFinalFee(carDetails exitedCar);
int searchActiveMembershipWithPlateNo(const parkingSystem &system, string plateNo);

void displayCurrentStatistics(parkingSystem &system);
void displayActiveCars(carDetails activeCars[], int activeCarsCount);
void displayActiveMembership(member activeMembership[], int activeMembershipCount);
void displayExitedCars(carDetails exitedCars[], int exitedCarsCount);
void displayAndWriteEndDayReport(parkingSystem &system,ofstream &outAllStatistics, ofstream &outActiveMembers, ofstream &outExitedCar);
double getDiscountRate(string memberLevel);
void writeAllStatistics(parkingSystem &system,ofstream &out);
void writeExitedCars(carDetails exitedCars[], int exitedCarsCount, ofstream &out);
void writeActiveMembership(member activeMembership[], int activeMembershipCount, ofstream &out);
void writeActiveCars(carDetails activeCars[], int activeCarsCount, ofstream &out);
void rewriteActiveCars(parkingSystem &system);
void rewriteActiveMembership(parkingSystem &system);
void displayReceipt(carDetails car);
void writeReceipt(carDetails car);

//3 membership levels
const membershipDetails membership[3] {
    {"GOLD", 20, 0.05},
    {"PLANTINUM", 27, 0.10},
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
    
    ofstream outAllStatistics("All Statistics Report.txt");
    ofstream outActiveMembers("Active Members Report.txt");
    ofstream outExitedCar("Exited Car Report.txt");

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
    assignMembershipToActiveCars(system);

    while(true) {
        cout << "Car Ticketing Menu, Please input the number as below\n";
        cout << "1. Vechicle Entry\n"
            << "2. Car Exit\n"
            << "3. View Parking Lot\n"
            << "4. Membership Management\n"
            << "5. Search Vechicle Location in Parking Lot\n"
            << "6. Current Statistics\n"
            << "7. End Day and Generate Report\n"
            << "8. Exit Program\n";

        int input;
        cout << "Enter Selection Number (1-8) ";

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
                addCar(system);
                break;
            }
            case 2: {
                removeCars(system);
                break;
            }
            case 3: {
                displayparkingLot(system);
                break;
            }
            case 4: {
                // POINTERRRRRR!!!!!!!!!!!!!!!!!
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
                cout << "\nGenerating End of Day Report...\n";
                inCar.close();
                displayAndWriteEndDayReport(system, outAllStatistics, outActiveMembers, outExitedCar);
                return 1;
            }
            default: {
                cout << "Invalid option.\n";
                cout << "\n\n =================================== \n\n";
                break;
            }
        }
        
    
        }

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

void assignMembershipToActiveCars(parkingSystem &system) {
    for (int i = 0; i < system.activeCarsCount; i++) {
        int memberIndex = searchActiveMembershipWithPlateNo(system, system.activeCars[i].plateNo);
        if (memberIndex != -1) {
            system.activeCars[i].isMember = true;
            system.activeCars[i].memberLevel = system.activeMembership[memberIndex].memberLevel;   
            addMemberPrice(system, system.activeCars[i].memberLevel, i);  
        }
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

void displayParkingLocation(const parkingSystem &system) {
    cout << "\n\n ============== SEARCH Car LOCATION ==================\n";

    string plateNo;
    cout << "Enter Plate No: ";
    getline(cin, plateNo);

    int targetIndex = getIndexFromPlateNo(system, plateNo);

    if (targetIndex == -1) {
        cout << "Car not found in the parking lot.\n";
        cout << "\n=============================================\n\n";
        return;
    }

    cout << "Car " << plateNo << " is parked at Row " << system.activeCars[targetIndex].parkingRow + 1 << " and column " << system.activeCars[targetIndex].parkingColumn + 1<< endl;

    cout << "\n=============================================\n\n";
}

void addCar(parkingSystem &system) {
    carDetails newCar;

    cout << "\n\n ============== CAR ENTRY ==================\n";

    cout << "Enter Plate No: ";
    getline(cin, newCar.plateNo);

    //check if Car is already is in parking lot or not
    if (getIndexFromPlateNo(system, newCar.plateNo) != -1) {
        cout << "Car already exists in the parking lot.\n";
        cout << "\n=============================================\n\n";
        return;
    }

    cout << "Enter Entry Time: ";
    cin >> newCar.entryTime;
    cin.ignore();

    bool isParked = assignParkingLot(newCar, system);

    if (!isParked) {
        cout << "Parking Full";
        return;
    }

    int memberIndex = searchActiveMembershipWithPlateNo(system, newCar.plateNo);
    if (memberIndex != -1) {
        newCar.isMember = true;
        newCar.memberLevel = system.activeMembership[memberIndex].memberLevel;
    }

    cout << "Found Parking at row " << newCar.parkingRow + 1<< " and column " << newCar.parkingColumn + 1  << endl;

    system.activeCars[system.activeCarsCount++] = newCar;

    cout << "\n =============================================\n\n";
}

void removeFromParkingLot(string plateNo, parkingSystem &system) {
    //upon removal of Cars, the parking lot will also be emptied
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

void removeCars(parkingSystem &system) {
    cout << "\n\n ============== CAR EXIT ==================\n";


    string exitCarPlate;
    cout << "No Plate To Remove: ";
    getline(cin, exitCarPlate);

    //getting the target index;
    int targetIndex = getIndexFromPlateNo(system, exitCarPlate);

    if (targetIndex == -1) {
        cout << "wrong plate no, please retry" << endl;
        cout << "\n =============================================\n\n";

        return;
    }

    int exitTime;
    cout << "Enter Exit Time for current Car: ";
    cin >> exitTime;
    cin.ignore();
    if (system.activeCars[targetIndex].penaltyPoints == 0 &&
        exitTime < system.activeCars[targetIndex].entryTime) {
        cout << "Invalid exit time. please retry" << endl;
        return;
    }

    removeFromParkingLot(exitCarPlate, system);

    addExitedCars(system, system.activeCars[targetIndex], exitTime);

    // Shift elements to the left to overwrite the target
    for (int i = targetIndex; i < system.activeCarsCount - 1; i++) {
        system.activeCars[i] = system.activeCars[i + 1];
    }

    system.activeCarsCount--;
    cout << "Car successfully exited" << endl;

    writeReceipt(system.exitedCars[system.exitedCarCount - 1]);
    displayReceipt(system.exitedCars[system.exitedCarCount - 1]);
    cout << "\n =============================================\n\n";

}

int getIndexFromPlateNo(const parkingSystem &system, string plateNo) {
     for (int i = 0; i < system.activeCarsCount; i++) {
        if (system.activeCars[i].plateNo == plateNo) {
            return i;
        }
    }
    return -1;
}


void addExitedCars(parkingSystem &system, carDetails exitingCars, int exitTime) {

    system.exitedCars[system.exitedCarCount].plateNo = exitingCars.plateNo;
    system.exitedCars[system.exitedCarCount].entryTime = exitingCars.entryTime;
    system.exitedCars[system.exitedCarCount].exitTime = exitTime;
    system.exitedCars[system.exitedCarCount].duration = calculateDuration(exitingCars.entryTime, exitTime, exitingCars.penaltyPoints);
    system.exitedCars[system.exitedCarCount].parkingFee = 2 * (system.exitedCars[system.exitedCarCount].duration / 60);
    system.exitedCars[system.exitedCarCount].memberLevel = exitingCars.memberLevel;
    system.exitedCars[system.exitedCarCount].isMember = exitingCars.isMember;
    system.exitedCars[system.exitedCarCount].penaltyPoints = exitingCars.penaltyPoints;
    system.exitedCars[system.exitedCarCount].membershipCost = exitingCars.membershipCost;
    system.exitedCars[system.exitedCarCount].finalFee = calculateFinalFee(system.exitedCars[system.exitedCarCount]);
    system.exitedCarCount++;
}


int calculateDuration(int entryTime, int exitTime, int daysParked) {
    int startMinutes = ((entryTime / 100) * 60) + (entryTime % 100);
    int endMinutes = ((exitTime / 100) * 60) + (exitTime % 100);

    return (endMinutes - startMinutes) + (daysParked * 24 * 60);
}


double calculateFinalFee(carDetails exitedCar) {
    double penaltyCost = exitedCar.penaltyPoints * 20;
    double discount = getDiscountRate(exitedCar.memberLevel);
    double total = exitedCar.parkingFee + exitedCar.membershipCost + penaltyCost;
    return total * (1 - discount);
}

/// FLOWCHART ENDS HERRE ///
////////////////////////////
////////////////////////////
////////////////////////////


void getMembershipmanagementInput(int *memberInput) {
    cout << "\n============= MEMBERSHIP MANAGEMENT ===============\n\n";
    cout << "1. Add Member\n"
    << "2. Remove Member\n"
    << "3. View Membership Details\n"
    << "4. View Active Membership\n"
    << "5. Back";
    
    cout << "\nPlease enter the number: ";
    cin >> *memberInput;
}

void processMembershipManagementInput(parkingSystem &system, int *memberInput) {
    switch (*memberInput) {
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
    //since people that is registering have to be inside the parking lot to register, we search if their Car is in the parking lot
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
    system.activeCars[targetIndex].isMember = true;
    system.activeMembership[system.activeMembershipCount].ownerName = ownerName; 
    system.activeMembershipCount++;

    //when adding member that means that a car is a new member and have to pay upfront. price will be added to the noPlate's total Cost
    addMemberPrice(system, membershipLevel, targetIndex);
    system.activeCars[targetIndex].memberLevel = membershipLevel;

    cout<< "\n Car ADDED!\n";
    cout << "\n==============================================================\n\n";
}


void displayMembershipDetails(){
    cout << "\n\n ============== MEMBERSHIP DETAILS ==================\n";
    cout<< setw(3) << "" << "________________________________________________" << endl;
    cout << left << setw(30) << "MEMBERSHIP NAME" << setw(30) <<"PRICE" << setw(30) << "DISCOUNT RATE" << endl;
    cout<< setw(3) << "" << "________________________________________________" << endl;
    
    for (int i = 0; i < 3; i++) {
        cout << left << fixed << setprecision(2) << setw(10) << ""
            << setw(30) << membership[i].memberLevel 
            << setw(35) << membership[i].memberPrice
            << setw(30) << membership[i].discountRate
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

void addMemberPrice(parkingSystem &system, string memberLevel, int targetIndex) {
    double price = 0;
    for (int i = 0; i < 3; i++) {
        if (membership[i].memberLevel == memberLevel) {
            price = membership[i].memberPrice;
            break;
        }
    }
    system.activeCars[targetIndex].membershipCost = price;
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


    int carIndex = getIndexFromPlateNo(system, plateNo);
    if (carIndex != -1) {
        system.activeCars[carIndex].isMember = false;
        system.activeCars[carIndex].memberLevel = "";
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
    displayExitedCars(system.exitedCars, system.exitedCarCount);
}

void displayActiveCars(carDetails activeCars[], int activeCarsCount) {
    cout << "\n\n============= CURRENT ACTIVE CARS ==================\n";
    cout << left
         << setw(30) << "Plate Number"
         << setw(30) << "Entry Time"
         << setw(8) << "Overstayed Days"
         << endl;

    for (int i = 0; i < activeCarsCount; i++) {
        cout << left 
            << setw(30) << activeCars[i].plateNo
            << setw(30) << activeCars[i].entryTime
            << setw(8) << activeCars[i].penaltyPoints << " Days"
            << endl;
    }

    cout << "\nTotal Active Cars: " << activeCarsCount << endl;
    cout << "\n==============================================================\n\n";
}

void displayActiveMembership(member activeMembership[], int activeMembershipCount) {
    cout << "\n\n============= CURRENT ACTIVE MEMBERSHIP ==================\n";
    cout << setw(30) << "Plate Number"
        << setw(30) << "Owner Name"
        << setw(30) << "Membership Level"
        << endl;

    for (int i = 0; i < activeMembershipCount; i++) {
        cout << setw(30) << activeMembership[i].plateNo
            << setw(30) << activeMembership[i].ownerName
            << setw(30) << activeMembership[i].memberLevel
            << endl;
    }

    cout << "Total Active Membership: " << activeMembershipCount << "\n\n";
    cout << "\n==============================================================\n\n";
}

void displayExitedCars(carDetails exitedCars[], int exitedCarsCount) {
    cout << "\n\n============= ALL EXITED CARS ==================\n";
    cout << left
         << setw(30) << "Plate Number"
         << setw(30) << "Entry Time"
         << setw(30) << "Exit Time"
         << setw(30) << "Overstayed Days"
         << setw(30) << "Total Fee"
         << endl;

    for (int i = 0; i < exitedCarsCount; i++) {

        cout << left
            << setw(30) << exitedCars[i].plateNo
            << setw(30) << exitedCars[i].entryTime
            << setw(30) << exitedCars[i].exitTime
            << setw(30) << exitedCars[i].penaltyPoints
            << fixed << setprecision(2)
            << setw(30) << exitedCars[i].finalFee
            << endl;
    }

    cout << "\nTotal Exited Cars Today: " << exitedCarsCount << endl;
    cout << "\n==============================================================\n\n";
}

void displayAndWriteEndDayReport(parkingSystem &system,ofstream &outAllStatistics, ofstream &outActiveMembers, ofstream &outExitedCar) {
    writeAllStatistics(system, outAllStatistics);
    writeExitedCars(system.exitedCars, system.exitedCarCount, outExitedCar);
    writeActiveMembership(system.activeMembership, system.activeMembershipCount, outActiveMembers);
    rewriteActiveCars(system);
    rewriteActiveMembership(system); 
}
void writeAllStatistics(parkingSystem &system,ofstream &out) {
    writeExitedCars(system.exitedCars, system.exitedCarCount, out);
    cout << "\n\n ======================================================= \n\n";
    writeActiveMembership(system.activeMembership, system.activeMembershipCount, out);
    cout << "\n\n ======================================================= \n\n";
    writeActiveCars(system.activeCars, system.activeCarsCount, out);

}

void writeExitedCars(carDetails exitedCars[], int exitedCarsCount, ofstream &out) {
    out << "\n\n============= ALL EXITED CARS ==================\n";
    out << left
         << setw(30) << "Plate Number"
         << setw(30) << "Entry Time"
         << setw(30) << "Exit Time"
         << setw(30) << "Overstayed Days"
         << setw(30) << "Total Fee"
         << endl;

    for (int i = 0; i < exitedCarsCount; i++) {

        out << left
            << setw(30) << exitedCars[i].plateNo
            << setw(30) << exitedCars[i].entryTime
            << setw(30) << exitedCars[i].exitTime
            << setw(30) << exitedCars[i].penaltyPoints
            << fixed << setprecision(2)
            << setw(30) << exitedCars[i].finalFee
            << endl;
    }

    out << "\nTotal Exited Cars Today: " << exitedCarsCount << endl;
    out << "\n==============================================================\n\n";
}

void writeActiveMembership(member activeMembership[], int activeMembershipCount, ofstream &out) {
    out << "\n\n============= CURRENT ACTIVE MEMBERSHIP ==================\n";
    out << setw(30) <<  "Plate Number"
        << setw(30) << "Owner Name"
        << setw(20) << "Membership Level"
        << endl;

    for (int i = 0; i < activeMembershipCount; i++) {
        out << setw(30) << activeMembership[i].plateNo;
        out << setw(30) << activeMembership[i].ownerName;
        out << setw(20) << activeMembership[i].memberLevel;
        out << endl;
    }

    out << "Total Active Membership: " << activeMembershipCount << "\n\n";
    out << "\n==============================================================\n\n";
}

void writeActiveCars(carDetails activeCars[], int activeCarsCount, ofstream &out) {
    out << "\n\n============= CURRENT ACTIVE CARS ==================\n";
    out << left
         << setw(20) << "Plate Number"
         << setw(30) << "Entry Time"
         << endl;

    for (int i = 0; i < activeCarsCount; i++) {
        out << left << setw(20) << activeCars[i].plateNo;
        out << activeCars[i].entryTime;
        out << endl;
    }

    out << "\nTotal Active Cars: " << activeCarsCount << endl;
    out << "\n==============================================================\n\n";
}

void rewriteActiveCars(parkingSystem &system) {
    //declaring this here as to not overrwrite the input file
    ofstream out("active car.txt"); 

    for (int i = 0; i < system.activeCarsCount; i++) {

        system.activeCars[i].penaltyPoints += 1;
        out << system.activeCars[i].plateNo << ";"
            << system.activeCars[i].entryTime << ";"
            << system.activeCars[i].parkingRow << ";"
            << system.activeCars[i].parkingColumn << ";"
            << system.activeCars[i].penaltyPoints;
        
        if (i < (system.activeCarsCount - 1)) {
            out << "\n";
        }
        
    }
}

void rewriteActiveMembership(parkingSystem &system) {
     ofstream out("active membership.txt"); 
    for (int i = 0; i < system.activeMembershipCount; i++) {
        out << system.activeMembership[i].plateNo << ";"
            << system.activeMembership[i].ownerName << ";"
            << system.activeMembership[i].memberLevel;
        if (i < (system.activeMembershipCount - 1)) {
            out << "\n";
        }
    }
}



double getDiscountRate(string memberLevel) {
    for (int i = 0; i < 3; i++) {
        if (memberLevel == membership[i].memberLevel) {
            return membership[i].discountRate;
        }
    }
    return 0;
}

void displayReceipt(carDetails car) {
    cout << "\n\n============= CUSTOMER RECEIPT ==================\n";

    cout << left;
    cout << setw(30) << "Plate Number:" << car.plateNo << endl;
    cout << setw(30) << "Entry Time:" << car.entryTime << endl;
    cout << setw(30) << "Exit Time:" << car.exitTime << endl;
    cout << setw(30) << "Total Duration:" << car.duration << " minutes" << endl;
    cout << setw(30) << "Parking Fee:" << "RM" << car.parkingFee << endl;
    cout << setw(30) << "Membership Cost:" << "RM" << car.membershipCost << endl;
    cout << setw(30) << "Membership Discount:" << getDiscountRate(car.memberLevel) * 100<< "%" << endl;
    cout << setw(30) << "Penalty Points:" << car.penaltyPoints << endl;
    cout << setw(30) << "Penalty Cost:" << "RM" << car.penaltyPoints * 20 << endl;
    cout << setw(30) << "Total Fee:" << "RM" << car.finalFee << endl;

    cout << "================================================\n";
}

void writeReceipt(carDetails car) {
    //Receipt Text file for latest customer
    ofstream out("customer receipt.txt");

    out << "\n\n============= CUSTOMER RECEIPT ==================\n";

    out << left;
    out << setw(30) << "Plate Number:" << car.plateNo << endl;
    out << setw(30) << "Entry Time:" << car.entryTime << endl;
    out << setw(30) << "Exit Time:" << car.exitTime << endl;
    out << setw(30) << "Total Duration:" << car.duration << " minutes" << endl;
    out << setw(30) << "Parking Fee:" << "RM" << car.parkingFee << endl;
    out << setw(30) << "Membership Cost:" << "RM" << car.membershipCost << endl;
    out << setw(30) << "Membership Discount:" << getDiscountRate(car.memberLevel) * 100 << "%" << endl;
    out << setw(30) << "Penalty Points:" << car.penaltyPoints << endl;
    out << setw(30) << "Penalty Cost:" << "RM" << car.penaltyPoints * 20 << endl;
    out << setw(30) << "Total Fee:" << "RM" << car.finalFee << endl;

    out << "================================================\n";
}

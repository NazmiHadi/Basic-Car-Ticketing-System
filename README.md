# Parking Management System - User Guide

This guide demonstrates how to use every feature of the Parking Management System, including normal operations and error handling scenarios.

---

# Before Running

Ensure the following files are in the same directory as the executable:

* `active car.txt`
* `active membership.txt`

The program will automatically load:

* Active vehicles currently parked
* Existing memberships

---

# Main Menu

```
1. Vehicle Entry
2. Car Exit
3. View Parking Lot
4. Membership Management
5. Search Vehicle Location
6. Current Statistics
7. End Day and Generate Report
8. Exit Program
```

---

# 1. Vehicle Entry

Adds a new vehicle into the parking lot.

### Example

```
Plate Number:
ABC1234

Entry Time:
1330
```

The system will:

* Check if the vehicle already exists
* Assign the first available parking space
* Detect whether the vehicle is already a registered member
* Add the vehicle into the active parking list

---

## Test Case 1 – Normal Entry

**Steps**

1. Select **Vehicle Entry**
2. Enter a new plate number
3. Enter a valid entry time

**Expected Result**

* Vehicle is parked successfully.
* Parking location is displayed.

---

## Test Case 2 – Duplicate Vehicle

**Steps**

1. Add vehicle `ABC1234`
2. Try adding `ABC1234` again

**Expected Result**

```
Car already exists in the parking lot.
```

---

## Test Case 3 – Returning Member

**Steps**

1. Register a vehicle as a member.
2. Exit the vehicle.
3. Enter the same vehicle again.

**Expected Result**

The system automatically recognises the vehicle as an existing member without requiring another registration.

---

# 2. Car Exit

Removes a vehicle from the parking lot and generates a receipt.

---

## Test Case 1 – Normal Exit

**Steps**

1. Select **Car Exit**
2. Enter an existing plate number
3. Enter a valid exit time

**Expected Result**

* Parking fee calculated
* Membership discount applied (if applicable)
* Receipt displayed
* Receipt saved as `customer receipt.txt`

---

## Test Case 2 – Vehicle Does Not Exist

**Steps**

Attempt to remove a vehicle that is not parked.

**Expected Result**

```
Wrong plate no, please retry
```

---

## Test Case 3 – Invalid Exit Time

**Steps**

Entry Time

```
1500
```

Exit Time

```
1300
```

(with no penalty days)

**Expected Result**

```
Invalid exit time. Please retry.
```

---

# 3. View Parking Lot

Displays the entire parking layout.

You can verify:

* Empty parking spaces
* Occupied parking spaces
* Vehicle locations

---

# 4. Membership Management

```
1. Add Member
2. Remove Member
3. View Membership Details
4. View Active Membership
5. Back
```

---

# Add Member

Registers a parked vehicle as a member.

---

## Test Case 1 – Normal Registration

**Steps**

1. Park a new vehicle.
2. Open Membership Management.
3. Select Add Member.
4. Enter:

* Plate Number
* Membership Level
* Owner Name

**Expected Result**

Vehicle becomes a member immediately.

---

## Test Case 2 – Register Existing Member

Attempt to register the same vehicle twice.

**Expected Result**

```
Plate No already registered!
```

---

## Test Case 3 – Register Vehicle Not Parked

Attempt to register a vehicle that is not inside the parking lot.

**Expected Result**

```
Plate No isn't in the parking lot!
```

---

## Test Case 4 – Invalid Membership Level

Example

```
SILVER
```

**Expected Result**

```
Membership doesn't exist!
```

---

# Remove Member

Removes an existing membership.

---

## Test Case 1 – Normal Removal

Select a registered member.

**Expected Result**

```
Member removed successfully!
```

---

## Test Case 2 – Remove Non-Existing Member

Enter an unknown plate number.

**Expected Result**

```
Plate No doesn't exist!
```

---

# Membership Persistence Test

This demonstrates that membership information is stored permanently.

## Steps

1. Add vehicle `ABC1234`
2. Register `ABC1234` as a GOLD member
3. Exit the vehicle
4. Enter `ABC1234` again
5. Exit the vehicle again

**Expected Result**

The second receipt automatically includes the GOLD membership discount without registering again.

---

# 5. Search Vehicle Location

Search for a parked vehicle using its plate number.

---

## Test Case 1 – Existing Vehicle

Enter a parked vehicle.

**Expected Result**

Displays:

* Row
* Column

---

## Test Case 2 – Vehicle Not Found

Enter a plate number that is not parked.

**Expected Result**

```
Car not found in the parking lot.
```

---

# 6. Current Statistics

Displays:

* Current parking lot
* Active vehicles
* Active members
* Vehicles exited today

Use this feature to verify that previous operations have updated the system correctly.

---

# 7. End Day and Generate Report

Generates all report files.

Created files:

* `All Statistics Report.txt`
* `Active Members Report.txt`
* `Exited Car Report.txt`
* `customer receipt.txt`

The system also updates:

* `active car.txt`
* `active membership.txt`

This preserves the latest parking and membership information for the next execution.

---

# Recommended Demonstration Flow

For a complete demonstration of the system:

1. Add a new vehicle.
2. Try adding the same vehicle again (duplicate validation).
3. Register the vehicle as a GOLD member.
4. Try registering it again (duplicate membership validation).
5. View Active Membership.
6. Search for the vehicle location.
7. Exit the vehicle and verify the receipt.
8. Try exiting the same vehicle again (vehicle not found validation).
9. Re-enter the same vehicle and verify it is automatically recognised as a member.
10. View Current Statistics.
11. Generate the End-of-Day Report.

This sequence demonstrates all major functionalities, data persistence, and error-handling capabilities of the Parking Management System.

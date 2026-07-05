# Parking System — Full Walkthrough (Sample Input & Expected Output)

This walkthrough assumes the program starts with the provided `active car.txt`
(33 pre-parked vehicles) and `active membership.txt` (22 pre-registered
members). It exercises every menu option at least once, in an order that
naturally sets up the data each later step needs.

Type each **Input** exactly as shown, one line at a time, at the matching
prompt. 

---

## Step 0 — Invalid main-menu input (error handling check)

| Prompt | Input |
|---|---|
| `Enter thee input` | `x` |

**Expected:** `Invalid input` message, menu reprints. No state changes.

---

## Step 1 — View Parking Lot (baseline)

| Prompt | Input |
|---|---|
| `Enter thee input` | `3` |

**Expected:** 10×20 grid printed. Rows 1–6 show plates in columns 1–8 (per
`active car.txt`); everything else is blank.

---

## Step 2 — Vehicle Entry

| Prompt | Input |
|---|---|
| `Enter thee input` | `1` |
| `Enter Plate No:` | `NEW001` |
| `Enter Entry Time:` | `2200` |

**Expected:** `Found Parking at row 1 and column 9` (first open slot — row 0
columns 0–7 are already full, so it lands at row index 0, column index 8).
`activeCarsCount` becomes 34.

---

## Step 3 — Search Vehicle Location

| Prompt | Input |
|---|---|
| `Enter thee input` | `5` |
| `Enter Plate No:` | `NEW001` |

**Expected:** `Vehicle NEW001 is parked at Row 1 and column 9` (confirms Step 2).

---

## Step 4 — Membership Management → Add Member

| Prompt | Input |
|---|---|
| `Enter thee input` | `4` |
| `Please enter the number:` | `1` |
| `Enter plateNo:` | `PAS909` |
| `Enter membershipLevel:` | `GOLD` |
| `Enter Owner Name:` | `Test Owner` |

**Expected:** `VEHICLE ADDED!`. `PAS909` (already parked, previously not a
member) is now GOLD, `activeMembershipCount` becomes 23.

---

## Step 5 — Membership Management → View Active Membership

| Prompt | Input |
|---|---|
| `Enter thee input` | `4` |
| `Please enter the number:` | `4` |

**Expected:** List of 23 members, including the new `PAS909 / Test Owner /
GOLD` entry at the bottom.

---

## Step 6 — Membership Management → View Membership Details

| Prompt | Input |
|---|---|
| `Enter thee input` | `4` |
| `Please enter the number:` | `3` |

**Expected:** Static table of GOLD (20, 0.05), PLANTIUM (27, 0.10), DIAMOND
(35, 0.15).

---

## Step 7 — Membership Management → Remove Member

| Prompt | Input |
|---|---|
| `Enter thee input` | `4` |
| `Please enter the number:` | `2` |
| `Enter Plate No:` | `PAS909` |

**Expected:** `Member removed successfully!`. `activeMembershipCount` back to
22. `PAS909`'s `isMember` flag is cleared.

> ⚠️ **Known gap to note in your report:** `removeMember()` clears
> `isMember` and `memberLevel` but does **not** reset `membershipCost` back
> to 0. If `PAS909` exits later in the same session, its old GOLD price
> (20) will still be added into `calculateFinalFee`, even though it's no
> longer a member. Step 9 below deliberately demonstrates this.

---

## Step 8 — Vehicle Exit (existing GOLD member)

| Prompt | Input |
|---|---|
| `Enter thee input` | `2` |
| `No Plate To Remove:` | `ABC123` |
| `Enter Exit Time for current vehicle:` | `1015` |

**Expected calculation** (assuming the `membershipCost` fix from this
session is applied — see note above the file section):
- Duration: 08:15 → 10:15 = 120 minutes
- Parking fee: 2 × 120 = 240
- Membership cost (GOLD): 20
- Penalty cost: 8 × 20 = 160
- Subtotal: 240 + 20 + 160 = 420
- GOLD discount 5%: 420 × 0.95 = **420.00 → 399.00 final fee**

Receipt should print these numbers; `activeCarsCount` drops to 33.

---

## Step 9 — Vehicle Exit (former member — demonstrates the leftover-cost gap)

| Prompt | Input |
|---|---|
| `Enter thee input` | `2` |
| `No Plate To Remove:` | `PAS909` |
| `Enter Exit Time for current vehicle:` | `2300` |

**Expected calculation:**
- Duration: 19:00 → 23:00 = 240 minutes
- Parking fee: 2 × 240 = 480
- Membership cost: **20** (leftover from Step 4/7 — the gap noted above; a
  fully-patched version would show 0 here)
- Penalty cost: 8 × 20 = 160
- Discount: 0% (not a member anymore)
- **Final fee: 480 + 20 + 160 = 660.00**

`activeCarsCount` drops to 32.

---

## Step 10 — Current Statistics

| Prompt | Input |
|---|---|
| `Enter thee input` | `6` |

**Expected:** Prints, in order — parking lot grid, active cars (32), active
membership (22 — note `ABC123` is still listed even though it exited; member
records aren't cleaned up on exit, a second minor gap worth mentioning if
your report covers data-consistency), exited vehicles (2: `ABC123`,
`PAS909`, with the fees from Steps 8–9).

---

## Step 11 — End Day and Generate Report

| Prompt | Input |
|---|---|
| `Enter thee input` | `7` |

**Expected:** Console prints `Generating End of Day Report...`, then writes:
- `All Statistics Report.txt`
- `Exited Vehicle Report.txt`
- `Active Members Report.txt`
- Rewrites `active car.txt` (remaining 32 cars, each with penalty points +1)
- Rewrites `active membership.txt` (remaining 22 members)

The program then **exits immediately** (`return 1`) — option `8` is never
reached in this walkthrough because option `7` already terminates the
program.

---

## Raw input sequence (for piping, e.g. `./program < inputs.txt`)

```
x
3
1
NEW001
2200
5
NEW001
4
1
PAS909
GOLD
Test Owner
4
4
4
3
4
2
PAS909
2
ABC123
1015
2
PAS909
2300
6
7
```

---

## Summary of what this run demonstrates

| Menu Option | Exercised in Step(s) |
|---|---|
| 1. Vehicle Entry | 2 |
| 2. Vehicle Exit | 8, 9 |
| 3. View Parking Lot | 1 |
| 4. Membership Management (all 4 sub-options) | 4, 5, 6, 7 |
| 5. Search Vehicle Location | 3 |
| 6. Current Statistics | 10 |
| 7. End Day Report | 11 |
| Invalid input handling | 0 |
[README.md](https://github.com/user-attachments/files/34965686/README.md)
# Employee Payroll Management System

A menu-driven console application written in **C** for managing employee records and payroll. It lets an administrator add, view, search, update and delete employees, calculates tax and net salary automatically, and generates a payslip for any employee. All records are stored permanently in a CSV file.

The project was developed as the term project (Complex Engineering Problem) of **CS-115 Computer Programming** at NED University of Engineering & Technology (FE Batch 2026, Fall Semester 2026).

## Table of Contents

- [Features](#features)
- [Contributors](#contributors)
- [Technologies](#technologies)
- [Project structure](#project-structure)
- [How to run](#how-to-run)
- [Using the program](#using-the-program)
- [Input validation rules](#input-validation-rules)
- [Salary calculation](#salary-calculation)
- [How the program works](#how-the-program-works)
- [Data file format](#data-file-format)
- [Known limitations](#known-limitations)
- [Future expansion](#future-expansion)

## Features

- **Admin login**: the menu is shown only after a correct username and password; the program closes after 3 failed attempts.
- **Add employee**: asks for 14 fields of data, validates every answer, and appends the record to the CSV file. Typing `exit` at the name prompt cancels the operation and returns to the menu.
- **Display all records**: shows every employee in one table, one employee per row.
- **Search**: finds an employee by ID (upper and lower case are treated the same) and shows all details.
- **Update**: changes a selected field of an employee. The new value is validated first, and tax and net salary are recalculated automatically when a salary value changes.
- **Delete**: removes an employee after a y/n confirmation.
- **Payslip**: prints a formatted payslip for the current month and year, with the bank account number partly hidden.
- **Permanent storage**: records are loaded from the CSV file at start-up and saved after every change, so nothing is lost when the program is closed.

## Contributors

| Contributor | Contribution |
|---|---|
| **Shehryar ul Haq** | Data entry (`input()`), adding an employee (`add_employee()`) and the input validation functions |
| **Muhammad Bilal Ansari** | Searching an employee by ID (`search()`, `display()`) and the file functions: loading the CSV file into memory (`load_data()`) and saving it (`savedata()`) |
| **Amin Aqil** | Updating an employee (`updateEmployee()`) and deleting an employee (`deleteEmployee()`) |
| **Qaim Abbas** | Displaying all records (`displayAllRecords()`), payslip generation (`generatePayslip()`) and the exit option |

The shared parts of the program (the main menu, login and the final integration) were done jointly by the group.

## Technologies

- **Language:** C
- **Standard headers:** `stdio.h`, `string.h`, `ctype.h`, `math.h`, `time.h` (no third-party libraries)
- **Compiler:** GCC (MinGW) on Windows
- **Editor:** Visual Studio Code
- **Data storage:** plain-text CSV file
- **Version control:** Git and GitHub

## Project structure

```
.
├── Main.c                          # complete source code of the program
├── Employee Payroll Records.csv    # data file (created automatically if missing)
└── README.md
```

## How to run

### Requirements

- A C compiler. The code uses the function `strcmpi()`, which is available in GCC on **Windows (MinGW)** but is not part of the C standard, so Windows with MinGW is the easiest platform.

### Windows (MinGW / GCC)

```
gcc -Wall -Wextra -o payroll Main.c
payroll.exe
```

### Linux or macOS

`strcmpi()` is not available there, but `strcasecmp()` does the same job. The following command makes it work without changing the code:

```
gcc -Wall -Wextra -include strings.h -Dstrcmpi=strcasecmp -o payroll Main.c
./payroll
```

### Important notes before running

- Run the program from the folder that contains the data file. The data file must be named exactly **`Employee Payroll Records.csv`** (with spaces). If it does not exist, the program creates it with the header line when the first employee is added.
- The username and password for the login are defined by the macros `ADMIN_USERNAME` and `ADMIN_PASSWORD` at the top of `Main.c`. Change them there if you wish.


## Using the program

After a successful login, the main menu is shown:

| Option | Action |
|---|---|
| 1 | Add a new employee |
| 2 | Display all employee records |
| 3 | Update an employee's record |
| 4 | Delete an employee record |
| 5 | Search for an employee record |
| 6 | Generate payslip for an employee |
| 7 | Exit from the management system |

After every action the program waits for the Enter key and then returns to the menu. A wrong menu choice (including text such as `abc`) only shows an error message.

**Fields that can be updated (option 3):** designation, department, email, bank name (with account number), bank account number, basic salary, allowance, bonus and overtime pay. 

## Input validation rules

Every value is checked before it is accepted, and the program asks again until the value is valid.

| Field | Rule |
|---|---|
| Name, designation, department, bank name | Letters and spaces only |
| Employee ID | Letters and digits only; must be unique (upper/lower case is ignored) |
| Email | At least 5 characters, no spaces or commas, exactly one `@` (not first) and a `.` after it |
| Date of joining | `dd/mm/yy`, a real calendar date (leap years are handled) |
| Bank account number | 8 to 34 letters or digits |
| Basic salary | A valid number greater than 0 |
| Allowance, overtime pay, bonus | A valid number, 0 or more |

Text such as `abc`, `nan` or `inf` in a number field is rejected.

## Salary calculation

```
Taxes       = 10% of Basic Salary
Net Salary  = Basic Salary + Allowance + Overtime Pay + Bonus - Taxes
```

The same formula is used when an employee is added and whenever a salary value is updated, so the values always stay consistent.

## How the program works

- All employees are held in a global array of `Employee` structures (up to 100). A counter tells how many are in use.
- After login, `load_data()` reads the CSV file into the array. Lines that cannot be read are skipped and a warning is shown.
- **Add:** the new record is appended to the end of the CSV file and to the array.
- **Update / Delete:** the array is changed first, then `savedata()` rewrites the whole CSV file from the array.
- **Display / Search / Payslip:** these only read the array, so what they show always agrees with the file.

## Data file format

`Employee Payroll Records.csv` is a plain-text file. The first line contains the column names and every other line is one employee with 14 comma-separated fields:

```
NAME, ID, DESIGNATION, DEPARTMENT, EMAIL, DATE OF JOINING, BANK NAME, BANK ACCOUNT,
BASIC SALARY, ALLOWANCE, OVERTIME PAY, BONUSES, TAXES, NET SALARY
```

The file can be opened in Excel or Notepad to check the data. 

## Known limitations

- A maximum of **100 employees** can be stored.
- The **tax rate is fixed** at 10% of the basic salary.
- The login details are **hard-coded** in the source code, and the password is visible while typing.
- Only one user (the admin) is supported.
- `strcmpi()` is not standard C, so the code needs MinGW/GCC on Windows (or the Linux/macOS workaround above).


## Future expansion

- **Sorting:** sort the employees by name, ID or salary, and add searching by name or department.
- **Graphical interface:** build a graphical (or web) interface on top of the same logic, replacing the console menu.
- **Flexible tax slabs:** calculate tax from income slabs and deductions instead of a fixed 10%, with rates the administrator can change.
- **More employees:** use dynamic memory or a linked list so that the number of employees is not limited to 100.
- **Better security:** hide the password while typing, store login details outside the source code, and support several users with different rights.
- **More payslip features:** save payslips to files and generate payslips for all employees at once.
- **More editing options:** allow the name, ID and joining date to be corrected, and make automatic backups of the data file.


[README.md](https://github.com/user-attachments/files/35100445/README.md)
# Employee Payroll Management System

## Overview

The **Employee Payroll Management System** is a C-based console application designed to manage employee records and calculate payroll information.

The system stores employee information such as personal details, job information, bank details, salary components, taxes, and net salary. Employee records are saved in a CSV file so that data can be loaded again when the program is restarted.

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

The shared parts of the program (the main menu, login, header files and the final integration) were done jointly by the group.

## Employee Information

Each employee record contains the following information:

- Name
- Employee ID
- Designation
- Department
- Email
- Date of joining
- Bank name
- Bank account
- Basic salary
- Allowance
- Overtime pay
- Bonuses
- Taxes
- Net salary

The net salary is calculated using:

```text
Net Salary = Basic Salary + Allowance + Overtime Pay + Bonuses - Taxes
```
The tax is calculated as:
```text
Taxes = 10% of Basic Salary 
```
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
## Project Structure

The program is divided into separate source and header files to make the code easier to maintain.

```text
Employee Payroll Management System/
│
├── main.c
├── employee.h
├── helper.c
├── helper.h
├── validation.c
├── validation.h
└── Employee Payroll Records.csv
```

### `main.c`

Contains the main program flow, menu handling, and application logic.

It includes the required project headers:

```c
#include "employee.h"
#include "helper.h"
#include "validation.h"
```

### `employee.h`

Contains the shared employee structure, payroll-related constants, and declarations for shared employee data.

The `Employee` structure contains all fields associated with an employee.

Shared variables are declared using `extern`, for example:

```c
extern Employee emp[Max_Employee];
extern int employee_count;
```

This allows multiple `.c` files to access the same employee data without creating multiple copies.

### `helper.c`

Contains general-purpose helper functions used by the program, including:

- CSV file handling
- Input buffer clearing
- Floating-point input
- Screen formatting
- CSV header writing
- Opening the CSV file
- Net salary calculation
- Loading employee data

CSV-specific definitions such as the CSV filename and CSV parsing format are kept in the helper implementation.

### `helper.h`

Contains declarations for helper functions used by other source files.

It also includes:

```c
#include <stdio.h>
#include "employee.h"
```

so that types such as `FILE` and `Employee` are available where required.

### `validation.c`

Contains validation functions for employee input, including validation of:

- Names
- Employee IDs
- Designations
- Departments
- Email addresses
- Dates
- Bank names
- Bank account numbers
- Basic salary
- Allowances
- Overtime pay
- Bonuses

### `validation.h`

Contains the function declarations for the validation functions implemented in `validation.c`.

## Data Storage

Employee records are stored in:

```text
Employee Payroll Records.csv
```

The CSV file contains a header followed by employee records.The file can be opened in Excel or Notepad to check the data.

Example structure:

```text
NAME,ID,DESIGNATION,DEPARTMENT,EMAIL,DATE OF JOINING,BANK NAME,BANK ACCOUNT,BASIC SALARY,ALLOWANCE,OVERTIME PAY,BONUSES,TAXES,NET SALARY
```

When the program opens the CSV file for appending:

- If the file does not exist, it is created.
- If the file is empty, the CSV header is written.
- If the last record does not end with a newline, a newline is added before the next record.

## Loading Data

When employee data is loaded, each CSV line is parsed into the `Employee` structure.

A valid record must contain all **14 fields**.

Invalid or damaged lines are skipped and a warning is displayed to the user.

For example:

```text
Warning: 1 line(s) in "Employee Payroll Records.csv" could not be read and were skipped.
```

## Input Validation

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

## Duplicate Employee IDs

Before adding an employee, the program checks whether the employee ID already exists.

The `id_exists()` function searches the existing employee records and prevents duplicate IDs.

## Net Salary Calculation

Net salary is calculated through the `calculate_net_salary()` function.

The calculation is:

```text
Net Salary = Basic Salary + Allowance + Overtime Pay + Bonuses - Taxes
```

The function receives a pointer to the employee:

```c
void calculate_net_salary(Employee *e);
```

This allows the calculated value to be stored directly in the employee record.

## Compilation

The program can be compiled using GCC:

```bash
gcc main.c helper.c validation.c -o payroll
```

On Windows using MinGW GCC, the same command can be used from PowerShell or Command Prompt:

```bash
gcc main.c helper.c validation.c -o payroll.exe
```

## Running the Program

After compilation, run:

```bash
./payroll.exe
```

The program then displays its menu and allows the user to perform employee and payroll operations.

## Header File Organization

The project uses separate header files so that different parts of the program remain organized.

```text
employee.h
    └── Employee structure
    └── Shared constants
    └── extern employee variables

helper.h
    └── Helper function declarations

validation.h
    └── Validation function declarations
```

The actual global variables should be **defined only once** in a `.c` file.

For example:

```c
Employee emp[Max_Employee];
int employee_count = 0;
```

The header should contain only their declarations:

```c
extern Employee emp[Max_Employee];
extern int employee_count;
```

This prevents multiple-definition linker errors.

## Technologies Used

- **Language:** C
- **Compiler:** GCC / MinGW GCC
- **Storage:** CSV file
- **Interface:** Console / Command Line
- **Version control:** Git and GitHub
- **Standard Libraries:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `math.h`, and other required standard headers

## Purpose of the Project

This project demonstrates the use of important C programming concepts, including:

- Structures
- Arrays
- Pointers
- Functions
- Header files
- Multiple source files
- File handling
- CSV data processing
- Input validation
- String manipulation
- Modular programming
- Payroll calculations
- Persistent data storage

## Future Expansion

- **Sorting:** sort the employees by name, ID or salary, and add searching by name or department.
- **Graphical interface:** build a graphical (or web) interface on top of the same logic, replacing the console menu.
- **Flexible tax slabs:** calculate tax from income slabs and deductions instead of a fixed 10%, with rates the administrator can change.
- **More employees:** use dynamic memory or a linked list so that the number of employees is not limited to 100.
- **Better security:** hide the password while typing, store login details outside the source code, and support several users with different rights.
- **More payslip features:** save payslips to files and generate payslips for all employees at once.
- **More editing options:** allow the name, ID and joining date to be corrected, and make automatic backups of the data file.

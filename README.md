# Employee Payroll Management System

## Overview

The **Employee Payroll Management System** is a C-based console application designed to manage employee records and calculate payroll information.

The system stores employee information such as personal details, job information, bank details, salary components, taxes, and net salary. Employee records are saved in a CSV file so that data can be loaded again when the program is restarted.

## Features

- Add new employee records
- View employee records
- Search employee records
- Update existing employee information
- Delete employee records
- Prevent duplicate employee IDs
- Validate employee input
- Calculate employee net salary
- Calculate and store payroll-related values
- Save employee records to a CSV file
- Load previously saved employee records when the program starts
- Automatically create the CSV header when required
- Handle invalid numeric input
- Display warnings for invalid or unreadable CSV records

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
Net Salary =
Basic Salary + Allowance + Overtime Pay + Bonuses - Taxes
```

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

The CSV file contains a header followed by employee records.

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

The program validates user input before accepting employee records.

For example, text fields can be checked so that they contain only permitted characters, while salary-related fields are read using a numeric input function.

Invalid numeric input such as:

```text
12abc
```

is rejected instead of being accepted as a valid salary value.

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

### Windows

```bash
.\payroll.exe
```

### Linux/macOS

```bash
./payroll
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

## Important Notes

- Keep `employee.h` focused on employee data and shared payroll-related definitions.
- Keep CSV-specific implementation details in `helper.c` unless another source file needs them.
- Do not define global variables directly inside a header file.
- Include `<string.h>` in files that use functions such as `strcmp()` and `strstr()`.
- The CSV file should remain in the working directory of the program unless the CSV path is changed in the source code.
- Back up the CSV file before manually editing it.
- Malformed CSV records may be skipped when loading data.

## Technologies Used

- **Language:** C
- **Compiler:** GCC / MinGW GCC
- **Storage:** CSV file
- **Interface:** Console / Command Line
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
